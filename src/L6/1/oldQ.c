#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <L6/1/oldTokenizer.h>
#include <stdlib.h>

Terminal getTerminal(Token token){
	return matchesTerminal(token.lexeme);
}

typedef struct{
	Terminal list[200];
	int count;
	Terminal* curr;
}TerminalStream;

void populateTerminalStream(TerminalStream* terminals, Token tokens[], int tokenCount){
	terminals->count = 0;
	for(int i=0;i<tokenCount;i++)
		terminals->list[terminals->count++] = getTerminal(tokens[i]);
}

void printMatched(char* s, int nestCount){
	for(int i=0;i<nestCount;i++)	printf(" ");
	printf("Matched %s\n",s);
}

void printFailed(char* s, int nestCount){
	for(int i=0;i<nestCount;i++)	printf(" ");
	printf("Failed %s\n",s);
}

void printState(Keyword nonTerminal, int nestCount){
	for(int i=0;i<nestCount;i++)	printf(" ");
	printf("State ");
	switch(nonTerminal){
		case S_:	printf("S_");	break;
		case S1:	printf("S1");	break;
		case S2:	printf("S2");	break;
		case S3:	printf("S3");	break;
		case T :	printf("T");	break;
		case T_:	printf("T_");	break;
		case T_1:	printf("T_1");	break;
		case T_2:	printf("T_2");	break;
		default:	printf("?");	break;
	}
	printf("\n");
}

int fS(TerminalStream* terminals);

int fS1(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	if(*terminals->curr == a){
		terminals->curr++;
		return 1;
	}
	terminals->curr = backup;
	return 0;
}

int fS2(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	if(*terminals->curr == MINU){
		terminals->curr++;
		return 1;
	}
	terminals->curr = backup;
	return 0;
}

int fT_(TerminalStream* terminals);

int fT_1(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	if(*terminals->curr == COMMA){
		terminals->curr++;
		int s = fS(terminals);
		if(s){
			int t_ = fT_(terminals);
			if(t_){
				return 1;
			}
		}
		return 1;
	}
	terminals->curr = backup;
	return 0;
}

int fT_2(TerminalStream* terminals){
	return 1;
}

int fT_(TerminalStream* terminals){
	int t_;
	t_ = fT_1(terminals);	if(t_)	return 1;
	t_ = fT_2(terminals);	if(t_)	return 1;
	return 0;
}

int fT(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	int s = fS(terminals);
	if(s) {
		int t_ = fT_(terminals);
		if(t_){
			return 1;
		}
	}
	terminals->curr = backup;
	return 0;
}

int fS3(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	if(*terminals->curr == L_PR){
		terminals->curr++;
		int t = fT(terminals);
		if(t){
			if(*terminals->curr == R_PR){
				terminals->curr++;
				return 1;
			}
		}
	}
	terminals->curr = backup;
	return 0;
}

int fS(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	int s;
	s = fS1(terminals);	if(s)	return s;
	s = fS2(terminals);	if(s)	return s;
	s = fS3(terminals);	if(s)	return s;
	terminals->curr = backup;
	return 0;
}

int fS_(TerminalStream* terminals){
	Terminal* backup = terminals->curr;
	int s = fS(terminals);
	if(s){
		if(*terminals->curr == DOL){
			terminals->curr++;
			return 1;
		}
	}
	terminals->curr = backup;
	return 0;
}

int getLanguageMembership(TerminalStream* terminals){
	return fS_(terminals);
}

int main(){
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens,&tokenCount);
	printf("\nTokens:\n");
	printTokens(tokens,tokenCount);

	TerminalStream terminals;
	terminals.count = 0;
	terminals.curr = terminals.list;
	populateTerminalStream(&terminals, tokens, tokenCount);
	for(int i=0;i<terminals.count;i++)
		printf("%d",getTerminal(tokens[i]));
	printf("\n");
	int isMember = getLanguageMembership(&terminals);
	printf("\nThe membership of the string is %d\n",isMember);
	return 0;
}
