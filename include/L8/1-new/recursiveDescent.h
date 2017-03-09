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
	Term terms[20];
	int termCount;
} Production;

typedef struct {
	NonTerminal head;
	int count;
	Production list[50];
} Productions;

typedef struct {
	Productions list[50];
	int count;
} Grammar;

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

Productions newProductions(NonTerminal nonTerminal) {
	Productions productions;
	productions.count = 0;
	productions.head = nonTerminal;
	return productions;
}

Grammar newGrammar() {
	Grammar grammar;
	grammar.count = 0;
	return grammar;
}

Production newProduction() {
	Production production;
	production.termCount = 0;
	return production;
}

Production addTerm(Production *prod, Term term) {
	prod->terms[prod->termCount++] = term;
}

Productions addProduction(Productions *prods, Production prod) {
	prods->list[prods->count++] = prod;
}

Grammar addProductions(Grammar *grammar, Productions prods) {
	grammar->list[grammar->count++] = prods;
}

void printNesting(int nesting) {
	for (int i = 0; i < nesting; i++) printf(" ");
}

void printState(int nesting, State state) {
	printNesting(nesting);
	printf("State %c\n", 'A' + state - 1);
}

void printSuccess(int nesting, State state) {
	printNesting(nesting);
	printf("Matched %c\n", 'A' + state - 1);
}

void printFailure(int nesting, State state) {
	printNesting(nesting);
	printf("Failed %c\n", 'A' + state - 1);
}

void printTerm(Term term) {
	if (term.type == TERM_TERMINAL) {
		printf("%d", term.data.terminal);
	} else {
		printf("%c", 'A' + term.data.nonTerminal - 1);
	}
}

void printProduction(Production prod) {
	printf("\t\tProduction(%d){ -> ", prod.termCount);
	for (int i = 0; i < prod.termCount; i++) {
		printTerm(prod.terms[i]);
		printf(" ");
	}
	printf("}\n");
}

void printProductions(Productions prods) {
	printf("\tProductions(%d %c){\n", prods.count, 'A' + prods.head - 1);
	for (int i = 0; i < prods.count; i++) {
		printProduction(prods.list[i]);
		printf(", ");
	}
	printf("\t}\n");
}

void printGrammar(Grammar grammar) {
	printf("Grammar(%d){\n", grammar.count);
	for (int i = 0; i < grammar.count; i++) {
		printProductions(grammar.list[i]);
		printf(", ");
	}
	printf("}");
}

int rd(TerminalStream *terminals, Grammar *grammar, NonTerminal state, int nesting);

int tryProduction(TerminalStream *terminals, Grammar *grammar, Production production, int nesting) {
	nesting++;
	if (production.termCount == 0) {
		return 1;
	}
	//printNesting(nesting);
	//printf("%d terms to match\n",production.termCount);

	Terminal *backup = terminals->curr;
	for (int j = 0; j < production.termCount; j++) {
		Term term = production.terms[j];
		if (term.type == TERM_TERMINAL) {
			//printNesting(nesting);
			//printf("Matching %s with %s\n",terminal[term.data.terminal],terminal[*(terminals->curr)]);
			if (*terminals->curr != term.data.terminal) {
				terminals->curr = backup;
				return 0;
			} else {
				terminals->curr++;
				//Continue with next term
			}
		} else if (term.type == TERM_NON_TERMINAL) {
			if (!rd(terminals, grammar, term.data.nonTerminal, nesting)) {
				terminals->curr = backup;
				return 0;
			} else {
				//terminals already incremented within rd pass by reference; continue with next term
			}
		}
	}
	return 1;
}

int
tryProductions(TerminalStream *terminals, Grammar *grammar, Productions productions, NonTerminal state, int nesting) {
	for (int j = 0; j < productions.count; j++) {
		//printNesting(nesting);
		//printf("Trying production: %d\n",j);
		if (tryProduction(terminals, grammar, productions.list[j], nesting)) return 1;
	}
	return 0;
}

int rd(TerminalStream *terminals, Grammar *grammar, NonTerminal state, int nesting) {
	printState(nesting, state);
	//printf("%d\n",productions.count);
	for (int i = 0; i < grammar->count; i++) {
		Productions productions = grammar->list[i];
		if (productions.head == state) {
			int success = tryProductions(terminals, grammar, productions, state, nesting);
			if (success) {
				printSuccess(nesting, state);
				return 1;
			} else {
				printFailure(nesting, state);
				return 0;
			}
		}
		//printNesting(nesting);
		//printf("Irrelevant production: %d\n",production.head);
	}
	printState(nesting, state);
	return 0;
}
