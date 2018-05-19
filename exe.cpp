#include <head.h>
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern int next;
extern CToken Token;
Block *exe_block;
extern DeclarationList     *decls[SIZE];



Block* getexe(FunctionDefinition* fd,ParameterList *parameter,Block* block){
	match(BEGIN);

	//match(LC);
	/*while(Token.type==TYPE){
		TypeSpecifier rt=CheckType();
		match(TYPE);
		if(rt==VOID){
			cerr<<"local variable define error"<<endl;
			exit(1);
		}
		Declaration *item =new Declaration;
		item->type=rt;
		item->name=Token.word;
		item->dim_list=NULL;
		item->dim_num=0;
		match(ID);
		if(Token.type==LB){
			match(LB);
			while(Token.type==NUM){
				if(item->dim_list==NULL){
					T *tmp=new T;
					tmp->dim=atoi(Token.word);
					tmp->next=NULL;
					item->dim_list=tmp;
				}
				else{
					T* p;
					for(p=item->dim_list;p->next!=NULL;p=p->next);
					p->next=new T;
					p->next->next=NULL;
					p->next->dim=atoi(Token.word);
				}
				item->dim_num++;
				match(NUM);
				if(Token.type==COMMA)
					match(COMMA);
				else
					break;
			}
		match(RB);
		assert(item->dim_num!=0);
		}
		if(id_search(item->name,exe_block->declaration_list)){
			cerr<<"variable "<<item->name<<" redefined"<<endl;
			exit(1);
		}
		match(SEMICOLON);
		//cout<<"right here"<<endl;
		if(exe_block->declaration_list){
			DeclarationList *p;
			for(p=exe_block->declaration_list;p->next;p=p->next);
			p->next=new DeclarationList;
			p->next->declaration=item;
			p->next->next=NULL;
		}
		else{
			exe_block->declaration_list=new DeclarationList;
			exe_block->declaration_list->declaration=item;
			exe_block->declaration_list->next=NULL;
		}
	}*/


/*if(Token.type==LC){
match(LC);
Block* nb=new Block;
nb->outer_block=block;
nb->declaration_list=NULL;
creat_block(nb);
}*/
//else if(Token.type!=RC){
//statements start
	int i;

	while(Token.type!=END)
		creat_stmt(fd,parameter,block);
	match(END);
		for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			if(d->type==FUNCTION||d->u.var->b!=block)
				break;
			else{
				delete d->u.var;
				delete d;
			}
			DeclarationList* q=p;
			p=p->next;
			delete q;
		}
		decls[i]=p;
	}
	return exe_block;
}

/*
implement muti-type
use struct rt_value{
type;
union{double d;
int i;
float f;
bool b;
}u;
};
exe_exp return the value whose type is struct rt_value, when to use ,to get the type through
type of struct rt_value*/


