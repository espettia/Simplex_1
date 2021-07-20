#include "matrix_vector.h"

matrix_vector::matrix_vector(std::vector<std::vector<rational>>& m_out, bool r_c_out, size_t pos_out) :
	m(m_out), r_c(r_c_out), pos(pos_out)
{
	if (r_c_out == 0 && pos > m[0].size()) {
		std::cout << "Out of bound";
		exit(1);
	}
	if (r_c_out == 1 && pos > m.size()) {
		std::cout << "Out of bound";
		exit(1);

	}
}

matrix_vector& matrix_vector::operator = (const matrix_vector& mv_out) {
	if (size() != mv_out.size()) {
		std::cout << "Size missmatch";
		exit(1);
	}

	for (int i = 0; i < mv_out.size(); ++i) {
		if (mv_out.r_c == 0) {
			this->operator[](i) = mv_out[i];
		}
	}
	return *this;
}

matrix_vector& matrix_vector::operator = (const std::vector<rational>& v_out) {

	if (this->size() != v_out.size()) {
		std::cout << "Size missmatch";
		exit(1);
	}

	for (size_t i = 0; i < this->size(); ++i)
		this->operator[](i) = v_out[i];

	return *this;
}

rational& matrix_vector::front() {
	return this->operator[](0);
}

rational& matrix_vector::back() {
	return this->operator[](-1);
}

void matrix_vector::print() {
	for (size_t i = 0; i < this->size(); ++i)
		std::cout << this->operator[](i).str(0x01) << " ";
	std::cout << std::endl;
}

std::vector<rational> matrix_vector::vector() const {
	std::vector<rational> ans;
	for (int i = 0; i < this->size(); ++i) ans.push_back(this->operator[](i));
	return ans;
}

rational& matrix_vector::operator [](int i) {

	if (i > this->size()) {
		std::cout << "Out of bound";
		exit(1);
	}
	if (r_c == 0)
		return i >= 0 ? m[pos][i] : m[pos][m[0].size() + i];
	else
		return i >= 0 ? m[i][pos] : m[m.size() + i][pos];
}

size_t matrix_vector::size() const {
	if (r_c == 0)
		return m[0].size();
	else
		return m.size();
}
