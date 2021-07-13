#include <iostream>
#include "useful.h"
#include "rational.h"

int main() {

	std::cout << "hello" << std::endl;
	const char s[] = "7/3";
	rational q(7, -3);
	rational q2(2, 7);
	rational q3;
	q3 = q + q2;
	std::cout << q3[0] << " " << q3[1] << std::endl;
	q3 = q*q2;
	std::cout << q3[0] << " " << q3[1] << std::endl;
	q3 = q/q2;
	std::cout << q3[0] << " " << q3[1] << std::endl;
	q3 = q - q2;
	std::cout << q3[0] << " " << q3[1] << std::endl;
	q3 = q3 * -1;
	std::cout << q3[0] << " " << q3[1] << std::endl;
	std::cout << (q3 <= q - q2) << std::endl;

	std::cout << (q3 < q - q2) << std::endl;
	std::cout << (q3 > q - q2) << std::endl;
	std::cout << (q3 >= q - q2) << std::endl;

	return 0;

}