#include <L8/1/recursiveDescent.h>

#define gram {grammar = newGrammar();}
#define ps(S) {productions = newProductions(S);}
#define p {production = newProduction(productions.head);}
#define nonT(X) {addTerm(&production,getNonT(X));}
#define T(X) {addTerm(&production,getT(X));}
#define _e {addProduction(&productions,production);}
#define _ep {addProductions(&grammar,productions);}
#define _gram ;
//#define addProd(A) {addProduction(ps,A)}

int main() {
	Token tokens[200];
	int tokenCount = 0;
	getTokenStream(tokens, &tokenCount);
	printTokens(tokens,tokenCount);

	TerminalStream terminals;
	populateTerminalStream(&terminals, tokens, tokenCount);


	Grammar grammar;
	Productions productions;
	Production production;

	gram
	ps(A_)	{p nonT(A) T(DOL) _e} _ep
	ps(A)	{p T(MAIN) T(L_PR) T(R_PR) T(L_CB) nonT(B) nonT(E) T(R_CB) _e} _ep
	ps(B)	{p nonT(C) nonT(D) T(SC) nonT(B) _e}	{p _e} _ep
	ps(C)	{p T(INT) _e}	{p T(CHAR) _e} _ep
	ps(D)	{p T(ID) _e}	{p T(ID) T(COMMA) nonT(D) _e}	{p T(ID) T(L_SB) T(NUM) T(R_SB) T(COMMA) nonT(D) _e}	{p T(ID) T(L_SB) T(NUM) T(R_SB) _e} _ep
	ps(E)	{p nonT(F) nonT(E) _e}	{p  _e}	_ep
	ps(F)	{p nonT(G) T(SC) _e}	_ep
	ps(G)	{p T(ID) T(EQ) nonT(H) _e}	_ep
	ps(H)	{p nonT(J) nonT(I) _e}	_ep
	ps(I)	{p nonT(O) nonT(J) _e}	{p  _e}	_ep
	ps(J)	{p nonT(L) nonT(K) _e}	_ep
	ps(K)	{p nonT(P) nonT(L) nonT(K) _e}	{p  _e}	_ep
	ps(L)	{p nonT(N) nonT(M) _e}	_ep
	ps(M)	{p nonT(Q) nonT(N) nonT(M) _e}	{p  _e}	_ep
	ps(N)	{p T(ID) _e}	{p T(NUM) _e}	_ep
	ps(O)	{p T(EQEQ) _e}	{p T(NTEQ) _e}	{p T(LTEQ) _e}	{p T(GTEQ) _e}	{p T(LT) _e}	{p T(GT) _e}	_ep
	ps(P)	{p T(PLUS) _e}	{p T(MINUS) _e}	_ep
	ps(Q)	{p T(STAR) _e}	{p T(DIV) _e}	{p T(PERC) _e}	_ep
	_gram

	printGrammar(grammar);

	printf("\n");
	int isMember = rd(&terminals, &grammar, A_, 0, 0);
	printf("\nThe membership of the string is %d\n", isMember);
	return 0;
}