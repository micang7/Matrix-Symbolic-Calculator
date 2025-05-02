#pragma once
#include "Napis.h"

enum nonterminals { S, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, T, U, V, W, X, Y, Z, AA };
const int nonterminalProductionRules[53][3] = {
	{ S, B, A },
	{ S, U, A },
	{ S, C, B },
	{ S, D, D },
	{ S, V, D },
	{ S, W, M },
	{ S, H, A },
	{ S, S, O },
	{ S, I, P },
	{ S, Q, P },
	{ S, R, P },
	{ S, R, T },
	{ S, I, T },
	{ S, Q, T },
	{ A, B, A },
	{ A, U, A },
	{ A, C, B },
	{ A, D, D },
	{ A, V, D },
	{ A, W, M },
	{ B, D, D },
	{ B, V, D },
	{ B, W, M },
	{ D, D, D },
	{ E, Z, E },
	{ E, X, M },
	{ F, AA, F },
	{ F, B, A },
	{ F, U, A },
	{ F, C, B },
	{ F, D, D },
	{ F, V, D },
	{ F, W, M },
	{ F, H, A },
	{ F, S, O },
	{ F, I, P },
	{ F, Q, P },
	{ F, R, P },
	{ F, R, T },
	{ F, I, T },
	{ F, Q, T },
	{ O, G, S },
	{ P, S, J },
	{ Q, H, I },
	{ R, S, I },
	{ T, P, S },
	{ U, C, B },
	{ V, D, K },
	{ W, L, E },
	{ X, L, F },
	{ Y, M, N },
	{ Z, X, Y },
	{ AA, S, N }
};

bool CYK(const Napis& expression);

int NWD(int a, int b);
Napis doubleToDivision(const Napis& Double);
void onlyIntegers(Napis& expression);

int precedence(char opr);
Napis infixToPrefix(Napis infix);



// Regu³y produkcji wyra¿enia algebraicznego
// S -> A | -A | SGS | (S) | -(S) | S(S) | S(S)S | (S)S | -(S)S
// A -> BA | CBA | CB | B | C
// B -> D | D.D | {E}
// C -> a-zA-Z
// D -> DD | 0-9
// E -> {F},E | {F}
// F -> S,F | S
// G -> + | - | * | / | ^
// 
// W postaci Chomsky'ego
// S -> BA | UA | CB | DD | 0-9 | VD | WM | a-zA-Z | HA | SO | IP | QP | RP | RT | IT | QT
// A -> BA | UA | CB | DD | 0-9 | VD | WM | a-zA-Z
// B -> DD | 0-9 | VD | WM
// C -> a-zA-Z
// D -> DD | 0-9
// E -> ZE | XM
// F -> AAF | BA | UA | CB | DD | 0-9 | VD | WM | a-zA-Z | HA | SO | IP | QP | RP | RT | IT | QT
// G -> + | - | * | / | ^
// H -> -
// I -> (
// J -> )
// K -> .
// L -> {
// M -> }
// N -> ,
// O -> GS
// P -> SJ
// Q -> HI
// R -> SI
// T -> PS
// U -> CB
// V -> DK
// W -> LE
// X -> LF
// Y -> MN
// Z -> XY
// AA -> SN