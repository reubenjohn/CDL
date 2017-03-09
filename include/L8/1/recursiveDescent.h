Terminal getNextTerminal(Token token) {
	return matchesTerminal(token.lexeme);
}

typedef struct {
	Terminal list[200];
	int count;
	Terminal *curr;
} TerminalStream;

void populateTerminalStream(TerminalStream *terminals, Token tokens[], int tokenCount) {
	terminals->count = 0;
	for (int i = 0; i < tokenCount; i++)
		terminals->list[terminals->count++] = getNextTerminal(tokens[i]);
	terminals->curr = terminals->list;
}

typedef enum {
	TERM_TERMINAL, TERM_NON_TERMINAL
} TermType;

typedef union {
	Terminal terminal;
	NonTerminal nonTerminal;
} TermData;

typedef struct {
	TermType type;
	TermData data;
} Term;

typedef struct {
	NonTerminal head;
	Term terms[20];
	int termCount;
} Production;

typedef struct {
	Production list[50];
	int count;
} Productions;

void printNesting(int nesting){
	for(int i=0;i<nesting;i++)	printf(" ");
}

void printState(int nesting, State state){
	printNesting(nesting);
	printf("State %c\n",'A'+state-1);
}

void printSuccess(int nesting, State state){
	printNesting(nesting);
	printf("Matched %c\n",'A'+state-1);
}

void printFailure(int nesting, State state){
	printNesting(nesting);
	printf("Failed %c\n",'A'+state-1);
}

int rd(TerminalStream *terminals, Productions productions, NonTerminal state, int nesting);

int tryProduction(TerminalStream *terminals, Productions productions, Production production, NonTerminal state, int nesting) {
	nesting++;
	if (production.termCount == 0) {
		return 1;
	}
	Terminal *backup = terminals->curr;
	for (int j = 0; j < production.termCount; j++) {
		Term term = production.terms[j];
			if(term.type == TERM_TERMINAL){
				printNesting(nesting);
				printf("Matching %d with %d\n",term.data.terminal,*(terminals->curr));
			}
		if (term.type == TERM_TERMINAL) {
			if (*terminals->curr != term.data.terminal) {
				terminals->curr = backup;
				return 0;
			} else {
				terminals->curr++;
				//Continue with next term
			}
		} else if (term.type == TERM_NON_TERMINAL) {
			if (!rd(terminals, productions, term.data.nonTerminal,nesting)) {
				terminals->curr = backup;
				return 0;
			} else {
				//terminals already incremented within rd pass by reference; continue with next term
			}
		}
	}
	return 1;
}

int rd(TerminalStream *terminals, Productions productions, NonTerminal state, int nesting) {
	printState(nesting, state);
	//printf("%d\n",productions.count);
	for (int i = 0; i < productions.count; i++) {
		Production production = productions.list[i];
		if (production.head == state) {
			printNesting(nesting);
			printf("Trying production: %d\n",i);
			int success = tryProduction(terminals, productions, production, state, nesting);
			if (success) {
				printSuccess(nesting, state);
				return 1;
			}else{
				printFailure(nesting, state);
			}
		}
		//printNesting(nesting);
		//printf("Irrelevant production: %d\n",production.head);
	}
	printState(nesting, state);
	return 0;
}

Term getNonT(NonTerminal nonTerminal) {
	Term term;
	term.type = TERM_NON_TERMINAL;
	term.data.nonTerminal = nonTerminal;
	return term;
}

Term getT(Terminal terminal) {
	Term term;
	term.type = TERM_TERMINAL;
	term.data.terminal = terminal;
	return term;
}

Production getProduction(NonTerminal nonTerminal, int termCount, Term terms[]) {
	Production production;
	production.head = nonTerminal;
	for (int i = 0; i < termCount; i++) {
		production.terms[i] = terms[i];
	}
	production.termCount = termCount;
	return production;
}

