#include "BigNum_v3.h"

// ----- begin constructor -----
BigNum::BigNum()
{
	// default number = 0
	is_neg = false;
	len = 1;
	val = '0';
}

BigNum::BigNum(const char *value)
{
	string tmp_str(value);
	this->set_val(tmp_str);
}
// ----- end constructor -----

// ----- begin ">>, <<" -----
istream& operator>>(istream &is, BigNum &bn)
{
	string tmp_str;
	is >> tmp_str;

	bn.set_val(tmp_str);

	return is;
}

ostream& operator<<(ostream &os, const BigNum &bn)
{
	if (bn.is_neg == true)
		os << "-";

	// other printing method
	/*
	string tmp_str = bn.val;
	reverse(tmp_str.begin(), tmp_str.end());
	os << tmp_str;
	*/
	for (int i = bn.len - 1; i >= 0; i--)
		os << bn.val[i];
	return os;
}
// ------ end ">>, <<" -----

// ----- begin "set_val" -----
void BigNum::set_val(string value)
{
	// TODO : if user inputs "abc" or "-1 2"
	if (value[0] == '-')
	{
		is_neg = true;
		value.erase(0, 1);
	}
	else
	{
		is_neg = false;
	}
	len = (int)value.length();
	reverse(value.begin(), value.end());
	val = value;

	// if input "-0", "-0"=>"0"
	if (len == 1 && val[0] == '0')
		is_neg = false;

	this->era_zero(false);
}
// ----- end "set_val" -----

// ----- begin "=, +=" -----
BigNum& BigNum::operator=(const BigNum &bn)
{
	// TODO : if (*this == bn)
	is_neg = bn.is_neg;
	len = bn.len;
	val = bn.val;
	return *this;
}

BigNum& operator+=(BigNum &bn_1, const BigNum &bn_2)
{
	bool tmp_bool = (bn_1 > bn_2);
	BigNum bn_lf = tmp_bool ? bn_1 : bn_2;
	BigNum bn_rt = tmp_bool ? bn_2 : bn_1;

	/*
	if (bn_lf.is_neg || bn_rt.is_neg)
	{
		// bn_lf:+	bn_rt:-. e.g. (+a) + (-b) = (+a) - (+b)
		if (!bn_lf.is_neg && bn_rt.is_neg)
		{
			bn_rt.negate();	// (-b)=>-(+b)
			return (bn_lf - bn_rt).era_zero(false);
		}
		// bn_lf:-	bn_rt:-
		if (bn_lf.is_neg && bn_rt.is_neg)
		{
			bn_lf.negate();
			bn_rt.negate();
			return (bn_lf + bn_rt).negate();
		}
		// due to bn_lf > bn_rt, "bn_lf:- bn_rt:+" cann't happen.
	}
	*/

	// ----- begin bn_lf:+	bn_rt:+ -----
	// ans.val isn't reversed. don't want to use "ans.val.insert(ans.val.end(), to_string(sum - BASE));" in each iteration.
	BigNum ans("");

	int carry = 0, sum = 0;
	int diff = bn_lf.len - bn_rt.len;
	// e.g. real val : 246 + 13 => 246 + 013 
	for (int i = 0; i < diff; i++)
	{
		bn_rt.val.insert(bn_rt.val.end(), '0');
	}

	for (int i = 0; i < bn_lf.len; i++)
	{
		sum = (bn_lf.val[i] - '0') + (bn_rt.val[i] - '0') + carry;
		if (sum >= BASE)
		{
			carry = 1;
			ans.val.insert(0, to_string(sum - BASE));
		}
		else
		{
			carry = 0;
			ans.val.insert(0, to_string(sum));
		}
	}
	// e.g. 9 + 9 = '1'8
	if (carry)
		ans.val.insert(0, to_string(carry));
	reverse(ans.val.begin(), ans.val.end());
	ans.len = (int)ans.val.length();
	// ----- end bn_lf:+ bn_rt:+ -----

	bn_1 = ans;
	return bn_1;
}