double exe_exp(FunctionDefinition* fun,Block* blk,Expression* e){
	Expression *exp,*t_exp,*e1,*e2;
	int flag,num,i,n;
	char *t=NULL;
	FunctionDefinition *f;
	ArrayExpression  *ae;
	BinaryExpression * be;
	IDExpression* ie;
	AssignExpression * assign;
	DeclarationList* dl,*p;
	FunctionDefinition *fd;
	ArgumentList* al;
	ParameterList* pl,*tmp_pl=NULL,*ql=NULL,*tpl;
	FunctionCallExpression* fcexp;
	Block *b;
	struct R *r;
	struct T *d;
	struct variable* v;
	Declaration* dec;
	double value_return_store;

	switch(e->kind){


		case ASSIGN_EXPRESSION:
		assign=e->u.assign_expression;

		assert(assign->left->kind==IDENTIFIER_EXPRESSION
		||assign->left->kind==ASSIGN_EXPRESSION);

		t=assign->left->u.id_expression->name;
		flag=0;
		/*for(b=blk;b!=NULL;b=b->outer_block)
			for(dl=b->declaration_list;dl!=NULL;dl=dl->next)
				if(!strcmp(t,dl->declaration->name)){
					dl->declaration->value=exe_exp(fun,blk,assign->right);
					flag=1;
					return dl->declaration->value;
				}
		if(flag==0&&fun!=NULL){
			for(pl=fun->parameter;pl!=NULL;pl=pl->next)
				if(!strcmp(pl->name,t)){
					pl->value=exe_exp(fun,blk,assign->right);
					return pl->value;
				}
		}
		if(flag==0)
			for(dl=declaration_list;dl!=NULL;dl=dl->next)
				if(!strcmp(t,dl->declaration->name)){
						dl->declaration->value=exe_exp(fun,blk,assign->right);
						flag=1;
						return dl->declaration->value;
				}*/
		i=hash(t);
		for(dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t)){
					flag=1;
					if(dl->declaration->u.var->dim_num==0){
						dl->declaration->u.var->value=exe_exp(fun,blk,assign->right);
						
						return dl->declaration->u.var->value;
					}
					else{
						num=0;
						i=0;
						for(r=assign->left->u.id_expression->index_list,d=dl->declaration->u.var->dim_list;
									r!=NULL&&d!=NULL;r=r->next,d=d->next){
							n=exe_exp(fun,blk,r->exp_s);
							//cout<<n<<" "<<d->dim<<endl;
							if(n>=d->dim){
								cerr<<"index of ARRAY "<<dl->declaration->u.var->name
										<<" out of bound, location: "<<i+1<<endl;
								exit(1);
							}
							for(struct T *dd=d->next;dd!=NULL;dd=dd->next)
								n=n*dd->dim;
							num=num+n;
							i++;
						}
						//cout<<"++++++++++++++++"<<endl;
						//cout<<i<<dl->declaration->u.var->name<<e->u.assign_expression->left->u.id_expression->dim_num<<endl;
						assert(i==dl->declaration->u.var->dim_num);
						dl->declaration->u.var->array_value[num]=exe_exp(fun,blk,assign->right);
						return dl->declaration->u.var->array_value[num];
					}
				}
			}
		assert(flag==1);
		break;


		case INT_EXPRESSION:
		return e->u.int_value;
		break;


		case PARENTHESES_EXPRESSION:
		n=exe_exp(fun,blk,e->u.binary_expression->left);
		//printf("====%d\n",n);
		return n;
		break;


		case IDENTIFIER_EXPRESSION:
		t=e->u.id_expression->name;
		/*if(fun==NULL){
			for(dl=exe_block->declaration_list;dl!=NULL;dl=dl->next)
				if(!strcmp(t,dl->declaration->name)){


					if(dl->declaration->dim_num==0){
						assert(dl->declaration->dim_num==0);
						return dl->declaration->value;
					}

					else{

						num=1;
						assert(e->u.id_expression->index_list!=NULL);

						for(r=e->u.id_expression->index_list,d=dl->declaration->dim_list;r!=NULL&&d!=NULL;
								r=r->next,d=d->next){
									if(exe_exp(fun,blk,r->exp_s)>=d->dim){
										cerr<<"index of ARRAY "<<t<<" out of bound"<<endl;
										exit(1);
									}
									num=num*exe_exp(fun,blk,r->exp_s);
						}

						dl->declaration->array_value=new double[num];

					}

				}

		}


		else{
			//printf("-----------------------------\n\n");
		for(b=blk;b!=NULL;b=b->outer_block)
			for(dl=b->declaration_list;dl!=NULL;dl=dl->next)
				if(!strcmp(t,dl->declaration->name)){
					return dl->declaration->value;
					//flag=1;
					
				}


			for(pl=fun->parameter;pl!=NULL;pl=pl->next)
				if(!strcmp(t,pl->name))
					return pl->value;
		}*/
		flag=0;
		i=hash(t);
		//cout<<"-----------"<<t<<endl;
		for(dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t)){
					flag=1;
					//dl->declaration->u.var->value=exe_exp(fun,blk,assign->right);
					//dl->declaration->u.var->value=
					if(dl->declaration->u.var->dim_num==0)
						return dl->declaration->u.var->value;
					else{
						num=0;
						i=0;
						for(r=e->u.id_expression->index_list,d=dl->declaration->u.var->dim_list;
									r!=NULL&&d!=NULL;r=r->next,d=d->next){
							n=exe_exp(fun,blk,r->exp_s);
							//cout<<n<<" "<<d->dim<<endl;
							if(n>=d->dim){
								cerr<<"index of ARRAY "<<dl->declaration->u.var->name
										<<" out of bound, location: "<<i+1<<endl;
								exit(1);
							}
							for(struct T *dd=d->next;dd!=NULL;dd=dd->next)
								n=n*dd->dim;
							num=num+n;
							i++;
						}
						//cout<<"**********"<<num<<endl;
						assert(i==e->u.id_expression->dim_num);
						return dl->declaration->u.var->array_value[num];
					}
				}
			}
		/*for(dl=declaration_list;dl!=NULL;dl=dl->next)
			if(!strcmp(t,dl->declaration->name))
				return dl->declaration->value;*/
		assert(flag==1);
		break;


		case LOGICAL_OR_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		if(exe_exp(fun,blk,e1)!=0)
			return 1;
		else
			return exe_exp(fun,blk,e2)==0;
		//return exe_exp(fun,blk,e1)||exe_exp(fun,blk,e2);
		break;


		case LOGICAL_AND_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		if(exe_exp(fun,blk,e1)==0)
			return 0;
		else{
			n=exe_exp(fun,blk,e2);
			//printf("====%d\n",n);
			return n!=0;
		}
		//return exe_exp(fun,blk,e1)&&exe_exp(fun,blk,e2);
		break;


		case EQ_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)==exe_exp(fun,blk,e2);
		break;


		case GE_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)>=exe_exp(fun,blk,e2);
		break;


		case GT_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)>exe_exp(fun,blk,e2);
		break;


		case LE_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)<=exe_exp(fun,blk,e2);
		break;


		case LT_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)<exe_exp(fun,blk,e2);
		break;


		case ADD_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)+exe_exp(fun,blk,e2);
		break;


		case SUB_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)-exe_exp(fun,blk,e2);
		break;


		case MUL_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)*exe_exp(fun,blk,e2);
		break;


		case DIV_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)/exe_exp(fun,blk,e2);
		break;


		case MOD_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		//printf("%g %g\n",exe_exp(fun,blk,e1),exe_exp(fun,blk,e2));
		return (int(exe_exp(fun,blk,e1)))%(int(exe_exp(fun,blk,e2)));
		break;


		case NE_EXPRESSION:
		e1=e->u.binary_expression->left;
		e2=e->u.binary_expression->right;
		return exe_exp(fun,blk,e1)!=exe_exp(fun,blk,e2);
		break;


		case LOGICAL_NOT_EXPRESSION:
		e1=e->u.binary_expression->left;
		return !exe_exp(fun,blk,e1);
		break;


		case NEGATIVE_EXPRESSION:
		e1=e->u.binary_expression->left;
		return -exe_exp(fun,blk,e1);
		break;


		//for the function_call_expression
		case FUNCTION_CALL_EXPRESSION:
		assert(e->kind==FUNCTION_CALL_EXPRESSION);
		fcexp=e->u.function_call_expression;
		/*for(fd=function_list;fd!=NULL;fd=fd->next)
			if(!strcmp(fcexp->function_name,fd->name))
				break;*/
		i=hash(fcexp->function_name);
		for(dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==FUNCTION)
				if(!strcmp(dl->declaration->u.fun->name,fcexp->function_name)){
					fd=dl->declaration->u.fun;
					break;
				}
		//need to store and back parameter
		n=++fd->level;
