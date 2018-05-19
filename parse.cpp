#include <head.h>
FunctionDefinition  *function_list = NULL;
DeclarationList     *declaration_list = NULL;
DeclarationList     *decls[SIZE];
DeclarationList     *gen_help[SIZE];
int num_gen_help=0;
//Block               *current_block;


vector<CToken> InputSequence;
int next=-1;
CToken Token;


int hash(char* key){
	int temp=0;
	int i=0;
	while(key[i]!=0){
		temp=((temp<<4)+key[i])%SIZE;
		++i;
	}
}


TypeSpecifier CheckType(){
	if(!strcmp(Token.word,"int"))
		return INT;
	else if(!strcmp(Token.word,"float"))
		return FLOAT;
	else if(!strcmp(Token.word,"double"))
		return DOUBLE;
	else if(!strcmp(Token.word,"void"))
		return VOID;
	else if(!strcmp(Token.word,"boolean"))
		return BOOLEAN;
	else
		cerr<<"type error"<<endl;
}

void add_symtab(char *s){
int i=hash(s);









}
void parame(FunctionDefinition* fd){
	if(Token.type==RP)
		return;
	TypeSpecifier rt=CheckType();
	match(TYPE);
	if(rt==VOID)
		return;
	else{
		ParameterList *item =new ParameterList;
		item->type=rt;
		item->name=Token.word;
		item->b=fd->block;
		match(ID);
		/*if(Token.type==LB){
			match(LB);
			match(RB);
		}*/
		item->next=NULL;
		variable *v=new variable;
		v->type=rt;
		v->array_value=NULL;
		v->dim_list=NULL;
		v->dim_num=0;
		v->name=item->name;
		v->b=fd->block;
		Declaration* d=new Declaration;
		d->type=VARIABLE;
		d->u.var=v;
		int i=hash(v->name);
		if(decls[i]==NULL){
			decls[i]=new DeclarationList;
			decls[i]->declaration=d;
			decls[i]->next=NULL;
		}
		else{
			DeclarationList* p=new DeclarationList;
			p->declaration=d;
			p->next=decls[i];
			decls[i]=p;
			/*for(p=decls[i];p->next!=NULL;p=p->next);
			p->next=new DeclarationList;
			p->next->declaration=item;
			p->next->next=NULL;*/
		}
		if(fd->parameter){
			fd->parameter->parame_num++;
			ParameterList *p;
			for(p=fd->parameter;p->next;p=p->next);
			p->next=item;
		}
		else{
			fd->parameter=item;
			fd->parameter->parame_num=1;
		}
	}
/*for(ParameterList *p=fd->parameter;p;p=p->next){
cout<<p->name<<endl;
}*/
}


void creat_parameList(FunctionDefinition* fd){
	parame(fd);
	while(Token.type==COMMA){
		match(COMMA);
		parame(fd);
	}
}


