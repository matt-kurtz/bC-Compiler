#include <iostream>
#include "semantics.h"
#include "symbolTable.h"
#include "treeUtils.h"
#include "parser.tab.h"

static int goffset = 0;
static int foffset = 0;
static int varCounter = 0;
static bool newScope = true;

void treeTraverse(TreeNode *syntree, SymbolTable *symtabX);


void checkTypeParameters(TreeNode *current, TreeNode *declaration, SymbolTable *symTabX) {
   TreeNode *params = current->child[0];
   TreeNode *temp;

   while (params) {
      temp = params->sibling;
      params->sibling = NULL;
      treeTraverse(params, symTabX);
      params->sibling = temp;
      params = params->sibling;
   }
}

bool InsertError(TreeNode *syntree, SymbolTable *symtabX) {
   if (!symtabX->insert(syntree->attr.name, syntree)) {
      return false;
      // ASN_6 error message goes here
   }
   return true;
}

void treeTraverseStmt(TreeNode *syntree, SymbolTable *symTabX) {
   if (syntree == NULL) { return; }
   if (syntree->kind.stmt != CompoundK) {
      newScope = true;
   }
   switch (syntree->kind.stmt) {
      case BreakK:
	break;
      case CompoundK:
	if (newScope) {
	   int rem_offset;
	   symTabX->enter((char *)"compoundStmt");
	   rem_offset = foffset;
	   treeTraverse(syntree->child[0], symTabX);
	   syntree->size = foffset;
	   treeTraverse(syntree->child[1], symTabX);
	   foffset = rem_offset;
	   symTabX->leave();
	}
	else {
	   newScope = true;
	   treeTraverse(syntree->child[0], symTabX);
	   syntree->size = foffset;
	   treeTraverse(syntree->child[1], symTabX);
	}
	break;
      case ForK:
	int rem_offset;
	symTabX->enter((char *)"ForStmt");
	rem_offset = foffset;
	treeTraverse(syntree->child[0], symTabX);
	foffset -= 2;
	syntree->size = foffset;
	treeTraverse(syntree->child[1], symTabX);
	treeTraverse(syntree->child[2], symTabX);
	foffset = rem_offset;
	symTabX->leave();
	break;
      default:
	treeTraverse(syntree->child[0], symTabX);
	treeTraverse(syntree->child[1], symTabX);
	treeTraverse(syntree->child[2], symTabX);
        break;
   }
}

void treeTraverseExp(TreeNode *syntree, SymbolTable *symTabX) {
   if (syntree == NULL) { return; }
   newScope = true;
   TreeNode *tmp;
   switch (syntree->kind.exp) {
      case IdK:
	if ((tmp = (TreeNode *)(symTabX->lookup(syntree->attr.name)))) {
	   syntree->type = tmp->type;
	   syntree->isStatic = tmp->isStatic;
	   syntree->isArray = tmp->isArray;
	   syntree->size = tmp->size;
	   syntree->varKind = tmp->varKind;
	   syntree->offset = tmp->offset;
	}
	break;
      case AssignK:
	 treeTraverse(syntree->child[0], symTabX);
	 treeTraverse(syntree->child[1], symTabX);
	 syntree->type = Integer;
	 //printf("%d\n",syntree->attr.op);
	 switch (syntree->attr.op) {
	    case '=':
	       //printf("in '='\n");
	    case '[':
	       //printf("%s\n", expTypeToStr(syntree->child[0]->type, syntree->isArray, syntree->isStatic));
	       syntree->type = syntree->child[0]->type;
	       break;
	    case EQ:
	    case NEQ:
	    case LEQ:
	    case '<':
	    case GEQ:
	    case '>':
	       syntree->type = Boolean;
	       break;
	 }
         // no break here update: works when I put a break...
	 break; 
      case OpK:
	//printf("We're in OpK\n");
	treeTraverse(syntree->child[0], symTabX);
	treeTraverse(syntree->child[1], symTabX);
	syntree->type = Integer;
	switch (syntree->attr.op) {
	   case '=':
	   case '[':
	      syntree->type = syntree->child[0]->type;
	      break;
	   case OR:
	   case AND:
	   case NOT:
	   case EQ:
	   case NEQ:
	   case LEQ:
	   case '<':
	   case GEQ:
	   case '>':
	      syntree->type = Boolean;
	      break;
	}
	break;
      case CallK:
 	if (tmp = (TreeNode *)(symTabX->lookup(syntree->attr.name))) {
	   syntree->type = tmp->type;
	   syntree->offset = tmp->offset; // assuming offset is size
	   syntree->size = tmp->size;
	   checkTypeParameters(syntree, tmp, symTabX);
	}
	break;
      case ConstantK:
	if (syntree->type == Char && syntree->isArray) {
	   syntree->varKind = Global;
	   syntree->offset = goffset - 1;
	   goffset -= syntree->size;
	} 
	    
	    	    
      default:
        treeTraverse(syntree->child[0], symTabX);
        treeTraverse(syntree->child[1], symTabX);
        treeTraverse(syntree->child[2], symTabX);
        break;
   }
}

