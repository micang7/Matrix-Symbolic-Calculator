#include "Variable.h"

#include <iostream>
#include "AbstractSyntaxTree.h"

Variable::Variable(char name)
{
	m_name = name;
}

TreeNode* Variable::clone() const
{
	return new Variable(*this);
}

Napis Variable::toNapis()
{
	return m_name;
}
