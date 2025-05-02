#include "BinaryOperation.h"

#include <iostream>
#include "AbstractSyntaxTree.h"
#include "Parser.h"
#include "UnaryOperation.h"

BinaryOperation::BinaryOperation(const Napis& opr) : Operation(opr) {}

BinaryOperation::BinaryOperation(Napis&& opr) : Operation(std::move(opr)) {}

TreeNode* BinaryOperation::clone() const
{
	return new BinaryOperation(*this);
}

TreeNode* BinaryOperation::nextFreeNode()
{
	if (!m_leftChild || !m_rightChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

void BinaryOperation::simplify(AbstractSyntaxTree& ast)
{
	m_leftChild->simplify(ast);
	m_rightChild->simplify(ast);
	putNegationBeforeOperation(ast);
	putVariableOut(ast);
	if (!calculate(ast)) {
		putScalarOut(ast);
		reduceFraction(ast);
	}
}

void BinaryOperation::putNegationBeforeOperation(AbstractSyntaxTree& ast)
{
	TreeNode* to_delete;

	if (m_leftChild->isNegation()) {
		if (m_rightChild->isNegation()) {
			if (m_opr == '*' || m_opr == '/') {
				to_delete = m_leftChild;
				m_leftChild = to_delete->m_leftChild;
				m_leftChild->m_parent = this;
				delete to_delete;

				to_delete = m_rightChild;
				m_rightChild = to_delete->m_leftChild;
				m_rightChild->m_parent = this;
				delete to_delete;

				m_leftChild->simplify(ast);
				m_rightChild->simplify(ast);
				return;
			}

			if (m_opr == '+') {
				m_opr = '-';
			}
			else if (m_opr == '-') {
				m_opr = '+';
			}
			else return;
			to_delete = m_rightChild;
			m_rightChild = to_delete->m_leftChild;
			m_rightChild->m_parent = this;
			delete to_delete;

			m_leftChild->simplify(ast);
			m_rightChild->simplify(ast);
			return;
		}

		if (m_parent && m_parent->isNegation()) {
			if (m_opr == '*' || m_opr == '/') {
				to_delete = m_parent;
				m_parent = to_delete->m_parent;
				if (m_parent) {
					if (m_parent->m_leftChild == to_delete)
						m_parent->m_leftChild = this;
					else
						m_parent->m_rightChild = this;
				}
				else ast.setRoot(this);
				delete to_delete;

				to_delete = m_leftChild;
				m_leftChild = to_delete->m_leftChild;
				m_leftChild->m_parent = this;
				delete to_delete;
			}
			m_leftChild->simplify(ast);
			return;
		}

		if (m_opr == '*' || m_opr == '/') {
			TreeNode* negation = m_leftChild;
			negation->m_parent = m_parent;
			if (m_parent) {
				if (m_parent->m_leftChild == this)
					m_parent->m_leftChild = negation;
				else
					m_parent->m_rightChild = negation;
			}
			else ast.setRoot(negation);
			negation->m_leftChild->m_parent = this;
			m_leftChild = negation->m_leftChild;
			negation->m_leftChild = this;
			m_parent = negation;

			m_leftChild->simplify(ast);
			return;
		}
	}

	if (m_rightChild->isNegation()) {
		if (m_opr == '*' || m_opr == '/') {
			if (m_parent && m_parent->isNegation()) {
				to_delete = m_parent;
				m_parent = to_delete->m_parent;
				if (m_parent) {
					if (m_parent->m_leftChild == to_delete)
						m_parent->m_leftChild = this;
					else
						m_parent->m_rightChild = this;
				}
				else ast.setRoot(this);
				delete to_delete;

				to_delete = m_rightChild;
				m_rightChild = to_delete->m_leftChild;
				m_rightChild->m_parent = this;
				delete to_delete;

				m_rightChild->simplify(ast);
				return;
			}

			TreeNode* negation = m_rightChild;
			negation->m_parent = m_parent;
			if (m_parent) {
				if (m_parent->m_leftChild == this)
					m_parent->m_leftChild = negation;
				else
					m_parent->m_rightChild = negation;
			}
			else ast.setRoot(negation);
			negation->m_leftChild->m_parent = this;
			m_rightChild = negation->m_leftChild;
			negation->m_leftChild = this;
			m_parent = negation;
			
			m_rightChild->simplify(ast);
			return;
		}

		if (m_opr == '+') {
			m_opr = '-';
		}
		else if (m_opr == '-') {
			m_opr = '+';
		}
		else return;
		to_delete = m_rightChild;
		m_rightChild = to_delete->m_leftChild;
		m_rightChild->m_parent = this;
		delete to_delete;

		m_rightChild->simplify(ast);
	}
}

char operationResult(Napis&& opr1, Napis&& opr2)
{
	if (opr1 == '+') {
		if (opr2 == '+') return '+';
		else return '-';
	}
	if (opr1 == '-') {
		if (opr2 == '+') return '-';
		else return '+';
	}
	if (opr1 == '*' && opr2 == '*') return '*';
	throw std::invalid_argument("Error: BinaryOperation: Invalid operators!");
}

void BinaryOperation::putVariableOut(AbstractSyntaxTree& ast)
{
	BinaryOperation* parent;
	if (m_parent && (parent = m_parent->isSameOperationType(std::move(m_opr)))) {
		if (m_leftChild->isVariable()) {
			if (m_rightChild->isVariable()) return;

			if (m_parent->m_leftChild == this) {
				m_opr = operationResult(std::move(m_opr), std::move(parent->m_opr));

				m_leftChild->m_parent = m_parent;
				m_parent->m_leftChild = m_leftChild;

				m_leftChild = m_rightChild;

				m_parent->m_rightChild->m_parent = this;
				m_rightChild = m_parent->m_rightChild;

				m_parent->m_rightChild = this;

				m_rightChild->simplify(ast);
			}
			else {
				if (parent->m_opr == '-') {
					TreeNode* negation = new UnaryOperation('-');
					negation->m_parent = this;
					negation->m_leftChild = m_leftChild;
					m_leftChild->m_parent = negation;
					m_leftChild = negation;
				}
				m_opr = operationResult(std::move(m_opr), std::move(parent->m_opr));

				if (m_opr == '+' || m_opr == '-') parent->m_opr = '+';
				else if (m_opr == '*') parent->m_opr = '*';

				m_parent->m_rightChild = m_parent->m_leftChild;

				m_leftChild->m_parent = m_parent;
				m_parent->m_leftChild = m_leftChild;

				m_parent->m_rightChild->m_parent = this;
				m_leftChild = m_parent->m_rightChild;

				m_parent->m_rightChild = this;

				m_leftChild->simplify(ast);
			}
		}
		else if (m_rightChild->isVariable()) {
			if (m_parent->m_leftChild == this) {
				m_parent->m_leftChild = m_parent->m_rightChild;

				m_rightChild->m_parent = m_parent;
				m_parent->m_rightChild = m_rightChild;

				m_parent->m_leftChild->m_parent = this;
				m_rightChild = m_parent->m_leftChild;

				m_parent->m_leftChild = this;

				m_rightChild->simplify(ast);
			}
			else {
				m_opr = parent->m_opr;
				parent->m_opr = operationResult(std::move(m_opr), std::move(parent->m_opr));

				m_rightChild->m_parent = m_parent;
				m_parent->m_rightChild = m_rightChild;

				m_rightChild = m_leftChild;

				m_parent->m_leftChild->m_parent = this;
				m_leftChild = m_parent->m_leftChild;

				m_parent->m_leftChild = this;

				m_leftChild->simplify(ast);
			}
		}
	}
}

BinaryOperation* BinaryOperation::isSameOperationType(Napis&& opr)
{
	if ((opr == '+' || opr == '-') && (m_opr == '+' || m_opr == '-')) return this;
	if (opr == '*' && m_opr == '*') return this;
	return nullptr;
}

void BinaryOperation::putScalarOut(AbstractSyntaxTree& ast)
{
	if (m_opr == '+' || m_opr == '-')
		m_leftChild->putScalarOut1(ast, this);
}

bool BinaryOperation::putScalarOut1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	if (m_opr == '*') {
		bool result = m_leftChild->putScalarOut1(ast, opr);
		if (!result) return m_rightChild->putScalarOut1(ast, opr);
	}
	return false;
}

int BinaryOperation::putScalarOut2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	if (m_opr == '*') {
		int result = m_leftChild->putScalarOut2(ast, left, opr);
		if (result > 1) return result;
		return m_rightChild->putScalarOut2(ast, left, opr);
	}
	return 1;
}

