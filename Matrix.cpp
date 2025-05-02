#include "Matrix.h"

#include <iostream>
#include <cmath>

Matrix::Matrix(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	m_matrix = new AbstractSyntaxTree**[m_rows];
	for (int r = 0; r < m_rows; r++) {
		m_matrix[r] = new AbstractSyntaxTree*[m_cols];
		for (int c = 0; c < m_cols; c++)
			m_matrix[r][c] = new AbstractSyntaxTree();
	}
}

Matrix::Matrix()
{
	m_rows = 0;
	m_cols = 0;
	m_matrix = nullptr;
}

Matrix::Matrix(const Napis& expression)
{
	m_rows = 0;
	m_cols = 0;
	Napis elements;

	int balance = 0;
	int cols = 0;
	int i = 1;

	while (i < expression.getLen() - 1) {
		char chr = expression[i];

		if (chr == '{') {
			if (balance == 0) m_rows++;
			else elements += chr;
			balance++;
		}
		else if (chr == '}') {
			balance--;
			if (balance == 0) {
				if (cols > m_cols) m_cols = cols;
				cols = 0;
				elements += ';';
				i++;
			}
			else
				elements += chr;
		}
		else if (balance != 0) {
			if (chr == ',' && balance == 1) {
				elements += ';';
				cols++;
			}
			else {
				elements += chr;
				if (cols == 0) cols = 1;
			}
		}
		i++;
	}

	i = 0;
	Napis element;
	m_matrix = new AbstractSyntaxTree * *[m_rows];
	for (int r = 0; r < m_rows; r++) {
		m_matrix[r] = new AbstractSyntaxTree * [m_cols];
		for (int c = 0; c < m_cols; c++) {
			element = "";
			while (elements[i] != ';') {
				element += elements[i];
				i++;
			}
			i++;
			m_matrix[r][c] = new AbstractSyntaxTree(element);
		}
	}
}

Matrix::Matrix(const Matrix& original)
{
	m_rows = original.m_rows;
	m_cols = original.m_cols;
	m_matrix = new AbstractSyntaxTree * *[m_rows];
	for (int r = 0; r < m_rows; r++) {
		m_matrix[r] = new AbstractSyntaxTree * [m_cols];
		for (int c = 0; c < m_cols; c++)
			m_matrix[r][c] = new AbstractSyntaxTree(*original.m_matrix[r][c]);
	}
}

Matrix& Matrix::operator=(const Matrix& original)
{
	if (this != &original) {
		for (int r = 0; r < m_rows; r++) {
			for (int c = 0; c < m_cols; c++)
				delete m_matrix[r][c];
			delete[] m_matrix[r];
		}
		delete[] m_matrix;

		m_rows = original.m_rows;
		m_cols = original.m_cols;
		m_matrix = new AbstractSyntaxTree * *[m_rows];
		for (int r = 0; r < m_rows; r++) {
			m_matrix[r] = new AbstractSyntaxTree * [m_cols];
			for (int c = 0; c < m_cols; c++)
				m_matrix[r][c] = new AbstractSyntaxTree(*original.m_matrix[r][c]);
		}
	}
	return *this;
}

Matrix::Matrix(Matrix&& original) noexcept
{
	m_rows = original.m_rows;
	m_cols = original.m_cols;
	m_matrix = original.m_matrix;

	original.m_rows = 0;
	original.m_cols = 0;
	original.m_matrix = nullptr;
}

Matrix& Matrix::operator=(Matrix&& original) noexcept
{
	if (this != &original) {
		for (int r = 0; r < m_rows; r++) {
			for (int c = 0; c < m_cols; c++)
				delete m_matrix[r][c];
			delete[] m_matrix[r];
		}
		delete[] m_matrix;

		m_rows = original.m_rows;
		m_cols = original.m_cols;
		m_matrix = original.m_matrix;

		original.m_rows = 0;
		original.m_cols = 0;
		original.m_matrix = nullptr;
	}
	return *this;
}

Matrix::~Matrix()
{
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++)
			delete m_matrix[r][c];
		delete[] m_matrix[r];
	}
	delete[] m_matrix;
}

int Matrix::getRows() const
{
	return m_rows;
}

int Matrix::getCols() const
{
	return m_cols;
}

const AbstractSyntaxTree& Matrix::getElement(int row, int col) const
{
	return *m_matrix[row][col];
}

void Matrix::setElement(int row, int col, AbstractSyntaxTree&& element)
{
	*m_matrix[row][col] = element;
}

bool Matrix::isSquare() const
{
	return m_rows == m_cols;
}

Matrix Matrix::transposed() const
{
	Matrix transposed(m_cols, m_rows);

	for (int r = 0; r < m_cols; r++)
		for (int c = 0; c < m_rows; c++)
			*transposed.m_matrix[r][c] = *m_matrix[c][r];
	
	return transposed;
}

Matrix Matrix::cofactorMatrix() const
{
	if (m_rows == 1) return *this;

	Matrix result(m_rows, m_cols);

	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			*result.m_matrix[r][c] = AbstractSyntaxTree((r + c) % 2 == 0 ? "1" : "-1") * submatrix(r, c).determinant();

	return result;
}

