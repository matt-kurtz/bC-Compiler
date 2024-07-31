#include "codegen.h"
#include "emitcode.h"
#include "treeUtils.h"
#include "parser.tab.h"
extern int numErrors;
extern int numWarnings;
//extern void yyparse();
extern int yydebug;
extern TreeNode *syntaxTree;
// extern char **largerTokens; didn't use this
extern void initTokenStrings();


//These offsets that never change
#define OFPOFF 0
#define RETURNOFFSET -1

int toffset; // available temporary space

FILE *code; 			// shared global code – already included
static bool linenumFlag; 	// mark with line numbers
static int breakloc; 		// which while to break to
static SymbolTable *globals; 	// the global symbol table

void codegenLibraryFun(TreeNode *currnode);
void codegenDecl(TreeNode *currnode);
void codegenStatement(TreeNode *currnode);
void codegenExpression(TreeNode *currnode);
void codegenFun(TreeNode *currnode);
void codegenGeneral(TreeNode *currnode);
void initAGlobalSymbol(std::string sym, void *ptr);

int offsetRegister(VarKind v) {
   switch (v) {
      case Local: return FP;
      case Parameter: return FP;
      case Global: return GP;
      case LocalStatic: return GP;
      default:
         printf((char *)"ERROR(codegen): looking up offset register for a variable of type %d\n", v);
	 return 666;
   }
}

void initGlobalArraySizes()
{
   emitComment((char *)"INIT GLOBALS AND STATICS");
   globals->applyToAllGlobal(initAGlobalSymbol);
   emitComment((char *)"END INIT GLOBALS AND STATICS");
}


void commentLineNum(TreeNode *currnode)
{
   char buf[16];
   if (linenumFlag) {
      sprintf(buf, "%d", currnode->lineno);
      emitComment((char *)"Line: ", buf);
   }
}


// Generate a header for our code
void codegenHeader(char *srcFile)
{
   emitComment((char *)"bC compiler version bC-Su23");
   emitComment((char *)"File compiled: ", srcFile);
}


void codegenFun(TreeNode *currnode)
{
   emitComment((char *)"");
   emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
   emitComment((char *)"FUNCTION", currnode->attr.name);
   toffset = currnode->size; // recover the end of activation record
   emitComment((char *)"TOFF set:", toffset);
   // IMPORTANT: For function nodes the offset is defined to be the position of the
   // function in the code space! This is accessible via the symbol table.
   // remember where this function is:
   currnode->offset = emitSkip(0); // offset holds the instruction address!!
   // Store return address
   emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");
   // Generate code for the statements...
   codegenGeneral(currnode->child[1]);
   // In case there was no return statement
   // set return register to 0 and return
   emitComment((char *)"Add standard closing in case there is no return statement");
   emitRM((char *)"LDC", RT, 0, 6, (char *)"Set return value to 0");
   emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
   emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
   emitGoto(0, AC, (char *)"Return");
   emitComment((char *)"END FUNCTION", currnode->attr.name);
}

void codegenGeneral(TreeNode *currnode)
{ 
   while (currnode) {
      switch (currnode->nodekind) {
         case StmtK:
	    codegenStatement(currnode);
	    break;
         case ExpK:
	    emitComment((char *)"EXPRESSION");
	    codegenExpression(currnode);
	    break;
	 case DeclK:
	    codegenDecl(currnode);
	    break;
      }
      currnode = currnode->sibling;
   }
}

