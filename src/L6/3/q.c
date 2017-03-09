#include <L6/3/tokenizer.h>
#include <L6/3/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 6;
	productions.list[0] = getProduction(S_, 2, (Term[]) {getNonT(S),getT(DOL)});
	productions.list[1] = getProduction(S, 5, (Term[]) {getT(a),getNonT(A),getT(c),getNonT(B),getT(e)});
	productions.list[2] = getProduction(A, 2, (Term[]) {getT(b),getNonT(A_)});
	productions.list[3] = getProduction(A_, 2, (Term[]) {getT(b),getNonT(A_)});
	productions.list[4] = getProduction(A_, 0, (Term[]) {});
	productions.list[5] = getProduction(B, 1, (Term[]) {getT(d)});
	printf("\n");
	int isMember = rd(&terminals, productions, S_, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}