Matrix Matrix::inversed() const
{
	// Metoda dope³nieñ algebraicznych
	AbstractSyntaxTree det = determinant();

	if (det == 0) throw std::invalid_argument("Error: Matrix: Determinant is zero. Inverse matrix does not exist.");
	
	Matrix inversed = (m_rows == 1 ? Matrix("{{1}}") : cofactorMatrix().transposed()); // macierz do³¹czona

	for (int r = 0; r < inversed.m_rows; r++)
		for (int c = 0; c < inversed.m_cols; c++)
			*inversed.m_matrix[r][c] = *inversed.m_matrix[r][c] / det;
	
	return inversed;
}

Matrix Matrix::submatrix(int row, int col) const
{
	if (row < 0 || row >= m_rows)
		throw std::invalid_argument("Error: Matrix: Row out of range!");
	if (col < 0 || col >= m_cols)
		throw std::invalid_argument("Error: Matrix: Column out of range!");

	Matrix submatrix(m_rows - 1, m_cols - 1);

	for (int r = 0; r < m_rows - 1; r++)
		for (int c = 0; c < m_cols - 1; c++)
			*submatrix.m_matrix[r][c] = *m_matrix[r + (r < row ? 0 : 1)][c + (c < col ? 0 : 1)];

	return submatrix;
}

AbstractSyntaxTree Matrix::determinant() const
{
	if (m_rows != m_cols)
		throw std::invalid_argument(
			"Error: Matrix: Matrix is not square. Determinant does not exist!");

	if (m_rows == 1) { // z rozwiniêcia Laplace'a
		return *m_matrix[0][0];
	}
	else if (m_rows == 2) { // ze wzoru
		return *m_matrix[0][0] * *m_matrix[1][1] -
			*m_matrix[0][1] * *m_matrix[1][0];
	}
	else if (m_rows == 3) { // regu³a Sarrusa
		return *m_matrix[0][0] * *m_matrix[1][1] * *m_matrix[2][2] +
			*m_matrix[0][1] * *m_matrix[1][2] * *m_matrix[2][0] +
			*m_matrix[0][2] * *m_matrix[1][0] * *m_matrix[2][1] -
			*m_matrix[2][0] * *m_matrix[1][1] * *m_matrix[0][2] -
			*m_matrix[2][1] * *m_matrix[1][2] * *m_matrix[0][0] -
			*m_matrix[2][2] * *m_matrix[1][0] * *m_matrix[0][1];
	}
	int j = 0; // rozwiniêciem Laplace'a wzglêdem  kolumny j
	AbstractSyntaxTree det;

	for (int i = 0; i < m_rows; i++)
		det = det + AbstractSyntaxTree((i + j) % 2 == 0 ? "1" : "-1") *
		*m_matrix[i][j] * submatrix(i, j).determinant();

	return det;
}

Matrix Matrix::operator~()
{
	Matrix result(m_rows, m_cols);
	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			*result.m_matrix[r][c] = ~(*m_matrix[r][c]);
	return result;
}

Matrix Matrix::operator+(Matrix& matrix2)
{
	if (m_rows != matrix2.m_rows || m_cols != matrix2.m_cols)
		throw std::invalid_argument(
			"Error: Matrix: Matrices have different sizes. Addition not possible!");

	Matrix result(m_rows, m_cols);

	for (int r = 0; r < result.m_rows; r++)
		for (int c = 0; c < result.m_cols; c++)
			*result.m_matrix[r][c] = *m_matrix[r][c] + *matrix2.m_matrix[r][c];

	return result;
}

Matrix Matrix::operator-(Matrix& matrix2)
{
	if (m_rows != matrix2.m_rows || m_cols != matrix2.m_cols)
		throw std::invalid_argument(
			"Error: Matrix: Matrices have different sizes. Subtraction not possible!");

	Matrix result(m_rows, m_cols);

	for (int r = 0; r < result.m_rows; r++)
		for (int c = 0; c < result.m_cols; c++)
			*result.m_matrix[r][c] = *m_matrix[r][c] - *matrix2.m_matrix[r][c];
	
	return result;
}

Matrix Matrix::operator*(Matrix& matrix2)
{
	if (m_cols != matrix2.m_rows)
		throw std::invalid_argument(
			"Error: Matrix: Incompatible matrix sizes. Multiplication not possible!");

	Matrix result(m_rows, matrix2.m_cols);

	for (int r = 0; r < result.m_rows; r++)
		for (int c = 0; c < result.m_cols; c++) {
			AbstractSyntaxTree scalarProduct;
			for (int k = 0; k < m_cols; k++)
				scalarProduct = scalarProduct + *m_matrix[r][k] * *matrix2.m_matrix[k][c];
			*result.m_matrix[r][c] = scalarProduct;
		}
	return result;
}

Matrix Matrix::operator*(int scalar2)
{
	Matrix result(m_rows, m_cols);

	for (int r = 0; r < result.m_rows; r++)
		for (int c = 0; c < result.m_cols; c++)
			*result.m_matrix[r][c] = *m_matrix[r][c] * Constant(scalar2);
	
	return result;
}

