#pragma once
#include "TreeNode.h"

#include "AbstractSyntaxTree.h"
#include "BinaryOperation.h"

class Matrix : public TreeNode
{
protected:
	int m_rows;
	int m_cols;
	AbstractSyntaxTree*** m_matrix;

	Matrix(int rows, int cols);
public:
	Matrix();

	Matrix(const Napis& expression);

	Matrix(const Matrix& original);
	Matrix& operator=(const Matrix& original);

	Matrix(Matrix&& original) noexcept;
	Matrix& operator=(Matrix&& original) noexcept;

	~Matrix();

	int getRows() const;
	int getCols() const;

	const AbstractSyntaxTree& getElement(int row, int col) const;
	void setElement(int row, int col, AbstractSyntaxTree&& element);

	bool isSquare() const;

	Matrix transposed() const;
	Matrix cofactorMatrix() const; // macierz dope³nieñ algebraicznych
	Matrix inversed() const;
	Matrix submatrix(int row, int col) const;

	AbstractSyntaxTree determinant() const;

	Matrix operator~();
	
	Matrix operator+(Matrix& matrix2);
	Matrix operator-(Matrix& matrix2);
	Matrix operator*(Matrix& matrix2);

	Matrix operator*(int scalar2);
	Matrix operator/(int scalar2);
	Matrix operator^(int exponent2);
	
	friend Matrix operator*(int scalar1, Matrix& matrix2);

	// metody wspomagaj¹ce tworzenie drzewa
	TreeNode* clone() const override;

	// metody upraszczaj¹ce drzewo
	bool calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr) override;
	bool calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr) override;

	// konwersja do postaci tekstowej
	Napis toNapis() override;
	Napis toNapisExpand() override;
};