//cout<<"+"<<n<<endl;
		/*for(pl=fd->parameter,al=fcexp->argument;pl!=NULL&&al!=NULL;pl=pl->next,al=al->next){
			v=new variable;
			dec=new Declaration;
			dl=new DeclarationList;
			dec->type=VARIABLE;
			dec->u.var=v;
			v->type=pl->type;
			v->name=pl->name;
			v->b=fd->block;
			v->level=n;
			v->value=exe_exp(fun,blk,al->expression);
			//v->value=exe_exp(fd,fd->block,al->expression);
			//cout<<(fd==fun)<<" "<<(blk==fd->block)<<endl;
			dl->declaration=dec;
			dl->next=NULL;
			i=hash(pl->name);
			if(decls[i]==NULL)
				decls[i]=dl;
			else{
				dl->next=decls[i];
				decls[i]=dl;
			}
		}*/

//first calulate the parame value and store them, then push them to the symtable
//or the scole would be error and the parame get the wrong value
//example is gcd(int u,int v)
//whem return gcd(v,u%v)
//the second parame would get the wrong value u=12
//the right value is u=18 then u%v=6
//or u%v=12%12=0. WRONG!!!

	for(pl=fd->parameter,al=fcexp->argument;pl!=NULL&&al!=NULL;pl=pl->next,al=al->next){
			/*v=new variable;
			dec=new Declaration;
			dl=new DeclarationList;
			dec->type=VARIABLE;
			dec->u.var=v;
			v->type=pl->type;
			v->name=pl->name;
			v->b=fd->block;
			v->level=n;
			v->value=exe_exp(fun,blk,al->expression);*/
			pl->value=exe_exp(fun,blk,al->expression);
			//v->value=exe_exp(fd,fd->block,al->expression);
			//cout<<(fd==fun)<<" "<<(blk==fd->block)<<endl;
			/*dl->declaration=dec;
			dl->next=NULL;
			i=hash(pl->name);
			if(decls[i]==NULL)
				decls[i]=dl;
			else{
				dl->next=decls[i];
				decls[i]=dl;
			}*/
		}
	for(pl=fd->parameter;pl!=NULL;pl=pl->next){
			v=new variable;
			dec=new Declaration;
			dl=new DeclarationList;
			dec->type=VARIABLE;
			dec->u.var=v;
			v->type=pl->type;
			v->name=pl->name;
			v->b=fd->block;
			v->level=n;
			v->value=pl->value;
			dl->declaration=dec;
			dl->next=NULL;
			i=hash(pl->name);
			if(decls[i]==NULL)
				decls[i]=dl;
			else{
				dl->next=decls[i];
				decls[i]=dl;
			}
	}
			//tmp_pl=new ParameterList;
			//tmp_pl->value=pl->value;
			//tmp_pl->next=NULL;
			
			//v->value=exe_exp(fd,fd->block,al->expression);
			//cout<<v->value<<endl;
			//if(v->value==2)
			//exit(0);
			/*if(ql==NULL)
				ql=tpl=tmp_pl;
			else{
				for(tpl=ql;tpl->next!=NULL;tpl=tpl->next);
				tpl->next=tmp_pl;
			}*/
		assert(pl==NULL&&al==NULL);
		//fd->is_returned=0;


		//cpoy fd to temp and exe(temp,temp->block))
		//cout<<fd->name<<endl;
		assert(fd->block!=NULL);
		exe(fd,fd->block);
		value_return_store=fd->u.value_double;
		