void creat_stmt(FunctionDefinition* fd,ParameterList *parameter,Block* block){
	Statement *s=new Statement;
	IfStatement     *if_s;
	WhileStatement  *while_s;
	ReturnStatement *return_s;
	Expression* exp;
	Block *bl;
	BreakStatement *bs;
	ContinueStatement *cs;
	struct variable *decl_s,*item;
	TypeSpecifier rt;
	int i;
	DeclarationList* dl;
	switch(Token.type){

	case IF:
	if_s=new IfStatement;
	s->type=IF_STATEMENT;
	match(IF);
	match(LP);
	if_s->condition=expression(parameter,block);
	match(RP);
	bl=new Block;
	bl->type=IF_STATEMENT_BLOCK;
	bl->outer_block=block;
	//bl->declaration_list=NULL;
	bl->statement_list=NULL;
	if(Token.type==LC){
		match(LC);
		creat_block(fd,parameter,bl);
	}
	else{
		assert(Token.type!=TYPE);
		creat_stmt(fd,parameter,bl);
	//match(SEMICOLON);
	}
	if_s->then_block=bl;
	if(Token.type==ELSE){
		match(ELSE);
		if_s->else_block=new Block;
		if_s->else_block->outer_block=block;
		if_s->else_block->type=ELSE__STATEMENT_BLOCK;
		if_s->else_block->declaration_list=NULL;
		if_s->else_block->statement_list=NULL;
		if(Token.type==LC){
			match(LC);
			creat_block(fd,parameter,if_s->else_block);
		}
		else{
			assert(Token.type!=TYPE);
			creat_stmt(fd,parameter,if_s->else_block);
		}
	}
	else
		if_s->else_block=NULL;
	s->u.if_s=if_s;
	break;


	case RETURN:
	match(RETURN);
	return_s=new ReturnStatement;
	return_s->return_value=NULL;
	s->type=RETURN_STATEMENT;
	if(fd->type==VOID){
		if(Token.type!=SEMICOLON){
			cerr<<"void function return expression"<<endl;
			exit(1);
		}
		return_s->return_value=NULL;
	}
	else{
		if(Token.type==SEMICOLON){
			cerr<<"none void function return without expression"<<endl;
			exit(1);
		}
		return_s->return_value=expression(parameter,block);
	}
	s->u.return_s=return_s;
	match(SEMICOLON);
	break;


	case BREAK:
	match(BREAK);
	bs=new BreakStatement;
	s->type=BREAK_STATEMENT;
	bs->break_block=NULL;
	for(Block *blk=block;blk!=NULL;blk=blk->outer_block)
		if(blk->type==WHILE_STATEMENT_BLOCK){
			bs->break_block=blk;
			break;
		}
	if(bs->break_block==NULL){
		cerr<<"break statement not in loop block"<<endl;
		exit(1);
	}
	s->u.break_s=bs;
	match(SEMICOLON);
	break;


	case CONTINUE:
	match(CONTINUE);
	cs=new ContinueStatement;
	s->type=CONTINUE_STATEMENT;
	cs->continue_block=NULL;
	for(Block *blk=block;blk!=NULL;blk=blk->outer_block)
		if(blk->type==WHILE_STATEMENT_BLOCK){
			cs->continue_block=blk;
			break;
		}
	if(cs->continue_block==NULL){
	cerr<<"continue statement not in loop block"<<endl;
	exit(1);
	}
	s->u.continue_s=cs;
	match(SEMICOLON);
	break;


	case WHILE:
	match(WHILE);
	while_s=new WhileStatement;
	s->type=WHILE_STATEMENT;
	match(LP);
	while_s->condition=expression(parameter,block);
	match(RP);
	//while(Token.type!=RC)
	/*if(Token.type==LC){
	match(LC);
	Block* nb=new Block;
	nb->outer_block=block;
	nb->declaration_list=NULL;
	creat_block(nb);
	}
	else*/
	while_s->block=new Block;
	while_s->block->outer_block=block;
	while_s->block->type=WHILE_STATEMENT_BLOCK;
	while_s->block->declaration_list=NULL;
	while_s->block->statement_list=NULL;
	if(Token.type==LC){
		match(LC);
		creat_block(fd,parameter,while_s->block);
	}
	else{
		assert(Token.type!=TYPE);
		creat_stmt(fd,parameter,while_s->block);
	}
	s->u.while_s=while_s;
	//match(RC);
	break;


	case ID:
	case NUM:
	s->type=EXPRESSION_STATEMENT;
	s->u.expression_s=expression(parameter,block);
	match(SEMICOLON);
	break;


	case LP:
	match(LP);
	s->type=EXPRESSION_STATEMENT;
	s->u.expression_s=expression(parameter,block);
	match(RP);
	match(SEMICOLON);
	break;


	case TYPE:
	s->type=DECL_STATEMENT;
	rt=CheckType();
	if(rt==VOID){
		cerr<<"variable type can't be void"<<endl;
		exit(1);
	}
	match(TYPE);
	decl_s=new variable;
	item=new variable;
	item->type=decl_s->type=rt;
	item->array_value=decl_s->array_value=NULL;
	item->dim_num=decl_s->dim_num=0;
	item->dim_list=decl_s->dim_list=NULL;
	item->name=decl_s->name=Token.word;
	item->b=decl_s->b=block;
	match(ID);
	//cout<<">>>>>>>>>>>>>>"<<endl;
	if(fd&&parameter)
		if(block->outer_block==NULL&&para_search(parameter,decl_s->name)){
			cerr<<"variable "<<decl_s->name<<" redefined"<<endl;
			exit(1);
		}


	i=hash(decl_s->name);
	for(dl=decls[i];dl!=NULL;dl=dl->next){
		if(dl->declaration->type==FUNCTION||dl->declaration->u.var->b!=block)
			break;
		else if(!strcmp(dl->declaration->u.var->name,decl_s->name)){
			cerr<<"variable "<<decl_s->name<<" redefined"<<endl;
			exit(1);
		}
	}
	if(decls[i]==NULL){
	decls[i]=new DeclarationList;
	decls[i]->declaration=new Declaration;
	decls[i]->declaration->type=VARIABLE;
	decls[i]->declaration->u.var=item;
	decls[i]->next=NULL;
	}
	else{
		DeclarationList* p=new DeclarationList;
		p->declaration=new Declaration;
		p->declaration->type=VARIABLE;
		p->declaration->u.var=item;
		p->next=decls[i];
		decls[i]=p;
		/*for(p=decls[i];p->next!=NULL;p=p->next);
		p->next=new DeclarationList;
		p->next->declaration=item;
		p->next->next=NULL;*/
	}
	match(SEMICOLON);
	s->u.decl_s=decl_s;
	break;


	default:
	cerr<<"unknow expression"<<endl;
	exit(1);
	break;
	}


	if(block->statement_list==NULL){
		block->statement_list=new StatementList;
		block->statement_list->statement=s;
		block->statement_list->next=NULL;
	}
	else{
		StatementList* p;
		for(p=block->statement_list;p->next;p=p->next);
		p->next=new StatementList;
		p->next->statement=s;
		p->next->next=NULL;
	}
}


