#include "treeUtils.h"
#include <string.h>

char expTypeToStrBuffer[80];
char *expTypeToStr(ExpType type, bool isArray, bool isStatic)
{
    char *typeName;

    switch (type) {
    case Void:
       typeName = (char *)"type void";
       break;
    case Integer:
       typeName = (char *)"type int";
       break;
    case Boolean:
       typeName = (char *)"type bool";
       break;
    case Char:
       typeName = (char *)"type char";
       break;
    case UndefinedType:
       typeName = (char *)"undefined type";
       break;
    default:
       char *buffer;
       buffer = new char [80];
       sprintf(buffer, "invalid expType: %d", (int)type);
       return buffer;
    }

    // add static and array attributes
    // static type int
    // static array of type int
    sprintf(expTypeToStrBuffer, "%s%s%s",
            (isStatic ? "static " : ""),
            (isArray ? "array of " : ""),
            typeName);

    return strdup(expTypeToStrBuffer); // memory leak
}

char *varKindToStr(int kind)
{
    switch (kind) {
    case None:
        return (char *)"None";
    case Local:
        return (char *)"Local";
    case Global:
        return (char *)"Global";
    case Parameter:
        return (char *)"Parameter";
    case LocalStatic:
        return (char *)"LocalStatic";
    default:
        return (char *)"unknownVarKind";
    }
}

TreeNode *newNodeFun(TreeNode *c0, TreeNode *c1, TreeNode *c2, TokenData *token) {
   TreeNode *newNode = new TreeNode;
   newNode->child[0] = c0;
   newNode->child[1] = c1;
   newNode->child[2] = c2;
   newNode->size = 1;
   newNode->isStatic = false;
   newNode->isArray = false;
   newNode->isConst = false;
   newNode->isUsed = false;
   newNode->isAssigned = false;

   newNode->sibling = NULL;

   if (token != NULL) {
      newNode->lineno = token->linenum;
      newNode->attr.name = token->svalue;
      
   }
   return newNode;
}

TreeNode *newDeclNode(DeclKind kind, ExpType type, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2)
{
   TreeNode *newNode = newNodeFun(c0, c1, c2, token);
   newNode->nodekind = DeclK;
   newNode->kind.decl = kind;
   newNode->type = type;
   
   return newNode;
}

TreeNode *newStmtNode(StmtKind kind, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2)
{
   TreeNode *newNode = newNodeFun(c0, c1, c2, token);
   newNode->nodekind = StmtK;
   newNode->kind.stmt = kind;
   return newNode;
}

TreeNode *newExpNode(ExpKind kind, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2)
{
   TreeNode *newNode = newNodeFun(c0, c1, c2, token);
   newNode->nodekind = ExpK;
   newNode->kind.exp = kind;
   if (kind == ConstantK) {
      newNode->attr.value = token->nvalue;
   } 
   
   return newNode;
}

static void printSpaces(FILE *listing, int depth)
{
    for (int i=0; i<depth; i++) fprintf(listing, ".   ");
}

void printTreeNode(FILE *listing,
                   TreeNode *tree)
{
   
   
   if (tree->nodekind == DeclK) {
      //fprintf(listing, "I am a DeclK node!");
      switch (tree->kind.decl) {
         case VarK:
	    fprintf(listing, "Var: %s of %s", tree->attr.name, expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            break;
         case ParamK:
            break;
         case FuncK:
            fprintf(listing, "Func: %s returns %s", tree->attr.name, expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            break;
      }
   }
   else if (tree->nodekind == StmtK) {
      switch(tree->kind.stmt) {
         case IfK:
            fprintf(listing, "If");
            break;
         case WhileK:
            fprintf(listing, "While");
            break;
         case ForK:
            fprintf(listing, "For");
            break;
         case CompoundK:
            fprintf(listing, "Compound");
            break;
         case ReturnK:
            fprintf(listing, "Return");
            break;
         case BreakK:
            fprintf(listing, "Break");
            break;
         case RangeK:
	    fprintf(listing, "Range");
            break;
         }
   }
   else if (tree->nodekind == ExpK) {
      //fprintf(listing, "I'm an ExpK node");
      switch(tree->kind.exp) {
         case OpK:
            fprintf(listing, "Var");
            break;
         case AssignK:
            fprintf(listing, "Assign: %s", tree->attr.name);
            //fprintf(listing, "Assign");
            break;
         case IdK:
	    fprintf(listing, "Id: %s", tree->attr.name);
	    break;
	 case ConstantK:
	    fprintf(listing, "Const %d", tree->attr.value);
	    break;
	 case CallK:
	    break;
         default:
	    fprintf(listing, "Default");
	    break;
      }
   }
   else { fprintf(listing, "Didn't print anything"); }
      
   
     fprintf(listing, " [line: %d]", tree->lineno);
   
   //fprintf(listing, "I am a node");
}


void printTreeRec(FILE *listing, int depth, int siblingCnt, TreeNode *tree)
{
   int childCnt;
   if (tree == NULL) return;
   if (tree!=NULL) {
      // print self
      printTreeNode(listing, tree);
      fprintf(listing, "\n");

      // print children
      for (childCnt = 0; childCnt<MAXCHILDREN; childCnt++) {
         if (tree->child[childCnt]) {
            printSpaces(listing, depth);
            fprintf(listing, "Child: %d  ", childCnt);
            printTreeRec(listing, depth+1, 1, tree->child[childCnt]);
         }
      }
   }
   // print sibling
   tree = tree->sibling;
   if (tree!=NULL) {
      if (depth) {
         printSpaces(listing, depth-1);
         fprintf(listing, "Sibling: %d  ", siblingCnt);
      }
      printTreeRec(listing, depth, siblingCnt+1, tree);
   }
   fflush(listing);
}

void printTree(FILE *listing, TreeNode *tree)
{
   if(tree == NULL){
      printf("NULL tree");
      return;
   }
   printTreeRec(listing, 1, 1, tree);
}


