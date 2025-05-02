#pragma once
#include "TreeNode.h"

class Constant : public TreeNode
{
	int m_val;

	bool ifEasyReducePossible(AbstractSyntaxTree& ast, BinaryOperation* opr);
public:
	Constant(int val);

	// metody wspomagaj¹ce tworzenie drzewa
	TreeNode* clone() const override;

	// porównywanie
	bool operator==(int value) override;

	// metody upraszczaj¹ce drzewo
	void reduceMultipleNegations(AbstractSyntaxTree& ast, int count = 0) override;

	bool isVariable() const override;

	bool putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	int putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;

	bool calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;
	bool calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr) override;
	bool calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr) override;
	bool calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr) override;

	bool reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	int reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;

	// konwersja do postaci tekstowej
	Napis toNapis() override;

	friend class Matrix;
};