void creat_block(FunctionDefinition* fd,ParameterList *parameter,Block* block){
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
		if(fd&&parameter)
			if(id_search(item->name,block->declaration_list)||(para_search(parameter,item->name)&&block->outer_block==NULL)){
				cerr<<"variable "<<item->name<<" redefined"<<endl;
				exit(1);
			}
		match(SEMICOLON);
		//cout<<"right here"<<endl;
		if(block->declaration_list){
			DeclarationList *p;
			for(p=block->declaration_list;p->next;p=p->next);
			p->next=new DeclarationList;
			p->next->declaration=item;
			p->next->next=NULL;
		}
		else{
			block->declaration_list=new DeclarationList;
			block->declaration_list->declaration=item;
			block->declaration_list->next=NULL;
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
	while(Token.type!=RC)
		creat_stmt(fd,parameter,block);
	match(RC);
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
}




void create_declaration(){
	int i,flag;
	DeclarationList* dl;
	TypeSpecifier rt=CheckType();
	match(TYPE);
	char *t=Token.word;
	match(ID);
	int n;
	if((Token.type==SEMICOLON||Token.type==LB)&&rt!=VOID){
		/*if(id_search(t,declaration_list)){
			cerr<<"variable "<<t<<" redefined"<<endl;
			exit(1);
		}*/
		i=hash(t);
		//cout<<i<<endl;
		for(dl=decls[i];dl!=NULL;dl=dl->next){
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t)){
					cerr<<"variable "<<t<<" redefined"<<endl;
					exit(1);
				}
			}
			else if(dl->declaration->type==FUNCTION){
				if(!strcmp(dl->declaration->u.fun->name,t)){
					cerr<<"variable "<<t<<" redefined"<<endl;
					exit(1);
				}
			}
		}
		Declaration* item=new Declaration;
		item->type=VARIABLE;
		item->u.var=new variable;
		item->u.var->name=t;
		item->u.var->array_value=NULL;
		//item->u.var->name=t;
		item->u.var->dim_num=0;
		item->u.var->dim_list=NULL;
		//item->u.var->base_list=NULL;
		item->u.var->type=rt;
		item->u.var->b=NULL;
		item->u.var->level=0;
		//item->type=rt;
		//item->dim_list=NULL;
		//item->dim_num=0;
		if(Token.type==LB){
			match(LB);
			n=1;
			while(Token.type==NUM){
				if(item->u.var->dim_list==NULL){
					T *tmp=new T;
					tmp->dim=atoi(Token.word);
					n=n*tmp->dim;
					tmp->next=NULL;
					item->u.var->dim_list=tmp;
				}
				else{
					T* p;
					for(p=item->u.var->dim_list;p->next!=NULL;p=p->next);
					p->next=new T;
					p->next->next=NULL;
					p->next->dim=atoi(Token.word);
					n=n*p->next->dim;
				}
				//cout<<"-----"<<n<<endl;
				item->u.var->dim_num++;
				match(NUM);
				if(Token.type==COMMA)
					match(COMMA);
				else
					break;
			}
			item->u.var->array_value=new double[n];
			//cout<<Token.word<<endl;
			match(RB);
			assert(item->u.var->dim_num!=0);
		}
		//cout<<">>>>>>>>>>>>>>>"<<endl;
		match(SEMICOLON);
		if(decls[i]==NULL){
			decls[i]=new DeclarationList;
			decls[i]->declaration=item;
			decls[i]->next=NULL;
			//gen_help[num_gen_help++]=decls[i];
		}
		else{
			DeclarationList* p=new DeclarationList;
			p->declaration=item;
			p->next=decls[i];
			decls[i]=p;
			//gen_help[num_gen_help++]=p;
			/*for(p=decls[i];p->next!=NULL;p=p->next);
			p->next=new DeclarationList;
			p->next->declaration=item;
			p->next->next=NULL;*/
		}

		gen_help[num_gen_help++]=decls[i];
		//cout<<"var: "<<gen_help[num_gen_help-1]->declaration->u.var->name<<endl;
	}
	
	else if(Token.type==LP){
		i=hash(t);
		//cout<<"----------"<<t<<endl;
		flag=0;
		FunctionDefinition* fd=NULL;
		for(dl=decls[i];dl!=NULL;dl=dl->next){
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t)){
					cerr<<"variable "<<t<<" has defineded"<<endl;
					exit(1);
				}
			}
			else if(dl->declaration->type==FUNCTION){
				if(!strcmp(dl->declaration->u.fun->name,t)&&dl->declaration->u.fun->block!=NULL){
					cerr<<"function "<<t<<" redefined"<<endl;
					exit(1);
				}
				if(!strcmp(dl->declaration->u.fun->name,t)&&dl->declaration->u.fun->block==NULL){
					fd=dl->declaration->u.fun;
					flag=1;
					gen_help[num_gen_help++]=dl;
				}
			}
		}
		match(LP);
		if(fd==NULL){
			fd=new FunctionDefinition;
			//add to gen_help
			fd->parameter=NULL;
			fd->type=rt;
			fd->block=NULL;
			fd->parameter_num=0;
		}
		assert(fd->type==rt);
		if(fd->parameter!=NULL){
			ParameterList *item=fd->parameter;
			while(item!=NULL){
					ParameterList *p=item->next;
					delete item;
					item=p;
			}
		}
		
		fd->name=t;
		fd->parameter=NULL;
		//fd->next=NULL;
		fd->block=NULL;
		fd->level=0;
		//cout<<Token.word<<endl;
		Declaration* item=new Declaration;
		item->type=FUNCTION;
		item->u.fun=fd;
		if(flag==0){
			if(decls[i]==NULL){
				decls[i]=new DeclarationList;
				decls[i]->declaration=item;
				decls[i]->next=NULL;
			}
			else{
				DeclarationList* p=new DeclarationList;
				p->declaration=item;
				p->next=decls[i];
				decls[i]=p;
				/*for(p=decls[i];p->next!=NULL;p=p->next);
				p->next=new DeclarationList;
				p->next->declaration=item;
				p->next->next=NULL;*/
			}
			gen_help[num_gen_help++]=decls[i];
		}
		//gen_help[num_gen_help++]=decls[i];
		//cout<<"++++++++"<<decls[i]->declaration->u.fun->name<<endl;
		//cout<<"fun: "<<gen_help[num_gen_help-1]->declaration->u.fun->name<<endl;
		Block* b=new Block;
		fd->block=b;
		creat_parameList(fd);
		if(flag==1)
			if(fd->parameter!=NULL)
				assert(fd->parameter_num==fd->parameter->parame_num);
			else
				assert(fd->parameter_num==0);

		if(fd->parameter!=NULL)
			fd->parameter_num=fd->parameter->parame_num;
		//cout<<">>>>>>>>>>>>>>>"<<endl;
		match(RP);
		match(LC);
		//assert(fd->parameter->parame_num)
		b->type=FUNCTION_BLOCK;
		b->outer_block=NULL;
		b->fun=fd;
		//b->declaration_list=NULL;
		b->statement_list=NULL;
		/*this implements cannot support recursive call*/
		//creat_block(fd->parameter,b);
		
		//cout<<">>>>>>>>>>>>>>>"<<endl;
		//block
		//match(RC);
		/*if(function_list==NULL)
		function_list=fd;
		else{
			FunctionDefinition* p;
			for(p=function_list;p->next!=NULL;p=p->next);
			p->next=fd;
		}*/


		/*in order to support recursive call*/
		creat_block(fd,fd->parameter,b);
		for(int i=0;i<SIZE;i++){
			DeclarationList* p=decls[i];
			while(p!=NULL){
				Declaration* d=p->declaration;
				if(d->type==FUNCTION||d->u.var->b!=b)
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
	}
	/*else if(Token.type==LB){
	match
	}*/
	else
		cerr<<"syntax error"<<endl;
}


