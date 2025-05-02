#include "Parser.h"

#include "Stos.h"
#include <iostream>

bool CYK(const Napis& expression)
{
	int n = expression.getLen();
	if (n <= 0) return false;

	bool*** P = new bool** [n];
	for (int i = 0; i < n; i++) {
		P[i] = new bool* [n];
		for (int j = 0; j < n; j++) {
			P[i][j] = new bool[27];
			for (int k = 0; k < 27; k++)
				P[i][j][k] = false;
		}
	}

	for (int s = 0; s < n; s++) {
		char chr = expression[s];
		if (std::isdigit(chr)) {
			P[0][s][S] = true;
			P[0][s][A] = true;
			P[0][s][B] = true;
			P[0][s][D] = true;
			P[0][s][F] = true;
		}
		else if (std::isalpha(chr)) {
			P[0][s][S] = true;
			P[0][s][A] = true;
			P[0][s][C] = true;
			P[0][s][F] = true;
		}
		else if (chr == '.') {
			P[0][s][K] = true;
		}
		else if (chr == '+' || chr == '*' || chr == '/' || chr == '^') {
			P[0][s][G] = true;
		}
		else if (chr == '-') {
			P[0][s][G] = true;
			P[0][s][H] = true;
		}
		else if (chr == '(') {
			P[0][s][I] = true;
		}
		else if (chr == ')') {
			P[0][s][J] = true;
		}
		else if (chr == '{') {
			P[0][s][L] = true;
		}
		else if (chr == '}') {
			P[0][s][M] = true;
		}
		else if (chr == ',') {
			P[0][s][N] = true;
		}
		else {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++)
					delete P[i][j];
				delete[] P[i];
			}
			delete[] P;
			return false;
		}
	}

	for (int l = 1; l < n; l++) {
		for (int s = 0; s < n - l; s++) {
			for (int p = 0; p < l; p++) {
				for (int i = 0; i < 53; i++) {
					if (P[p][s][nonterminalProductionRules[i][1]] &&
						P[l - p - 1][s + p + 1][nonterminalProductionRules[i][2]])
						P[l][s][nonterminalProductionRules[i][0]] = true;
				}
			}
		}
	}

	// Wyœwietl tabelkê CYK
	// wynik wstaw do Excel->Dane->Tekst_jako_kolumny rozdzielaj¹c znakiem |
	//for (int i = n - 1; i >= 0; i--) {
	//	for (int j = 0; j < n; j++) {
	//		bool first = true;
	//		for (int k = 0; k < 27; k++) {
	//			if (P[i][j][k]) {
	//				if (!first) std::cout << ',';
	//				first = false;
	//				char c = k > 0 ? k + 64 : 'S';
	//				std::cout << c;
	//			}
	//		}
	//		if (j < n - 1) std::cout << " | ";
	//	}
	//	std::cout << std::endl;
	//}

	bool result = P[n - 1][0][0];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			delete P[i][j];
		delete[] P[i];
	}
	delete[] P;

	return result;
}

int NWD(int a, int b)
{
	int c;
	while (a > 0) {
		c = a;
		a = b % a;
		b = c;
	}
	return b;
}

Napis doubleToDivision(const Napis& Double)
{
	int licznik = 0, mianownik = 1, okres = 1;
	bool czyCzescUlamkowa = false;
	bool czyOkres = false;

	int i = 0;
	while (i < Double.getLen()) {
		char chr = Double[i];

		if (chr == '.')
			czyCzescUlamkowa = true;
		else if (chr == '(')
			czyOkres = true;
		else if (chr == ')')
			czyOkres = false;
		else {
			licznik = 10 * licznik + chr - 48;
			if (czyCzescUlamkowa) mianownik *= 10;
			if (czyOkres) okres *= 10;
		}
		i++;
	}

	if (okres > 1) {
		licznik -= licznik / okres;
		mianownik -= mianownik / okres;
	}

	int nwd = NWD(licznik, mianownik);
	licznik /= nwd;
	mianownik /= nwd;

	return '(' + Napis(licznik) + '/' + Napis(mianownik) + ')';
}

