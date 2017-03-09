#include <L6/2/tokenizer.h>
#include <L6/2/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 9;
	productions.list[0] = getProduction(E_, 2, (Term[]) {getNonT(E),getT(DOL)});
	productions.list[1] = getProduction(E, 2, (Term[]) {getNonT(T),getNonT(E__)});
	productions.list[2] = getProduction(E__, 3, (Term[]) {getT(PLUS),getNonT(T),getNonT(E__)});
	productions.list[3] = getProduction(E__, 0, (Term[]) {});
	productions.list[4] = getProduction(T, 2, (Term[]) {getNonT(F), getNonT(T_)});
	productions.list[5] = getProduction(T_, 3, (Term[]) {getT(STAR), getNonT(F),getNonT(T_)});
	productions.list[6] = getProduction(T_, 0, (Term[]) {});
	productions.list[7] = getProduction(F, 3, (Term[]) {getT(L_PR), getNonT(E), getT(R_PR)});
	productions.list[8] = getProduction(F, 1, (Term[]) {getT(ID)});
	printf("\n");
	int isMember = rd(&terminals, productions, E_, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}

