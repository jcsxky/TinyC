#include <head.h>
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern DeclarationList     *decls[SIZE];


/*bool id_search(char* s,DeclarationList *dl){
	DeclarationList *p;
	for(p=dl;p!=NULL;p=p->next)
		if(!strcmp(s,p->declaration->name))
			return true;
	return false;
}*/

bool fun_search(char* s){
	/*FunctionDefinition *p;
	for(p=function_list;p!=NULL;p=p->next)
		if(!strcmp(s,p->name))
			return true;*/
	DeclarationList *dl;
	int i;
	i=hash(s);
	for(dl=decls[i];dl!=NULL;dl=dl->next)
		if(dl->declaration->type==FUNCTION)
			if(!strcmp(dl->declaration->u.fun->name,s))
				return true;
	return false;
}

bool para_search(ParameterList *para,char *s){
	if(!para)
		return false;
	ParameterList *p;
	for(p=para;p!=NULL;p=p->next)
		if(!strcmp(p->name,s))
			return true;
	return false;
}

bool expression_id_fun_search(ParameterList *parameter,Block* block,char *s){
	DeclarationList *p;
	Block* b;
	ParameterList *q;
	/*for(b=block;b!=NULL;b=b->outer_block)
		for(p=b->declaration_list;p!=NULL;p=p->next)
			if(!strcmp(s,p->declaration->name))
				return true;*/
	for(q=parameter;q!=NULL;q=q->next)
		if(!strcmp(s,q->name))
			return true;
	/*for(p=declaration_list;p!=NULL;p=p->next)
		if(!strcmp(s,p->declaration->name))
			return true;*/
	DeclarationList *dl;
	int i;
	i=hash(s);
	for(dl=decls[i];dl!=NULL;dl=dl->next)
		if(dl->declaration->type==VARIABLE){
			if(!strcmp(dl->declaration->u.var->name,s))
				return true;
		}
		else if(dl->declaration->type==FUNCTION){
			if(!strcmp(dl->declaration->u.fun->name,s))
				return true;
		}
	return false;
}

int count_para(ParameterList *parameter){
	int i=0;
	ParameterList* p;
	for(p=parameter;p!=NULL;p=p->next)
		i++;
	return i;
}

FunctionDefinition* Find_function(char* s){
	int i=hash(s);
	DeclarationList* p;
	for(p=decls[i];p!=NULL;p=p->next)
		if(p->declaration->type==FUNCTION
							&&strcmp(s,p->declaration->u.fun->name)==0)
			return p->declaration->u.fun;
	return NULL;
}