//cout<<fd->level<<"-"<<n<<endl;
	for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			//if(d->type==VARIABLE)
			//cout<<d->u.var->b->fun->name<<endl;
			if(d->type==VARIABLE&&d->u.var->b==fd->block&&d->u.var->level==fd->level){
				//cout<<">>>>>>>>>>>>>>>"<<endl;
				delete d->u.var;
				delete d;
			}
			else{
				//cout<<d->u.fun->name<<endl;
				break;
			}
			DeclarationList* q=p;
			p=p->next;
			delete q;
		}
		decls[i]=p;
	}
		--fd->level;

		//fd->is_returned=0;
		//for(pl=fd->parameter;pl!=NULL;pl=pl->next,ql=ql->next)
			//pl->value=ql->value;
		//cout<<"="<<fd->level<<endl;
		if(fd->type==BOOLEAN)
			return value_return_store==0;
		return value_return_store;
		break;
		

		case BUILT_IN_FUNCTION:
		fcexp=e->u.function_call_expression;
			for(al=fcexp->argument;al!=NULL;al=al->next){
				p=NULL;
				//printf("%s\n",fcexp->function_name);
				if(!strcmp(fcexp->function_name,"input")){
					assert(al->expression->kind==IDENTIFIER_EXPRESSION);
					/*for(b=blk;b!=NULL;b=b->outer_block)
						for(p=b->declaration_list;p!=NULL;p=p->next)
							if(!strcmp(p->declaration->name,al->expression->u.id_expression->name)){
								flag=100;
								break;
							}*/
					//cout<<">>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
					/*if(fun!=NULL)
						for(pl=fun->parameter;pl!=NULL;pl=pl->next){
							//cout<<t<<endl;
							if(!strcmp(pl->name,al->expression->u.id_expression->name)){
								//cout<<">>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
								flag=300;
								break;
							}
						}*/
					/*if(p==NULL)
						for(p=declaration_list;p!=NULL;p=p->next)
							if(!strcmp(p->declaration->name,al->expression->u.id_expression->name)){
									flag=200;
									break;
							}*/
					//assert(p!=NULL);
					//assert(p!=NULL||pl!=NULL);
					/*if(flag==200||flag==100)
						scanf("%lf",&(p->declaration->value));
					else if(flag==300)
						scanf("%lf",&(pl->value));*/
					i=hash(al->expression->u.id_expression->name);
					for(dl=decls[i];dl!=NULL;dl=dl->next)
						if(dl->declaration->type==VARIABLE)
							if(!strcmp(dl->declaration->u.var->name,al->expression->u.id_expression->name))
								if(dl->declaration->u.var->dim_num==0){
									//cout<<al->expression->u.id_expression->name<<endl;
									assert(al->expression->u.id_expression->dim_num==0);
									scanf("%lf",&(dl->declaration->u.var->value));
									break;
							}
					/*else{
						num=1;
						i=0;
						for(r=e->u.id_expression->index_list;r!=NULL;r=r->next){
							num=num*exe_exp(fun,blk,r->exp_s);
							i++;
						}
						assert(i==e->u.id_expression->dim_num);
						scanf("%lf",&(dl->declaration->u.var->array_value[num]));
					}*/
					else{
						num=0;
						i=0;
						for(r=al->expression->u.id_expression->index_list,d=dl->declaration->u.var->dim_list;
									r!=NULL&&d!=NULL;r=r->next,d=d->next){
							n=exe_exp(fun,blk,r->exp_s);
							//cout<<n<<" "<<d->dim<<endl;
							if(n>=d->dim){
								cerr<<"index of ARRAY "<<dl->declaration->u.var->name
										<<" out of bound, location: "<<i+1<<endl;
								exit(1);
							}
							for(struct T *dd=d->next;dd!=NULL;dd=dd->next)
								n=n*dd->dim;
							num=num+n;
							i++;
						}
						//cout<<i<<"**********"<<al->expression->u.id_expression->dim_num<<endl;
						assert(i==al->expression->u.id_expression->dim_num);
						scanf("%lf",&(dl->declaration->u.var->array_value[num]));
					}


				}


				else if(!strcmp(fcexp->function_name,"output")){
					//if(flag==100)
						//printf("exe_block variable : ");
					printf("output : %g\n",exe_exp(fun,blk,al->expression));
				}

				else if(!strcmp(fcexp->function_name,"sqrt")){
					//if(flag==100)
						//printf("exe_block variable : ");
					return sqrt(exe_exp(fun,blk,al->expression));
				}

				else if(!strcmp(fcexp->function_name,"log")){
					//if(flag==100)
						//printf("exe_block variable : ");
					return log(exe_exp(fun,blk,al->expression));
				}
		}
		return 0;
		break;


		default:
		cerr<<"unknow type of expression"<<endl;
		exit(1);
	}


	return 0;
}


