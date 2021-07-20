#pragma once
#include "useful.h"
#include <iostream>
#include <vector>

class rational {

private:

	int numerator = 1;
	int denominator = 1;

public:

	//Constructor
	rational(int a = 0, int b = 1);

	void simplify();

	//Basic Arithmetic

	rational operator +(const rational& q2) const;
	rational operator -(const rational& q2) const;
	rational operator *(const rational& q2) const;
	rational operator /(const rational& q2) const;

	friend rational operator +(const int n, const rational& q) { return q + n; }
	friend rational operator -(const int n, const rational& q) { return rational(n) - q; }
	friend rational operator *(const int n, const rational& q) { return q * n; }
	friend rational operator /(const int n, const rational& q) { return rational(n) / q; }

	std::vector<rational> operator +(const std::vector<rational>& v) const;
	std::vector<rational> operator *(const std::vector<rational>&) const;
	std::vector<rational> operator -(const std::vector<rational>& v) const;
	std::vector<rational> operator /(const std::vector<rational>& v) const;

	friend std::vector<rational> operator +(const std::vector<rational>& v, const rational& q) { return q + v; }
	friend std::vector<rational> operator -(const std::vector<rational>& v, const rational& q) { return v + (-1 * q); }
	friend std::vector<rational> operator *(const std::vector<rational>& v, const rational& q) { return q * v; }
	friend std::vector<rational> operator /(const std::vector<rational>& v, const rational& q) { return v * (1 / q); }

	template <typename T>
	T& operator +=(const T& x) { return *this = *this + x; }

	template <typename T>
	T& operator -=(const T& x) { return *this = *this - x; }

	template <typename T>
	T& operator /=(const T& x) { return *this = *this / x; }

	template <typename T>
	T& operator *=(const T& x) { return *this = *this * x; }

	bool operator ==(const rational& q2) const;
	bool operator !=(const rational& q2) const;
	bool operator >(const rational& q2) const;
	bool operator <(const rational& q2) const;
	bool operator >=(const rational& q2) const;
	bool operator <= (const rational& q2) const;

	int sign() const;

	//Useful functions

	double dec() const;

	int floor() const;

	rational frac() const;

	int ceil() const;

	int& num();

	const int& num() const;

	int& den();

	const int& den() const;

	std::string str(int option = 0);

	void print(int z = 0);

	friend std::ostream& operator<<(std::ostream& os, rational q1);

};
