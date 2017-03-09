#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum{
	START,
	HASH,
	COMMENT,
	SINGLE_LINE,
	MULTI_LINE,
	POTENTIAL_MULTI_END,
	STRING_LITERAL,
	CHARACTER_LITERAL
}State;

typedef enum {A_,A,B,C,D,E}NonTerminal;
char* nonTerminals[] = {};
const int nonTerminalSize = 0;

typedef enum {MAIN,L_PR,R_PR,L_CB,R_CB,SC,INT,CHAR,ID,NUM,COMMA,EQ,DOL}Terminal;
char* terminal[] = {"main","(",")","{","}",";","int","char","id","num",",","=","$"};
const int terminalSize = 10;

typedef enum{Int,Double,Char,Float,Void,IntPtr,DoublePtr,CharPtr,FloatPtr,VoidPtr,KEYWORD,FUNCTION,IDENTIFIER,TERMINAL,LITERAL_STRING,LITERAL_CHARACTER,LITERAL_NUMERIC}Type;
typedef struct{
	char lexeme[20];
	Type type;
}Token;

void pushToken(Token t, Token tokens[], int *count){
	tokens[(*count)++] = t;
}

void insertToken(Token tokens[], int* tokenCount, Type type, char* lexeme, int lexemeSize){
	Token t;
	t.type = type;
	if(lexemeSize == -1){
		strcpy(t.lexeme,lexeme);
	}
	else{
		for(int i=0;i<lexemeSize; i++)
			t.lexeme[i] = lexeme[i];
		t.lexeme[lexemeSize] = '\0';
	}
	pushToken(t,tokens,tokenCount);
}

void printToken(Token token){
	printf("{type: %d, lexeme: %s}",token.type,token.lexeme);
}

void printTokens(Token tokens[], int count){
	for(int i=0;i<count;i++){
		printf("%d ",i);
		printToken(tokens[i]);
		printf("\n");
	}
}

int matchesListPostFix(char* list[], int size, char* s, char* postFix){
	for(int i=0;i<size;i++){
		char tmp[50];
		strcpy(tmp,list[i]);
		strcat(tmp,postFix);
		if(strcmp(s,tmp)==0)
			return i;
	}
	return -1;
}

int matchesList(char* list[], int size, char* s){
	return matchesListPostFix(list,size,s,"");
}

int matchesId(char* s){
	int i=0;
	if(s[0] != '_' && !isalpha(s[0]))
		return -1;
	for(i=0;s[i]!='\0' && (s[i] == '_' || isalnum(s[i]));i++){
	}
	return i;
}

NonTerminal matchesNonTerminals(char *s){
	int i=0;
	if(s[0] != '_' && !isalpha(s[0]))
		return -1;
	int n = matchesId(s);
	char s2[100];
	strcpy(s2,s);
	s2[n] = '\0';
	return matchesList(nonTerminals,nonTerminalSize,s2);
}

Terminal matchesTerminal(char* s){
	return matchesList(terminal,terminalSize,s);
}

void print(char* s, int n){
	for(int i=0;i<n;i++){
		printf("%c",s[i]);
	}
}

int matchesNumericLiteral(char* s){
	int i=0;
	if(!isdigit(s[0]))
		return -1;
	for(i=0;s[i]!='\0' && (isdigit(s[i]));i++){
	}
	return i;
}

void clearBuf(int* ptr, char* s, int n){
	if(n==-1){
		(*ptr)=0;
		s[0]='\0';
		return;
	}
	int i;
	for(i=n;s[i];i++){
		s[i-n] = s[i];
	}
	(*ptr)-=n;
	s[i-n] = '\0';
}

void printAndClear(char* s, int *ptr, int n){
	print(s,n);
	clearBuf(ptr,s,n);
}

int isWhiteSpace(char c){
	return c==' ' || c=='\t' || c=='\n';
}

