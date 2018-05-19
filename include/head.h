#ifndef _HEAD_H_
#define _HEAD_H_
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#define SIZE 49999
using namespace std;


enum KeyWords{
	UNKNOWKW,IF,ELSE,WHILE,RETURN,LP, RP, LC, RC, SEMICOLON, COLON, COMMA, ASSIGN_T,
	EQ, NE, GT, GE, LT, LE, ADD, SUB, MUL, DIV, MOD, TRUE_T, FALSE_T, ID, TYPE, NUM,
	AND, OR, NOT, LB, RB, BIT_AND, BIT_OR, BREAK, CONTINUE,BEGIN,END,EXTERN
};

enum TypeSpecifier{
	UNKONWT,INT,DOUBLE,FLOAT,VOID,BOOLEAN,FUNCTION,VARIABLE
};


typedef enum {
	NEGATIVE_EXPRESSION,
	BUILT_IN_FUNCTION,
	BOOLEAN_EXPRESSION,
	INT_EXPRESSION,
	DOUBLE_EXPRESSION,
	STRING_EXPRESSION,
	IDENTIFIER_EXPRESSION,
	COMMA_EXPRESSION,
	ASSIGN_EXPRESSION,
	ADD_EXPRESSION,
	SUB_EXPRESSION,
	MUL_EXPRESSION,
	DIV_EXPRESSION,
	MOD_EXPRESSION,
	EQ_EXPRESSION,
	NE_EXPRESSION,
	GT_EXPRESSION,
	GE_EXPRESSION,
	LT_EXPRESSION,
	LE_EXPRESSION,
	ARRAY_EXPRESSION,
	BINARY,
	LOGICAL_AND_EXPRESSION,
	LOGICAL_OR_EXPRESSION,
	MINUS_EXPRESSION,
	LOGICAL_NOT_EXPRESSION,
	FUNCTION_CALL_EXPRESSION,
	INCREMENT_EXPRESSION,
	DECREMENT_EXPRESSION,
	CAST_EXPRESSION,
	EXPRESSION_KIND_COUNT_PLUS_1,
	PARENTHESES_EXPRESSION

}ExpressionKind;



struct CToken{
	char* word;
	KeyWords type;
	int LineNO;

	CToken(){
		word=NULL;
		type=UNKNOWKW;
		LineNO=0;
	}

	CToken(const char* w,KeyWords t,int n){
		word=new char[25];
		strcpy(word,w);
		type=t;
		LineNO=n;
	}

	CToken(const CToken& t){
		word=new char[25];
		strcpy(word,t.word);
		type=t.type;
		LineNO=t.LineNO;
	}

	bool operator==(CToken& t){
		return strcmp(word,t.word)==0&&type==t.type;
	}

	bool operator!=(CToken& t){
		return strcmp(word,t.word)!=0||type!=t.type;
	}

	CToken& operator=(CToken& t){
		word=new char[25];
		strcpy(word,t.word);
		type=t.type;
		LineNO=t.LineNO;
		return *this;
	}

};


struct B{
	const char* word;
	KeyWords type;
};

typedef struct Block_tag Block;

typedef struct ParameterList_tag {
    char                *name;
    TypeSpecifier       type;
    int                 parame_num;
		double 							value;
    struct ParameterList_tag *next;
		Block * b;
		int level;
} ParameterList;

struct T{
	int dim;
	struct T* next;
};


struct variable{
	TypeSpecifier type;
	double value;
	double *array_value;
  char        *name;
	struct T* dim_list,*base_list;
	int dim_num;
	Block * b;
	int level;
};

typedef struct E Declaration;

typedef struct DeclarationList_tag {
    Declaration *declaration;
    struct DeclarationList_tag *next;
} DeclarationList;


struct Expression_tag;


typedef struct {
    struct Expression_tag  *left;
    struct Expression_tag  *right;
} BinaryExpression;

typedef struct {
		int is_array;
		char *index;
    struct Expression_tag  *left;
    struct Expression_tag  *right;
} AssignExpression;

