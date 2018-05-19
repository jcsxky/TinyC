#include <head.h>
extern FunctionDefinition  *function_list;
extern DeclarationList     *declaration_list;
extern int next;
extern CToken Token;
#define CODESIZE 1024
extern vector<CToken> InputSequence;
extern Block *exe_block;
extern DeclarationList     *decls[SIZE];


void gen(FunctionDefinition* fd,Block* blk,char * label){
char code[CODESIZE];
if(blk==NULL)
	return ;
StatementList* sl;
Statement *s;
IfStatement     *if_s;
WhileStatement  *while_s;
Expression* exp;
for(sl=blk->statement_list;sl!=NULL;sl=sl->next){
s=sl->type;
switch(s){
case IF_STATEMENT:
if_s=s->u.if_s;
assert(if_s->condition->kind==INT_EXPRESSION);
exp=if_s->condition;
if(exp->int_value==0)
	printf("ldc false");
else
	printf("ldc true");













}
}
}