int exe_if(FunctionDefinition* fun,Block* blk,Statement* s){
	int i;

	if(!s)
		return 0;

	assert(s->type==IF_STATEMENT);
	IfStatement* ist=s->u.if_s;
	int rt_value=0;
	if(exe_exp(fun,blk,ist->condition)){
		rt_value=exe(fun,ist->then_block);
		//cout<<rt_value<<endl;
		/*if(rt_value==100)
			return 100;
		else if(rt_value==200)
			return 200;
		else if(rt_value==300)
			return 300;*/
		//if((rt_value&1)==1||(rt_value&2)==2||(rt_value&4)==4)
			//return rt_value;
	for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			if(d->type==FUNCTION||d->u.var->b!=ist->then_block)
				break;
			else{
				//cout<<">>>>>>>>>>>>>>>"<<endl;
				delete d->u.var;
				delete d;
			}
			DeclarationList* q=p;
			p=p->next;
			delete q;
		}
		decls[i]=p;
	}


	}


	else if(ist->else_block!=NULL){
		rt_value=exe(fun,ist->else_block);
		/*if(rt_value==100)
			return 100;
		else if(rt_value==200)
			return 200;
		else if(rt_value==300)
			return 300;*/
		//if((rt_value&1)==1||(rt_value&2)==2||(rt_value&4)==4)
			//return rt_value;
	for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			if(d->type==FUNCTION||d->u.var->b!=ist->else_block)
				break;
			else{
				//cout<<">>>>>>>>>>>>>>>"<<endl;
				delete d->u.var;
				delete d;
			}
			DeclarationList* q=p;
			p=p->next;
			delete q;
		}
		decls[i]=p;
	}


	}


	return rt_value;
}



