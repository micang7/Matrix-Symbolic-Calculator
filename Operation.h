#pragma once
#include "TreeNode.h"
#include "Napis.h"

class Operation : public TreeNode
{
protected:
	Napis m_opr;
public:
	Operation(const Napis& opr);
	Operation(Napis&& opr);
	
	Operation(const Operation& original);

	Operation(Operation&& original) noexcept;

	const Napis& getOpr() const;
	void setOpr(Napis&& opr);
};

