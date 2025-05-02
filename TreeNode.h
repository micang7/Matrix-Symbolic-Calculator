#pragma once
#include "Napis.h"

class AbstractSyntaxTree;
class Constant;
class Matrix;
class BinaryOperation;

class TreeNode
{
public:
	TreeNode* m_parent;
	TreeNode* m_leftChild;
	TreeNode* m_rightChild;

	TreeNode();
	virtual ~TreeNode();

	// tworzenie drzewa
	virtual TreeNode* clone() const = 0;
	virtual TreeNode* nextFreeNode();

	// porównywanie (potrzebne tylko w jednym miejscu)
	virtual bool operator==(int value);

	// upraszczanie drzewa
	virtual void simplify(AbstractSyntaxTree& ast);

	// usuwanie wielokrotnych negacji
	virtual void reduceMultipleNegations(AbstractSyntaxTree& ast, int count = 0);

	// wyci¹gniêcie negacji przed nawias
	virtual void putNegationBeforeOperation(AbstractSyntaxTree& ast);
	virtual bool isNegation() const;

	// zmiana kolejnoœci dzia³añ, aby zmienne nie przeszkadza³y obliczyæ wartoœci
	virtual void putVariableOut(AbstractSyntaxTree& ast);
	virtual BinaryOperation* isSameOperationType(Napis&& opr);
	virtual bool isVariable() const;

	// wyci¹gniêcie czynnika przed nawias
	virtual void putScalarOut(AbstractSyntaxTree& ast);
	virtual bool putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr);
	virtual int putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr);

	// obliczenie wartoœci, któr¹ mo¿na obliczyæ
	virtual bool calculate(AbstractSyntaxTree& ast);
	virtual bool calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr);
	virtual bool calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr);
	virtual bool calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr);
	virtual bool calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr);
	virtual bool calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr);

	// skrócenie u³amków
	virtual void reduceFraction(AbstractSyntaxTree& ast);
	virtual bool reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr);
	virtual int reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr);

	// konwersja do postaci tekstowej
	virtual Napis toNapis() = 0;
	virtual Napis toNapisExpand();

	virtual bool isLowerOperatorPriority(Napis&& opr) const;
	virtual bool isNegationSignificant() const;
};