#include <tokenizer.h>
#include <L8/1/recursiveDescent.h>

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);
	printTokens(tokens,tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);

	Productions productions;
	productions.count = 36;
	productions.list[0] = getProduction(A_, 2, (Term[]) {getNonT(A),getT(DOL)});
	productions.list[1] = getProduction(A, 7, (Term[]) {getT(MAIN),getT(L_PR),getT(R_PR),getT(L_CB),getNonT(B),getNonT(E),getT(R_CB)});
	productions.list[2] = getProduction(B, 4, (Term[]) {getNonT(C),getNonT(D),getT(SC),getNonT(B)});
	productions.list[3] = getProduction(B, 0, (Term[]) {});
	productions.list[4] = getProduction(C, 1, (Term[]) {getT(INT)});
	productions.list[5] = getProduction(C, 1, (Term[]) {getT(CHAR)});
	productions.list[6] = getProduction(D, 1, (Term[]) {getT(ID)});
	productions.list[7] = getProduction(D, 3, (Term[]) {getT(ID),getT(COMMA),getNonT(D)});
	productions.list[8] = getProduction(D, 6, (Term[]) {getT(ID),getT(L_SB),getT(NUM),getT(R_SB),getT(COMMA),getNonT(D)});
	productions.list[9] = getProduction(D, 4, (Term[]) {getT(ID),getT(L_SB),getT(NUM),getT(R_SB)});
	productions.list[10] = getProduction(E, 2, (Term[]) {getNonT(F),getNonT(E)});
	productions.list[11] = getProduction(E, 0, (Term[]) {});
	productions.list[12] = getProduction(F, 2, (Term[]) {getNonT(G),getT(SC)});
	productions.list[13] = getProduction(G, 3, (Term[]) {getT(ID),getT(EQ),getNonT(H)});
	productions.list[14] = getProduction(H, 2, (Term[]) {getNonT(J),getNonT(I)});
	productions.list[15] = getProduction(I, 2, (Term[]) {getNonT(O),getNonT(J)});
	productions.list[16] = getProduction(I, 0, (Term[]) {});
	productions.list[17] = getProduction(J, 2, (Term[]) {getNonT(L),getNonT(K)});
	productions.list[18] = getProduction(K, 3, (Term[]) {getNonT(P),getNonT(L),getNonT(K)});
	productions.list[19] = getProduction(K, 0, (Term[]) {});
	productions.list[20] = getProduction(L, 2, (Term[]) {getNonT(N),getNonT(M)});
	productions.list[21] = getProduction(M, 3, (Term[]) {getNonT(Q),getNonT(N),getNonT(M)});
	productions.list[22] = getProduction(M, 0, (Term[]) {});
	productions.list[23] = getProduction(N, 1, (Term[]) {getT(ID)});
	productions.list[24] = getProduction(N, 1, (Term[]) {getT(NUM)});
	productions.list[25] = getProduction(O, 1, (Term[]) {getT(EQEQ)});
	productions.list[26] = getProduction(O, 1, (Term[]) {getT(NTEQ)});
	productions.list[27] = getProduction(O, 1, (Term[]) {getT(LTEQ)});
	productions.list[28] = getProduction(O, 1, (Term[]) {getT(GTEQ)});
	productions.list[29] = getProduction(O, 1, (Term[]) {getT(LT)});
	productions.list[30] = getProduction(O, 1, (Term[]) {getT(GT)});
	productions.list[31] = getProduction(P, 1, (Term[]) {getT(PLUS)});
	productions.list[32] = getProduction(P, 1, (Term[]) {getT(MINUS)});
	productions.list[33] = getProduction(Q, 1, (Term[]) {getT(STAR)});
	productions.list[34] = getProduction(Q, 1, (Term[]) {getT(DIV)});
	productions.list[35] = getProduction(Q, 1, (Term[]) {getT(PERC)});
	printf("\n");
	int isMember = rd(&terminals, productions, A_, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}