// ----- end "=, +=" -----

// ----- begin "==, >, <, >=, <=" -----
bool operator==(const BigNum &bn_1, const BigNum &bn_2)
{
	if (bn_1.is_neg != bn_2.is_neg)	return false;
	if (bn_1.len != bn_2.len)	return false;
	if (bn_1.val.compare(bn_2.val) != 0)	return false;
	return true;
}

bool operator>(const BigNum &bn_1, const BigNum &bn_2)
{
	if (bn_1 == bn_2)	return false;

	// bn_1:+	bn_2:-
	if (!bn_1.is_neg && bn_2.is_neg) return true;
	// bn_1:-	bn_2:+
	if (bn_1.is_neg && !bn_2.is_neg) return false;

	// bn_1:+	bn_2:+
	if (!bn_1.is_neg && !bn_2.is_neg)
	{
		// e.g. bn_1 = 135	bn_2 = 24
		if (bn_1.len > bn_2.len)	return true;
		// e.g. bn_1 = 13	bn_2 = 246
		if (bn_1.len < bn_2.len)	return false;

		// bn_1.len == bn_2.len. e.g. bn_1.val = 35	bn_2.val = 24
		string tmp_str1 = bn_1.val;
		string tmp_str2 = bn_2.val;
		reverse(tmp_str1.begin(), tmp_str1.end());
		reverse(tmp_str2.begin(), tmp_str2.end());
		// e.g. tmp_str1 = 53	tmp_str2 = 42
		if (tmp_str1.compare(tmp_str2) > 0)	return true;

		// tmp_str1.compare(tmp_str2) < 0. e.g. 42 < 53
		return false;
	}

	// bn_1:-	bn_2:-
	if (bn_1.is_neg && bn_2.is_neg)
	{
		// e.g. bn_1 = -135	bn_2 = -24
		if (bn_1.len > bn_2.len)	return false;
		// e.g. bn_1 = -13	bn_2 = -246
		if (bn_1.len < bn_2.len)	return true;

		// bn_1.len == bn_2.len. e.g. bn_1.val = 35	bn_2.val = 24
		string tmp_str1 = bn_1.val;
		string tmp_str2 = bn_2.val;
		reverse(tmp_str1.begin(), tmp_str1.end());
		reverse(tmp_str2.begin(), tmp_str2.end());
		// e.g. tmp_str1 = 53	tmp_str2 = 42
		if (tmp_str1.compare(tmp_str2) > 0)	return false;

		// tmp_str1.compare(tmp_str2) < 0. e.g. -42 > -53
		return true;
	}
}

bool operator<(const BigNum &bn_1, const BigNum &bn_2)
{
	if (bn_1 == bn_2) return false;
	if (bn_1 > bn_2) return false;
	return true;
}

bool operator>=(const BigNum &bn_1, const BigNum &bn_2)
{
	if (bn_1 > bn_2 || bn_1 == bn_2)
		return true;
	else
		return false;
}

bool operator<=(const BigNum &bn_1, const BigNum &bn_2)
{
	if (bn_1 < bn_2 || bn_1 == bn_2)
		return true;
	else
		return false;
}
// ----- end "==, >, <, >=, <=" -----

// ----- begin "negate, era_zero" -----
BigNum BigNum::negate()
{
	is_neg = !is_neg;
	return *this;
}

// all numbers are regarded as reversed numbers. e.g. real value "53". reversed "35"
BigNum BigNum::era_zero(bool rev = false)
{
	if (rev)
		reverse(val.begin(), val.end());

	// i > 0, not i >= 0. because "000"=>"" is wrong, "000"=>"0" is right.
	for (int i = len - 1; i > 0; i--)
	{
		if (val[i] == '0')
			val.erase(i);
		else
			break;
	}

	//if (rev)
	//	reverse(val.begin(), val.end());
	len = (int)val.length();

	return *this;
}
// ----- end "negate, era_zero" -----