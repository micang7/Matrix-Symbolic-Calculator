#include "Operation.h"

#include <iostream>

Operation::Operation(const Napis& opr) : m_opr(opr) {}

Operation::Operation(Napis&& opr) : m_opr(std::move(opr)) {}

Operation::Operation(const Operation& original) : m_opr(original.m_opr) {}

Operation::Operation(Operation&& original) noexcept
	: m_opr(std::move(original.m_opr)) {}

const Napis& Operation::getOpr() const
{
	return m_opr;
}

void Operation::setOpr(Napis&& opr)
{
	m_opr = std::move(opr);
}