// given the syntax tree for declarations generate the code
void codegenDecl(TreeNode *currnode)
{
   //printf("\nIn codegenDecl\n"); 
   commentLineNum(currnode);
   switch(currnode->kind.decl) {
      case VarK:
         // You have a LOT to do here!!!!
	 if (currnode->isArray) {
	    switch (currnode->varKind) {
	       case Local:
	          emitRM((char *)"LDC", AC, currnode->size-1, 6, (char *)"load size of array", currnode->attr.name);
		  emitRM((char *)"ST", AC, currnode->offset+1, offsetRegister(currnode->varKind), (char *)"save size of array", currnode->attr.name);
	          break;
	       case LocalStatic:
	       case Parameter:
	       case Global:
	  // do nothing here
	          break;
	       case None:
		  // Error condition
		  break;
	    }
	    // ARRAY VALUE initialization
	    if (currnode->child[0]) {
	       codegenExpression(currnode->child[0]);
	       emitRM((char *)"LDA", AC1, currnode->offset, offsetRegister(currnode->varKind), (char *)"address of lhs");
	       emitRM((char *)"LD", AC2, 1, AC, (char *)"size of rhs");
	       emitRM((char *)"LD", AC3, 1, AC1, (char *)"size of lhs");
	       emitRO((char *)"SWP", AC2, AC3, 6, (char *)"pick smallest size");
	       emitRO((char *)"MOV", AC1, AC, AC2, (char *)"array op =");
	    }
	   
	 }
	    else { // !currnode->isArray
	       // SCALAR VALUE initialization
	       if (currnode->child[0]) {
	          switch (currnode->varKind) {
		     case Local:
			// compute rhs -> AC;
			codegenExpression(currnode->child[0]);
			
			// save it
			emitRM((char *)"ST", AC, currnode->offset, FP, (char *)"Store variable", currnode->attr.name);
		     case LocalStatic:
		     case Parameter:
		     case Global:
			// do nothing
		        break;
		     case None:
			// Error condition
		        break;
		  }
	       }
	    }
         break;
      case FuncK:
         if (currnode->lineno == -1) { // These are the library functions we just added
            codegenLibraryFun(currnode);
         }
	 else {
	    codegenFun(currnode);
 	 }
	 break;
      case ParamK:
	 printf("\n We're in here\n");
	 // IMPORTANT: no instructions need to be allocated for parameters here
	 break;
   }
}

