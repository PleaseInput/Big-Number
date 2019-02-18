/*
(1)
In BigNum class, the autor uses "len" and "val". However,
in parameter and function, uses "lenth" and "value"
which mean "length" and "value" respectively.

(2)
tmp_something is a temporary variable of something.
e.g. tmp_int, tmp_char and so on.

(3)
about the format of "*" and "&"
e.g. "int*" and "int *num"

(4)
-2's len is 1, only including the part of number.

(5)
bn : big number
bn_lf : bn_left(larger number)
bn_rt : bn_right(smaller number)
e.g. bn_lf + bn_rt. put the larger number at bn_lf.

(6)
len = tmp_str.length();
len's type is int, 2^32, which only use 2^31 because it is big number's length, don't need < 0.
tmp_str.length()'s type is size_t. 2^64.
expect a big number is up to 2^1024 = 2^(2^10), and the max number which the class can represent is 10^(2^31).
2^(2^10) < 10^(2^31).it's enough. changing size_t to int doesn't matter.

(7)
ans.val isn't reversed. don't want to use "ans.val.insert(ans.val.end(), to_string(sum - BASE));" in each iteration.
*/
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

		static const BigNum zero, one, two;

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

		// ----- begin "=, +=, +, -=, -, *=" -----
		BigNum& operator=(const BigNum &bn);
		friend BigNum& operator+=(BigNum &bn_1, const BigNum &bn_2);
		friend BigNum operator+(const BigNum &bn_1, const BigNum &bn_2);
		friend BigNum& operator-=(BigNum &bn_1, const BigNum &bn_2);
		friend BigNum operator-(const BigNum &bn_1, const BigNum &bn_2);
		friend BigNum& operator*=(BigNum &bn_1, const BigNum &bn_2);
		// ----- end "=, +=, +, -=, -, *=" -----

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