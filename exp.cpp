#include <head.h>
extern vector<CToken> InputSequence;
extern int next;
extern CToken Token;
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern DeclarationList     *decls[SIZE];


/* in assign expression which the left value is an array type, implement             */



/* is ugly , to be implement better. Also, the index is only id, expression avoid   */


Expression* expression(ParameterList *parameter,Block* block){
	Expression* exp=boolbool(parameter,block);
	while(Token.type==ASSIGN_T){
		/*if(!expression_id_fun_search(parameter,block,exp->u.id)){
		cerr<<"variable "<<exp->u.id<<" undefined"<<endl;
		exit(1);
		}*/
		//cout<<exp->kind<<endl;
		if(exp->kind!=IDENTIFIER_EXPRESSION&&exp->kind!=ASSIGN_EXPRESSION){
			//cout<<"**"<<exp->kind<<endl;
			cerr<<"assign object not l-value"<<endl;
			exit(1);
		}
		match(ASSIGN_T);
		AssignExpression * assign=new AssignExpression;
		assign->left=exp;
		exp=new Expression;
		exp->kind=ASSIGN_EXPRESSION;
		assign->right=expression(parameter,block);
		if(assign->right->kind==FUNCTION_CALL_EXPRESSION){
			if((Find_function(assign->right->u.function_call_expression->function_name))
							->type==VOID)
					cerr<<assign->right->u.function_call_expression->function_name<<endl;
			assert((Find_function(assign->right->u.function_call_expression->function_name))
							->type!=VOID);
		}
		exp->u.assign_expression=assign;
		//cout<<"====="<<exp->kind<<endl;
	}
	return exp;
}

/*
char *t=Token.word,*s;
if(Token.type==ID){
match(ID);
int array=0;
if(Token.type==LB){
match(LB);
if(!expression_id_fun_search(parameter,block,Token.word)){
cerr<<"variable "<<Token.word<<" undefined"<<endl;
exit(1);
}
s=Token.word;
match(ID);
match(RB);
array=1;
}
if(Token.type==ASSIGN_T){
if(!expression_id_fun_search(parameter,block,t)){
cerr<<"variable "<<t<<" undefined"<<endl;
exit(1);
}
match(ASSIGN_T);
Expression* exp=new Expression;
AssignExpression * assign=new AssignExpression;
assign->left=t;
assign->right=expression(parameter,block);
if(array){
assign->index=s;
assign->is_array=10000;
}
//cout<<assign->right->u.id<<"----------------"<<endl;
exp->kind=ASSIGN_EXPRESSION;
exp->u.assign_expression=assign;
return exp;
}
else{
if(array==0)
unGetToken();
else{
for(int i=0;i<4;i++)
unGetToken();
}
//cout<<Token.word<<endl;
return boolbool(parameter,block);
}
}
else
return boolbool(parameter,block);
}
*/

Expression* boolbool(ParameterList *parameter,Block* block){
	Expression* exp=join(parameter,block);
	while(Token.type==OR){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		exp->kind=LOGICAL_OR_EXPRESSION;
		match(OR);
		be->right=join(parameter,block);
		exp->u.binary_expression=be;
	}
	return exp;
}


Expression* join(ParameterList *parameter,Block* block){
	Expression* exp=equality(parameter,block);
	while(Token.type==AND){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		exp->kind=LOGICAL_AND_EXPRESSION;
		match(AND);
		be->right=equality(parameter,block);
		exp->u.binary_expression=be;
	}
	return exp;
}


Expression* equality(ParameterList *parameter,Block* block){
	Expression* exp=simple_expression(parameter,block);
	while(Token.type==NE||Token.type==EQ){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		exp->u.binary_expression=be;
		if(Token.type==NE){
			match(NE);
			exp->kind=NE_EXPRESSION;
		}
		else if(Token.type==EQ){
			match(EQ);
			exp->kind=EQ_EXPRESSION;
		}
		be->right=simple_expression(parameter,block);
	}
	return exp;
}

Expression* simple_expression(ParameterList *parameter,Block* block){
	Expression* exp=additive_expression(parameter,block);
	while(Token.type==GE||Token.type==GT||Token.type==LE||Token.type==LT){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		if(Token.type==GE){
			match(GE);
			exp->kind=GE_EXPRESSION;
		}
		else if(Token.type==GT){
			match(GT);
			exp->kind=GT_EXPRESSION;
		}
		else if(Token.type==LE){
			match(LE);
			exp->kind=LE_EXPRESSION;
		}
		else if(Token.type==LT){
			match(LT);
			exp->kind=LT_EXPRESSION;
		}
		be->right=additive_expression(parameter,block);
		exp->u.binary_expression=be;
	}
	return exp;
}


