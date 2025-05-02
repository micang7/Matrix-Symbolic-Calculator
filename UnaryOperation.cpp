#include "UnaryOperation.h"

#include <iostream>
#include "AbstractSyntaxTree.h"

UnaryOperation::UnaryOperation(const Napis& opr) : Operation(opr) {}

UnaryOperation::UnaryOperation(Napis&& opr) : Operation(std::move(opr)) {}

TreeNode* UnaryOperation::clone() const
{
	return new UnaryOperation(*this);
}

TreeNode* UnaryOperation::nextFreeNode()
{
	if (!m_leftChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

void UnaryOperation::simplify(AbstractSyntaxTree& ast)
{
	reduceMultipleNegations(ast);
}

void UnaryOperation::reduceMultipleNegations(AbstractSyntaxTree& ast, int count)
{
	if (m_opr == '-')
		m_leftChild->reduceMultipleNegations(ast, count + 1);
	else {
		if (count > 1) {
			int negations = 2 * (count / 2);
			TreeNode* negation = m_parent;
			TreeNode* parent;
			for (int i = 0; i < negations; i++) {
				if (parent = negation->m_parent) {
					if (parent->m_leftChild == negation)
						parent->m_leftChild = this;
					else
						parent->m_rightChild = this;
				}
				else ast.setRoot(this);
				delete negation;
				negation = parent;
			}
			m_parent = negation;
		}
		simplify(ast);
	}
}

bool UnaryOperation::isNegation() const
{
	return m_opr == '-';
}

bool UnaryOperation::isVariable() const
{
	return m_leftChild->isVariable();
}

bool UnaryOperation::putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return m_leftChild->putScalarOut1(ast, opr);
}

int UnaryOperation::putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	return m_leftChild->putScalarOut2(ast, left, opr);
}

bool UnaryOperation::calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return m_leftChild->calculate1(ast, opr);
}

bool UnaryOperation::calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	return m_leftChild->calculate2_leftIsConstant(ast, left, opr);
}

bool UnaryOperation::calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr)
{
	return m_leftChild->calculate2_leftIsMatrix(ast, left, opr);
}

bool UnaryOperation::calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr)
{
	return m_leftChild->calculate2_leftIsFraction(ast, left, opr);
}

bool UnaryOperation::calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return m_leftChild->calculate2_leftIsOther(ast, opr);
}

Napis UnaryOperation::toNapis()
{
	bool parentheses = m_leftChild->isNegationSignificant();

	return m_opr + (parentheses ? "(" : "") + m_leftChild->toNapis() + (parentheses ? ")" : "");
}
