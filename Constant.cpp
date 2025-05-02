#include "Constant.h"

#include <iostream>
#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "Parser.h"
#include "BinaryOperation.h"
#include "Matrix.h"

#include <cmath>

bool Constant::ifEasyReducePossible(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	if (m_val == 1 && (opr->getOpr() == '*' || opr->getOpr() == '/')) {
		TreeNode* parent = opr->m_parent;
		if (parent) {
			if (parent->m_leftChild == opr)
				parent->m_leftChild = opr->m_leftChild;
			else
				parent->m_rightChild = opr->m_leftChild;
		}
		else ast.setRoot(opr->m_leftChild);
		opr->m_leftChild->m_parent = parent;
		ast.removeSubtree(opr->m_rightChild);
		delete opr;
		return true;
	}
	else if (m_val == 0) {
		if (opr->getOpr() == '+' || opr->getOpr() == '-') {
			TreeNode* parent = opr->m_parent;
			if (parent) {
				if (parent->m_leftChild == opr)
					parent->m_leftChild = opr->m_leftChild;
				else
					parent->m_rightChild = opr->m_leftChild;
			}
			else ast.setRoot(opr->m_leftChild);
			opr->m_leftChild->m_parent = parent;
			ast.removeSubtree(opr->m_rightChild);
			delete opr;
			return true;
		}
		else if (opr->getOpr() == '*') {
			TreeNode* parent = opr->m_parent;
			ast.removeSubtree(opr);
			ast.addChild(parent, Constant(0));
			return true;
		}
		else if (opr->getOpr() == '/')
			throw std::runtime_error("Error: AST: Division by zero!");
	}
	return false;
}

Constant::Constant(int val)
{
	m_val = val;
}

TreeNode* Constant::clone() const
{
	return new Constant(*this);
}

bool Constant::operator==(int value)
{
	return m_val == value;
}

void Constant::reduceMultipleNegations(AbstractSyntaxTree& ast, int count)
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
	else if (count == 1 && m_val == 0) {
		TreeNode* negation = m_parent;
		if (negation->m_parent) {
			if (negation->m_parent->m_leftChild == negation)
				negation->m_parent->m_leftChild = this;
			else
				negation->m_parent->m_rightChild = this;
		}
		else ast.setRoot(this);
		m_parent = negation->m_parent;
		delete negation;
	}
	simplify(ast);
}

bool Constant::isVariable() const
{
	return false;
}

bool Constant::putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	int nwd = opr->m_rightChild->putScalarOut2(ast, this, opr);
	if (nwd > 1) {
		m_val /= nwd;
		if (m_val == 1) m_parent->calculate(ast);
	}
	return true;
}

int Constant::putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	int a = (left->m_parent->isNegation() ? -left->m_val : left->m_val);
	int b = (m_parent->isNegation() ? -m_val : m_val);
	int nwd = NWD(a, b);
	if (nwd > 1) {
		TreeNode* multiplication = new BinaryOperation('*');
		multiplication->m_parent = opr->m_parent;
		multiplication->m_rightChild = opr;
		if (opr->m_parent) {
			if (opr->m_parent->m_leftChild == opr)
				opr->m_parent->m_leftChild = multiplication;
			else
				opr->m_parent->m_rightChild = multiplication;
		}
		else ast.setRoot(multiplication);
		opr->m_parent = multiplication;
		multiplication->m_leftChild = new Constant(nwd);
		multiplication->m_leftChild->m_parent = multiplication;
		m_val /= nwd;
		if (m_val == 1) m_parent->calculate(ast);
	}
	return nwd;
}

