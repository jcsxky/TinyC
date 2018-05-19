#include <head.h>
extern vector<CToken> InputSequence;
extern int next;
extern CToken Token;
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern Block *exe_block;
extern DeclarationList     *decls[SIZE];

extern DeclarationList     *gen_help[SIZE];
extern int num_gen_help;


int main(){
	int i;
	/*for(i=0;i<SIZE;i++)
		decls[i]=NULL;*/
	memset(decls,0,sizeof(decls));
	cout<<"================================================="<<endl;
	FILE* fp=fopen("test.txt","r");
	scan(fp);
	if(InputSequence.size()>0)
		getToken();
	else{
		cerr<<"blank file"<<endl;
		exit(1);
	}
	exe_block=new Block;
	exe_block->type=EXE_BLOCK;
	exe_block->statement_list=NULL;
	exe_block->outer_block=NULL;
	exe_block->declaration_list=NULL;
	creare_declaration_list();

	FunctionDefinition *fd =new FunctionDefinition;
	exe_block->fun=fd;
	fd->name=(char*)"main";
	fd->type=INT;
	fd->level=0;
	fd->block=exe_block;
	fd->parameter=NULL;
	//assert(next==InputSequence.size());
	getexe(fd,fd->parameter,fd->block);
	if(next!=InputSequence.size()){
		cout<<"end file error"<<endl;
		exit(1);
	}
	cout<<"\n\n\ncompile right\n\n\n";
	//cout<<InputSequence.size()<<endl;
	cout<<"================================================="
			<<endl<<endl<<endl;
	checkType();
	

	cout<<endl<<endl<<endl<<"****************************************************"
												<<endl<<endl<<endl;
	for(i=0;i<SIZE;i++)
		for(DeclarationList* dl=decls[i];dl!=NULL;dl=dl->next)
			if(dl->declaration->type==VARIABLE)
				cout<<"variable: "<<dl->declaration->u.var->name<<" "
						<<dl->declaration->u.var->level<<endl;
			else if(dl->declaration->type==FUNCTION)
				cout<<"function: "<<dl->declaration->u.fun->name<<" "
						<<dl->declaration->u.fun->level<<endl;


	cout<<endl<<endl<<endl<<"****************************************************"<<endl;

	for(i=0;i<num_gen_help;i++){
		if(gen_help[i]->declaration->type==VARIABLE)
			cout<<"variable: "<<gen_help[i]->declaration->u.var->name<<endl;
		else if(gen_help[i]->declaration->type==FUNCTION)
			cout<<"function: "<<gen_help[i]->declaration->u.fun->name<<endl;
	}

	cout<<"=========================================================="<<endl<<endl;
//emit code for variables
	cout<<";variables below"<<endl;
	for(i=0;i<num_gen_help;i++){
		if(gen_help[i]->declaration->type==VARIABLE){
			struct variable *var=gen_help[i]->declaration->u.var;
			switch(var->type){
				case INT:
				printf("%s DD 0\n",var->name);
				break;
			}
		}
	}
	//code of functions
	cout<<";functions below"<<endl;
	for(i=0;i<num_gen_help;i++){
		if(gen_help[i]->declaration->type==FUNCTION){
			FunctionDefinition* fd=gen_help[i]->declaration->u.fun;
			//gen(fd,fd->block,NULL);
		}
	}


	DeclarationList* dl;
	for(i=0;i<SIZE;i++){
		DeclarationList* p=decls[i];
		while(p!=NULL){
			Declaration* d=p->declaration;
			if(d->type==FUNCTION||d->u.var->b!=fd->block)
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

	return 0;

}