Expression* additive_expression(ParameterList *parameter,Block* block){
	Expression* exp=term(parameter,block);
	while(Token.type==ADD||Token.type==SUB){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		if(Token.type==ADD){
			match(ADD);
			exp->kind=ADD_EXPRESSION;
		}
		else if(Token.type==SUB){
			match(SUB);
			exp->kind=SUB_EXPRESSION;
		}
		be->right=term(parameter,block);
		exp->u.binary_expression=be;
	}
	return exp;
}


Expression* term(ParameterList *parameter,Block* block){
	Expression* exp=unary(parameter,block);
	while(Token.type==MUL||Token.type==DIV||Token.type==MOD){
		BinaryExpression * be=new BinaryExpression;
		be->left=exp;
		exp=new Expression;
		if(Token.type==MUL){
			match(MUL);
			exp->kind=MUL_EXPRESSION;
		}
		else if(Token.type==DIV){
			match(DIV);
			exp->kind=DIV_EXPRESSION;
		}
		else if(Token.type==MOD){
		//cout<<Token.word<<endl;
		match(MOD);
		exp->kind=MOD_EXPRESSION;
		}
		be->right=unary(parameter,block);
		exp->u.binary_expression=be;
	}
	return exp;
}


Expression* unary(ParameterList *parameter,Block* block){
	Expression* exp;
	if(Token.type==NOT){
		match(NOT);
		BinaryExpression * be=new BinaryExpression;
		be->left=unary(parameter,block);
		exp=new Expression;
		exp->kind=LOGICAL_NOT_EXPRESSION;
		exp->u.binary_expression=be;
		return exp;
	}

	else if(Token.type==SUB){
		match(SUB);
		BinaryExpression * be=new BinaryExpression;
		be->left=unary(parameter,block);
		exp=new Expression;
		exp->kind=NEGATIVE_EXPRESSION;
		exp->u.binary_expression=be;
		return exp;
	}

	else
		return factor(parameter,block);
}