void treeTraverseDecl(TreeNode *syntree, SymbolTable *symTabX) {
	if (syntree == NULL) { return; }
	newScope = true;
	switch(syntree->kind.decl) {
           case VarK:
	    treeTraverse(syntree->child[0], symTabX);
	    // starting here and to the don't add break comment
	    if (syntree->isArray) {
	       //printf("This is an array\n");
	       syntree->offset--;
	    }
	    // don't add break 
	   case ParamK:
	       if (InsertError(syntree, symTabX)) {
                  if (symTabX->depth() == 1) {
                     syntree->varKind = Global;
		     syntree->offset = goffset;
		     goffset -= syntree->size;
		  }
 		  else if (syntree->isStatic) {
 		     syntree->varKind = LocalStatic;
		     syntree->offset = goffset;
		     goffset -= syntree->size;
		     {
		        char *newName;
			newName = new char[strlen(syntree->attr.name) + 10];
			sprintf(newName, "%s-%d", syntree->attr.name, ++varCounter);
			symTabX->insertGlobal(newName, syntree);
			delete [] newName;
		     }
		  }
		  else {
		     syntree->varKind = Local;
		     syntree->offset = foffset;
		     foffset -= syntree->size;
		  }
	       }
	     if (syntree->kind.decl == ParamK) {
	        syntree->varKind = Parameter;
	     } else if (syntree->isArray) {
	        syntree->offset--;
	     }
	     break;
	   case FuncK:
	       foffset = -2;
	       InsertError(syntree, symTabX);
	       symTabX->enter(syntree->attr.name);
	       newScope = false;
	       treeTraverse(syntree->child[0], symTabX);
	       syntree->size = foffset;
	       treeTraverse(syntree->child[1], symTabX);
	       syntree->varKind = Global;
	       symTabX->leave();
               break;
         }



}

void treeTraverse(TreeNode *syntree, SymbolTable *symTabX) {
   if (syntree == NULL) {
      return;
   }
      
   if (syntree->nodekind == DeclK) {
      treeTraverseDecl(syntree, symTabX);
   }
   else if (syntree->nodekind == StmtK) {
      treeTraverseStmt(syntree, symTabX);
   }
   else {
      treeTraverseExp(syntree, symTabX);
   }
   if (syntree->sibling) {
      treeTraverse(syntree->sibling, symTabX);
   }
}



TreeNode *loadIOLib(TreeNode *syntree) {
   TreeNode *input, *output, *param_output;
   TreeNode *inputb, *outputb, *param_outputb;
   TreeNode *inputc, *outputc, *param_outputc;
   TreeNode *outnl;

   // Stuff from next slides
   input = newDeclNode(FuncK, Integer); //1
      input->lineno = -1; // all are -1
      input->attr.name = strdup("input"); // we named the variables well
      input->type = Integer;
   inputb = newDeclNode(FuncK, Boolean); //2
      inputb->lineno = -1;
      inputb->attr.name = strdup("inputb");
      inputb->type = Boolean;
   inputc = newDeclNode(FuncK, Boolean); //3
      inputc->lineno = -1;
      inputc->attr.name = strdup("inputc");
      inputc->type = Char;
   param_output = newDeclNode(ParamK, Void); //4
      param_output->lineno = -1;
      param_output->attr.name = strdup("*dummy*");
      param_output->type = Integer;
   output = newDeclNode(FuncK, Void); //5
      output->lineno = -1;
      output->attr.name = strdup("output");
      output->type = Void;
      output->child[0] = param_output;
   param_outputb = newDeclNode(ParamK, Void); //6
      param_outputb->lineno = -1;
      param_outputb->attr.name = strdup("*dummy*");
      param_outputb->type = Boolean;
   outputb = newDeclNode(FuncK, Void); //7
      outputb->lineno = -1;
      outputb->attr.name = strdup("outputb");
      outputb->type = Void;
      outputb->child[0] = param_outputb;
   param_outputc = newDeclNode(ParamK, Void); //8
      param_outputc->lineno = -1;
      param_outputc->attr.name = strdup("*dummy*");
      param_outputc->type = Char;
   outputc = newDeclNode(FuncK, Void); //9
      outputc->lineno = -1;
      outputc->attr.name = strdup("outputc");
      outputc->type = Void;
      outputc->child[0] = param_outputc;
   outnl = newDeclNode(FuncK, Void); //10
      outnl->lineno = -1;
      outnl->attr.name = strdup("outnl");
      outnl->type = Void;
      outnl->child[0] = NULL;
   
   // link them and prefix the tree we are interested in traversing
   // This will put the symbols in the symbol table.

   input->sibling = output;
   output->sibling = inputb;
   inputb->sibling = outputb;
   outputb->sibling = inputc;
   inputc->sibling = outputc;
   outputc->sibling = outnl;
   outnl->sibling = syntree; // add in the tree we were given
   return input;

}

TreeNode *semanticAnalysis(TreeNode *syntree,          // pass in and return an annotated syntax tree
                           SymbolTable *symtabX,       // pass in and return the symbol table
                           int &globalOffset            // return the offset past the globals
    ) 
{
   //printf("We're in here");
   syntree = loadIOLib(syntree);

   treeTraverse(syntree, symtabX);
   globalOffset = goffset;
   return syntree;

}

