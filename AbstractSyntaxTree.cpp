#include "AbstractSyntaxTree.h"

#include "Parser.h"
#include "Constant.h"
#include "Matrix.h"
#include "Variable.h"
#include "BinaryOperation.h"
#include "UnaryOperation.h"

#include <iostream>

// bool SIMPLIFY_ON = true;
bool SIMPLIFY_ON = false;

void AbstractSyntaxTree::reflectExpression(Napis&& expression)
{
	if (!CYK(expression)) throw std::invalid_argument("Error: AST: Invalid expression!");

	expression = infixToPrefix(expression);

	m_root = nullptr;
	TreeNode* current = m_root;
	Napis constant;

	int i = 0;
	while (i < expression.getLen()) {
		char chr = expression[i];

		if (chr == '{') {
			int balance = 1;
			Napis matrix = "{";
			while (balance) {
				i++;
				chr = expression[i];
				if (chr == '{') balance++;
				else if (chr == '}') balance--;
				matrix += chr;
			}
			current = addChild(current, Matrix(matrix))->nextFreeNode();
			i++;
		}
		else if (std::isdigit(chr))
			constant += chr;
		else if (chr == ' ') {
			current = addChild(current, Constant(constant.toInt()))->nextFreeNode();
			constant = "";
		}
		else if (std::isalpha(chr)) {
			current = addChild(current, Variable(chr))->nextFreeNode();
			i++;
		}
		else if (chr == '~') {
			current = addChild(current, UnaryOperation("-"))->nextFreeNode();
			i++;
		}
		else {
			current = addChild(current, BinaryOperation(chr))->nextFreeNode();
			i++;
		}
		i++;
	}
	if (constant.getLen()) addChild(current, Constant(constant.toInt()));

    // if (SIMPLIFY_ON) m_root->simplify(*this);
    m_root->simplify(*this);
}

AbstractSyntaxTree::AbstractSyntaxTree()
{
	TreeNode* copy = new Constant(0);
	m_root = copy;
}

AbstractSyntaxTree::AbstractSyntaxTree(const Napis& expression)
{
	reflectExpression(Napis(expression));
}

AbstractSyntaxTree::AbstractSyntaxTree(Napis&& expression)
{
	reflectExpression(std::move(expression));
}

AbstractSyntaxTree::AbstractSyntaxTree(const Constant& constant)
{
	TreeNode* copy = new Constant(constant);
	m_root = copy;
}

AbstractSyntaxTree::AbstractSyntaxTree(const Variable& variable)
{
	TreeNode* copy = new Variable(variable);
	m_root = copy;
}

AbstractSyntaxTree::AbstractSyntaxTree(const Matrix& matrix)
{
	TreeNode* copy = new Matrix(matrix);
	m_root = copy;
}

AbstractSyntaxTree::AbstractSyntaxTree(Matrix&& matrix)
{
	TreeNode* copy = new Matrix(std::move(matrix));
	m_root = copy;
}

AbstractSyntaxTree::AbstractSyntaxTree(const AbstractSyntaxTree& original)
{
	addSubtree(nullptr, original.m_root);
}

AbstractSyntaxTree& AbstractSyntaxTree::operator=(const AbstractSyntaxTree& original)
{
	if (this != &original) {
		removeSubtree(m_root);
		addSubtree(nullptr, original.m_root);
	}
	return *this;
}

AbstractSyntaxTree::AbstractSyntaxTree(AbstractSyntaxTree&& original) noexcept
{
	m_root = original.m_root;

	original.m_root = nullptr;
}

AbstractSyntaxTree& AbstractSyntaxTree::operator=(AbstractSyntaxTree&& original) noexcept
{
	if (this != &original) {
		removeSubtree(m_root);

		m_root = original.m_root;

		original.m_root = nullptr;
	}
	return *this;
}

AbstractSyntaxTree::~AbstractSyntaxTree()
{
	removeSubtree(m_root);
}

TreeNode* AbstractSyntaxTree::getRoot() const
{
	return m_root;
}

void AbstractSyntaxTree::setRoot(TreeNode* root)
{
	m_root = root;
}

