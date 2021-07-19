#include "useful.h"
#include <iostream>
#include <vector>

#ifndef RATIONAL
#define RATIONAL

class rational {

private:
	int q[2] = { 1,1 };

public:

	//Constructor
	rational(int a = 0, int b = 1);

	void simplify();

	//Access elements

	int& operator [](int i);

	//Assignment

	rational operator =(rational q2);

	rational operator =(int q2);

	rational operator =(const char* s);

	//Basic Arithmetic

	rational operator +(rational q2);

	rational operator +(int n);

	std::vector<rational> operator +(std::vector<rational>);

	rational operator -(rational q2);

	rational operator -(int n);

	rational operator *(rational q2);

	rational operator *(int n);

	std::vector<rational> operator *(std::vector<rational>);

	rational operator /(rational q2);

	rational operator /(int n);

	template <typename T>
	rational operator +=(T x) {
		return *this = *this + x;
	}

	template <typename T>
	rational operator -=(T x) {
		return *this = *this - x;
	}

	template <typename T>
	rational operator /=(T x) {
		return *this = *this / x;
	}

	template <typename T>
	rational operator *=(T x) {
		return *this * x;
	}

	bool operator ==(rational q2);

	bool operator !=(rational q2);

	bool operator >(rational q2);

	bool operator <(rational q2);

	bool operator >=(rational q2);

	bool operator <= (rational q2);

	int sign();

	//Useful functions

	double dec();

	int floor();

	rational frac();

	int ceil();

	int& num();

	int& den();

	std::string str(int option = 0);

	void print(int z = 0);

};

#endif