/*
void print_statement(Statement *st){
	assert(st!=NULL);
	StatementType type=st->type;
	Expression      *expression_s;
	IfStatement     *if_s;
	WhileStatement  *while_s;
	ReturnStatement *return_s;
	BreakStatement  *break_s;
	ContinueStatement   *continue_s;

	switch(type){
		case IF_STATEMENT:
		if_s=st->u.if_s;
		cout<<"if statement:"<<endl;
		cout<<"condition: "<<endl; //to be added
		print_expression(if_s->condition);
		cout<<endl;
		if(if_s->then_block){
			cout<<"if block: **************"<<endl;
			print_block(if_s->then_block);
		}
		if(if_s->else_block){
			cout<<"else block: **************"<<endl;
			print_block(if_s->else_block);
		}
		break;


		case WHILE_STATEMENT:
		while_s=st->u.while_s;
		cout<<"while statement:"<<endl;
		cout<<"condition: "<<endl; //to be added
		print_expression(while_s->condition);
		cout<<endl;
		if(while_s->block){
			cout<<"while block: **************"<<endl;
			print_block(while_s->block);
		}
		break;


		case RETURN_STATEMENT:
		return_s=st->u.return_s;
		if(return_s&&return_s->return_value){
			cout<<"return with expression ";
			print_expression(return_s->return_value);
			cout<<endl;
		}
		else
			cout<<"return without expression"<<endl;
		break;


		case BREAK_STATEMENT:
		break_s=st->u.break_s;
		//cout<<"???????????????????"<<endl;
		if(break_s)
			cout<<"break"<<endl;
		break;


		case CONTINUE_STATEMENT:
		continue_s=st->u.continue_s;
		if(continue_s)
			cout<<"continue"<<endl;
		break;


		case EXPRESSION_STATEMENT:
		expression_s=st->u.expression_s;
		print_expression(expression_s);
		cout<<endl;
		//cout<<"expression statement not implements"<<endl; //to be implemention
		break;


		default:
		cout<<"??????????????"<<type<<endl;
		cout<<"error in function print_statement"<<endl;
		exit(1);
		break;


	}
}


void print_block_statements(StatementList *stl){
	Statement *st;
	StatementList *p;
	for(p=stl;p!=NULL;p=p->next){
		st=p->statement;
		print_statement(st);
	}
}


void print_block(Block *block){
	cout<<"block type: ";
	BlockType_print(block->type);
	Block *bl=block->outer_block;
	if(bl==NULL)
		cout<<"no outer block"<<endl;
	else{
		cout<<"outer block's type: ";
		BlockType_print(bl->type);
	}
	cout<<"function local variables: ";
	if(block->declaration_list!=NULL)
		for(DeclarationList* p=block->declaration_list;p!=NULL;p=p->next){
			cout<<"name: "<<p->declaration->name<<" / type: ";
			TypeSpecifier_print(p->declaration->type);
		}
	else
		cout<<"none"<<endl;
	if(block->statement_list==NULL)
		cout<<"no block statements"<<endl;
	else{
		cout<<"block statements: "<<endl;
		print_block_statements(block->statement_list);
	}
}


void print(){
	DeclarationList *dl;
	FunctionDefinition* fd;
	ParameterList* pl;
	Block *bl;
	if(declaration_list){
		cout<<"global variables below: "<<endl;
		for(dl=declaration_list;dl!=NULL;dl=dl->next){
			Declaration* d=dl->declaration;
			cout<<"name: "<<d->name<<" / type: ";
			TypeSpecifier_print(d->type);
		}
		cout<<"****************global variables end****************"<<endl;
	}
	else
		cout<<"no global variables"<<endl;
	if(function_list){
		cout<<"all function below: "<<endl;
		for(fd=function_list;fd!=NULL;fd=fd->next){
			cout<<"return value: ";
			TypeSpecifier_print(fd->type);
			cout<<"function name: "<<fd->name<<endl;
			cout<<"function parameter: ";
			if(fd->parameter)
				for(pl=fd->parameter;pl!=NULL;pl=pl->next){
					cout<<pl->name<<" / ";
					TypeSpecifier_print(pl->type);
				}
			else
				cout<<"void"<<endl;
			bl=fd->block;
			cout<<"block information: "<<endl;
			if(bl==NULL)
				cout<<"no statement in block"<<endl;
			else
				print_block(bl);
			if(fd->next!=NULL)
				cout<<"++++++++++++++++++++++++++++++next function+++++++++++++++++++++++++++++++++++"<<endl;
		}
	}
	else
	cout<<"no function defined"<<endl;
}


void TypeSpecifier_print(TypeSpecifier type){
	switch(type){

		case INT:
		cout<<"int"<<endl;
		break;

		case DOUBLE:
		cout<<"double"<<endl;
		break;

		case FLOAT:
		cout<<"float"<<endl;
		break;

		case VOID:
		cout<<"void"<<endl;
		break;

		case BOOLEAN:
		cout<<"int"<<endl;
		break;

		default:
		cout<<"unknow"<<endl;
		cout<<"?????????????????????????"<<endl;
		exit(1);

	}
}

void BlockType_print(BlockType b){
	switch(b){
		case UNDEFINED_BLOCK:
		cout<<"undefined_block"<<endl;
		break;

		case FUNCTION_BLOCK:
		cout<<"function block"<<endl;
		break;

		case WHILE_STATEMENT_BLOCK:
		cout<<"while_statement_block"<<endl;
		break;

		case IF_STATEMENT_BLOCK:
		cout<<"if_statement_block"<<endl;
		break;

		case ELSE__STATEMENT_BLOCK:
		cout<<"else_statement_block"<<endl;
		break;

		default:
		cerr<<"????????????????????????"<<endl;
		break;

	}
}

void print_expression(Expression* exp){
	//to be implemention
	ExpressionKind kind=exp->kind;
	AssignExpression  *ass;
	BinaryExpression * be;
	ArrayExpression  *ae;
	FunctionCallExpression *fc;
	ArgumentList* al;
	switch(kind){


			case INT_EXPRESSION:
			cout<<exp->u.int_value;
			break;

			case IDENTIFIER_EXPRESSION:
			cout<<exp->u.id;
			break;

			case ASSIGN_EXPRESSION:
			ass=exp->u.assign_expression;
			cout<<ass->left;
			if(ass->is_array==10000)
				cout<<"["<<ass->index<<"]";
			cout<<"=";
			print_expression(ass->right);
			break;

			case ADD_EXPRESSION:
			case SUB_EXPRESSION:
			case MUL_EXPRESSION:
			case DIV_EXPRESSION:
			case EQ_EXPRESSION:
			case NE_EXPRESSION:
			case GT_EXPRESSION:
			case GE_EXPRESSION:
			case LT_EXPRESSION:
			case LE_EXPRESSION:
			case LOGICAL_AND_EXPRESSION:
			case LOGICAL_OR_EXPRESSION:

			be=exp->u.binary_expression;
			print_expression(be->left);

			if(kind==ADD_EXPRESSION)
				cout<<"+";
			else if(kind==SUB_EXPRESSION)
				cout<<"-";
			else if(kind==MUL_EXPRESSION)
				cout<<"*";
			else if(kind==DIV_EXPRESSION)
				cout<<"/";
			else if(kind==EQ_EXPRESSION)
				cout<<"==";
			else if(kind==NE_EXPRESSION)
				cout<<"!=";
			else if(kind==GT_EXPRESSION)
				cout<<">";
			else if(kind==GE_EXPRESSION)
				cout<<">=";
			else if(kind==LT_EXPRESSION)
				cout<<"<";
			else if(kind==LE_EXPRESSION)
				cout<<"<=";
			else if(kind==LOGICAL_AND_EXPRESSION)
				cout<<"&&";
			else if(kind==LOGICAL_OR_EXPRESSION)
				cout<<"||";
			print_expression(be->right);
			break;


			case LOGICAL_NOT_EXPRESSION:
			be=exp->u.binary_expression;
			cout<<"!";
			print_expression(be->left);
			break;


			case ARRAY_EXPRESSION:
			ae=exp->u.array_expression;
			cout<<ae->array_name<<"["<<ae->index<<"]";
			break;

			case FUNCTION_CALL_EXPRESSION:
			fc=exp->u.function_call_expression;
			cout<<fc->function_name<<"(";
			if(fc->argument==NULL)
				cout<<")";
			else{
				for(al=fc->argument;al!=NULL;al=al->next){
					print_expression(al->expression);
					if(al->next!=NULL)
						cout<<",";
				}//print argument
				cout<<")";
			}
			//cout<<"function call expression has not implements"<<endl;
			break;


			case PARENTHESES_EXPRESSION:
			cout<<"(";
			be=exp->u.binary_expression;
			print_expression(be->left);
			cout<<")";
			break;

			default:
			cerr<<"?????????????????????????"<<endl;
			exit(1);

		}
}
*/