Matrix Matrix::operator/(int scalar2)
{
	Matrix result(m_rows, m_cols);

	for (int r = 0; r < result.m_rows; r++)
		for (int c = 0; c < result.m_cols; c++)
			*result.m_matrix[r][c] = *m_matrix[r][c] / Constant(scalar2);
	
	return result;
}

Matrix Matrix::operator^(int exponent2)
{
	if (m_rows != m_cols)
		throw std::invalid_argument("Error: Matrix: Matrix is not square. Exponentiation not possible.");

	Matrix result(*this);

	if (exponent2 < 0) {
		result = result.inversed();
		exponent2 = -exponent2;
	}
	
	int count = log2(exponent2);
	int rest = exponent2 - pow(2, count);

	for (int i = 0; i < count; i++)
		result = result * result;

	for (int i = 0; i < rest; i++)
		result = result * *this;

	return result;
}

TreeNode* Matrix::clone() const
{
	return new Matrix(*this);
}

bool Matrix::calculate1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
	return opr->m_rightChild->calculate2_leftIsMatrix(ast, this, opr);
}

bool Matrix::calculate2_leftIsConstant(AbstractSyntaxTree& ast, Constant* left, BinaryOperation* opr)
{
	if (opr->getOpr() == '*') {
		int scalar = (m_parent->isNegation() ? -left->m_val : left->m_val);

		TreeNode* parent = opr->m_parent;
		Matrix result = scalar * *this;
		ast.removeSubtree(opr);
		ast.addChild(parent, result);

		return true;
	}
	throw std::invalid_argument("Error: AST: Unknown matrix operation!");
}

bool Matrix::calculate2_leftIsMatrix(AbstractSyntaxTree& ast, Matrix* left, BinaryOperation* opr)
{
	Matrix result;
	if (opr->getOpr() == '+')
		result = *left + *this;
	else if (opr->getOpr() == '-')
		result = *left - *this;
	else if (opr->getOpr() == '*')
		result = *left * *this;
	else throw std::invalid_argument("Error: AST: Unknown matrix operation!");

	TreeNode* parent = opr->m_parent;
	ast.removeSubtree(opr);
	ast.addChild(parent, Matrix(result));
	
	return true;
}

Napis Matrix::toNapis()
{
	if (m_rows == 0 || m_cols == 0) return "";

	Napis wyrazenie = "{";
	for (int i = 0; i < m_rows - 1; i++) {
		wyrazenie += "{";
		for (int j = 0; j < m_cols - 1; j++) {
			wyrazenie += m_matrix[i][j]->toNapis();
			wyrazenie += ",";
		}
		wyrazenie += m_matrix[i][m_cols - 1]->toNapis();
		wyrazenie += "},";
	}
	wyrazenie += "{";
	for (int j = 0; j < m_cols - 1; j++) {
		wyrazenie += m_matrix[m_rows - 1][j]->toNapis();
		wyrazenie += ",";
	}
	wyrazenie += m_matrix[m_rows - 1][m_cols - 1]->toNapis();
	wyrazenie += "}}";

	return wyrazenie;
}

Napis Matrix::toNapisExpand()
{
	if (m_rows == 0 || m_cols == 0) return "";

	int* maxWidth = new int[m_cols] {0};
	int len;

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			len = m_matrix[i][j]->toNapis().getLen();
			if (len > maxWidth[j]) maxWidth[j] = len;
		}
	}

	Napis element;
	Napis separator = "  ";
	Napis wyrazenie = "";
	for (int i = 0; i < m_rows - 1; i++) {
		for (int j = 0; j < m_cols - 1; j++) {
			element = m_matrix[i][j]->toNapis();
			wyrazenie += element;
			for (int k = 0; k < maxWidth[j] - element.getLen(); k++)
				wyrazenie += " ";
			wyrazenie += separator;
		}
		wyrazenie += m_matrix[i][m_cols - 1]->toNapis();
		wyrazenie += '\n';
	}
	for (int j = 0; j < m_cols - 1; j++) {
		element = m_matrix[m_rows - 1][j]->toNapis();
		wyrazenie += element;
		for (int k = 0; k < maxWidth[j] - element.getLen(); k++)
			wyrazenie += " ";
		wyrazenie += separator;
	}
	wyrazenie += m_matrix[m_rows - 1][m_cols - 1]->toNapis();

	delete[] maxWidth;

	return wyrazenie;
}

Matrix operator*(int scalar1, Matrix& matrix2)
{
	Matrix result(matrix2.m_rows, matrix2.m_cols);

	result.m_matrix = new AbstractSyntaxTree * *[matrix2.m_rows];
	for (int r = 0; r < matrix2.m_rows; r++) {
		result.m_matrix[r] = new AbstractSyntaxTree * [matrix2.m_cols];
		for (int c = 0; c < matrix2.m_cols; c++)
			result.m_matrix[r][c] = new AbstractSyntaxTree(
				Constant(scalar1) * *matrix2.m_matrix[r][c]
			);
	}
	return result;
}