void codegenStatement(TreeNode *currnode)
{
   // local state to remember stuff
   int skiploc=0, skiploc2=0, currloc=0; // some temporary instuction addresses
   TreeNode *loopindex=NULL; // a pointer to the index variable declaration node
   commentLineNum(currnode);
   switch (currnode->kind.stmt) {
   /////////////////Other cases
      case CompoundK:
      { 
         int savedToffset;
	 savedToffset = toffset;
	 toffset = currnode->size; // recover the end of activation record
	 emitComment((char *)"COMPOUND");
	 emitComment((char *)"TOFF set:", toffset);
	 codegenGeneral(currnode->child[0]); // process inits
	 emitComment((char *)"Compound Body");
	 codegenGeneral(currnode->child[1]); // process body
	 toffset = savedToffset;
	 emitComment((char *)"TOFF set:", toffset);
	 emitComment((char *)"END COMPOUND");
      }
         break;
      case WhileK:
	 emitComment((char *)"WHILE");
	 currloc = emitSkip(0);			// return to here to do the test
	 codegenExpression(currnode->child[0]); // test expression
         
         emitRM((char *)"JNZ", AC, 1, PC, (char *)"Jump to while part");
	 emitComment((char *)"DO");

	 skiploc = breakloc; 		// save the told break statement return point
	 breakloc = emitSkip(1); // addr of instr that jumps to end of loop
				 // this is also the backpatch point
	 codegenGeneral(currnode->child[1]); // do body of loop
	 //emitRM((char *)"JMP", PC, skiploc, PC, (char *)"break");
	 emitGotoAbs(currloc, (char *)"go to beginning of loop");
	 backPatchAJumpToHere(breakloc, (char *)"Jump past loop [backpatch]");
						// backpatch jump to end of loop
	 breakloc = skiploc; // restore for break statement
	 emitComment((char *)"END WHILE");
         break;
      case BreakK:
	 {
	    emitComment((char *)"BREAK");
	    emitGotoAbs(breakloc, (char *)"break");

	 }   
	 break;
      case ForK:
	 {
	    int savedToffset, startoff, stopoff, stepoff, currloc, /*breakloc*/ skiploc;
	    TreeNode *loopindex, *rangenode;
	    savedToffset = toffset;
	    toffset = currnode->size;
	    emitComment((char *)"TOFF set:", toffset);
	    emitComment((char *)"FOR");
	    
	    loopindex = currnode->child[0];
	    startoff = loopindex->offset;
	    stopoff = startoff-1;
	    stepoff = startoff-2;
	    //  --------Processing RangeK inside ForK
	    rangenode = currnode->child[1];
	    codegenExpression(rangenode->child[0]);
	    emitRM((char *)"ST", AC, startoff, FP, (char *)"save starting value in index variable");
	    codegenExpression(rangenode->child[1]);
	    emitRM((char *)"ST", AC, stopoff, FP, (char *)"save stop value");
	    if (rangenode->child[2] == NULL) { emitRM((char *)"LDC", AC, 1, 6, (char *)"default increment by 1"); emitRM((char *)"ST", AC, stepoff, FP, (char *)"save step value"); }
            if (rangenode->child[2] != NULL) { codegenExpression(rangenode->child[2]); emitRM((char *)"ST", AC, stepoff, FP, (char *)"save step value"); }
	    //  --------Processing RangeK inside ForK now complete
	    currloc = emitSkip(0); 
	    emitRM((char *)"LD", AC1, startoff, FP, (char *)"loop index");
	    emitRM((char *)"LD", AC2, stopoff, FP, (char *)"stop value");
	    emitRM((char *)"LD", AC, stepoff, FP, (char *)"step value");
	    emitRO((char *)"SLT", AC, AC1, AC2, (char *)"Op <");
	    emitRM((char *)"JNZ", AC, 1, PC, (char *)"Jump to loop body");
	    skiploc = breakloc;
	    breakloc = emitSkip(1);
	    codegenGeneral(currnode->child[2]);
	    emitComment((char *)"Bottom of loop increment and jump");
	    emitRM((char *)"LD", AC, startoff, FP, (char *)"Load index");
	    emitRM((char *)"LD", AC2, stepoff, FP, (char *)"Load step");
	    emitRO((char *)"ADD", AC, AC, AC2, (char *)"increment");
	    emitRM((char *)"ST", AC, startoff, FP, (char *)"store back to index");
	    emitGotoAbs(currloc, (char *)"go to beginning of loop");
	    backPatchAJumpToHere(breakloc, (char *)"Jump past loop [backpatch]");
	    breakloc = skiploc;
	    emitComment((char *)"END LOOP");
	 }
	 break;
      case IfK:
	 int skiploc, skiploc2;
	 emitComment((char *)"IF");
	 codegenExpression(currnode->child[0]);
	 skiploc = emitSkip(1);
	 emitComment((char *)"THEN");
	 codegenGeneral(currnode->child[1]);
	 if (currnode->child[2]) {
	    skiploc2 = emitSkip(1);
	 } 
	 backPatchAJumpToHere((char *)"JZR", AC, skiploc, (char *)"Jump around the THEN if false [backpatch]");
	 if (currnode->child[2]) {
	    emitComment((char *)"ELSE");
	    codegenGeneral(currnode->child[2]);
	    backPatchAJumpToHere((char *)"JMP", 7, skiploc2, (char *)"Jump around the ELSE [backpatch]");
	 }
	 // could put error here for no if statement condition
	 emitComment((char *)"END IF");
	 break;
      case ReturnK:
	 emitComment((char *)"RETURN");
	 if (currnode->child[0]) {
	    codegenExpression(currnode->child[0]);
            emitRM((char *)"LDA", RT, 0, AC, (char *)"Copy result to return register");
	 }
         emitRM((char *)"LD", AC, -1, FP, (char *)"Load return address");
	 emitRM((char *)"LD", FP, 0, FP, (char *)"Adjust fp");
	 emitRM((char *)"JMP", PC, 0, AC, (char *)"Return");
	 break;
      default:
         printf("We're in default\n");
	 break;
   }
}

