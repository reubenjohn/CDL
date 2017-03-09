#include <L7/1/tokenizer.h>
#include <L7/1/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 11;
	productions.list[0] = getProduction(A_, 2, (Term[]) {getNonT(A),getT(DOL)});
	productions.list[1] = getProduction(A, 7, (Term[]) {getT(MAIN),getT(L_PR),getT(R_PR),getT(L_CB),getNonT(B),getNonT(E),getT(R_CB)});
	productions.list[2] = getProduction(B, 4, (Term[]) {getNonT(C),getNonT(D),getT(SC),getNonT(B)});
	productions.list[3] = getProduction(B, 0, (Term[]) {});
	productions.list[4] = getProduction(C, 1, (Term[]) {getT(INT)});
	productions.list[5] = getProduction(C, 1, (Term[]) {getT(CHAR)});
	productions.list[6] = getProduction(D, 1, (Term[]) {getT(ID)});
	productions.list[7] = getProduction(D, 3, (Term[]) {getT(ID),getT(COMMA),getNonT(D)});
	productions.list[7] = getProduction(D, 0, (Term[]) {});
	productions.list[8] = getProduction(E, 4, (Term[]) {getT(ID),getT(EQ),getT(ID),getT(SC)});
	productions.list[9] = getProduction(E, 4, (Term[]) {getT(ID),getT(EQ),getT(NUM),getT(SC)});
	printf("\n");
	int isMember = rd(&terminals, productions, A_, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}

