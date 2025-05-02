#pragma once
#include "Operation.h"

class BinaryOperation : public Operation
{
public:
	BinaryOperation(const Napis& opr);
	BinaryOperation(Napis&& opr);

	// metody wspomagaj¹ce tworzenie drzewa
	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	// metody upraszczaj¹ce drzewo
	void simplify(AbstractSyntaxTree& ast) override;

	void putNegationBeforeOperation(AbstractSyntaxTree& ast) override;

	void putVariableOut(AbstractSyntaxTree& ast) override;
	BinaryOperation* isSameOperationType(Napis&& opr) override;

	void putScalarOut(AbstractSyntaxTree& ast) override;
	bool putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	int putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;

	bool calculate(AbstractSyntaxTree& ast) override;
	bool calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;
	bool calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr) override;
	bool calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr) override;

	void reduceFraction(AbstractSyntaxTree& ast) override;
	bool reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr);
	int reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;

	// konwersja do postaci tekstowej
	Napis toNapis() override;

	bool isLowerOperatorPriority(Napis&& opr) const override;
	bool isNegationSignificant() const override;
};