#pragma once

class Napis
{
	char* m_str;
	int m_len;
public:
	Napis(const char* str = "");
	Napis(char chr);
	
	Napis(const Napis& original);
	Napis& operator=(const Napis& napis2);

	Napis(Napis&& original) noexcept;
	Napis& operator=(Napis&& napis2) noexcept;
	
	Napis(int num);
	
	~Napis();

	const char* getStr() const;
	int getLen() const;

	void setStr(const char* str);

	int toInt() const;
	
	void reverse();
	Napis reversed() const;

	void insert(const Napis& fragment, int startIndex);
	void insert(const char* fragment, int startIndex);
	void insert(char chr, int startIndex);

	void remove(int startIndex);
	void remove(int startIndex, int count);

	Napis& operator=(const char* str2);
	Napis& operator=(char chr2);

	Napis operator+(const Napis& napis2) const;
	Napis operator+(const char* str2) const;
	Napis operator+(char chr2) const;

	friend Napis operator+(const char* str1, const Napis& napis2);
	friend Napis operator+(char chr1, const Napis& napis2);
	
	void operator+=(const Napis& napis2);
	void operator+=(const char* str2);
	void operator+=(char chr2);

	bool operator==(const Napis& napis2) const;
	bool operator==(const char* str2) const;
	bool operator==(char chr2) const;
	
	char operator[](int index) const;
	char& operator[](int index);
};

