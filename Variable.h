#pragma once
#include "TreeNode.h"

class Variable : public TreeNode
{
	char m_name;
public:
	Variable(char name);

	// metody wspomagaj¹ce tworzenie drzewa
	TreeNode* clone() const override;
	
	// konwersja do postaci tekstowej
	Napis toNapis() override;
};

