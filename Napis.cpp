#define _CRT_SECURE_NO_WARNINGS
#include "Napis.h"

#include <iostream>
#include <cstring>

Napis::Napis(const char* str)
{
	m_len = strlen(str);
	m_str = new char[m_len + 1];
	strcpy(m_str, str);
}

Napis::Napis(char chr)
{
	m_len = 1;
	m_str = new char[2];
	m_str[0] = chr;
	m_str[1] = '\0';
}

Napis::Napis(const Napis& original)
{
	m_len = original.m_len;
	m_str = new char[m_len + 1];
	strcpy(m_str, original.m_str);
}

Napis& Napis::operator=(const Napis& napis2)
{
	if (this != &napis2) {
		//this->~Napis();
		delete[] m_str;

		//new (this) Napis(napis2);
		m_len = napis2.m_len;
		m_str = new char[m_len + 1];
		strcpy(m_str, napis2.m_str);
	}
	return *this;
}

Napis::Napis(Napis&& original) noexcept
{
	m_len = original.m_len;
	m_str = original.m_str;

	original.m_len = 0;
	original.m_str = nullptr;
}

Napis& Napis::operator=(Napis&& napis2) noexcept
{
	if (this != &napis2) {
		m_len = napis2.m_len;
		m_str = napis2.m_str;

		napis2.m_len = 0;
		napis2.m_str = nullptr;
	}
	return *this;
}

Napis::Napis(int num)
{
	char* buffer = new char[11]; // liczba int mo¿e mieæ maksymalnie 10 cyfr
	sprintf(buffer, "%i", num);

	m_len = strlen(buffer);
	m_str = new char[m_len + 1];
	strcpy(m_str, buffer);

	delete[] buffer;
}

Napis::~Napis()
{
	delete[] m_str;
	m_str = nullptr;
	m_len = 0;
}

const char* Napis::getStr() const
{
	return m_str;
}

int Napis::getLen() const
{
	return m_len;
}

void Napis::setStr(const char* str)
{
	delete[] m_str;
	m_len = strlen(str);
	m_str = new char[m_len + 1];
	strcpy(m_str, str);
}

int Napis::toInt() const
{
	int integer = 0;

	for (int i = 0; i < m_len; i++) {
		if (!std::isdigit(m_str[i]))
			throw std::invalid_argument("Error: Napis: Cannot convert from \"Napis\" to \"int\"!");
		integer = 10 * integer + m_str[i] - 48;
	}

	return integer;
}

void Napis::reverse()
{
	if (m_len < 2) return;

	char* reversed = new char[m_len + 1];

	for (int i = 0; i < m_len; i++)
		reversed[i] = m_str[m_len - i - 1];
	reversed[m_len] = '\0';

	setStr(reversed);
	delete[] reversed;
}

Napis Napis::reversed() const
{
	if (m_len < 2) return Napis(m_str);

	char* reversed = new char[m_len + 1];

	for (int i = 0; i < m_len; i++)
		reversed[i] = m_str[m_len - i - 1];
	reversed[m_len] = '\0';

	Napis reversedNapis(reversed);

	delete[] reversed;

	return reversedNapis;
}

void Napis::insert(const Napis& fragment, int startIndex)
{
	if (startIndex > m_len || startIndex < 0)
		throw std::out_of_range("Error: Napis: Index out of range!");

	int d = fragment.m_len;
	if (d == 0) return;

	char* modified = new char[m_len + d + 1];

	int i = 0;
	while (i < startIndex) {
		modified[i] = m_str[i];
		i++;
	}
	while (i < startIndex + d) {
		modified[i] = fragment[i - startIndex];
		i++;
	}
	while (i < m_len + d) {
		modified[i] = m_str[i - d];
		i++;
	}
	modified[i] = '\0';

	setStr(modified);
	delete[] modified;
}

void Napis::insert(const char* fragment, int startIndex)
{
	if (startIndex > m_len || startIndex < 0)
		throw std::out_of_range("Error: Napis: Index out of range!");

	int d = strlen(fragment);
	if (d == 0) return;

	char* modified = new char[m_len + d + 1];

	int i = 0;
	while (i < startIndex) {
		modified[i] = m_str[i];
		i++;
	}
	while (i < startIndex + d) {
		modified[i] = fragment[i - startIndex];
		i++;
	}
	while (i < m_len + d) {
		modified[i] = m_str[i - d];
		i++;
	}
	modified[i] = '\0';

	setStr(modified);
	delete[] modified;
}

void Napis::insert(char chr, int startIndex)
{
	if (startIndex > m_len || startIndex < 0)
		throw std::out_of_range("Error: Napis: Index out of range!");

	if (chr == '\0') return;

	char* modified = new char[m_len + 2];

	int i = 0;
	while (i < startIndex) {
		modified[i] = m_str[i];
		i++;
	}
	modified[i] = chr;
	i++;
	while (i < m_len + 1) {
		modified[i] = m_str[i - 1];
		i++;
	}
	modified[i] = '\0';

	setStr(modified);
	delete[] modified;
}

