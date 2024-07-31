#include <iostream>
#include "semantics.h"
#include "symbolTable.h"
#include "treeUtils.h"
#include <string.h>
#include "parser.tab.h"

static int goffset = 0;
static int foffset = 0;
static int varCounter = 0;
static bool newScope = true;

static bool arrayCheck = false;

extern int numErrors;

void reportError(int lineno, const char *msg) {
   printf("SEMANTIC ERROR(%d): %s\n", lineno, msg);
   numErrors++;
}

void reportErrorArr(int lineno, const char *msg, const char *type) {
   printf("SEMANTIC ERROR(%d): Array %s should be indexed by type %s\n", lineno, msg, type);
   numErrors++;
}

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

//static int num_params;
static TreeNode *funcNode;
//static TreeNode *funcParams;

void treeTraverseStmt(TreeNode *syntree, SymbolTable *symTabX) {
   if (syntree == NULL) { return; }
   if (syntree->kind.stmt != CompoundK) {
      newScope = true;
   }
   switch (syntree->kind.stmt) {
      case ReturnK:
	if (syntree->child[0]) {
	   if (syntree->child[0]->kind.exp == IdK) {
	      TreeNode *node = (TreeNode*)symTabX->lookup(syntree->child[0]->attr.name);
	      if (node != NULL) {
	         if (node->isArray) {
	            reportError(syntree->lineno, "Cannot return an array.");
	            node = NULL;
	         }
	      } else if (strcmp(funcNode->attr.name, "main") == 0) {
	         printf("SEMANTIC ERROR(%d): Symbol '%s' is not declared.\n", syntree->lineno, syntree->child[0]->attr.name);
	         numErrors++;
	      }
	   }
	}
	if (strcmp(funcNode->attr.name, "main") != 0) {
	   if (funcNode->type != Void && syntree->child[0] == NULL) {
	      printf("SEMANTIC ERROR(%d): Function '%s' at line %d is expecting to return %s but return has no value.\n", syntree->lineno, funcNode->attr.name, funcNode->lineno, expTypeToStr(funcNode->type, funcNode->isArray, funcNode->isStatic));
	      numErrors++;
	   }
	   if (funcNode->type == Void && syntree->child[0] != NULL) {
	      printf("SEMANTIC ERROR(%d): Function '%s' at line %d is expecting no return value, but return has a value.\n", syntree->lineno, funcNode->attr.name, funcNode->lineno);
	      numErrors++;
	   }
	   if (funcNode->type == Char && syntree->child[0] != NULL && syntree->child[0]->type == Integer) {
	      printf("SEMANTIC ERROR(%d): Function '%s' at line %d is expecting to return %s but returns %s.\n", syntree->lineno, funcNode->attr.name, funcNode->lineno, expTypeToStr(funcNode->type, funcNode->isArray, funcNode->isStatic), expTypeToStr(syntree->child[0]->type, syntree->child[0]->isArray, syntree->child[0]->isStatic));
	      numErrors++;
	   }
	}
	break;
      case BreakK:
        if (newScope) { reportError(syntree->lineno, "Cannot have a break statement outside of loop."); /* this is probably wrong */ }
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
      case WhileK:
	//printf("In While\n");
	treeTraverse(syntree->child[0], symTabX);
        if (syntree->child[0]->type != Boolean) {
	   if (syntree->child[0]->type == Integer) {
	      reportError(syntree->lineno, "Expecting Boolean test condition in while statement but got type int.");
	   } else if (syntree->child[0]->type == Char) {
	      reportError(syntree->lineno, "Expecting Boolean test condition in while statement but got type char.");
	      if (syntree->child[0]->isArray) { reportError(syntree->lineno, "Cannot use array as test condition in while statement.");}
	   } else if (syntree->child[0]->isArray) {
	      reportError(syntree->lineno, "Cannot use array as test condition in while statement.");
	   } else {
	      reportError(syntree->lineno, "Something else we're missing in WhileK");
	   }
	}
        treeTraverse(syntree->child[1], symTabX);
        break;
      case IfK:
	treeTraverse(syntree->child[0], symTabX);
	if (syntree->child[0]->type != Boolean) {
	   if (syntree->child[0]->type == Integer) {
	      reportError(syntree->lineno, "Expecting Boolean test condition in if statement but got type int.");
	   }
	   else if (syntree->child[0]->type == Char) {
	      reportError(syntree->lineno, "Expecting Boolean test condition in if statement but got type char.");
	      if (syntree->child[0]->isArray) { reportError(syntree->lineno, "Cannot use array as test condition in if statement.");  }
	   }
	   else if (syntree->child[0]->isArray) {
	      reportError(syntree->lineno, "Cannot use array as test condition in if statement.");
	   }
	   else {
	      printf("There's something else wrong\n");
	   }
	}
        treeTraverse(syntree->child[1], symTabX);
        treeTraverse(syntree->child[2], symTabX);
	break; 
      default:
	//printf("We're in default here\n");
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
	if (tmp = (TreeNode *)(symTabX->lookup(syntree->attr.name))) {
	   if (tmp->kind.decl == FuncK) {
	      //printf("%s is FuncK\n", tmp->attr.name);
	      printf("SEMANTIC ERROR(%d): Cannot use function '%s' as a variable.\n", syntree->lineno, syntree->attr.name);
	      numErrors++;
	   }
	}
	//printf("We're in IdK\n");
	//printf("Lineno: %d\n", syntree->lineno);
	break;
      case AssignK:
	 treeTraverse(syntree->child[0], symTabX);
	 treeTraverse(syntree->child[1], symTabX);
	 syntree->type = Integer;
         // this is for the arr int error message:
	 // We need to make sure the child of the child exists and that it's an array or we mess up the switch operations below
	 /*if (syntree->child[0]->child[0] != NULL && syntree->child[0]->child[0]->isArray) {
	    TreeNode *indexNode = syntree->child[0]->child[1];
	    TreeNode *arrNode = syntree->child[0]->child[0];
            if (indexNode->type != Integer) {
	       if (indexNode->type == Char) {
	          printf("SEMANTIC ERROR(%d): Array '%s' should be indexed by type int but got type char.\n", syntree->lineno, arrNode->attr.name);
	          numErrors++;
	       }
	       if (indexNode->type == Boolean) {
	          printf("SEMANTIC ERROR(%d): Array '%s' should be indexed by type int but got type bool.\n", syntree->lineno, arrNode->attr.name);
	          numErrors++;
	       }
	    }
	    if (indexNode->isArray) {
	       printf("SEMANTIC ERROR(%d): Array index is the unindexed array '%s'.\n", syntree->lineno, indexNode->attr.name);
	       numErrors++;
	    }*/
	    
	 
	 //printf("lineno: %d syntree->attr.name: %s\n", syntree->lineno, syntree->attr.name);
         switch (syntree->attr.op) {
	    case SUBASS:
	       if (syntree->child[0]->type != Integer) {
                  if (syntree->child[0]->type == Char) {
                     reportError(syntree->lineno, "'-=' requires operands of type int but lhs is of type char.");
                  }
                  if (syntree->child[0]->type == Boolean) {
                     reportError(syntree->lineno, "'-=' requires operands of type int but lhs is of type bool.");
                  }
               }
               if (syntree->child[1]->type != Integer) {
                  if (syntree->child[1]->type == Char) {
                     reportError(syntree->lineno, "'-=' requires operands of type int but rhs is of type char.");
                  }
                  if (syntree->child[1]->type == Boolean) {
                     reportError(syntree->lineno, "'-=' requires operands of type int but rhs is of type bool.");
                  }
               }
               if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                  reportError(syntree->lineno, "The operation '-=' does not work with arrays.");
               }
               break;
	    case ADDASS:
	       if (syntree->child[0]->type != Integer) {
                  if (syntree->child[0]->type == Char) {
                     reportError(syntree->lineno, "'+=' requires operands of type int but lhs is of type char.");
                  }
                  if (syntree->child[0]->type == Boolean) {
                     reportError(syntree->lineno, "'+=' requires operands of type int but lhs is of type bool.");
                  }
               }
               if (syntree->child[1]->type != Integer) {
                  if (syntree->child[1]->type == Char) {
                     reportError(syntree->lineno, "'+=' requires operands of type int but rhs is of type char.");
                  }
                  if (syntree->child[1]->type == Boolean) {
                     reportError(syntree->lineno, "'+=' requires operands of type int but rhs is of type bool.");
                  }
               }
               if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                  reportError(syntree->lineno, "The operation '+=' does not work with arrays.");
               }
               break;
	    case MULASS:
	       if (syntree->child[0]->type != Integer) {
                  if (syntree->child[0]->type == Char) {
                     reportError(syntree->lineno, "'*=' requires operands of type int but lhs is of type char.");
                  }
                  if (syntree->child[0]->type == Boolean) {
                     reportError(syntree->lineno, "'*=' requires operands of type int but lhs is of type bool.");
                  }
               }
               if (syntree->child[1]->type != Integer) {
                  if (syntree->child[1]->type == Char) {
                     reportError(syntree->lineno, "'*=' requires operands of type int but rhs is of type char.");
                  }
                  if (syntree->child[1]->type == Boolean) {
                     reportError(syntree->lineno, "'*=' requires operands of type int but rhs is of type bool.");
                  }
               }
               if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                  reportError(syntree->lineno, "The operation '*=' does not work with arrays.");
               }
               break;
	    case DIVASS:
	       if (syntree->child[0]->type != Integer) {
                  if (syntree->child[0]->type == Char) {
                     reportError(syntree->lineno, "'/=' requires operands of type int but lhs is of type char.");
                  }
                  if (syntree->child[0]->type == Boolean) {
                     reportError(syntree->lineno, "'/=' requires operands of type int but lhs is of type bool.");
                  }
               }
               if (syntree->child[1]->type != Integer) {
                  if (syntree->child[1]->type == Char) {
                     reportError(syntree->lineno, "'/=' requires operands of type int but rhs is of type char.");
                  }
                  if (syntree->child[1]->type == Boolean) {
                     reportError(syntree->lineno, "'/=' requires operands of type int but rhs is of type bool.");
                  }
               }
               if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                  reportError(syntree->lineno, "The operation '/=' does not work with arrays.");
               }
               break;
	    case '=':
	       //printf("in '='\n");
	       if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;
	    case '[':
	       //printf("%s\n", expTypeToStr(syntree->child[0]->type, syntree->isArray, syntree->isStatic));
	       syntree->type = syntree->child[0]->type;
	       break;
	    case EQ:
	    case DEC:
	       //printf("Here\n");
	       break;
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
	   case DEC:
	   case INC:
	      printf("We're in DEC or INC");
	      break;
	   case '=':
	      //printf("We're here\n");
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) { 
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) { 
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) { 
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) { 
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {                  
                    reportError(syntree->lineno, "'=' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;

	   case '[':
	      //printf("syntree->child[0]->attr.name: %s\n", syntree->child[0]->attr.name);
	      /*if (arrayCheck == true) {
	      if (!syntree->child[0]->isArray && arrayCheck) { 
		 printf("We got an issue\n");
		 printf("SEMANTIC ERROR(%d): Cannot index nonarray '%s'.\n", syntree->lineno, syntree->child[0]->attr.name);
		 numErrors++;
	      } else if (syntree->child[0]->child[0] == NULL && syntree->child[1]->type != Integer) {
		 if (syntree->child[1]->type == Char) {
		    printf("We're Char condition\n");
		    printf("SEMANTIC ERROR(%d): Array '%s' should be indexed by type int but got type char.\n", syntree->lineno, syntree->child[0]->attr.name);
		    numErrors++;
		 } else if (syntree->child[1]->type == Boolean) {
		    printf("SEMANTIC ERROR(%d): Array '%s' should be indexed by type int but got type bool.\n", syntree->lineno, syntree->child[0]->attr.name);
		    numErrors++;
		 }
	      
	      }
	      if (syntree->child[1]->isArray) {
	         printf("SEMANTIC ERROR(%d): Array index is the unindexed array '%s'.\n", syntree->lineno, syntree->child[1]->attr.name);
		 numErrors++;
	      } 
	      */
	      syntree->type = syntree->child[0]->type;
	    
	      //printf("syntree->child[1]->attr.name: %s\n", syntree->child[1]->attr.name);
	      break;
	   case MIN:
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "':<:' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "':<:' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "':<:' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "':<:' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation ':<:' does not work with arrays.");
              }
	      break;
	   case MAX:
	      if (syntree->child[0]->type != Integer) {
	         if (syntree->child[0]->type == Char) {
		    reportError(syntree->lineno, "':>:' requires operands of type int but lhs is of type char.");
		 }
		 if (syntree->child[0]->type == Boolean) {
		    reportError(syntree->lineno, "':>:' requires operands of type int but lhs is of type bool.");
		 }
	      }
	      if (syntree->child[1]->type != Integer) {
	         if (syntree->child[1]->type == Char) {
		    reportError(syntree->lineno, "':>:' requires operands of type int but rhs is of type char.");
		 }
		 if (syntree->child[1]->type == Boolean) {
		    reportError(syntree->lineno, "':>:' requires operands of type int but rhs is of type bool.");
		 }
	      }
	      if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation ':>:' does not work with arrays.");
              }
	      break;
	   case OR:
	      if (syntree->child[0]->type != Boolean) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'or' requires operands of type bool but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Integer) {
                    reportError(syntree->lineno, "'or' requires operands of type bool but lhs is of type int.");
                 }
              }
              if (syntree->child[1]->type != Boolean) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'or' requires operands of type bool but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'or' requires operands of type bool but rhs is of type int.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation 'or' does not work with arrays.");
              }
              break;
	   case AND:
	     if (syntree->child[0]->type != Boolean) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'and' requires operands of type bool but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Integer) {
                    reportError(syntree->lineno, "'and' requires operands of type bool but lhs is of type int.");
                 }
              }
              if (syntree->child[1]->type != Boolean) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'and' requires operands of type bool but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'and' requires operands of type bool but rhs is of type int.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation 'and' does not work with arrays.");
              }
              break; 
	   case NOT:
	   case EQ:
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'==' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;
	   case NEQ:
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'!=' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;
	   case LEQ:
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'<=' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;
	   case '<':
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {                 
                    reportError(syntree->lineno, "'<' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
	      break;
	   case GEQ:
	      if (syntree->child[0]->type != syntree->child[1]->type) {
                 if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type int and rhs is type char.");
                 }
                 else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type int and rhs is type bool.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type char and rhs is type int.");
                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type bool and rhs is type int.");
                 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type char and rhs is type bool.");

                 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'>=' requires operands of the same type but lhs is type bool and rhs is type char.");
                 }
              }
              break;
	   case '>':
	      //printf("syntree->child[0]->attr.name: %s\n", syntree->child[0]->attr.name);
	      //printf("syntree->child[1]->attr.name: %s\n", syntree->child[1]->attr.name);
	      if (syntree->child[0]->type != syntree->child[1]->type) {
	         if (syntree->child[0]->type == Integer && syntree->child[1]->type == Char) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type int and rhs is type char.");
		 }
	         else if (syntree->child[0]->type == Integer && syntree->child[1]->type == Boolean) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type int and rhs is type bool.");
		 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Integer) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type char and rhs is type int.");
		 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Integer) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type bool and rhs is type int.");
		 } else if (syntree->child[0]->type == Char && syntree->child[1]->type == Boolean) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type char and rhs is type bool.");

		 } else if (syntree->child[0]->type == Boolean && syntree->child[1]->type == Char) {
		    reportError(syntree->lineno, "'>' requires operands of the same type but lhs is type bool and rhs is type char.");
		 }
	      }
	      syntree->type = Boolean;
	      break;
	   case '+':
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'+' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "'+' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'+' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'+' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation '+' does not work with arrays.");
              }
	      break;
	   case '-':
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'-' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "'-' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'-' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'-' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation '-' does not work with arrays.");
              }
              break;
	   case '*':
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'*' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "'*' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'*' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'*' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation '*' does not work with arrays.");
              }
              break;
	   case '/':
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'/' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "'/' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'/' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'/' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation '/' does not work with arrays.");
              }
              break;
	   case '%':
	      if (syntree->child[0]->type != Integer) {
                 if (syntree->child[0]->type == Char) {
                    reportError(syntree->lineno, "'%' requires operands of type int but lhs is of type char.");
                 }
                 if (syntree->child[0]->type == Boolean) {
                    reportError(syntree->lineno, "'%' requires operands of type int but lhs is of type bool.");
                 }
              }
              if (syntree->child[1]->type != Integer) {
                 if (syntree->child[1]->type == Char) {
                    reportError(syntree->lineno, "'%' requires operands of type int but rhs is of type char.");
                 }
                 if (syntree->child[1]->type == Boolean) {
                    reportError(syntree->lineno, "'%' requires operands of type int but rhs is of type bool.");
                 }
              }
              if (syntree->child[0]->isArray || syntree->child[1]->isArray) {
                 reportError(syntree->lineno, "The operation '%' does not work with arrays.");
              }
              break;
	   default:
	      //printf("In the default section here\n");
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
	//printf("We'll be in here and funcNode->attr.name: %s\n", funcNode->attr.name);
	//printf("syntree->attr.name: %s\n", syntree->attr.name);
	//TreeNode *lookupParams = (TreeNode *)symTabX->lookup(syntree->attr.name);

	// First param check
	TreeNode *lookupParams;
	if (lookupParams = (TreeNode *)symTabX->lookup(syntree->attr.name)) {
	   lookupParams = lookupParams->child[0];
	   //printf("lookupParams->attr.name: %s\n", lookupParams->attr.name);
	   TreeNode *tmp = syntree->child[0];
	   int total_params = 0;
	   int call_params = 0;
	   int lookuplineno = lookupParams->lineno;
	   //printf("BEFORE WHILE: total_params: %d call_params: %d\n", total_params, call_params);
	   while (lookupParams != NULL) {
	      //printf("%s ", lookupParams->attr.name);
	      total_params++;
	      lookupParams = lookupParams->sibling;
	   }
	   //printf("\n");
	   while (tmp != NULL) {
	      call_params++;
	      tmp = tmp->sibling;
	   }
	   //printf("total_params: %d call_params: %d\n", total_params, call_params);
	   if (total_params != call_params) {
	      if (total_params < call_params) {
	         printf("SEMANTIC ERROR(%d): Too many parameters passed for function '%s' declared on line %d.\n", syntree->lineno, syntree->attr.name, lookuplineno);
	      }
	      else {
	         printf("SEMANTIC ERROR(%d): Too few parameters passed for function '%s' declared on line %d.\n", syntree->lineno, syntree->attr.name, lookuplineno);
	      }
	      numErrors++;
	   }
	   else {
	      tmp = syntree->child[0];
	      lookupParams = (TreeNode *)symTabX->lookup(syntree->attr.name);
	      TreeNode *funcName = lookupParams;
	      lookupParams = lookupParams->child[0];
	      //printf("We're here before for\n");
	      //printf("lookupParams->type: %s, tmp->type: %s\n", expTypeToStr(lookupParams->type, lookupParams->isArray, lookupParams->isStatic), expTypeToStr(tmp->type, tmp->isArray, tmp->isStatic));
	      for (int i = 1; i <= total_params; i++) {
	         if (tmp->type != lookupParams->type) {
	            //printf("In if\n");
		    //printf("%s \n",expTypeToStr(lookupParams->type, lookupParams->isArray, lookupParams->isStatic));
		    //printf("%s \n",expTypeToStr(tmp->type, tmp->isArray, tmp->isStatic));
		    printf("SEMANTIC ERROR(%d): Expecting %s in parameter %d of call to '%s' declared on line %d but got %s.\n", syntree->lineno, expTypeToStr(lookupParams->type, lookupParams->isArray, lookupParams->isStatic), i, funcName->attr.name, lookuplineno, expTypeToStr(tmp->type, 0, 0));
		    numErrors++;
	         }
		 else if (tmp->isArray != lookupParams->isArray) {
		    if (tmp->isArray == true && lookupParams->isArray == false) {
		       printf("SEMANTIC ERROR(%d): Not expecting array in parameter %d of call to '%s' declared on line %d.\n", syntree->lineno, i, funcName->attr.name, lookuplineno); 
                       numErrors++;
		    } else {
		       printf("ERROR IS HERE\n");
		    }
		    
		 }
	         tmp = tmp->sibling;
		 lookupParams = lookupParams->sibling;
	      }
	   }
	   lookupParams = NULL;
	}
	// Second param check
	/*if (lookupParams = (TreeNode *)symTabX->lookup(syntree->attr.name)) {
	   lookupParams = lookupParams->child[0];
	   TreeNode *tmp = syntree->child[0];
	   int lookuplineno = lookupParams->lineno;
	   
	}*/
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
		else {
		    TreeNode *paramCheck = (TreeNode*)symTabX->lookup(syntree->attr.name);
	            if (paramCheck != NULL) {
		       printf("SEMANTIC ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntree->lineno, syntree->attr.name, paramCheck->lineno);
	               numErrors++;
		    }
	       }
	     
	     if (syntree->kind.decl == ParamK) {
	        syntree->varKind = Parameter;
	     } else if (syntree->isArray) {
	        syntree->offset--;
	     }
	     //printf("funcParams->attr.name: %s\n", funcParams->attr.name);
	     break;
	   case FuncK:
	       TreeNode *funcExists = (TreeNode*)symTabX->lookup(syntree->attr.name);
	       if (funcExists != NULL) {
	          printf("SEMANTIC ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntree->lineno, funcExists->attr.name, funcExists->lineno);
		  numErrors++;
	       } else {
	          foffset = -2;
	          InsertError(syntree, symTabX);
	          symTabX->enter(syntree->attr.name);
	          newScope = false;
	          /* funcParams = syntree; // inserted for checking params
		  if ((funcParams->child[0]) && !((strcmp(funcParams->attr.name, "output") == 0) || (strcmp(funcParams->attr.name, "outputb") == 0) || (strcmp(funcParams->attr.name, "outputc") == 0))) {
		     num_params = 0;
		     TreeNode *tmp = funcParams->child[0];
		     while (tmp != NULL) {
		        printf("Func is: %s\n", funcParams->attr.name);
		        printf("Func Param: %s\n", tmp->attr.name);
			num_params++;
			printf("Num of params: %d\n", num_params);
			tmp = tmp->sibling;
		     }
		  }*/
		  // end of checking params
	          funcNode = syntree; // inserted for ReturnK error messages
	          //printf("funcNode->attr.name: %s funcNode->type: %s\n", funcNode->attr.name, expTypeToStr(funcNode->type, funcNode->isArray, funcNode->isStatic));
	          treeTraverse(syntree->child[0], symTabX);
	          syntree->size = foffset;
	          treeTraverse(syntree->child[1], symTabX);
	          syntree->varKind = Global;
	          symTabX->leave();
	          funcNode = syntree;
                  break;
	       }
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
   //TreeNode *tmp = (TreeNode *)(symtabX->lookup("main"));
   if (symtabX->lookup("main") == NULL) {
      printf("LINKER ERROR: A function named 'main' with no parameters must be defined.\n");
      return NULL; // since we have an error, we need to return null.
   }
   globalOffset = goffset;
   return syntree;
}

