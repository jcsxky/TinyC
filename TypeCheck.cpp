#include <head.h>
extern vector<CToken> InputSequence;
extern int next;
extern CToken Token;
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern DeclarationList     *decls[SIZE];
extern Block *exe_block;



void checkType(){
	int i;
	ParameterList* pl;
	Block * blk,*b;
	for(i=0;i<SIZE;i++){
		DeclarationList* p;
		p=decls[i];
		while(p!=NULL){
				if(p->declaration->type==VARIABLE){
					struct variable* v=p->declaration->u.var;
					assert(p->declaration->type!=VOID);
				}

			else if(p->declaration->type==FUNCTION){
				FunctionDefinition* fd=p->declaration->u.fun;
				for(pl=fd->parameter;pl!=NULL;pl=pl->next)
					assert(pl->type!=VOID);
				blk=fd->block;
				check_block(fd,blk);
			}
			else{
				cerr<<"declaration TYPE error"<<endl;
				exit(1);
			}
			p=p->next;
		}
	}
	check_block(NULL,exe_block);
}


void check_block(FunctionDefinition* fd,Block* blk){
	if(blk==NULL)
		return ;
	assert(blk->type==FUNCTION_BLOCK||blk->type==WHILE_STATEMENT_BLOCK||
		  	 blk->type==IF_STATEMENT_BLOCK||blk->type==ELSE__STATEMENT_BLOCK||
					blk->type==EXE_BLOCK);
	if(blk->type==FUNCTION_BLOCK)
		assert(blk->outer_block==NULL);
	StatementList* sl;
	struct variable *v;
	ReturnStatement* rs;
	Block *bk;
	IfStatement     *if_s;
	WhileStatement  *while_s;

	for(sl=blk->statement_list;sl!=NULL;sl=sl->next){
		Statement *s=sl->statement;

		switch(s->type){
			case DECL_STATEMENT:
			v=s->u.decl_s;
			assert(v->type!=VOID);
			break;

			case RETURN_STATEMENT:
			rs=s->u.return_s;
			if(rs->return_value==NULL)
			assert(fd->type==VOID);
			else{
			assert(fd->type!=VOID);
			checkExp(fd,rs->return_value);
			}
			break;


			case BREAK_STATEMENT:
			bk=s->u.break_s->break_block;
			while(bk!=NULL){
			if(bk->type==WHILE_STATEMENT_BLOCK)
			break;
			bk=bk->outer_block;
			}
			assert(bk!=NULL);
			break;


			case CONTINUE_STATEMENT:
			bk=s->u.continue_s->continue_block;
			while(bk!=NULL){
			if(bk->type==WHILE_STATEMENT_BLOCK)
			break;
			bk=bk->outer_block;
			}
			assert(bk!=NULL);
			break;


			case IF_STATEMENT:
			if_s=s->u.if_s;
			checkExp(fd,if_s->condition);
			check_block(fd,if_s->then_block);
			if(if_s->else_block!=NULL)
			check_block(fd,if_s->else_block);
			break;


			case WHILE_STATEMENT:
			while_s=s->u.while_s;
			checkExp(fd,while_s->condition);
			check_block(fd,while_s->block);
			break;


			case EXPRESSION_STATEMENT:
			checkExp(fd,s->u.expression_s);
			break;


			default:
			cerr<<"unknow type of statement"<<endl;
			break;
		}
	}
}


void checkExp(FunctionDefinition* fd,Expression *exp){
	FunctionCallExpression *fc;
	FunctionDefinition* f,*fun;
	int i,n,num;
	ArgumentList* al;
	if(exp==NULL)
		return ;
	switch(exp->kind){
		case PARENTHESES_EXPRESSION:
		checkExp(fd,exp->u.binary_expression->left);
		break;

		case INT_EXPRESSION:
		case IDENTIFIER_EXPRESSION:
		break;

		case FUNCTION_CALL_EXPRESSION:
		f=NULL;
		fc=exp->u.function_call_expression;
		f=Find_function(fc->function_name);
		assert(f!=NULL);
		n=0;
		for(al=fc->argument;al!=NULL;al=al->next){
			n++;
			if(al->expression->kind==FUNCTION_CALL_EXPRESSION){
				fun=Find_function(al->expression->u.function_call_expression->function_name);
				assert(fun->type!=VOID);
			}
			checkExp(fd,al->expression);
		}
		if(f->parameter==NULL)
			assert(n==0);
		else
			assert(n==f->parameter->parame_num);
		break;


		case BUILT_IN_FUNCTION:
		n=0;
		fc=exp->u.function_call_expression;
		for(al=fc->argument;al!=NULL;al=al->next){
			n++;
			if(!strcmp(fc->function_name,"input"))
				assert(al->expression->kind==IDENTIFIER_EXPRESSION);
			if(al->expression->kind==FUNCTION_CALL_EXPRESSION){
				fun=Find_function(al->expression->u.function_call_expression->function_name);
				assert(fun->type!=VOID);
			}
			checkExp(fd,al->expression);
		}
		if(strcmp(fc->function_name,"sqrt")==0||strcmp(fc->function_name,"log")==0)
			assert(n==1);
		break;


		case LOGICAL_NOT_EXPRESSION:
		case NEGATIVE_EXPRESSION:
		if(exp->u.binary_expression->left->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.binary_expression->
													left->u.function_call_expression->function_name)->type!=VOID);
		checkExp(fd,exp->u.binary_expression->left);
		break;

		case EQ_EXPRESSION:
		case NE_EXPRESSION:
		case GT_EXPRESSION:
		case GE_EXPRESSION:
		case LT_EXPRESSION:
		case LE_EXPRESSION:
		case LOGICAL_AND_EXPRESSION:
		case LOGICAL_OR_EXPRESSION:
		case ADD_EXPRESSION:
		case SUB_EXPRESSION:
		case MUL_EXPRESSION:
		case DIV_EXPRESSION:
		if(exp->u.binary_expression->left->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.binary_expression->
													left->u.function_call_expression->function_name)->type!=VOID);
		checkExp(fd,exp->u.binary_expression->left);
		if(exp->u.binary_expression->right->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.binary_expression->
													left->u.function_call_expression->function_name)->type!=VOID);
		checkExp(fd,exp->u.binary_expression->right);
		break;


		case MOD_EXPRESSION:
		if(exp->u.binary_expression->left->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.binary_expression->
													left->u.function_call_expression->function_name)->type==INT);
		checkExp(fd,exp->u.binary_expression->left);
		if(exp->u.binary_expression->right->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.binary_expression->
													left->u.function_call_expression->function_name)->type==INT);
		checkExp(fd,exp->u.binary_expression->right);
		break;


		case ASSIGN_EXPRESSION:
		assert(exp->u.assign_expression->left->kind==IDENTIFIER_EXPRESSION||
		exp->u.assign_expression->left->kind==ASSIGN_EXPRESSION);
		if(exp->u.assign_expression->right->kind==FUNCTION_CALL_EXPRESSION)
			assert(Find_function(exp->u.assign_expression->
													right->u.function_call_expression->function_name)->type!=VOID);
		checkExp(fd,exp->u.binary_expression->right);
		break;

		default:
		cerr<<"unknow type of Expression"<<endl;
		break;


	}
}