typedef struct ArgumentList_tag {
    struct Expression_tag *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct {
    char     *function_name;
    ArgumentList                   *argument;
} FunctionCallExpression;


typedef struct{
	char  *array_name, *index;
}ArrayExpression;

struct R{
	struct Expression_tag* exp_s;
	struct R* next;
};

typedef struct{
	char *name;
	//TypeSpecifier type;
	int dim_num;
	struct R* index_list;
	//double *value;
}IDExpression;


typedef struct Expression_tag {
    //TypeSpecifier *type;
    ExpressionKind kind;
    //int line_number;
    union {
        bool            boolean_value;
        int                     int_value;
        double                  double_value;
        //DVM_Char                *string_value;
        //IdentifierExpression    identifier;
				IDExpression*           id_expression;
        FunctionCallExpression  *function_call_expression;
        //CommaExpression         comma;
        AssignExpression        *assign_expression;
        BinaryExpression        *binary_expression;
        //Expression              *minus_expression;
        //Expression              *logical_not;

				//unused below
				char *id;
        ArrayExpression         *array_expression;
        //CastExpression          cast;
    } u;
}Expression;


struct StatementList_tag;


typedef struct StatementList_tag StatementList;

typedef enum {
    UNDEFINED_BLOCK = 1,
    FUNCTION_BLOCK,
    WHILE_STATEMENT_BLOCK,
    IF_STATEMENT_BLOCK,
		ELSE__STATEMENT_BLOCK,
		EXE_BLOCK
} BlockType;

struct label{
	char type;
	struct label* next;
};

typedef struct FunctionDefinition_tag FunctionDefinition;

typedef struct Block_tag {
    BlockType           type;
    struct Block_tag    *outer_block;
    StatementList       *statement_list;
    DeclarationList     *declaration_list;
		//struct label *label_return, *label_continue, *label_break;
    /*union {
        StatementBlockInfo      statement;
        FunctionBlockInfo       function;
    } parent;*/
		FunctionDefinition* fun;
} Block;


typedef enum {
    EXPRESSION_STATEMENT = 1,
		DECL_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    ELSE_STATEMENT,
    FOREACH_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    TRY_STATEMENT,
    THROW_STATEMENT,
    DECLARATION_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;
//struct Statement_tag;


typedef struct {
    Expression  *condition;
    Block       *then_block;
    //struct Statement_tag   *then_statement;
    Block       *else_block;
		//struct Statement_tag   *else_statement;
} IfStatement;

typedef struct {
    //char        *label;
    Expression  *condition;
    Block       *block;
} WhileStatement;


typedef struct {
    Expression *return_value;
} ReturnStatement;


typedef struct {
    Block *break_block;
} BreakStatement;


typedef struct {
    Block *continue_block;
} ContinueStatement;



typedef struct Statement_tag {
    StatementType       type;
    //int                 line_number;
    union {
				struct variable *decl_s;
        Expression      *expression_s;
        IfStatement     *if_s;
        WhileStatement  *while_s;
        ReturnStatement *return_s;
        BreakStatement  *break_s;
        ContinueStatement   *continue_s;
    } u;
}Statement;

typedef struct StatementList_tag {
    Statement   *statement;
    struct StatementList_tag    *next;
} StatementList;

typedef struct FunctionDefinition_tag {
    TypeSpecifier       type;
		//bool is_returned;//to determin whether has return value;
		union{
			int value_int;
			bool value_bool;
			float value_float;
			double value_double;
		}u;//get returned value from union u
    char                *name;
    ParameterList       *parameter;
		int parameter_num;
    Block               *block;
    //Declaration         **local_variable;
    //struct FunctionDefinition_tag       *next;
		int level;
}FunctionDefinition;


typedef struct E{
		TypeSpecifier type;
		union{
    	struct variable* var;
			FunctionDefinition* fun;
		}u;
} Declaration;




extern int hash(char* key);
extern TypeSpecifier CheckType();
extern void scan(FILE* fp);
extern void getToken();
extern void match(KeyWords k);
extern void create_declaration();
extern void creare_declaration_list();
extern Expression* expression(ParameterList *parameter,Block* block);
extern Expression* simple_expression(ParameterList *parameter,Block* block);
extern Expression* additive_expression(ParameterList *parameter,Block* block);
extern Expression* term(ParameterList *parameter,Block* block);
extern Expression* factor(ParameterList *parameter,Block* block);
extern void unGetToken();
extern void creat_block(FunctionDefinition* fd,ParameterList *parameter,Block* block);
extern void creat_stmt(FunctionDefinition* fd,ParameterList *parameter,Block* block);
extern bool id_search(char* s,DeclarationList *dl);
extern bool fun_search(char* s);
extern bool para_search(ParameterList *para,char *s);
extern bool expression_id_fun_search(ParameterList *parameter,Block* block,char *s);
extern int count_para(ParameterList *parameter);
extern void print();
extern void print_block(Block *block);
extern void print_block_statements(StatementList *stl);
extern void print_statement(Statement *st);
extern void TypeSpecifier_print(TypeSpecifier type);
extern void BlockType_print(BlockType b);
extern Expression* unary(ParameterList *parameter,Block* block);
extern Expression* term(ParameterList *parameter,Block* block);
extern Expression* equality(ParameterList *parameter,Block* block);
extern Expression* join(ParameterList *parameter,Block* block);
extern Expression* boolbool(ParameterList *parameter,Block* block);
extern void print_expression(Expression* exp);
extern FunctionDefinition* Find_function(char* s);


//function for exe below


extern int exe(FunctionDefinition* fd,Block* blk);
extern Block* getexe(FunctionDefinition* fd,ParameterList *parameter,Block* block);
extern int exe_while(FunctionDefinition* fun,Block* blk,Statement* s);
extern int exe_if(FunctionDefinition* fun,Block* blk,Statement* s);
extern double exe_exp(FunctionDefinition* fun,Block* blk,Expression* e);


//gen machine code
//unimplements



extern void gen(FunctionDefinition* fd,Block* blk,char * label);

//check type


extern void checkType();
extern void check_block(FunctionDefinition* fd,Block* blk);
extern void checkExp(FunctionDefinition* fd,Expression *exp);


#endif