void getTokenStream(Token tokens[], int *tokenCount){
	State state = START, prevState = START;
	char s[100];
	s[0] = '\0';
	int ptr = 0;
	char c;
	while(scanf("%c",&c)>0 && c!='$'){

		prevState = state;

		switch(state){
			case START:
				if(c=='#')
					state = HASH;
				else if(c=='/')
					state = COMMENT;
				else if(c=='"')
					state = STRING_LITERAL;
				else if(c=='\'')
					state = CHARACTER_LITERAL;
				break;

			case HASH:
				if(c=='\n')
					state = START;
				break;

			case COMMENT:
				if(c=='/')
					state = SINGLE_LINE;
				else if(c=='*')
					state = MULTI_LINE;
				break;
			case SINGLE_LINE:
				if(c=='\n')
					state = START;
				break;
			case MULTI_LINE:
				if(c=='*')
					state = POTENTIAL_MULTI_END;
				break;
			case POTENTIAL_MULTI_END:
				if(c=='/')
					state = START;
				break;

			case STRING_LITERAL:
				if(c=='"')
					state = START;
				break;
			case CHARACTER_LITERAL:
				if(c=='\'')
					state = START;
				break;
		}

		//printf("(+%c)",c);

		switch(state){
			case START:
				if(!isWhiteSpace(c) && c !='/' && prevState != STRING_LITERAL && prevState != CHARACTER_LITERAL){
					s[ptr++]=c;
					s[ptr]='\0';
					//printf("(-%s)",s);
				}
				break;
			case STRING_LITERAL: case CHARACTER_LITERAL:
				s[ptr++]=c;
				s[ptr]='\0';
				break;
		}
		if(c!='/' && (state == START)){
			char cStr[] = {c,'\0'};

			//printf("\n(%s),%d\n",s,ptr);
			if(isWhiteSpace(c) || matchesTerminal(cStr)!=-1 || matchesTerminal(s)!=-1
			   || prevState==STRING_LITERAL || prevState == CHARACTER_LITERAL){
				int i=0;
				if(ptr>0)
					;//printf("\n(%s),%d\n",s,ptr);
				if(prevState==STRING_LITERAL){
					insertToken(tokens,tokenCount,LITERAL_STRING,s+1,-1);
					printf("<l%d %s\"\t> ",prevState,s);
					clearBuf(&ptr,s,-1);
				} else if(prevState == CHARACTER_LITERAL){
					insertToken(tokens,tokenCount,LITERAL_CHARACTER,s+1,-1);
					printf("<l%d %s\'\t> ",prevState,s);
					clearBuf(&ptr,s,-1);
				}else if(matchesNonTerminals(s)!=-1){
					insertToken(tokens,tokenCount,KEYWORD,s,-1);
					printf("<%s>\t",s);
					clearBuf(&ptr,s,-1);
				}else if((i=matchesId(s))!=-1 && 0){
					insertToken(tokens,tokenCount,IDENTIFIER,s,i);
					printf("<id ");printAndClear(s,&ptr,i);printf(">\t");
				}else if((i=matchesNumericLiteral(s))!=-1){
					insertToken(tokens,tokenCount,LITERAL_NUMERIC,s,i);
					printf("<li ");printAndClear(s,&ptr,i);printf(">\t");
				}else{
					//printf("===============");
				}
				if(matchesTerminal(s)!=-1){
					insertToken(tokens,tokenCount,TERMINAL,s,-1);
					printf("<sp %s>\t",s);
					clearBuf(&ptr,s,-1);
				}
			}
		}else{
			switch(state){
				case HASH:
				case STRING_LITERAL:
				case CHARACTER_LITERAL:
				case COMMENT: case SINGLE_LINE: case MULTI_LINE: case POTENTIAL_MULTI_END:
					break;
				default:
					switch(prevState){
						case SINGLE_LINE: case POTENTIAL_MULTI_END:
							break;
						default:
							printf("%c",c);
					}
			}
		}
	}
	s[0]='$';s[1]='\0';
	insertToken(tokens,tokenCount,TERMINAL,s,-1);
}