void creare_declaration_list(){

	char *t=NULL;
	int i,flag;
	DeclarationList* dl;
	FunctionDefinition* fd=NULL;
	Declaration* item=NULL;
	TypeSpecifier rt;

	while(Token.type==TYPE||Token.type==EXTERN){

		fd=NULL;
		switch(Token.type){

		case TYPE:
		create_declaration();
		break;


		case EXTERN:
		match(EXTERN);
		rt=CheckType();
		match(TYPE);
		t=Token.word;
		i=hash(t);
		flag=0;
		match(ID);
		for(dl=decls[i];dl!=NULL;dl=dl->next){
			if(dl->declaration->type==VARIABLE){
				if(!strcmp(dl->declaration->u.var->name,t)){
					cerr<<"variable "<<t<<" has defineded"<<endl;
					exit(1);
				}
			}

			else if(dl->declaration->type==FUNCTION){
				/*if(!strcmp(dl->declaration->u.fun->name,t)&&dl->declaration->u.fun->block!=NULL){
					cerr<<"function "<<t<<" redefined"<<endl;
					exit(1);
				}
				if(!strcmp(dl->declaration->u.fun->name,t)&&dl->declaration->u.fun->block==NULL){
					fd=dl->declaration->u.fun;
					flag=1;
				}*/
			if(!strcmp(dl->declaration->u.fun->name,t)){
					cerr<<"function "<<t<<" redefined"<<endl;
					exit(1);
				}
			}
		}

		if(fd==NULL){
			fd=new FunctionDefinition;
			// add to gen_help
			fd->block=NULL;
			fd->parameter=NULL;
			fd->level=0;
			fd->type=rt;
			fd->parameter_num=0;
		}

		if(fd->parameter!=NULL){
			ParameterList *item=fd->parameter;
			while(item!=NULL){
					ParameterList *p=item->next;
					delete item;
					item=p;
			}
		}

		fd->block=NULL;
		fd->parameter=NULL;
		item=new Declaration;
		item->type=FUNCTION;
		item->u.fun=fd;
		if(flag==0)
			if(decls[i]==NULL){
				decls[i]=new DeclarationList;
				decls[i]->declaration=item;
				decls[i]->next=NULL;
			}

			else{
				DeclarationList* p=new DeclarationList;
				p->declaration=item;
				p->next=decls[i];
				decls[i]=p;
				/*for(p=decls[i];p->next!=NULL;p=p->next);
				p->next=new DeclarationList;
				p->next->declaration=item;
				p->next->next=NULL;*/
			}
		fd->name=t;

//fd->parameter->parame_num=0;

			
			match(LP);
			while(Token.type!=RP){
				TypeSpecifier rt=CheckType();
				match(TYPE);
				if(rt==VOID)
					break;

				else{
					ParameterList *item =new ParameterList;
					item->type=rt;
					item->name=Token.word;
					//item->b=fd->block;
					fd->parameter_num++;
					match(ID);
					/*if(Token.type==LB){
						match(LB);
						match(RB);
					}*/
					item->next=NULL;
					if(fd->parameter!=NULL){
						fd->parameter->parame_num++;
						ParameterList *p;
						for(p=fd->parameter;p->next;p=p->next);
						p->next=item;
					}
					else{
						fd->parameter=item;
						fd->parameter->parame_num=1;
					}
				fd->parameter_num=fd->parameter->parame_num;
				}

				if(Token.type==COMMA)
					match(COMMA);
				else
					break;
			}
			match(RP);
			match(SEMICOLON);
			break;

			default:
			cerr<<"unknow declration"<<endl;
			break;



		}


	}
}