bool BinaryOperation::calculate(AbstractSyntaxTree& ast)
{
	return m_leftChild->calculate1(ast, this);
}

bool BinaryOperation::calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	if (m_opr == '/') return opr->m_rightChild->calculate2_leftIsFraction(ast, this, opr);
	return opr->m_rightChild->calculate2_leftIsOther(ast, opr);
}

bool BinaryOperation::calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	if (m_opr == '/') {
		if (opr->getOpr() == '*') {
			BinaryOperation* iloczyn = new BinaryOperation('*');

			iloczyn->m_leftChild = m_leftChild;
			m_leftChild->m_parent = iloczyn;

			iloczyn->m_rightChild = opr->m_leftChild;
			opr->m_leftChild->m_parent = iloczyn;

			m_leftChild = iloczyn;
			iloczyn->m_parent = this;

			opr->m_rightChild->m_parent = opr->m_parent;
			if (opr->m_parent) {
				if (opr->m_parent->m_leftChild == opr)
					opr->m_parent->m_leftChild = opr->m_rightChild;
				else
					opr->m_parent->m_rightChild = opr->m_rightChild;
			}
			else ast.setRoot(opr->m_rightChild);

			delete opr;

			simplify(ast);
			return true;
		}
		if (opr->getOpr() == '/') {
			BinaryOperation* iloczyn = new BinaryOperation('*');

			iloczyn->m_leftChild = opr->m_leftChild;
			opr->m_leftChild->m_parent = iloczyn;

			iloczyn->m_rightChild = m_rightChild;
			m_rightChild->m_parent = iloczyn;

			m_rightChild = m_leftChild;

			m_leftChild = iloczyn;
			iloczyn->m_parent = this;

			opr->m_rightChild->m_parent = opr->m_parent;
			if (opr->m_parent) {
				if (opr->m_parent->m_leftChild == opr)
					opr->m_parent->m_leftChild = opr->m_rightChild;
				else
					opr->m_parent->m_rightChild = opr->m_rightChild;
			}
			else ast.setRoot(opr->m_rightChild);

			delete opr;

			simplify(ast);
			return true;
		}
		if (opr->getOpr() == '+' || opr->getOpr() == '-') {
			BinaryOperation* iloczyn = new BinaryOperation('*');
			TreeNode* kopia_mianownika = m_rightChild->clone();

			iloczyn->m_leftChild = opr->m_leftChild;
			opr->m_leftChild->m_parent = iloczyn;

			iloczyn->m_rightChild = kopia_mianownika;
			kopia_mianownika->m_parent = iloczyn;

			opr->m_leftChild = iloczyn;
			iloczyn->m_parent = opr;

			opr->m_rightChild->m_parent = opr->m_parent;
			if (opr->m_parent) {
				if (opr->m_parent->m_leftChild == opr)
					opr->m_parent->m_leftChild = opr->m_rightChild;
				else
					opr->m_parent->m_rightChild = opr->m_rightChild;
			}
			else ast.setRoot(opr->m_rightChild);

			opr->m_rightChild = m_leftChild;
			m_leftChild->m_parent = opr;

			m_leftChild = opr;
			opr->m_parent = this;

			simplify(ast);
			return true;
		}
	}
	return false;
}