int exe_while(FunctionDefinition* fun,Block* blk,Statement* s){
	DeclarationList* dl;
	if(!s)
		return 0;
	assert(s->type==WHILE_STATEMENT);
	WhileStatement* wst=s->u.while_s;
	int rt_value=0,i;

//decide break is set
/*
while(exe_exp(fun,blk,wst->condition))
		if(blk->label_break!='b')
			exe(fun,wst->block);
*/

	while(exe_exp(fun,blk,wst->condition)){
		rt_value=exe(fun,wst->block);

		/*if(blk->label_break!=NULL&&blk->label_break->type=='b')
			break;
		if(blk->label_continue==NULL)
			exe(fun,wst->block);
		else if(blk->label_break->type!='b')
			exe(fun,wst->block);
	}
	if(blk->label_break!=NULL){
		label* l=blk->label_break;
		blk->label_break=blk->label_break->next;
		delete l;
	}*/
	//if(rt_value==200)
		//cout<<rt_value<<endl;
	if((rt_value&1)==1||(rt_value&4)==4)
		break;
		//return rt_value;
	/*else if((rt_value&2)==2)
		continue;*/
	/*else if(rt_value==300)
		return 300;*/
		
	}


	for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			if(d->type==FUNCTION||d->u.var->b!=blk)
				break;
			else{
				//cout<<">>>>>>>>>>>>>>>"<<endl;
				delete d->u.var;
				delete d;
			}
			DeclarationList* q=p;
			p=p->next;
			delete q;
		}
		decls[i]=p;
	}


	return rt_value;
}