void codegenLibraryFun(TreeNode *currnode)
{

   //printf("\n\nWe're in codegenLibraryFun\n\n");
 
   emitComment((char *)"");
   emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
   emitComment((char *)"FUNCTION", currnode->attr.name);
   // remember where this function is
   currnode->offset = emitSkip(0);
   // Store return address
   emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");
   // Next slides here
   
   if (strcmp(currnode->attr.name, (char *)"input")==0) {
      emitRO((char *)"IN", RT, RT, RT, (char *)"Grab int input");
   }
   else if (strcmp(currnode->attr.name, (char *)"inputb")==0) {
      emitRO((char *)"INB", RT, RT, RT, (char *)"Grab bool input");
   }
   else if (strcmp(currnode->attr.name, (char *)"inputc")==0) {
      emitRO((char *)"INC", RT, RT, RT, (char *)"Grab char input");
   }
   else if (strcmp(currnode->attr.name, (char *)"output")==0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUT", AC, AC, AC, (char *)"Output integer");
   }
   else if (strcmp(currnode->attr.name, (char *)"outputb")==0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUTB", AC, AC, AC, (char *)"Output bool");
   }
   else if (strcmp(currnode->attr.name, (char *)"outputc")==0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUTC", AC, AC, AC, (char *)"Output char");
   }
   else if (strcmp(currnode->attr.name, (char *)"outnl")==0) {
      emitRO((char *)"OUTNL", AC, AC, AC, (char *)"Output a newline");
   }
   else {
      emitComment((char *)"ERROR(LINKER): No support for special function");
      emitComment(currnode->attr.name);
   }

   emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
   emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
   emitGoto(0, AC, (char *)"Return");
   emitComment((char *)"END FUNCTION", currnode->attr.name);
}