bool BinaryOperation::calculate2_leftIsFraction(AbstractSyntaxTree& ast, BinaryOperation* left, BinaryOperation* opr)
{
	if (m_opr == '/') {
		if (opr->m_opr == '*') {
			opr->m_opr = '/';
			left->m_opr = '*';
			m_opr = '*';
		
			TreeNode* myLeft = m_leftChild;
			m_leftChild = left->m_rightChild;
			left->m_rightChild->m_parent = this;
		
			left->m_rightChild = myLeft;
			myLeft->m_parent = left;
		
			opr->simplify(ast);
			return true;
		}
		if (opr->m_opr == '+' || opr->m_opr == '-') {
			AbstractSyntaxTree* kopia_mianownika_left = new AbstractSyntaxTree();
			kopia_mianownika_left->removeSubtree(kopia_mianownika_left->getRoot());
			kopia_mianownika_left->addSubtree(nullptr, left->m_rightChild);

			AbstractSyntaxTree* kopia_mianownika_right = new AbstractSyntaxTree();
			kopia_mianownika_right->removeSubtree(kopia_mianownika_right->getRoot());
			kopia_mianownika_right->addSubtree(nullptr, m_rightChild);

			left->m_opr = '*';
			m_opr = '*';

			TreeNode* myRight = m_rightChild;
			m_rightChild = left->m_rightChild;
			left->m_rightChild->m_parent = this;

			left->m_rightChild = myRight;
			myRight->m_parent = left;

			BinaryOperation* iloraz = new BinaryOperation('/');
			iloraz->m_parent = opr->m_parent;
			if (opr->m_parent) {
				if (opr->m_parent->m_leftChild == opr)
					opr->m_parent->m_leftChild = iloraz;
				else
					opr->m_parent->m_rightChild = iloraz;
			}
			else ast.setRoot(iloraz);

			iloraz->m_leftChild = opr;
			opr->m_parent = iloraz;

			BinaryOperation* iloczyn = new BinaryOperation('*');

			iloraz->m_rightChild = iloczyn;
			iloczyn->m_parent = iloraz;

			iloczyn->m_leftChild = kopia_mianownika_left->getRoot();
			kopia_mianownika_left->getRoot()->m_parent = iloczyn;

			iloczyn->m_rightChild = kopia_mianownika_right->getRoot();
			kopia_mianownika_right->getRoot()->m_parent = iloczyn;

			iloraz->simplify(ast);
			return true;
		}
	}
	return false;
}

