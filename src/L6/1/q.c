#include <L6/1/tokenizer.h>
#include <L6/1/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 7;
	productions.list[0] = getProduction(S_, 2, (Term[]) {getNonT(S), getT(DOL)});
	productions.list[1] = getProduction(S, 1, (Term[]) {getT(a)});
	productions.list[2] = getProduction(S, 1, (Term[]) {getT(MINU)});
	productions.list[3] = getProduction(S, 3, (Term[]) {getT(L_PR), getNonT(T), getT(R_PR)});
	productions.list[4] = getProduction(T, 2, (Term[]) {getNonT(S), getNonT(T_)});
	productions.list[5] = getProduction(T_, 3, (Term[]) {getT(COMMA), getNonT(S), getNonT(T_)});
	productions.list[6] = getProduction(T_, 0, (Term[]) {});
	int isMember = rd(&terminals, productions, S_);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}
