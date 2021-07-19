#include "matrix.h"

bool matrix::empty() {
	return m.empty();
}

rational& matrix::at(size_t r, size_t c) {
	if (r >= this->rows() || c >= this->columns()) {
		std::cout << "Out of bound" << std::endl;
		exit(1);
	}
	return m[r][c];
}

size_t matrix::rows() {
	return rows_in;
}

size_t matrix::columns() {
	return cols_in;
}

void matrix::print() {
	for (size_t r_i = 0; r_i < this->rows(); ++r_i) {
		for (auto& c : m[r_i]) {
			std::cout << c.num() << "/" << c.den() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool is_canonical(std::vector<rational> v) {
	int flag = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != 0) {
			if (v[i] == 1 && flag == 0) flag = 1;
			else return 0;
		}
	}
	return 1;
}

//----------------------------------------


matrix::matrix_vector::matrix_vector(matrix& m_out, bool r_c_out, size_t pos_out) :
	m(m_out.m), r_c(r_c_out), pos(pos_out)
{
	if (r_c_out == 0 && pos > m_out.rows_in) {
		std::cout << "Out of bound";
		exit(1);
	}
	if (r_c_out == 1 && pos > m_out.cols_in) {
		std::cout << "Out of bound";
		exit(1);

	}
}

matrix::matrix_vector& matrix::matrix_vector::operator = (const matrix_vector& mv_out) {
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

matrix::matrix_vector& matrix::matrix_vector::operator = (const std::vector<rational>& v_out) {

	if (this->size() != v_out.size()) {
		std::cout << "Size missmatch";
		exit(1);
	}

	for (size_t i = 0; i < this->size(); ++i)
		this->operator[](i) = v_out[i];

	return *this;
}

rational& matrix::matrix_vector::front() {
	return this->operator[](0);
}

rational& matrix::matrix_vector::back() {
	return this->operator[](-1);
}

void matrix::matrix_vector::print() {
	for (size_t i = 0; i < this->size(); ++i)
		std::cout << this->operator[](i).str(0x01) << " ";
	std::cout << std::endl;
}

std::vector<rational> matrix::matrix_vector::vector() const {
	std::vector<rational> ans;
	for (int i = 0; i < this->size(); ++i) 
		ans.push_back(this->operator[](i));
	return ans;
}

rational& matrix::matrix_vector::operator [](int i) {

	if (i > this->size()) {
		std::cout << "Out of bound";
		exit(1);
	}
	if (r_c == 0)
		return i >= 0 ? m[pos][i] : m[pos][m[0].size() + i];
	else
		return i >= 0 ? m[i][pos] : m[m.size() + i][pos];
}

const rational& matrix::matrix_vector::operator [](int i) const {

	if (i > this->size()) {
		std::cout << "Out of bound";
		exit(1);
	}
	if (r_c == 0)
		return i >= 0 ? m[pos][i] : m[pos][m[0].size() + i];
	else
		return i >= 0 ? m[i][pos] : m[m.size() + i][pos];
}

size_t matrix::matrix_vector::size() const {
	if (r_c == 0)
		return m[0].size();
	else
		return m.size();
}


//----------------------------------------

matrix::matrix(void) { ; }
matrix::matrix(std::vector<std::vector<rational>> v) {
	//If empty, matrix = { {0} }
	//if { {} }, matrix = {{0}}
	if (!v.empty()) {
		m = v;
		rows_in = v.size();
		cols_in = v[0].size() > 1 ? v[0].size() : 1;
		for (int i = 1; i < v.size(); ++i) {
			matrix_vector_rows.push_back(matrix_vector(*this, 0, i));
			for (int j = 1; j < v[i].size(); ++j) {
				if (j == matrix_vector_columns.size()) {
					matrix_vector_columns.push_back(matrix_vector(*this, 1, j));
				}
				if (j == cols_in) {
					++cols_in;
				}
			}
		}

		for (int i = 0; i < v.size(); ++i) {
			for (int j = v[i].size(); j < cols_in; ++j) {
				m[i].push_back(0);
			}
		}
	}
}

matrix matrix::operator =(matrix m_out) {
	m = m_out.m;
	rows_in = m_out.rows_in;
	cols_in = m_out.cols_in;
	matrix_vector_rows = m_out.matrix_vector_rows;
	matrix_vector_columns = m_out.matrix_vector_columns;
	return *this;
}

matrix matrix::operator =(std::vector<std::vector<rational>> v) {
	if (!v.empty()) {
		m = v;
		int max_columns = v[0].size() > 1 ? v[0].size() : 1;
		for (int i = 1; i < v.size(); ++i) {
			matrix_vector_rows.push_back(matrix_vector(*this, 0, i));
			for (int j = 1; j < v[i].size(); ++j) {
				if (j == max_columns) {
					matrix_vector_columns.push_back(matrix_vector(*this, 1, j));
					++max_columns;
				}
			}
		}

		for (int i = 0; i < v.size(); ++i) {
			for (int j = v.size(); j < max_columns; ++j) {
				m[i].push_back(0);
			}
		}
	}
	return *this;
}
//-------------------------------------------

matrix::matrix_vector matrix::row(int i)
{
	return matrix_vector_rows.at(i);
}
matrix::matrix_vector matrix::col(int j) {
	return matrix_vector_columns.at(j);
}

matrix::matrix_vector matrix::last_r() {
	return matrix_vector_rows.back();
}
matrix::matrix_vector matrix::last_c() {
	return matrix_vector_columns.front();
}


matrix matrix::mult_r(size_t r, rational v) {

	//Check if index is valid
	if (r >= this->rows())
		exit(3);

	for (size_t i = 0; i < this->columns(); ++i)
	{
		m[r][i] = v * m[r][i];
	}

	return *this;
}

matrix matrix::mult_c(size_t c, rational v) {

	//Check if index is valid
	if (c >= columns())
		exit(3);

	//Multiply c elelemnt rational of every row with ratinoal v
	for (size_t i = 0; i < rows(); ++i) {
		m[i][c] = m[i][c] * v;
	}

	return *this;
}

matrix matrix::sum_r(size_t r, std::vector<rational> v) {

	//Check if index is valid
	if (r >= this->rows())
		exit(3);

	//Check if vector size is compatible
	if (v.size() != this->columns())
		exit(-1);

	for (size_t i = 0; i < this->columns(); ++i)
	{
		m[r][i] = v[i] + m[r][i];
	}
	return *this;
}

matrix matrix::sum_c(size_t c, std::vector<rational> v) {

	//Check if index is valid
	if (c >= this->columns())
		exit(3);

	//Check if vector size is compatible
	if (v.size() != this->rows())
		exit(-1);

	for (size_t i = 0; i < this->rows(); ++i)
	{
		m[i][c] = v[i] + m[i][c];
	}
	return *this;
}

matrix matrix::piv(size_t r, size_t c) {
	if (r >= rows() || c >= columns())
		exit(3);
#ifndef DEBUG
	if (m[r][c] == 0)
		std::cout << r << " " << c << std::endl;
#endif // !DEBUG

	mult_r(r, rational(1) / m[r][c]);
	for (size_t r_i = 0; r_i < rows(); ++r_i) {
		if (r_i != r) {
			sum_r(r_i, (m[r_i][c] * -1) * m[r]);
		}
	}
	return *this;
}



matrix matrix::push_r(std::vector<rational> v) {
	if (v.size() != this->columns()) {
		exit(2);
	}

	m.push_back(v);
	++rows_in;
	matrix_vector_rows.push_back(matrix_vector(*this, 0, rows_in - 1));
	
	return *this;
}

matrix matrix::push_c(std::vector<rational> v) {
	if (v.size() != rows_in) {
		exit(2);
	}

	for (size_t r_i = 0; r_i < rows_in; ++r_i) {
		m[r_i].push_back(v[r_i]);
	}
	++cols_in;
	matrix_vector_columns.push_back(matrix_vector(*this, 1, cols_in - 1));
	return *this;
}

matrix matrix::pop_r() {
	if (!m.empty()) {
		m.pop_back();
		matrix_vector_rows.pop_back();
		--rows_in;
	}
		
	return *this;
}

matrix matrix::pop_c() {
	if (!m.empty()) {
		for (size_t r_i = 0; r_i < this->rows(); ++r_i) {
			size_t last_c = this->columns() - 1;
			m[r_i].pop_back();
		}
		matrix_vector_columns.pop_back();
		--cols_in;
	}
	return *this;
}
//
//matrix matrix::gauss() { ; }
//
matrix matrix::transpose() {

	std::vector<std::vector<rational>> m1;
	for (size_t j = 0; j < this->columns(); ++j) {
		std::vector<rational> tmp;
		for (size_t i = 0; i < this->rows(); ++i)
			tmp.push_back(m[i][j]);
		m1.push_back(tmp);
	}
	
	while(rows() != matrix_vector_rows.size()){
		if (rows() > matrix_vector_rows.size()) matrix_vector_rows.push_back(matrix_vector(*this, 0, matrix_vector_rows.size()));
		else if (rows() < matrix_vector_rows.size()) matrix_vector_rows.pop_back();
	}

	while (columns() != matrix_vector_columns.size()) {
		if (columns() > matrix_vector_columns.size()) matrix_vector_columns.push_back(matrix_vector(*this, 0, matrix_vector_columns.size()));
		else if (columns() < matrix_vector_columns.size()) matrix_vector_columns.pop_back();
	}

	return m1;

}