bool BinaryOperation::calculate2_leftIsOther(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return calculate2_leftIsConstant(ast, nullptr, opr);
}

void BinaryOperation::reduceFraction(AbstractSyntaxTree& ast)
{
	if (m_opr == '/')
		m_leftChild->reduceFraction1(ast, this);
}

bool BinaryOperation::reduceFraction1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	if (m_opr == '*') {
		bool result = m_leftChild->reduceFraction1(ast, opr);
		if (!result) return m_rightChild->reduceFraction1(ast, opr);
	}
	return false;
}

int BinaryOperation::reduceFraction2(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	if (m_opr == '*') {
		int result = m_leftChild->reduceFraction2(ast, left, opr);
		if (result > 1) return result;
		return m_rightChild->reduceFraction2(ast, left, opr);
	}
	return 1;
}

Napis BinaryOperation::toNapis()
{
	bool left_parentheses = m_leftChild->isLowerOperatorPriority(std::move(m_opr));
	bool right_parentheses = m_rightChild->isLowerOperatorPriority(std::move(m_opr)) ||
		(m_opr == '-' && m_rightChild->m_leftChild) ||
		(m_opr == '/' && m_rightChild->m_leftChild) ||
		(m_rightChild->isNegation() && isNegationSignificant());
	return
		(left_parentheses ? "(" : "") + m_leftChild->toNapis() + (left_parentheses ? ")" : "")
		+ m_opr +
		(right_parentheses ? "(" : "") + m_rightChild->toNapis() + (right_parentheses ? ")" : "");
}

bool BinaryOperation::isLowerOperatorPriority(Napis&& opr) const
{
	return precedence(m_opr.getStr()[0]) < precedence(opr.getStr()[0]);
}

bool BinaryOperation::isNegationSignificant() const
{
	return m_opr == "+" || m_opr == "-";
}