int exe(FunctionDefinition* fd,Block* blk){
	Block* p_exe;
	StatementList* st;
	DeclarationList* dl=exe_block->declaration_list,*p;
	Statement* s_exe;
	Block* b;
	label* l;
	int rt_value=0,i,flag;
	struct variable *decl_s;
	Declaration* item;

//1 break
//2 continue
//4 return 
//8 type



	//FunctionDefinition *fd;
	/*for(p_exe=blk;p_exe!=NULL;p_exe=p_exe->outer_block)
		if(p_exe->type==FUNCTION_BLOCK)
			break ;
	for(fd=function_list;fd!=NULL;fd=fd->next)
		if(p_exe==fd->block)
			break;*/
	for(st=blk->statement_list;st!=NULL;st=st->next){
		//if(fd!=NULL&&fd->is_returned==1)
			//return 0;
//decide continue is set
/*find outer_block whose type is while and set its label_continue to 'c'
if its its label_continue is set then return, which back to exe_while and exe the condition
to decide whether to continue the loop.
*/
		s_exe=st->statement;
		switch(s_exe->type){
			case IF_STATEMENT:
			rt_value=exe_if(fd,s_exe->u.if_s->then_block,s_exe);
			if((rt_value&1)==1||(rt_value&2)==2||(rt_value&4)==4)
				return rt_value;
			/*else if(rt_value==200)
				return 200;
			else if(rt_value==300)
				return 300;*/
			break;

			case WHILE_STATEMENT:
			rt_value=exe_while(fd,s_exe->u.while_s->block,s_exe);
			/*if(rt_value==100)
				return 100;
			else if(rt_value==200)
				return 200;*/
			if((rt_value&4)==4){
				//cout<<"--------------------------------------------------"<<endl;
				return rt_value;
			}
			break;

			case EXPRESSION_STATEMENT:
			exe_exp(fd,blk,s_exe->u.expression_s);
			//return 0;
			break;
			
/*
continue and break statements are unsupported
Don't Use !!!
*/
//continue 100
//break 200
//return 300

			case CONTINUE_STATEMENT:
			/*for(b=blk;b!=NULL;b=b->outer_block)
				if(b->type==WHILE_STATEMENT_BLOCK){
					if(b->label_continue==NULL){
						b->label_continue=new label;
						b->label_continue->type='c';
						b->label_continue->next=NULL;
					}
					else{
						l=new label;
						l->type='c';
						l->next=b->label_continue;
						b->label_continue=l;
					}
					break;
				}*/
			rt_value=rt_value|2;
			return rt_value;
			break;


			case BREAK_STATEMENT:
			/*for(b=blk;b!=NULL;b=b->outer_block)
				if(b->type==WHILE_STATEMENT_BLOCK){
					if(b->label_break==NULL){
						b->label_break=new label;
						b->label_break->type='b';
						b->label_break->next=NULL;
					}
					else{
						l=new label;
						l->type='c';
						l->next=b->label_break;
						b->label_break=l;
					}
					break;
				}*/
			rt_value=rt_value|1;
			return rt_value;
			break;


			case RETURN_STATEMENT:
			/*for(p_exe=blk;p_exe!=NULL;p_exe=p_exe->outer_block){
				//p_exe->label_return='r';
				if(p_exe->type==FUNCTION_BLOCK)
					break;
			}
			for(fd=function_list;fd!=NULL;fd=fd->next)
				if(p_exe==fd->block)
					break;*/
			//fd->is_returned=1;
			if(s_exe->u.return_s->return_value!=NULL)
				fd->u.value_double=exe_exp(fd,blk,s_exe->u.return_s->return_value);
			rt_value=rt_value|4;
			return rt_value;
			break;


			case DECL_STATEMENT:
			flag=0;
			decl_s=s_exe->u.decl_s;
			rt_value=rt_value|8;
			i=hash(decl_s->name);
			for(dl=decls[i];dl!=NULL;dl=dl->next)
				if(dl->declaration->type==VARIABLE)
					if(dl->declaration->u.var->b==blk&&strcmp(decl_s->name,dl->declaration->u.var->name)==0
						&&dl->declaration->u.var->level==decl_s->level){
							flag=1;
							break;
					}
			if(flag==0){
				item=new Declaration;
				item->type=VARIABLE;
				item->u.var=new variable;
				item->u.var->name=decl_s->name;
				item->u.var->array_value=NULL;
				//item->u.var->name=t;
				item->u.var->dim_num=0;
				item->u.var->dim_list=NULL;
				item->u.var->type=decl_s->type;
				item->u.var->b=blk;
				item->u.var->level=fd->level;
				//cout<<decl_s->name<<" "<<item->u.var->level<<endl;
				if(decls[i]==NULL){
					decls[i]=new DeclarationList;
					decls[i]->declaration=item;
					decls[i]->next=NULL;
				}
				else{
					p=new DeclarationList;
					p->declaration=item;
					p->next=decls[i];
					decls[i]=p;
					/*for(p=decls[i];p->next!=NULL;p=p->next);
					p->next=new DeclarationList;
					p->next->declaration=item;
					p->next->next=NULL;*/
				}
			}
			break;


			default:
				cerr<<"unknow statement"<<endl;
				exit(1);
				return 0;
				break;


		}
	}
	return 0;
}