void onlyIntegers(Napis& expression)
{
	Napis Double;
	bool czyNiecalkowita = false;

	int i = 0;
	while (i < expression.getLen()) {
		char chr = expression[i];

		if (std::isdigit(chr)) {
			Double += chr;
		}
		else if (chr == '.') {
			Double += chr;
			czyNiecalkowita = true;
		}
		else if (chr == '(' && czyNiecalkowita && std::isdigit(expression[i + 1]))
			Double += chr;
		else {
			if (chr == ')' && czyNiecalkowita) {
				Double += chr;
				i++;
			}

			int d = Double.getLen();
			if (czyNiecalkowita && d != 0) {
				i -= d;
				expression.remove(i, d);
				Napis division = doubleToDivision(Double);
				expression.insert(division, i);
				i += division.getLen();
			}

			Double = "";
			czyNiecalkowita = false;
		}
		i++;
	}

	int d = Double.getLen();
	if (czyNiecalkowita && d != 0) {
		i -= d;
		expression.remove(i, d);
		expression.insert(doubleToDivision(Double), i);
	}
}

int precedence(char opr)
{
	switch (opr) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return 4;
	}
}

Napis infixToPrefix(Napis infix) // zadzia³a konstruktor kopiuj¹cy
{
	onlyIntegers(infix);

	if (infix[0] == '-') infix[0] = '~';
	int i = 0;
	while (i < infix.getLen() - 1) {
		char a = infix[i], b = infix[i + 1];

		if (std::isdigit(a) && std::isalpha(b) || std::isalpha(a) && std::isdigit(b) ||
			b == '(' && (std::isalnum(a) || a == ')') ||
			a == ')' && (std::isalnum(b) || b == '(') ||
			b == '{' && (std::isalnum(a) || a == ')' || a == '}') ||
			a == '}' && (std::isalnum(b) || b == '(' || b == '{')) {
			infix.insert('*', i + 1);
			i++;
		}
		if (b == '-' && (a == '+' || a == '-' || a == '*' || a == '/' || a == '^' || a == '('))
			infix[i + 1] = '~';
		i++;
	}

	Stos stos;
	Napis prefix;
	bool lastPrintedIsSameOperand = true;
	int matrixParenthesesBalance = 0;

	for (int i = infix.getLen() - 1; i >= 0; i--) {
		char chr = infix[i];
		if (chr == '{') {
			matrixParenthesesBalance--;
			lastPrintedIsSameOperand = matrixParenthesesBalance != 0;
			prefix += chr;
		}
		else if (std::isalnum(chr) || chr == '}' || chr == ',' || matrixParenthesesBalance != 0) {
			if (!lastPrintedIsSameOperand) prefix += ' ';
			lastPrintedIsSameOperand = true;
			prefix += chr;
			if (chr == '}')	matrixParenthesesBalance++;
		}
		else if (chr == '~') {
			prefix += ' ';
			lastPrintedIsSameOperand = true;
			prefix += chr;
		}
		else if (chr == ')') {
			lastPrintedIsSameOperand = prefix == "";
			stos.push(chr);
		}
		else if (chr == '(') {
			lastPrintedIsSameOperand = false;
			while (!stos.isEmpty() && stos.top() != ')') {
				prefix += ' ';
				prefix += stos.pop();
			}
			if (stos.isEmpty()) throw std::invalid_argument("Error: Parser: Unbalanced parentheses!");
			stos.pop();
		}
		else {
			lastPrintedIsSameOperand = false;
			while (!stos.isEmpty() && stos.top() != ')' && precedence(stos.top()) > precedence(chr)) {
				prefix += ' ';
				prefix += stos.pop();
			}
			stos.push(chr);
		}
	}

	while (!stos.isEmpty() && stos.top() != ')') {
		prefix += ' ';
		prefix += stos.pop();
	}

	if (!stos.isEmpty()) throw std::invalid_argument("Error: Parser: Unbalanced parentheses!");

	return prefix.reversed();
}