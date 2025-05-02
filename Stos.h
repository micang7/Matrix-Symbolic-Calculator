#pragma once

class ElementStosu
{
	char m_key;
public:
	ElementStosu* m_next;

	ElementStosu(char key);
	
	char getKey() const;
};

class Stos
{
	ElementStosu* m_top;
public:
	Stos();
	~Stos();

	bool isEmpty() const;
	void push(char key);
	char top() const;
	char pop();
};