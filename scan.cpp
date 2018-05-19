#include <head.h>
//#include <QTextEdit>
//#include "mainwindow.h"
//#include "ui_mainwindow.h"
extern vector<CToken> InputSequence;
extern int next;
extern CToken Token;


struct B ReservedWords[]={
	{"if",IF},{"else",ELSE},{"while",WHILE},{"return",RETURN},{"(",LP},{")",RP},{"{",LC},{"}",RC},
	{";",SEMICOLON},{",",COMMA},{"=",ASSIGN_T},{"==",EQ},{"<",LT},{"<=",LE},{">",GT},{">=",GE},{"!=",NE},{"+",ADD},
	{"-",SUB},{"*",MUL},{"/",DIV},{"int",TYPE},{"float",TYPE},{"double",TYPE},{"void",TYPE},{"boolean",TYPE},{"[",LB},
	{"]",RB},{"&&",AND},{"||",OR},{"!",NOT},{"&",BIT_AND},{"|",BIT_OR},{"break",BREAK},{"continue",CONTINUE},
	{"begin",BEGIN},{"end",END},{"extern",EXTERN},{"%",MOD}
};


bool check(char *s,int n){
	for(int i=0;i<sizeof(ReservedWords)/sizeof(ReservedWords[0]);i++)
		if(!strcmp(s,ReservedWords[i].word)){
			InputSequence.push_back(CToken(s,ReservedWords[i].type,n));
			return true;
		}
	return false;
}


const char* Expect(KeyWords type){
	int i;
	if(type==NUM)
		return "num";
	else if(type==ID)
		return "id";
	else if(type==TYPE)
		return "Type: int boolean double float void(function)";
	else{
		for(i=0;i<sizeof(ReservedWords)/sizeof(ReservedWords[0]);i++)
			if(ReservedWords[i].type==type)
				return ReservedWords[i].word;
	}
	cerr<<"unknow keywords"<<endl;
	exit(1);
	return NULL;
}


void scan(FILE* fp){
	char c;
	char s[25];
	int index,n=1;
    string s=ui->t
	while((c=getc(fp))!=EOF){
		if(c=='\t'||c==' ')
			;
		else if(c=='\n')
			n++;

		else if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'){
			index=0;
			do{
				s[index++]=c;
				c=getc(fp);
			}
			while((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_')||(c>='0'&&c<='9'));
			s[index]=0;
			ungetc(c,fp);
			if(!check(s,n))
				InputSequence.push_back(CToken(s,ID,n));
		}

		else if(c>='0'&&c<='9'){
			index=0;
			do{
				s[index++]=c;
				c=getc(fp);
			}
			while(c>='0'&&c<='9');
			s[index]=0;
			ungetc(c,fp);
			InputSequence.push_back(CToken(s,NUM,n));
		}

		else if(c=='='){
			c=' ';
			if((c=getc(fp))=='=')
			InputSequence.push_back(CToken("==",EQ,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken("=",ASSIGN_T,n));
			}
		}

		else if(c=='&'){
			c=' ';
			if((c=getc(fp))=='&')
				InputSequence.push_back(CToken("&&",AND,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken("&",BIT_AND,n));
			}
		}

		else if(c=='|'){
			c=' ';
			if((c=getc(fp))=='|')
				InputSequence.push_back(CToken("||",OR,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken("|",BIT_OR,n));
			}
		}

		else if(c=='<'){
			c=' ';
			if((c=getc(fp))=='=')
				InputSequence.push_back(CToken("<=",LE,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken("<",LT,n));
			}
		}

		else if(c=='>'){
			c=' ';
			if((c=getc(fp))=='=')
				InputSequence.push_back(CToken("==",GE,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken(">",GT,n));
			}
		}

		else if(c=='!'){
			c=' ';
			if((c=getc(fp))=='=')
				InputSequence.push_back(CToken("!=",NE,n));
			else{
				ungetc(c,fp);
				InputSequence.push_back(CToken("!",NOT,n));
			}
		}

		else switch(c){

			case '+':
			InputSequence.push_back(CToken("+",ADD,n));
			break;

			case '-':
			InputSequence.push_back(CToken("-",SUB,n));
			break;

			case '*':
			InputSequence.push_back(CToken("*",MUL,n));
			break;

			case '/':
			InputSequence.push_back(CToken("/",DIV,n));
			break;

			case '(':
			InputSequence.push_back(CToken("(",LP,n));
			break;

			case ')':
			InputSequence.push_back(CToken(")",RP,n));
			break;

			case '{':
			InputSequence.push_back(CToken("{",LC,n));
			break;

			case '}':
			InputSequence.push_back(CToken("}",RC,n));
			break;

			case ';':
			InputSequence.push_back(CToken(";",SEMICOLON,n));
			break;

			case ',':
			InputSequence.push_back(CToken(",",COMMA,n));
			break;

			case '[':
			InputSequence.push_back(CToken("[",LB,n));
			break;

			case ']':
			InputSequence.push_back(CToken("]",RB,n));
			break;

			case '%':
			InputSequence.push_back(CToken("%",MOD,n));
			break;

			default:
			fprintf(stderr,"lexical error\n");
			break;

		}
	}
}


void getToken(){
	if(++next<int(InputSequence.size()))
		Token=InputSequence[next];
}


void unGetToken(){
	if(--next>=0)
		Token=InputSequence[next];
}


void match(KeyWords k){
	if(k==Token.type){
		getToken();
	}

	else{
		fprintf(stderr,"match error in Line: %d\n",Token.LineNO);
		cerr<<"current word: "<<Token.word<<endl;
		cerr<<"expect  word: "<<Expect(k)<<endl;
		exit(1);
	}
}


