#ifndef HUGEINTEGER_H_
#define HUGEINTEGER_H_
#include <iostream>
#include <vector>

class HugeInteger
{
private:
	std::vector<short> u_digits;
	bool negative = false;
	unsigned int b10_mag = 0;
	void prune_L_zeros();
	HugeInteger add_same_sign(const HugeInteger & h);
	HugeInteger add_dif_sign(const HugeInteger & h);
	HugeInteger multiply_d10(unsigned short h, int digit_shift, bool neg);
	int compareToABS(const HugeInteger& h);
public:
	// Required methods
	HugeInteger();
	HugeInteger(const HugeInteger & h);
	HugeInteger(const std::string& val);
	HugeInteger(int n);
	HugeInteger add(const HugeInteger& h);
	HugeInteger subtract(const HugeInteger& h);
	HugeInteger multiply(const HugeInteger& h);
	int compareTo(const HugeInteger& h);
	std::string toString();
};

#endif /* HUGEINTEGER_H_ */
