#include "TreeNode.h"

#include "AbstractSyntaxTree.h"
#include "Matrix.h"
#include "BinaryOperation.h"

#include <iostream>

TreeNode::TreeNode()
{
	m_parent = nullptr;
	m_leftChild = nullptr;
	m_rightChild = nullptr;
}

TreeNode::~TreeNode() {}

TreeNode* TreeNode::nextFreeNode()
{
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool TreeNode::operator==(int value)
{
	return false;
}

void TreeNode::simplify(AbstractSyntaxTree& ast) {}


void TreeNode::reduceMultipleNegations(AbstractSyntaxTree& ast, int count)
{
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

void TreeNode::putNegationBeforeOperation(AbstractSyntaxTree& ast) {}

bool TreeNode::isNegation() const
{
	return false;
}

void TreeNode::putVariableOut(AbstractSyntaxTree& ast) {}

BinaryOperation* TreeNode::isSameOperationType(Napis&& opr)
{
	return nullptr;
}

bool TreeNode::isVariable() const
{
	return true;
}

void TreeNode::putScalarOut(AbstractSyntaxTree& ast) {}

bool TreeNode::putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return false;
}

int TreeNode::putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	return 1;
}

bool TreeNode::calculate(AbstractSyntaxTree& ast)
{
	return false;
}

bool TreeNode::calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return opr->m_rightChild->calculate2_leftIsOther(ast, opr);
}

bool TreeNode::calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	return false;
}

bool TreeNode::calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr)
{
	return false;
}

bool TreeNode::calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr)
{
	if (opr->getOpr() == '*') {
		BinaryOperation* iloczyn = new BinaryOperation('*');

		iloczyn->m_leftChild = left->m_leftChild;
		left->m_leftChild->m_parent = iloczyn;

		iloczyn->m_rightChild = opr->m_rightChild;
		opr->m_rightChild->m_parent = iloczyn;

		left->m_leftChild = iloczyn;
		iloczyn->m_parent = left;

		opr->m_leftChild->m_parent = opr->m_parent;
		if (opr->m_parent) {
			if (opr->m_parent->m_leftChild == opr)
				opr->m_parent->m_leftChild = opr->m_leftChild;
			else
				opr->m_parent->m_rightChild = opr->m_leftChild;
		}
		else ast.setRoot(opr->m_leftChild);

		delete opr;

		left->simplify(ast);
		return true;
	}
	if (opr->getOpr() == '/') {
		BinaryOperation* iloczyn = new BinaryOperation('*');

		iloczyn->m_leftChild = left->m_rightChild;
		left->m_rightChild->m_parent = iloczyn;

		iloczyn->m_rightChild = opr->m_rightChild;
		opr->m_rightChild->m_parent = iloczyn;

		left->m_rightChild = iloczyn;
		iloczyn->m_parent = left;

		opr->m_leftChild->m_parent = opr->m_parent;
		if (opr->m_parent) {
			if (opr->m_parent->m_leftChild == opr)
				opr->m_parent->m_leftChild = opr->m_leftChild;
			else
				opr->m_parent->m_rightChild = opr->m_leftChild;
		}
		else ast.setRoot(opr->m_leftChild);

		delete opr;

		left->simplify(ast);
		return true;
	}
	if (opr->getOpr() == '+' || opr->getOpr() == '-') {
		BinaryOperation* iloczyn = new BinaryOperation('*');
		TreeNode* kopia_mianownika = left->m_rightChild->clone();

		iloczyn->m_leftChild = opr->m_rightChild;
		opr->m_rightChild->m_parent = iloczyn;

		iloczyn->m_rightChild = kopia_mianownika;
		kopia_mianownika->m_parent = iloczyn;

		opr->m_rightChild = iloczyn;
		iloczyn->m_parent = opr;

		opr->m_leftChild->m_parent = opr->m_parent;
		if (opr->m_parent) {
			if (opr->m_parent->m_leftChild == opr)
				opr->m_parent->m_leftChild = opr->m_leftChild;
			else
				opr->m_parent->m_rightChild = opr->m_leftChild;
		}
		else ast.setRoot(opr->m_leftChild);

		opr->m_leftChild = left->m_leftChild;
		left->m_leftChild->m_parent = opr;

		left->m_leftChild = opr;
		opr->m_parent = left;

		left->simplify(ast);
		return true;
	}
	return false;
}

bool TreeNode::calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return false;
}

void TreeNode::reduceFraction(AbstractSyntaxTree& ast) {}

bool TreeNode::reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return false;
}

int TreeNode::reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	return 1;
}

Napis TreeNode::toNapisExpand()
{
	return toNapis();
}

bool TreeNode::isLowerOperatorPriority(Napis&& opr) const
{
	return false;
}

bool TreeNode::isNegationSignificant() const
{
	return false;
}