TreeNode* AbstractSyntaxTree::addChild(TreeNode* node, const TreeNode& child)
{
	TreeNode* copy = child.clone();

	if (!node) {
		m_root = copy;
		return m_root;
	}

	copy->m_parent = node;

	if (node->m_leftChild)
		node->m_rightChild = copy;
	else
		node->m_leftChild = copy;

	return copy;
}

void AbstractSyntaxTree::addSubtree(TreeNode* node, const TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	TreeNode* copy = subtreeRoot->clone();

	if (node) {
		copy->m_parent = node;
		if (node->m_leftChild)
			node->m_rightChild = copy;
		else
			node->m_leftChild = copy;
	}
	else m_root = copy;
	if (subtreeRoot->m_leftChild) addSubtree(copy, subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) addSubtree(copy, subtreeRoot->m_rightChild);
}

void AbstractSyntaxTree::removeSubtree(TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	if (subtreeRoot->m_leftChild) removeSubtree(subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) removeSubtree(subtreeRoot->m_rightChild);

	if (subtreeRoot->m_parent) {
		if (subtreeRoot->m_parent->m_leftChild == subtreeRoot)
			subtreeRoot->m_parent->m_leftChild = nullptr;
		else if (subtreeRoot->m_parent->m_rightChild == subtreeRoot)
			subtreeRoot->m_parent->m_rightChild = nullptr;
	}

	delete subtreeRoot;
}

bool AbstractSyntaxTree::operator==(int value) const
{
	return !m_root->m_leftChild && !m_root->m_rightChild && *m_root == value;
}

AbstractSyntaxTree AbstractSyntaxTree::operator~()
{
	AbstractSyntaxTree nowe;
	nowe.addSubtree(nowe.addChild(nullptr, UnaryOperation("-")), m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator+(const AbstractSyntaxTree& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("+"));
	nowe.addSubtree(nowe.m_root, m_root);
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator-(const AbstractSyntaxTree& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("-"));
	nowe.addSubtree(nowe.m_root, m_root);
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator*(const AbstractSyntaxTree& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("*"));
	nowe.addSubtree(nowe.m_root, m_root);
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator/(const AbstractSyntaxTree& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("/"));
	nowe.addSubtree(nowe.m_root, m_root);
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator^(const AbstractSyntaxTree& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("^"));
	nowe.addSubtree(nowe.m_root, m_root);
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator+(AbstractSyntaxTree&& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("+"));
	nowe.addSubtree(nowe.m_root, m_root);
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator-(AbstractSyntaxTree&& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("-"));
	nowe.addSubtree(nowe.m_root, m_root);
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator*(AbstractSyntaxTree&& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("*"));
	nowe.addSubtree(nowe.m_root, m_root);
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator/(AbstractSyntaxTree&& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("/"));
	nowe.addSubtree(nowe.m_root, m_root);
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree AbstractSyntaxTree::operator^(AbstractSyntaxTree&& ast2) const
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("^"));
	nowe.addSubtree(nowe.m_root, m_root);
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

Napis AbstractSyntaxTree::toNapis(TreeNode* node) const
{
	if (!node) node = m_root;
	if (!node) return "";

	return node->toNapis();
}

Napis AbstractSyntaxTree::toNapisExpand(TreeNode* node) const
{
	if (!node) node = m_root;
	if (!node) return "";

	return node->toNapisExpand();
}

AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("+"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("-"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("*"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("/"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("^"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	nowe.addSubtree(nowe.m_root, ast2.m_root);
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("+"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("-"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("*"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("/"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}

AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree nowe;
	nowe.addChild(nullptr, BinaryOperation("^"));
	ast1.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_leftChild = ast1.m_root;
	ast1.m_root = nullptr;
	ast2.m_root->m_parent = nowe.m_root;
	nowe.m_root->m_rightChild = ast2.m_root;
	ast2.m_root = nullptr;
	if (SIMPLIFY_ON) nowe.m_root->simplify(nowe);
	return nowe;
}