void Napis::remove(int startIndex)
{
	if (startIndex > m_len || startIndex < 0)
		throw std::out_of_range("Error: Napis: Index out of range!");

	char* modified = new char[startIndex + 1];
	int i = 0;
	while (i < startIndex) {
		modified[i] = m_str[i];
		i++;
	}
	modified[i] = '\0';

	setStr(modified);
	delete[] modified;
}

void Napis::remove(int startIndex, int count)
{
	if (startIndex > m_len || startIndex < 0)
		throw std::out_of_range("Error: Napis: Index out of range!");

	if (count < 0 || startIndex + count > m_len)
		throw std::out_of_range("Error: Napis: Invalid char count to remove!");

	char* modified = new char[m_len - count + 1];

	int i = 0;
	while (i < startIndex) {
		modified[i] = m_str[i];
		i++;
	}
	while (i < m_len - count) {
		modified[i] = m_str[i + count];
		i++;
	}
	modified[i] = '\0';

	setStr(modified);
	delete[] modified;
}

Napis& Napis::operator=(const char* str2)
{
	delete[] m_str;
	m_len = strlen(str2);
	m_str = new char[m_len + 1];
	strcpy(m_str, str2);
	return *this;
}

Napis& Napis::operator=(char chr2)
{
	delete[] m_str;
	m_len = 1;
	m_str = new char[2];
	m_str[0] = chr2;
	m_str[1] = '\0';
	return *this;
}

Napis Napis::operator+(const Napis& napis2) const
{
	if (napis2.m_len == 0) return *this;

	char* modified = new char[m_len + napis2.m_len + 1];

	strcpy(modified, m_str);
	strcat(modified, napis2.m_str);

	Napis nowy(modified);
	delete[] modified;

	return nowy;
}

Napis Napis::operator+(const char* str2) const
{
	if (strlen(str2) == 0) return *this;

	char* modified = new char[m_len + strlen(str2) + 1];

	strcpy(modified, m_str);
	strcat(modified, str2);

	Napis nowy(modified);
	delete[] modified;

	return nowy;
}

Napis Napis::operator+(char chr2) const
{
	if (chr2 == '\0') return *this;

	char* modified = new char[m_len + 2];

	strcpy(modified, m_str);
	modified[m_len] = chr2;
	modified[m_len + 1] = '\0';

	Napis nowy(modified);
	delete[] modified;

	return nowy;
}

Napis operator+(const char* str1, const Napis& napis2)
{
	if (napis2.m_len == 0) return Napis(str1);

	char* modified = new char[strlen(str1) + napis2.m_len + 1];

	strcpy(modified, str1);
	strcat(modified, napis2.m_str);

	Napis nowy(modified);
	delete[] modified;

	return nowy;
}

Napis operator+(char chr1, const Napis& napis2)
{
	if (napis2.m_len == 0) return Napis(chr1);

	char* modified = new char[napis2.m_len + 2];

	modified[0] = chr1;
	modified[1] = '\0';
	strcat(modified, napis2.m_str);

	Napis nowy(modified);
	delete[] modified;

	return nowy;
}

void Napis::operator+=(const Napis& napis2)
{
	if (napis2.m_len == 0) return;

	char* modified = new char[m_len + napis2.m_len + 1];

	strcpy(modified, m_str);
	strcat(modified, napis2.m_str);

	setStr(modified);
	delete[] modified;
}

void Napis::operator+=(const char* str2)
{
	if (strlen(str2) == 0) return;

	char* modified = new char[m_len + strlen(str2) + 1];

	strcpy(modified, m_str);
	strcat(modified, str2);

	setStr(modified);
	delete[] modified;
}

void Napis::operator+=(char chr2)
{
	if (chr2 == '\0') return;

	char* modified = new char[m_len + 2];

	strcpy(modified, m_str);
	modified[m_len] = chr2;
	modified[m_len + 1] = '\0';

	setStr(modified);
	delete[] modified;
}

bool Napis::operator==(const Napis& napis2) const
{
	return strcmp(m_str, napis2.m_str) == 0;
}

bool Napis::operator==(const char* str2) const
{
	return strcmp(m_str, str2) == 0;
}

bool Napis::operator==(char chr2) const
{
	return m_len == 1 && m_str[0] == chr2;
}

char Napis::operator[](int index) const
{
	if (index < 0 || index >= m_len)
		throw std::out_of_range("Error: Napis: Index out of range!");

	return m_str[index];
}

char& Napis::operator[](int index)
{
	if (index < 0 || index >= m_len)
		throw std::out_of_range("Error: Napis: Index out of range!");

	return m_str[index];
}