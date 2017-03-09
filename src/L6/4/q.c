#include <L6/4/tokenizer.h>
#include <L6/4/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 9;
	productions.list[0] = getProduction(S_, 2, (Term[]) {getNonT(S),getT(DOL)});
	productions.list[1] = getProduction(S, 1, (Term[]) {getNonT(A)});
	productions.list[2] = getProduction(S, 1, (Term[]) {getNonT(B)});
	productions.list[3] = getProduction(A, 1, (Term[]) {getT(number)});
	productions.list[4] = getProduction(A, 1, (Term[]) {getT(identifier)});
	productions.list[5] = getProduction(B, 3, (Term[]) {getT(L_PR),getNonT(C),getT(R_PR)});
	productions.list[6] = getProduction(C, 2, (Term[]) {getNonT(S),getNonT(C_)});
	productions.list[7] = getProduction(C_, 1, (Term[]) {getNonT(S),getNonT(C_)});
	productions.list[8] = getProduction(C_, 0, (Term[]) {});
	printf("\n");
	int isMember = rd(&terminals, productions, S_, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}

