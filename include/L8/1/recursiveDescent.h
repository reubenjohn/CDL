#include <L8/1/tokenizer.h>

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

void printState(int nesting, State state, int index) {
	printNesting(nesting);
	printf("State %c[%d]\n", 'A' + state - 1, index);
}

void printSuccess(int nesting, State state) {
	printNesting(nesting);
	printf("Succeeded %c\n", 'A' + state - 1);
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

int getPermutationCount(Grammar *grammar, Production production, int permutations[]) {
	int permutationCount = 1;
	for (int i = 0; i < production.termCount; i++) {
		Term term = production.terms[i];
		permutations[i] = 1;
		if (term.type == TERM_NON_TERMINAL) {
			for (int j = 0; j < grammar->count; j++) {
				if (term.data.nonTerminal == grammar->list[j].head) {
					permutations[i] = grammar->list[j].count;
					permutationCount *= permutations[i];
				}
			}
		}
	}
	return permutationCount;
}

void getSelection(int permutations[], int count, int index, int selection[]) {
	for (int i = 0; i < count; i++) {
		selection[i] = index % permutations[i];
		index /= permutations[i];
//		printf("%d", selection[i]);
	}
//	printf("\n");
}

int rd(TerminalStream *, Grammar *, NonTerminal, int, int);

int trySelectedProduction(TerminalStream *terminals, Grammar *grammar, Production production, int selection[],
						  int nesting) {
	Terminal *backup = terminals->curr;
	for (int j = 0; j < production.termCount; j++) {
		Term term = production.terms[j];
		if (term.type == TERM_TERMINAL) {
			if (*terminals->curr != term.data.terminal) {
				terminals->curr = backup;
				return 0;
			} else {
				printNesting(nesting);
				printf("Matched %s\n", terminal[*terminals->curr]);
				terminals->curr++;
				//Continue with next term
			}
		} else if (term.type == TERM_NON_TERMINAL) {
			if (!rd(terminals, grammar, term.data.nonTerminal, selection[j], nesting)) {
				terminals->curr = backup;
				return 0;
			} else {
				//terminals already incremented within rd pass by reference; continue with next term
			}
		}
	}
	return 1;
}

int tryProductionPermutations(TerminalStream *terminals, Grammar *grammar, Production production, int nesting) {
	nesting++;
	if (production.termCount == 0) {
		printNesting(nesting);
		printf("Matched Epsilon\n");
		return 1;
	}
	int permutations[64];
	int permutationCount = getPermutationCount(grammar, production, permutations);
	for (int i = 0; i < permutationCount; i++) {
		int selection[64];
		getSelection(permutations, production.termCount, i, selection);
		if (trySelectedProduction(terminals, grammar, production, selection, nesting))
			return 1;
	}
	return 0;
}

int rd(TerminalStream *terminals, Grammar *grammar, NonTerminal head, int index, int nesting) {
	printState(nesting, head, index);
	for (int i = 0; i < grammar->count; i++) {
		Productions productions = grammar->list[i];
		if (productions.head == head) {
			int success = tryProductionPermutations(terminals, grammar, productions.list[index], nesting);
			if (success) {
				printSuccess(nesting, head);
				return 1;
			} else {
				printFailure(nesting, head);
				return 0;
			}
		}
	}
	printState(nesting, head, index);
	return 0;
}
