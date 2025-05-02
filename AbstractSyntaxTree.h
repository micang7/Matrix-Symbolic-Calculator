#pragma once
#include "TreeNode.h"
#include "Napis.h"
#include "Constant.h"
#include "Variable.h"
class Matrix;

class AbstractSyntaxTree
{
	TreeNode* m_root;

	void reflectExpression(Napis&& expression);
public:
	AbstractSyntaxTree();

	AbstractSyntaxTree(const Napis& expression);
	AbstractSyntaxTree(Napis&& expression);

	AbstractSyntaxTree(const Constant& constant);
	AbstractSyntaxTree(const Variable& variable);
	AbstractSyntaxTree(const Matrix& matrix);
	AbstractSyntaxTree(Matrix&& matrix);

	AbstractSyntaxTree(const AbstractSyntaxTree& original);
	AbstractSyntaxTree& operator=(const AbstractSyntaxTree& original);
	
	AbstractSyntaxTree(AbstractSyntaxTree&& original) noexcept;
	AbstractSyntaxTree& operator=(AbstractSyntaxTree&& original) noexcept;

	~AbstractSyntaxTree();

	TreeNode* getRoot() const;
	void setRoot(TreeNode* root);
	
	TreeNode* addChild(TreeNode* node, const TreeNode& child);

	void addSubtree(TreeNode* node, const TreeNode* subtreeRoot);
	void removeSubtree(TreeNode* subtreeRoot);

	bool operator==(int value) const;
	
	AbstractSyntaxTree operator~();

	// l-value + l_value
	AbstractSyntaxTree operator+(const AbstractSyntaxTree& ast2) const;
	AbstractSyntaxTree operator-(const AbstractSyntaxTree& ast2) const;
	AbstractSyntaxTree operator*(const AbstractSyntaxTree& ast2) const;
	AbstractSyntaxTree operator/(const AbstractSyntaxTree& ast2) const;
	AbstractSyntaxTree operator^(const AbstractSyntaxTree& ast2) const;

	// l-value + r_value
	AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast2) const;
	AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast2) const;
	AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast2) const;
	AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast2) const;
	AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast2) const;

	// r-value + l_value
	friend AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2);
	friend AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2);
	friend AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2);
	friend AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2);
	friend AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree& ast2);

	// r-value + r_value
	friend AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);

	// konwersja do postaci tekstowej
	Napis toNapis(TreeNode* node = nullptr) const;
	Napis toNapisExpand(TreeNode* node = nullptr) const;
};