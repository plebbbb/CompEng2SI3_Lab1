#include "HugeInteger.h"

HugeInteger::HugeInteger(const std::string& val) {
	if (val.length() == 0) throw "Empty input string";
	
	if (val[0] == '-'){
		len_bin--;
		negative = true;
	}
	
	len_bin += val.length();
	unsigned_i = (uint8_t*)calloc(len_bin/2 + 1, 1); 

	int8_t tmp;

	//maximized memory efficiency for non-base conversion case, each base 10 digit gets the bare minimum 4 bits required to store 0-9, with padding space either 0 or 4 bits
	for(int i = 0; i < len_bin; i++){
		tmp = (val[i+negative]-'0');
		if (tmp < 0 || tmp > 9) throw "Invalid character";
		unsigned_i[(i)/2] += tmp << ((i % 2) ? 0 : 4);
	}

}

HugeInteger::HugeInteger(){}

HugeInteger::HugeInteger(int n) {
	if (n <= 0) throw "Out of bounds";
	len_bin = n;
	unsigned_i = (uint8_t*)calloc(len_bin/2 + 1, 1); 
	unsigned_i[0] = (1 + (rand() % 9)) << 4;
	for (int i = 1; i < len_bin-1 ; i++) {
		unsigned_i[i/2] += (rand() % 10) << ((i % 2) ? 0 : 4);
	}
}

bool HugeInteger::carryshiftcheck(const HugeInteger& h){
	int s_val_bin = (len_bin > h.len_bin) ? len_bin : h.len_bin;

	//Case: addition with negative
	if (negative || h.negative) return false; //negatives cannot have an extra digit from carry

	//std::cout << len_bin << " " <<h.len_bin << "\n";
	//Case: positive-positive addition
	int offset = 0;
	while (s_val_bin - offset >= 0){
		int self_v = (offset > len_bin) ? 0 : ((negative ? -1 : 1)*((unsigned_i[offset/2] >> (((offset) % 2) ? 0: 4) & 15)));
		int h_v = (offset > h.len_bin) ? 1 : ((h.negative ? -1 : 1)*((h.unsigned_i[(offset)/2] >> (((offset) % 2) ? 0 : 4) & 15)));
		int result = self_v + h_v;
		
		//std::cout << offset << " " <<  self_v << " " << h_v << "\n";

		if(result >= 10) return true; //indicates a forced carry
		if(result < 9) return false; //if the total is exactly 9, a carry from the next digit down could force carry digit. Thus, we check the next digit down by iterating this loop
		
		offset++;
	}
	return false; //if we somehow made it through the whole integer, then there is no next digit to force carries
}


HugeInteger HugeInteger::add(const HugeInteger& h) {
	HugeInteger new_hi;
	//can't assume len_bin == h.len_bin, as there exists case, 99999, 1, where carry is required.
	bool cscs = carryshiftcheck(h);
	new_hi.len_bin = cscs + ((len_bin > h.len_bin) ? len_bin : h.len_bin);

	new_hi.unsigned_i = (uint8_t*)calloc(new_hi.len_bin/2 + 1, 1);

	int16_t sum_v;
	int8_t carry = 0;
	//std::cout << "LB" << new_hi.len_bin << "  " << h.len_bin  << "  "<<  len_bin<< " \n";

	for(int i = 0; i < new_hi.len_bin-cscs; i++){
		sum_v = 
		  ((i > h.len_bin-1) ? 0 : ((h.negative ? -1 : 1)*((h.unsigned_i[(h.len_bin-1-i)/2] >> (((h.len_bin-1-i) % 2) ? 0: 4)) & 15)))
		+ ((i > len_bin-1) ? 0 : ((negative ? -1 : 1)*((unsigned_i[(len_bin-1-i)/2] >> (((len_bin-1-i) % 2) ? 0: 4)) & 15)))
		+ carry;
		if (sum_v > 9) {
			carry = 1;
			sum_v -= 10;
		} else if (sum_v < 0) {
			carry = -1;
			sum_v += 10;
		} else {
			carry = 0;
		}
		//std::cout << (h.negative ? -1 : 1)*((h.unsigned_i[(h.len_bin-1-i)/2] >> (((h.len_bin-1-i) % 2) ? 0: 4)) & 15) << " + " << (negative ? -1 : 1)*((unsigned_i[(len_bin-1-i)/2] >> (((len_bin-1-i) % 2) ? 0: 4)) & 15)  << " + " << carry << " = " << sum_v;
		new_hi.unsigned_i[(new_hi.len_bin-i-1)/2] += sum_v << (((new_hi.len_bin-i-1) % 2) ? 0 : 4);
		//std::cout << " " << std::bitset<8>(new_hi.unsigned_i[(new_hi.len_bin-i)/2]) << "\n";
	}
	//std::cout << new_hi.toString() << " " << carry <<  "\n";
	if (carry > 0){
		new_hi.unsigned_i[0] += 1 << 4;
	//std::cout << new_hi.toString();

		return new_hi;
	}
	if (carry < 0){
		new_hi.negative = true;
	}
	return new_hi;

}

HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}

int HugeInteger::compareTo(const HugeInteger& h) {
	// TODO
	return 0;
}

std::string HugeInteger::toString() {
	std::string output = negative ? "-" : ""; 
	for(int i = 0; i < len_bin; i++){
		output+=(char)('0' + ((unsigned_i[i/2] >> ((i % 2) ? 0: 4)) & 15));
	}
	return output;
}
