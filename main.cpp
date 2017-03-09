#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <tokenizer.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

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

Terminal getNextTerminal(TerminalStream* terminals){
	return *(terminals->curr++);
}

void retract(TerminalStream* terminals){
	terminals->curr--;
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

int rd(TerminalStream*,Terminal,Keyword,int);

typedef enum{S_S,T_S,T__S,T_T_,T__S_S}Callback;

int callback(TerminalStream*, Terminal, Callback, int);

int forany(TerminalStream* terminals, Terminal terminal, Keyword nonTerminal,Callback callbackId, int nestCount){
	int start,end;
	switch(nonTerminal){
		case S:
			start = S1;
			end = S3;
			break;
		case T_:
			start = T_1;
			end = T_2;
			break;
		default:
			printf("WARNING Unknown non terminal %d specified to forany!\n", nonTerminal);
			exit(0);
	}
	for(int i=start;i<=end;i++){
		if(rd(terminals,terminal,(Keyword)i,nestCount)){
			if(callback(terminals,terminal,callbackId,nestCount))	return 1;
		}
	}
	return 0;
}

int callback(TerminalStream* terminals, Terminal terminal, Callback callbackId, int nestCount){
	switch(callbackId){
		case S_S:
			printMatched("S",nestCount);
			if(getNextTerminal(terminals)==DOL){
				printf("Matched $");
				return 1;
			}
			break;
		case T_S:
			printMatched("S",nestCount);
			terminal = getNextTerminal(terminals);
			if(forany(terminals,terminal,T_,T_T_,nestCount)){
				return 1;
			}
			break;
		case T__S:
			printMatched("S",nestCount);
			//terminal = getNextTerminal(terminals);
			if(forany(terminals,terminal,T_,T__S_S,nestCount)){
				return 1;
			}
			break;
		case T_T_:
		case T__S_S:
			printMatched("T_",nestCount);
			return 1;
		default:
			printf("WARNING Unknown callback specified!\n");
	}
	retract(terminals);
	return 0;
}

int rd(TerminalStream* terminals,Terminal terminal,Keyword nonTerminal, int nestCount){
	nestCount++;
	printState(nonTerminal, nestCount);
	switch(nonTerminal){
		case S_:
			if(forany(terminals,terminal,S,S_S,nestCount)){
				return 1;
			}
			printFailed("S_",nestCount);
			return 0;
		case S1:
			if(terminal==a){
				printMatched("a",nestCount);
				return 1;
			}
			printFailed("S1",nestCount);
			return 0;
		case S2:
			if(terminal==MINU){
				printMatched("->",nestCount);
				return 1;
			}
			printFailed("S2",nestCount);
			return 0;
		case S3:
			if(terminal==L_PR){
				printMatched("(",nestCount);
				if(rd(terminals,getNextTerminal(terminals),T,nestCount)){
					printMatched("T",nestCount);
					if(getNextTerminal(terminals)==R_PR){
						printMatched(")",nestCount);
						return 1;
					}
					retract(terminals);
				}
				retract(terminals);
			}
			printFailed("S3",nestCount);
			return 0;
		case T:
			if(forany(terminals,terminal,S,T_S,nestCount)){
				return 1;
			}
			printFailed("T",nestCount);
			return 0;
		case T_1:
			if(terminal==COMMA){
				printMatched(",",nestCount);
				Terminal terminal = getNextTerminal(terminals);
				if(forany(terminals,terminal,S,T__S,nestCount))	return 1;
				retract(terminals);
			}
			printFailed("T_",nestCount);
			return 0;
		case T_2:
			printMatched("`",nestCount);
			return 1;
		default:
			printf("WARNING Unknown non terminal %d specified!\n",nonTerminal);
	}
}

int getLanguageMembership(TerminalStream* terminals){
	Terminal terminal = getNextTerminal(terminals);
	return rd(terminals,terminal,S_,0);
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