Expression* factor(ParameterList *parameter,Block* block){
	Expression *exp,*t_exp;
	int cnt=1;
	char *t=NULL;
	FunctionDefinition *f;
	ArrayExpression  *ae;
	BinaryExpression * be;
	IDExpression* ie;
	DeclarationList *dl;
	int i,j,k;
	ParameterList *q;
	switch(Token.type){
		case NUM:
		exp=new Expression;
		exp->kind=INT_EXPRESSION;
		exp->u.int_value=atoi(Token.word);
		match(NUM);
		break;

		case LP:
		match(LP);
		exp=new Expression;
		exp->kind=PARENTHESES_EXPRESSION;
		be=new BinaryExpression;
		be->left=expression(parameter,block);
		exp->u.binary_expression=be;
		match(RP);
		break;

		case ID:
		t=Token.word;
		match(ID);


		if(Token.type==LP){
			exp=new Expression;
			if(strcmp(t,"input")==0||strcmp(t,"output")==0||
							strcmp(t,"sqrt")==0||strcmp(t,"log")==0)
				exp->kind=BUILT_IN_FUNCTION;
			else if(!fun_search(t)){
				cerr<<"function "<<t<<" undefined"<<endl;
				exit(1);
			}
			else
				exp->kind=FUNCTION_CALL_EXPRESSION;
			match(LP);
			FunctionCallExpression *fc=new FunctionCallExpression;
			fc->function_name=t;
			exp->u.function_call_expression=fc;
			if(Token.type==RP){
				match(RP);
				fc->argument=NULL;
				if(strcmp(t,"input")!=0&&strcmp(t,"output")!=0&&
					strcmp(t,"sqrt")!=0&&strcmp(t,"log")!=0){
					i=hash(t);
					for(dl=decls[i];dl!=NULL;dl=dl->next)
						if(dl->declaration->type==FUNCTION)
							if(!strcmp(dl->declaration->u.fun->name,t)){
								f=dl->declaration->u.fun;
								break;
							}
					if(count_para(f->parameter)!=0){
						cerr<<"parameter cannot match"<<endl;
						exit(1);
					}
				}
				assert(strcmp(t,"sqrt")!=0&&strcmp(t,"log")!=0);
			}


			else{
				ArgumentList* al=new ArgumentList;
				al->expression=expression(parameter,block);//process args
				al->next=NULL;
				fc->argument=al;
				while(Token.type!=RP){
					match(COMMA);
					cnt++;
					ArgumentList* p=new ArgumentList;
					p->expression=expression(parameter,block);
					p->next=NULL;
					al->next=p;
					al=al->next;
				}
				match(RP);

				if(strcmp(t,"input")!=0&&strcmp(t,"output")!=0&&
								strcmp(t,"sqrt")!=0&&strcmp(t,"log")!=0){
					i=hash(t);
					for(dl=decls[i];dl!=NULL;dl=dl->next)
						if(dl->declaration->type==FUNCTION)
							if(!strcmp(dl->declaration->u.fun->name,t))
								break;
					f=dl->declaration->u.fun;
					//cout<<f->parameter->parame_num<<cnt<<f->name<<endl;
					assert(f->parameter!=NULL&&f->parameter->parame_num==cnt);
					if(count_para(f->parameter)!=cnt){
						//cout<<count_para(f->parameter)<<" "<<cnt<<endl;
						cerr<<"parameter cannot match"<<endl;
						exit(1);
					}
				}
				if(strcmp(t,"sqrt")==0||strcmp(t,"log")==0)
					if(cnt!=1){
						cerr<<"parameter cannot match"<<endl;
						exit(1);
					}
			}
		}


	/* in assign expression which the left value is an array type, implement             */


	/* is ugly , to be implement better. Also, the index is only id, expression avoid   */


		else if(Token.type==LB){
			/*if(!expression_id_fun_search(parameter,block,t)){
				//for(ParameterList *p=parameter;p;p=p->next)
				//cout<<p->name<<endl;
				cerr<<"variable "<<t<<" undefined"<<endl;
				//cerr<<"="<<endl;
				exit(1);
			}*/
		i=hash(t);
		for(dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t))
					break;
			}			
			//cout<<dl->declaration->u.var->dim_num<<endl;
			assert(dl!=NULL&&dl->declaration->u.var->dim_num!=0);
			match(LB);
			exp=new Expression;
			exp->kind=IDENTIFIER_EXPRESSION;




			/*if(!expression_id_fun_search(parameter,block,Token.word)){
				cerr<<"variable "<<Token.word<<" undefined"<<endl;
				exit(1);
			}*/


			/*ae=new ArrayExpression;
			ae->array_name=t;
			ae->index=Token.word;*/





			ie=new IDExpression;
			exp->u.id_expression=ie;
			ie->name=t;
			ie->dim_num=0;
			ie->index_list=NULL;
			while(Token.type!=RB){
				//cout<<Token.word<<endl;
				t_exp=expression(parameter,block);
				//cout<<t_exp->kind<<endl;
				ie->dim_num++;
				if(ie->index_list==NULL){
					R *r=new R;
					r->next=NULL;
					r->exp_s=t_exp;
					ie->index_list=r;
				}
				else{
					R* p;
					for(p=ie->index_list;p->next!=NULL;p=p->next);
					p->next=new R;
					p->next->next=NULL;
					p->next->exp_s=t_exp;
				}
				if(Token.type==COMMA)
					match(COMMA);
				else
					break;
			}
			assert(ie->dim_num==dl->declaration->u.var->dim_num);
			//match(ID);
			match(RB);
			//assert()
			//cout<<Token.word<<endl;
		}

		else{
			if(!expression_id_fun_search(parameter,block,t)){
				//for(ParameterList *p=parameter;p;p=p->next)
				//cout<<p->name<<endl;
				cerr<<"variable "<<t<<" undefined"<<endl;
				//cerr<<"="<<endl;
				exit(1);
			}
		i=hash(t);
		for(dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t))
					break;
			}
			assert(dl!=NULL&&dl->declaration->u.var->dim_num==0);
			exp=new Expression;
			exp->kind=IDENTIFIER_EXPRESSION;
			ie=new IDExpression;
			exp->u.id_expression=ie;
			ie->name=t;
			ie->dim_num=0;
			ie->index_list=NULL;
		}
		break;



		default:
		cerr<<"factor error"<<endl;
		exit(1);
		break;
	}
	return exp;
}



