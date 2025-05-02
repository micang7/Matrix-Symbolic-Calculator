#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);
	UnaryOperation(Napis&& opr);

	// metody wspomagaj¹ce tworzenie drzewa
	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	// metody upraszczaj¹ce drzewo
	void simplify(AbstractSyntaxTree& ast) override;

	void reduceMultipleNegations(AbstractSyntaxTree& ast, int count = 0) override;

	bool isNegation() const override;

	bool isVariable() const override;

	bool putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	int putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;

	bool calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;
	bool calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr) override;
	bool calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr) override;
	bool calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr) override;

	// konwersja do postaci tekstowej
	Napis toNapis() override;
};