void codegenExpression(TreeNode *currnode) {
   /*
   • AssignK
   • CallK
   • ConstantK
   • IdK
   • OpK
   */
   switch (currnode->kind.exp) {
      case AssignK:
	 TreeNode *rhs, *lhs;
	 TreeNode *var, *index;
	 lhs = currnode->child[0];
         rhs = currnode->child[1];
	 if (lhs->attr.op == '[') {
	    var = lhs->child[0];
	    index = lhs->child[1];
	    //printf("currnode: %s\n", currnode->attr.name);
	    codegenExpression(index);
	    if (rhs) {
	       emitRM((char *)"ST", AC, toffset, 1, (char *)"Push index");
	       toffset--; emitComment((char *)"TOFF dec:", toffset);
	       codegenExpression(rhs);
	       toffset++; emitComment((char *)"TOFF inc:", toffset);
	       emitRM((char *)"LD", AC1, toffset, 1, (char *)"Pop index");
	       if (var->varKind == Parameter) {
	          emitRM((char *)"LD", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name);
	       }
	       else if (var->varKind == Local) {
	          emitRM((char *)"LDA", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name);
	       }
	       else {
	          emitRM((char *)"LDA", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name);
	       }
	       emitRO((char *)"SUB", AC2, AC2, AC1, (char *)"Compute offset of value");
	    }
	    else {
	       if (var->varKind == Global) {
	          emitRM((char *)"LDA", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name);
	          emitRO((char *)"SUB", AC2, AC2, AC, (char *)"Compute offset of value");
	       } else if (var->varKind == Parameter) {
	          emitRM((char *)"LD", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name);
	          emitRO((char *)"SUB", AC2, AC2, AC, (char *)"Compute offset of value");
	       } else if (var->varKind == Local) {
	          emitRM((char *)"LDA", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name); 
                  emitRO((char *)"SUB", AC2, AC2, AC, (char *)"Compute offset of value");
	       } else {
	          emitRM((char *)"LDA", AC2, var->offset, offsetRegister(var->varKind), (char *)"Load address of base of array", var->attr.name); // this was LDA
                  emitRO((char *)"SUB", AC2, AC2, AC, (char *)"Compute offset of value");
	       }  
	    
	    }
	    // Is there a way to reuse codegenExpression to get the INC token below? Ask Dr. BC
	    //codegenExpression(currnode);
	    //emitRM((char *)"LD", AC, 0, AC2, (char *)"load lhs variable", var->attr.name);
	    if (currnode->attr.op == INC) {
	       emitRM((char *)"LD", AC, 0, AC2, (char *)"load lhs variable", var->attr.name);
	       emitRM((char *)"LDA", AC, 1, AC, (char *)"increment value of", var->attr.name);
	    }
	    else if (currnode->attr.op == ADDASS) {
	       emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name);
	       emitRO((char *)"ADD", AC, AC1, AC, (char *)"op +=");
	    }
	    else if (currnode->attr.op == DIVASS) {
	       emitRM((char *)"LD", AC1, lhs->offset, AC2, (char *)"load lhs variable", var->attr.name);
               emitRO((char *)"DIV", AC, AC1, AC, (char *)"op /=");
               //emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
	    }
	    else if (currnode->attr.op == DEC) {
	       emitRM((char *)"LD", AC, 0, AC2, (char *)"load lhs variable", var->attr.name);
	       emitRM((char *)"LDA", AC, -1, AC, (char *)"decrement value of", var->attr.name);
	    }
	    else if (currnode->attr.op == MULASS) {
	       emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name);
	       emitRO((char *)"MUL", AC, AC1, AC, (char *)"op *=");
	       
	    }
	    else if (currnode->attr.op == SUBASS) {
	       emitRM((char *)"LD", AC1, lhs->offset, AC2, (char *)"load lhs variable", var->attr.name);
               emitRO((char *)"SUB", AC, AC1, AC, (char *)"op -=");
	    }
            emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
	 }
	 else {
	    int offReg;
	    //offReg = offsetRegister(currnode->varKind);
	    // Lots of cases that use it. Here is a sample:
	    if (currnode->isArray == false) {
	       switch (currnode->attr.op) {
	          case '=':
		     //offReg = offsetRegister(currnode->varKind);
		     if (rhs) {
		        //offReg = offsetRegister(currnode->child[1]->varKind);
		        codegenExpression(rhs);
			emitRM((char *)"ST", AC, lhs->offset, offsetRegister(lhs->varKind), (char *)"Store variable", lhs->attr.name);
		     }
		     //emitRM((char *)"ST", AC, lhs->offset, offsetRegister(lhs->varKind), (char *)"Store variable", lhs->attr.name);
	             break;
	          case '+':
		     printf("\nWe got a +\n");
		     break;
	          case ADDASS:
		     
		     if (rhs) {
			offReg = offsetRegister(lhs->varKind);
			codegenExpression(rhs);
		        emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
		        emitRO((char *)"ADD", AC, AC1, AC, (char *)"op +=");
		        emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
		     }
		     break;
		  case DEC:
		     if (lhs) {
			offReg = offsetRegister(lhs->varKind);
		        //codegenExpression(lhs);
			emitRM((char *)"LD", AC, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                        emitRM((char *)"LDA", AC, -1, AC, (char *)"decrement value of", lhs->attr.name);
                        emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
		     }
		     break;
		  case INC:
		     if (lhs) {
		        offReg = offsetRegister(lhs->varKind);
			emitRM((char *)"LD", AC, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
			emitRM((char *)"LDA", AC, 1, AC, (char *)"increment value of", lhs->attr.name);
                        emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
		     }
		     break;
		  case DIVASS:
		     // check for div by 0 here
		     if (rhs) {
		        offReg = offsetRegister(lhs->varKind);
		        codegenExpression(rhs);
		        emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
			emitRO((char *)"DIV", AC, AC1, AC, (char *)"op /=");
			emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
		     }
		     break;
		  case MULASS:
		     if (rhs) {
                        offReg = offsetRegister(lhs->varKind);
                        codegenExpression(rhs);
                        emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                        emitRO((char *)"MUL", AC, AC1, AC, (char *)"op *=");
                        emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                     } 
		     break;
		  case SUBASS:
		     if (rhs) {
                        offReg = offsetRegister(lhs->varKind);
                        codegenExpression(rhs);
                        emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                        emitRO((char *)"SUB", AC, AC1, AC, (char *)"op -=");
                        emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                     }
		     break;
	       }
	    }
	    break;
      case CallK:
	 TreeNode *funcNode;
	 int callLoc, savedtoffset;
	 emitComment((char *)"CALL", currnode->attr.name);
	 funcNode = ((TreeNode *)(globals->lookup(currnode->attr.name)));
	 callLoc = funcNode->offset;
	 savedtoffset = toffset;
	 emitRM((char *)"ST", FP, savedtoffset, FP, (char *)"Store fp in ghost frame for", currnode->attr.name);
	 //emitRM((char *)"ST", FP, -2, FP, (char *)"Store fp in ghost frame for", currnode->attr.name);
	 toffset--; emitComment((char *)"TOFF dec:", toffset);
	 toffset--; emitComment((char *)"TOFF dec:", toffset);
	 if (currnode->child[0]) {
	    //int paramOffset = -4;
	    TreeNode *param = currnode->child[0];
	    int param_num = 0;
	    while (param) {
	       param_num++;
	       emitComment((char *)"Param", param_num);
	       codegenExpression(param);
	       emitRM((char *)"ST", AC, toffset, FP, (char *)"Push parameter");
	       toffset--; emitComment((char *)"TOFF dec:", toffset);
	       param = param->sibling;
	       //paramOffset--;
	    }
	 }
	 emitComment((char *)"Param end", currnode->attr.name);
	 emitRM((char *)"LDA", FP, savedtoffset, FP, (char *)"Ghost frame becomes new active frame");
	 //emitRM((char *)"LDA", FP, -2, FP, (char *)"Ghost frame becomes new active frame");
	 emitRM((char *)"LDA", AC, 1, PC, (char *)"Return address in ac");
	 emitRMAbs((char *)"JMP", PC, callLoc, (char *)"CALL", currnode->attr.name);
	 emitRM((char *)"LDA", AC, 0, RT, (char *)"Save the result in ac");
	 emitComment((char *)"Call end", currnode->attr.name);
	 toffset = savedtoffset;
	 emitComment((char *)"TOFF set:", toffset);
	 break;
      case ConstantK:
         switch (currnode->type) {
            case Char:
	       if (currnode->isArray) {
	          emitStrLit(currnode->offset, currnode->attr.string);
		  emitRM((char *)"LDA", AC, currnode->offset, 0, (char *)"Load address of char array");
	       }
	       else {
	          emitRM((char *)"LDC", AC, int(currnode->attr.cvalue), 6, (char *)"Load char constant");
	       }
	       break;
	    case Boolean:
	       emitRM((char *)"LDC", AC, int(currnode->attr.value), 6, (char *)"Load Boolean constant");
	       break;
	    case Integer:
	       emitRM((char *)"LDC", AC, int(currnode->attr.value), 6, (char *)"Load integer constant");
	       break;
	 }
	 break; // added this
      case IdK:
	 if (currnode->isArray) {
	    if (currnode->varKind != Parameter) {
	       emitRM((char *)"LDA", AC, currnode->offset, offsetRegister(currnode->varKind), (char *)"Load address of base of array", currnode->attr.name);
	    } else {
	       emitRM((char *)"LD", AC, currnode->offset, offsetRegister(currnode->varKind), (char *)"Load address of base of array", currnode->attr.name);
	    }
	 } else {
	    if (currnode->child[0] == NULL) {
	       emitRM((char *)"LD", AC, currnode->offset, offsetRegister(currnode->varKind) ,(char *)"Load variable", currnode->attr.name);
	    }
	 }
	 break;
      case OpK:
	 codegenExpression(currnode->child[0]);
	 if (currnode->child[1]) {
	    //codegenExpression(currnode->child[0]); // added this 
	    emitRM((char *)"ST", AC, toffset, FP, (char *)"Push left side");
	    toffset--; emitComment((char *)"TOFF dec:", toffset);
	    codegenExpression(currnode->child[1]);
	    toffset++; emitComment((char *)"TOFF inc:", toffset);
	    emitRM((char *)"LD", AC1, toffset, FP, (char *)"Pop left into ac1");
	 }
	 /*if (currnode->child[0]->isArray) {
	    if (currnode->child[0]->varKind == Parameter) {
	       if (currnode->isArray) {
               emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
	       emitRM((char *)"LD", AC, 0, AC, (char *)"Load array element");
	       }
	       else {
	          //printf("We're here\n");
	          emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
                  emitRM((char *)"LD", AC, 0, AC, (char *)"Load array size");
	       }
            }
	    else if (currnode->attr.op == SIZEOF) {
	       emitRM((char *)"LD", AC, currnode->size, AC, (char *)"Load array size");
	    } else if(currnode->attr.op == '[') {
	      emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
	      //printf("Op is [\n");
	      emitRM((char *)"LD", AC, 0, AC, (char *)"Load array element");
	    } else {
	       emitRM((char *)"LD", AC, 0, AC, (char *)"Load array element");
            }*/
            if (currnode->child[0]->isArray && (currnode->attr.op == LEQ || currnode->attr.op == currnode->attr.op == '<' || currnode->attr.op == '>' || currnode->attr.op == GEQ
  	    || currnode->attr.op == EQ || currnode->attr.op == NEQ)) {
	    
	       emitRM((char *)"LD", AC2, 1, AC, (char *)"AC2 <- |RHS|");
    	       emitRM((char *)"LD", AC3, 1, AC1, (char *)"AC3 <- |LHS|");
    	       emitRM((char *)"LDA", 2, 0, AC2, (char *)"R2 <- |RHS|");
    	       emitRO((char *)"SWP", AC2, AC3, 6, (char *)"pick smallest size");
    	       emitRM((char *)"LD", AC3, 1, AC1, (char *)"AC3 <- |LHS|");
    	       emitRO((char *)"CO", AC1, AC, AC2, (char *)"setup array compare  LHS vs RHS");
    	       emitRO((char *)"TNE", AC2, AC1, AC, (char *)"if not equal then test (AC1, AC)");
    	       emitRO((char *)"JNZ", AC2, 2, 7, (char *)"jump not equal");
    	       emitRM((char *)"LDA", AC, 0, 2, (char *)"AC1 <- |RHS|");
    	       emitRM((char *)"LDA", AC1, 0, AC3, (char *)"AC <- |LHS|");
	    }
	 }
	 // more code will go here
	 switch (currnode->attr.op) {
 	    case '+':
	       emitRO((char *)"ADD", AC, AC1, AC, (char *)"Op +");
	       break;
	    case '-':
	       emitRO((char *)"SUB", AC, AC1, AC, (char *)"Op -");
	       break;
	    case '*':
	       emitRO((char *)"MUL", AC, AC1, AC, (char *)"Op *");
	       break;
	    case '/':
	       emitRO((char *)"DIV", AC, AC1, AC, (char *)"Op /");
	       break;
	    case '%':
	       emitRO((char *)"MOD", AC, AC1, AC, (char *)"Op %");
	       break;
	    case AND:
	       emitRO((char *)"AND", AC, AC1, AC, (char *)"Op AND");
	       break;
	    case OR:
	       emitRO((char *)"OR", AC, AC1, AC, (char *)"Op OR");
	       break;
	    case LEQ:
	       emitRO((char *)"TLE", AC, AC1, AC, (char *)"Op <=");
	       break;
	    case GEQ:
               emitRO((char *)"TGE", AC, AC1, AC, (char *)"Op >=");
               break;
            case '<':
	       emitRO((char *)"TLT", AC, AC1, AC, (char *)"Op <");
	       break;
	    case '>':
               emitRO((char *)"TGT", AC, AC1, AC, (char *)"Op >");
               break;
	    case EQ:
	       emitRO((char *)"TEQ", AC, AC1, AC, (char *)"Op ==");
	       break;
	    case NEQ:
               emitRO((char *)"TNE", AC, AC1, AC, (char *)"Op !=");
               break;
	    case MAX:
	       emitRO((char *)"SWP", AC1, AC, AC, (char *)"Op :>:");
	       break;
	    case MIN:
	       emitRO((char *)"SWP", AC, AC1, AC, (char *)"Op :<:");
	       break;
	    case NOT:
	       emitRM((char *)"LDC", AC1, 1, 6, (char *)"Load 1");
	       emitRO((char *)"XOR", AC, AC, AC1, (char *)"Op XOR to get logical not");
	       break;
	    case '[':
	       emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
	       emitRM((char *)"LD", AC, 0, AC, (char *)"Load array element");
	       break;
	    case '?':
	       if (currnode->child[0]->isArray) {
	          emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
		  toffset--; emitComment((char *)"TOFF dec:", toffset);
		  codegenExpression(currnode->child[0]);
		  emitRM((char *)"LD", 3, 1, 3, (char *)"Op ?");
		  toffset++; emitComment((char *)"TOFF inc:", toffset);
		  emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");
		  emitRO((char *)"SUB", 3, 4, 3, (char *)"compute location from index");
		  emitRM((char *)"LD", 3, 0, 3, (char *)"Load array element");
	       } else {
	          emitRO((char *)"RND", AC, AC, 6, (char *)"Op ?");
	       }
	       break;
	    case SIZEOF:
	       emitRM((char *)"LD", AC, 1, AC, (char *)"Load array size");
	       break;
	    case CHSIGN:
	       emitRO((char *)"NEG", AC, AC, AC, (char *)"Op unary -");
	       break;
	    default:
	       //printf("Stuff goes here\n");
	       break;
	 }
         break;
   }
   return;
}


