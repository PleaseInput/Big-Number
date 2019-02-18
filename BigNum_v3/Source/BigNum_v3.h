#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#define BASE 10
typedef unsigned int uint;
using namespace std;

class BigNum {
	private:
		bool is_neg;
		int len;
		string val;
	public:

		// ----- begin constructor -----
		BigNum();
		BigNum(const char *value);
		// ----- end constructor -----

		// ----- begin ">>, <<" -----
		friend istream& operator>>(istream &is, BigNum &bn);
		friend ostream& operator<<(ostream &os, const BigNum &bn);
		// ------ end ">>, <<" -----

		// ----- begin "set_val" -----
		void set_val(string value);
		// ----- end "set_val" -----

		// ----- begin "=, +=" -----
		BigNum& operator=(const BigNum &bn);
		friend BigNum& operator+=(BigNum &bn_1, const BigNum &bn_2);
		friend BigNum operator+(const BigNum &bn_1, const BigNum &bn_2);
		// ----- end "=, +=" -----

		// ----- begin "==, >, <, >=, <=" -----
		friend bool operator==(const BigNum &bn_1, const BigNum &bn_2);
		friend bool operator>(const BigNum &bn_1, const BigNum &bn_2);
		friend bool operator<(const BigNum &bn_1, const BigNum &bn_2);
		friend bool operator>=(const BigNum &bn_1, const BigNum &bn_2);
		friend bool operator<=(const BigNum &bn_1, const BigNum &bn_2);
		// ----- end "==, >, <, >=, <=" -----

		// ----- begin "negate, era_zero" -----
		BigNum negate();

		// erase zero. e.g. "008"=>"8". rev:if it need to be reversed.
		BigNum era_zero(bool rev);
		// ----- end "negate, era_zero" -----
};