bool Constant::calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	if (m_val == 1 && opr->getOpr() == '*') {
		TreeNode* parent = opr->m_parent;
		if (parent) {
			if (parent->m_leftChild == opr)
				parent->m_leftChild = opr->m_rightChild;
			else
				parent->m_rightChild = opr->m_rightChild;
		}
		else ast.setRoot(opr->m_rightChild);
		opr->m_rightChild->m_parent = parent;
		ast.removeSubtree(opr->m_leftChild);
		delete opr;
		return true;
	}
	else if	(m_val == 0) {
		if (opr->getOpr() == '-' && (!opr->m_parent || !opr->m_parent->isNegation())) {
			TreeNode* negation = new UnaryOperation('-');
			TreeNode* parent = opr->m_parent;
			negation->m_parent = parent;
			if (parent) {
				if (parent->m_leftChild == opr)
					parent->m_leftChild = negation;
				else
					parent->m_rightChild = negation;
			}
			else ast.setRoot(negation);
			negation->m_leftChild = opr->m_rightChild;
			opr->m_rightChild->m_parent = negation;
			ast.removeSubtree(opr->m_leftChild);
			delete opr;
			negation->reduceMultipleNegations(ast);
			return true;
		}
		else if (opr->getOpr() == '-' || opr->getOpr() == '+') {
			TreeNode* parent = opr->m_parent;
			if (parent) {
				if (parent->m_leftChild == opr)
					parent->m_leftChild = opr->m_rightChild;
				else
					parent->m_rightChild = opr->m_rightChild;
			}
			else ast.setRoot(opr->m_rightChild);
			opr->m_rightChild->m_parent = parent;
			ast.removeSubtree(opr->m_leftChild);
			delete opr;
			return true;
		}
		else if (opr->getOpr() == '*' || opr->getOpr() == '/') {
			TreeNode* parent = opr->m_parent;
			ast.removeSubtree(opr);
			ast.addChild(parent, Constant(0));
			if (parent) parent->reduceMultipleNegations(ast);
			return true;
		}
	}
	return opr->m_rightChild->calculate2_leftIsConstant(ast, this, opr);
}

bool Constant::calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	if (ifEasyReducePossible(ast, opr)) return true;

	int a = (left->m_parent->isNegation() ? -left->m_val : left->m_val);
	int b = m_parent->isNegation() ? -m_val : m_val;
	int result = 0;

	if (opr->getOpr() == '+') result = a + b;
	else if (opr->getOpr() == '-') result = a - b;
	else if (opr->getOpr() == '*') result = a * b;
	else if (opr->getOpr() == '^') result = pow(a, b);
	else if (opr->getOpr() == '/') {
		int nwd = NWD(a, b);
		if (nwd == b)
			result = a / b;
		else return false;
	}
	else return false;

	TreeNode* parent = opr->m_parent;
	ast.removeSubtree(opr);
	if (result < 0)
		ast.addChild(ast.addChild(parent, UnaryOperation('-')), Constant(-result));
	else
		ast.addChild(parent, Constant(result));
	if (parent && parent->isNegation()) {
		if (result == 0) {
			if (parent->m_parent) {
				if (parent->m_leftChild == parent)
					parent->m_leftChild = this;
				else
					parent->m_rightChild = this;
			}
			else ast.setRoot(this);
			m_parent = parent->m_parent;
			delete parent;
		}
		else
			parent->reduceMultipleNegations(ast);
	}
	return true;
}

bool Constant::calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr)
{
	if (ifEasyReducePossible(ast, opr)) return true;

	Matrix result;

	if (opr->getOpr() == '^') {
		result = *left ^ (m_parent->isNegation() ? -m_val : m_val);
	}
	else {
		int scalar = (left->m_parent->isNegation() && m_parent->isNegation() ||
			!left->m_parent->isNegation() && !m_parent->isNegation()) ?	m_val : -m_val;

		if (opr->getOpr() == '*')
			result = *left * scalar;
		else if (opr->getOpr() == '/')
			result = *left / scalar;
		else
			throw std::invalid_argument("Error: AST: Unknown matrix operation!");
	}
	TreeNode* parent = opr->m_parent;
	ast.removeSubtree(opr);
	ast.addChild(parent, Matrix(result));
	return true;
}

bool Constant::calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr)
{
	if (ifEasyReducePossible(ast, opr)) return true;
	return TreeNode::calculate2_leftIsFraction(ast, left, opr);
}

bool Constant::calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return ifEasyReducePossible(ast, opr);
}

bool Constant::reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	int nwd = opr->m_rightChild->reduceFraction2(ast, this, opr);

	if (nwd > 1) {
		m_val /= nwd;
		if (m_val == 1) m_parent->calculate(ast);
		return true;
	}
	return false;
}

int Constant::reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	int a = (left->m_parent->isNegation() ? -left->m_val : left->m_val);
	int b = m_parent->isNegation() ? -m_val : m_val;
	int nwd = NWD(a, b);
	if (nwd > 1) {
		m_val /= nwd;
		if (m_val == 1) m_parent->calculate(ast);
	}
	return nwd;
}

Napis Constant::toNapis()
{
	return m_val;
}
