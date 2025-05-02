#include "Stos.h"

#include <iostream>

ElementStosu::ElementStosu(char key)
{
	m_key = key;
	m_next = nullptr;
}

char ElementStosu::getKey() const
{
	return m_key;
}

Stos::Stos()
{
	m_top = nullptr;
}

Stos::~Stos()
{
	ElementStosu* top = m_top;
	while (top) {
		top = m_top->m_next;
		delete m_top;
	}
}

bool Stos::isEmpty() const
{
	return !m_top;
}

void Stos::push(char key)
{
	ElementStosu* nowy = new ElementStosu(key);
	nowy->m_next = m_top;
	m_top = nowy;
}

char Stos::top() const
{
	if (isEmpty()) throw std::underflow_error("Error: Stos: Stack is empty!");
	return m_top->getKey();
}

char Stos::pop()
{
	if (isEmpty()) throw std::underflow_error("Error: Stos: Stack is empty!");
	ElementStosu* top = m_top;
	m_top = m_top->m_next;
	return top->getKey();
}