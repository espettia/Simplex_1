#include <iostream>
#include <string>
#include "rational.h"

rational::rational(int a, int b) 
:numerator(abs(a)* sgn(a* b)), denominator(denominator = abs(b))
{
	if (denominator == 0) {
		std::cout << "Divison by zero";
		exit(4);
	}
	simplify();
}

void rational::simplify() {
	int div = gcd(numerator, denominator);
	numerator = numerator / div;
	denominator = denominator / div;
}

//Basic Arithmetic

rational rational::operator +(const rational& q2) const {
	int den = denominator * q2.denominator;
	int num = (numerator * q2.denominator + denominator * q2.numerator);
	rational ans(num, den);
	return ans;
}

std::vector<rational> rational::operator +(const std::vector<rational>& v) const {
	std::vector<rational> ans = v;
	for (auto& i : ans)
		i = *this + i;
	return ans;
}

rational rational::operator -(const rational& q2) const{
	return *this + q2 * -1;
}

std::vector<rational> rational::operator -(const std::vector<rational>& v) const {
	std::vector<rational> ans = v;
	for (auto& i : ans)
		i = *this - i;
	return ans;
}

rational rational::operator *(const rational& q2) const{
	int den = denominator * q2.denominator;
	int num = numerator * q2.numerator;
	rational ans(num, den);
	return ans;
}

std::vector<rational> rational::operator *(const std::vector<rational>& v) const {
	std::vector<rational> ans = v;
	for (auto& i : ans)
		i = *this * i;
	return ans;
}

rational rational::operator /(const rational& q2) const{
	if (q2.num() == 0) {
		std::cout << "Divison by zero";
		exit(4);
	}
	rational q2_inv(q2.denominator, q2.numerator);
	return *this * q2_inv;
}

std::vector<rational> rational::operator /(const std::vector<rational>& v) const {
	std::vector<rational> ans = v;
	for (auto& i : ans)
		i = *this / i;
	return ans;
}

int rational::sign() const {
	return sgn(numerator);
}

bool rational::operator ==(const rational& q2) const {
	return *this >= q2 && *this <= q2;
}

bool rational::operator !=(const rational& q2) const {
	return !(*this == q2);
}

bool rational::operator >(const rational& q2) const {
	return !(*this <= q2);
}

bool rational::operator <(const rational& q2)const {
	return !(*this >= q2);
}

bool rational::operator >=(const rational& q2) const {
	return (*this - q2).sign() >= 0 ? true : false;
}

bool rational::operator <=(const rational& q2) const {
	return (*this - q2).sign() <= 0 ? true : false;
}


double rational::dec() const {
	return 1.0 * numerator / denominator;
}

int rational::floor() const{

	return  (numerator > 0) ? (numerator / denominator) : (numerator - 1) / denominator - 1;
}

rational rational::frac() const{
	return *this - this->floor();
}

int rational::ceil() const{
	return  (numerator > 0) ? 1 + (numerator - 1) / denominator : (numerator / denominator);
}

int& rational::num() {
	return numerator;
}

const int& rational::num() const {
	return numerator;
}

int& rational::den() {
	return denominator;
}

const int& rational::den() const{
	return denominator;
}

std::string rational::str(int option) {
	//options:
	//0 -> no positive sign, empty space when zero
	//1 -> zero
	//2 -> postiive sign
	//3 -> zero and positive sign
	std::string ans;
	simplify();
	if (*this == 0) {
		if (option & 0x01)
			ans.append("0");
	}
	else {
		if (option & 0x10) {
			if (*this > 0) {
				ans.append("+");
			}
		}
		ans.append(std::to_string(numerator));
		if (denominator != 1)
			ans.append("/" + std::to_string(denominator));
	}
	return ans;
}

void rational::print(int z){
	
	this->simplify();
	if (this->num() == 0) {
		if (z) std::cout << "0";
		else
			std::cout << " ";
	}
	else if (this->den() == 1) {
		std::cout << this->num();
	}
	else
		std::cout << this->num() << "/" << this->den();
}

std::ostream& operator<<(std::ostream& os, rational q1) {
	q1.simplify();
	os << q1.num();
	if (q1.num() != 0)
		os <<"/" << q1.den();
	return os;
}