void initAGlobalSymbol(std::string sym, void *ptr)
{
   TreeNode *currnode;
   // printf("Symbol: %s\n", sym.c_str()); // dump the symbol table
   currnode = (TreeNode *)ptr;
   // printf("lineno: %d\n", currnode->lineno); // dump the symbol table
   if (currnode->lineno != -1) {
      if (currnode->isArray) {
         emitRM((char *)"LDC", AC, currnode->size-1, 6, (char *)"load size of array", currnode->attr.name);
         emitRM((char *)"ST", AC, currnode->offset+1, GP, (char *)"save size of array", currnode->attr.name);
      }
      if (currnode->kind.decl==VarK &&
	 (currnode->varKind == Global || currnode->varKind == LocalStatic)) {
	 if (currnode->child[0]) {
	    // compute rhs -> AC;
	    codegenExpression(currnode->child[0]);
            // save it
	    emitRM((char *)"ST", AC, currnode->offset, GP,
	    (char *)"Store variable", currnode->attr.name);
	 }
      }
   }
}


// Generate init code ...
void codegenInit(int initJump, int globalOffset)
{ 
   backPatchAJumpToHere(initJump, (char *)"Jump to init [backpatch]");
   emitComment((char *)"INIT");
   //OLD pre 4.6 TM emitRM((char *)"LD", GP, 0, 0, (char *)"Set the global pointer");
   emitRM((char *)"LDA", FP, globalOffset, GP, (char *)"set first frame at end of globals");
   emitRM((char *)"ST", FP, 0, FP, (char *)"store old fp (point to self)");
   initGlobalArraySizes();
   emitRM((char *)"LDA", AC, 1, PC, (char *)"Return address in ac");
   {  // jump to main
      TreeNode *funcNode;
      funcNode = (TreeNode *)(globals->lookup((char *)"main"));
      if (funcNode) {
         emitGotoAbs(funcNode->offset, (char *)"Jump to main");
      }
      else {
         printf((char *)"ERROR(LINKER): Procedure main is not defined.\n");
         numErrors++;
      }
   }
   emitRO((char *)"HALT", 0, 0, 0, (char *)"DONE!");
   emitComment((char *)"END INIT");

}


// this is the top level code generator call
void codegen(FILE *codeIn, 	// where the code should be written
	char *srcFile, 		// name of file compiled
	TreeNode *syntaxTree, 	// tree to process
	SymbolTable *globalsIn, // globals so function info can be found
	int globalOffset,
	bool linenumFlagIn)
{ 
   int initJump;
   code = codeIn;
   globals = globalsIn;
   linenumFlag = linenumFlagIn;
   breakloc = 0;
   initJump = emitSkip(1); 		// save a place for the jump to init
   codegenHeader(srcFile); 		// nice comments describing what is compiled
   codegenGeneral(syntaxTree); 		// general code generation including I/O library
   codegenInit(initJump, globalOffset); // generation of initialization for run
}
