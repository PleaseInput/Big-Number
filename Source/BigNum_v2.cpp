#include "BigNum.h"

BigNum BigNum::zero("0");
BigNum BigNum::one("1");
BigNum BigNum::two("2");

// TODO:if input "008"

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
BigNum BigNum::set_val(string value)
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

	return *this;
}
// ----- end "set_val" -----

// ----- begin "=, +, -, *" -----
BigNum& BigNum::operator=(const BigNum &bn)
{
	is_neg = bn.is_neg;
	len = bn.len;
	val = bn.val;
	return *this;
}

BigNum operator+(BigNum &bn_1, BigNum &bn_2)
{
	bool tmp_bool = (bn_1 > bn_2);
	BigNum bn_lf = tmp_bool ? bn_1 : bn_2;
	BigNum bn_rt = tmp_bool ? bn_2 : bn_1;

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
	return ans;
}

BigNum operator-(BigNum &bn_1, BigNum &bn_2)
{
	// if bn_1 == bn_2, return 0 and the default number is 0.
	if (bn_1 == bn_2)
	{
		BigNum ans;
		return ans;
	}
	
	bool tmp_bool = (bn_1 > bn_2);
	BigNum bn_lf = tmp_bool ? bn_1 : bn_2;
	BigNum bn_rt = tmp_bool ? bn_2 : bn_1;
	BigNum ans("");

	// ----- begin bn_1:+/bn_2:-   bn_1:-/bn_2:+ -----
	if (bn_lf.is_neg ^ bn_rt.is_neg)
	{
		bn_rt.negate();
		ans = bn_lf + bn_rt;
		if (!tmp_bool)
			ans.negate();
		return ans;
	}
	// ----- end bn_1:+/bn_2:-   bn_1:-/bn_2:+ -----

	// ----- begin bn_1:+/bn_2:+    bn_1:-/bn_2:- -----
	int borrow = 0, sum = 0;
	int diff = bn_lf.len - bn_rt.len;
	// e.g. real val : 246 - 13 => 246 - 013 
	for (int i = 0; i < diff; i++)
	{
		bn_rt.val.insert(bn_rt.val.end(), '0');
	}

	for (int i = 0; i < bn_lf.len; i++)
	{
		// x[i] - y[i]
		int x = bn_lf.val[i] - '0' - borrow;
		int y = bn_rt.val[i] - '0';
		
		if (x >= y)
		{
			sum = x - y;
			borrow = 0;
		}
		else
		{
			sum = x + BASE - y;
			borrow = 1;
		}
		ans.val.insert(0, to_string(sum));
	}
	ans.len = (int)ans.val.length();
	ans.era_zero(true);
	
	if (!tmp_bool)
		ans.negate();
	// ----- end bn_1:+/bn_2:+    bn_1:-/bn_2:- -----

	return ans;
}

BigNum operator*(BigNum &bn_1, BigNum &bn_2)
{
	BigNum ans("");

	if (bn_1 == BigNum::zero || bn_2 == BigNum::zero)
		return BigNum::zero;

	/*
		99...9 * 99...9 = (10^i - 1) * (10^k - 1) = 10^(i+k) - 10^i - 10^k + 1
		<  i  >  <  k  >
		ans.len = bn_1.len + bn_2.len. enough.
					i			k
	*/
	for (int i = 0; i < bn_1.len + bn_2.len; i++)
	{
		ans.val.insert(ans.val.begin(), '0');
	}
	
	/*
		  bn_1
		* bn_2
	*/
	int carry = 0;
	int sum = 0;
	for (int i = 0; i < bn_2.len; i++)
	{
		for (int j = 0; j < bn_1.len; j++)
		{
			sum = (bn_2.val[i] - '0') * (bn_1.val[j] - '0') + carry + (ans.val[i + j] - '0');
			carry = sum / BASE;
			sum = sum % BASE;
			ans.val[i + j] = sum + '0';
		}
		if (carry)
			ans.val[i + bn_1.len] += carry;
		carry = 0;
	}
	ans.len = (int)ans.val.length();
	ans.era_zero(false);

	if (bn_1.is_neg ^ bn_2.is_neg)
		ans.is_neg = true;

	return ans;
}
// ----- end "=, +, -, *" -----

// ----- begin "/, %, sf_lf, sf_rt, mul_int" -----
/*
	+p/+k : p = q * k + r.
	-p/+k : (-p) =  (-q) * k + (-r) 
	+p/-k : p =  (-q) * (-k) + r
	-p/-k : (-p) =  q * (-k) + (-r)
	http://highscope.ch.ntu.edu.tw/wordpress/?p=12324
*/
BigNum operator/(BigNum &bn_1, BigNum &bn_2)
{
	BigNum ans;
	
	// ----- begin bn_2 == 0 -----
	if (bn_2 == BigNum::zero)
	{
		ans.is_neg = false;
		ans.len = -1;
		ans.val = "NAN";
		cout << "erroe s.e./0\n";
		
		return ans;
	}
	// ----- end bn_2 == 0 -----
	
	/*
		step 1 : compute +p/+k
		step 2 : consider the sign of p and k
	*/
	// ----- begin +p/+k -----
	BigNum bn_lf = bn_1;
	BigNum bn_rt = bn_2;
	bn_lf.is_neg = false;
	bn_rt.is_neg = false;

	if (bn_lf < bn_rt)
	{
		ans.set_val("0");
		return ans;
	}

	if (bn_lf == bn_rt)
	{
		ans.set_val("1");
		// consider the sign
		if (bn_1.is_neg ^ bn_2.is_neg)
			ans.is_neg = true;
		return ans;
	}

	// bn_lf > bn_rt
	// TODO : not use tmp_bn_2, but change bn_2.is_neg, and recover it at the end.
	BigNum tmp_bn_2 = bn_2;
	tmp_bn_2.is_neg = false;
	int sf_len = bn_lf.len - bn_rt.len;

	bn_rt = bn_rt.sf_lf(sf_len);
	ans = ans.sf_lf(sf_len+1);
	while (bn_lf >= tmp_bn_2)
	{
		while (bn_lf < bn_rt)
		{
			bn_rt = bn_rt.sf_rt(1);
			sf_len--;
		}
		while (bn_lf >= bn_rt)
		{
			bn_lf = bn_lf - bn_rt;
			ans.val[sf_len] = ans.val[sf_len] + 1;
		}
	}
	ans.len = (int)ans.val.length();
	ans.era_zero(false);
	// consider the sign
	if (bn_1.is_neg ^ bn_2.is_neg)
		ans.is_neg = true;
	return ans;
	// ----- end +p/+k -----
}

BigNum operator%(BigNum &bn_1, BigNum &bn_2)
{
	BigNum ans;

	// ----- begin bn_2 == 0 -----
	if (bn_2 == BigNum::zero)
	{
		ans.is_neg = false;
		ans.len = -1;
		ans.val = "NAN";
		cout << "erroe s.e./0\n";

		return ans;
	}
	// ----- end bn_2 == 0 -----

	/*
		step 1 : compute +p/+k
		step 2 : consider the sign of p and k
	*/
	// ----- begin +p/+k -----
	BigNum bn_lf = bn_1;
	BigNum bn_rt = bn_2;
	bn_lf.is_neg = false;
	bn_rt.is_neg = false;

	if (bn_lf < bn_rt)
	{
		return bn_1;
	}

	if (bn_lf == bn_rt)
	{
		return BigNum::zero;
	}

	// bn_lf > bn_rt
	// TODO : not use tmp_bn_2, but change bn_2.is_neg, and recover it at the end.
	BigNum tmp_bn_2 = bn_2;
	tmp_bn_2.is_neg = false;
	int sf_len = bn_lf.len - bn_rt.len;

	bn_rt = bn_rt.sf_lf(sf_len);
	while (bn_lf >= tmp_bn_2)
	{
		while (bn_lf < bn_rt)
		{
			bn_rt = bn_rt.sf_rt(1);
		}
		while (bn_lf >= bn_rt)
		{
			bn_lf = bn_lf - bn_rt;
		}
	}
	ans = bn_lf;
	ans.len = (int)ans.val.length();
	ans.era_zero(false);
	// consider the sign
	if (bn_1.is_neg && !(ans.len == 1 && ans.val[0] == '0'))
		ans.is_neg = true;
	return ans;
	// ----- end +p/+k -----
}

BigNum BigNum::sf_lf(int sf_len)
{
	BigNum ans = *this;

	for (int i = 0; i < sf_len; i++)
	{
		ans.val.insert(ans.val.begin(), '0');
	}
	ans.len = (int)ans.val.length();
	return ans;
}

BigNum BigNum::sf_rt(int sf_len)
{
	BigNum ans = *this;

	for (int i = 0; i < sf_len; i++)
	{
		if (ans.val[0] == '0')
			ans.val.erase(ans.val.begin());
		else
		{
			cout << "in " << i << "'s iteration. val[0] = " << ans.val[0] << "\n";
			break;
		}
	}
	ans.len = (int)ans.val.length();
	return ans;
}

BigNum BigNum::mul_int(int tmp_int)
{
	BigNum ans = *this;

	// tmp_int == 0
	if (tmp_int == 0)
	{
		ans.set_val(to_string(0));
		return ans;
	}

	// tmp_int != 0
	int carry = 0, sum = 0;
	for (int i = 0; i < ans.len; i++)
	{
		sum = (ans.val[i] - '0') * tmp_int + carry;
		carry = sum / BASE;
		sum = sum % BASE;
		ans.val[i] = sum + '0';
	}
	if (carry)
	{
		ans.val.insert(ans.val.end(), '0' + carry);
	}
	ans.len = (int)ans.val.length();
	return ans;
}
// ----- end "/, %, sf_lf, sf_rt, mul_int" -----

// UNDONE
// ----- begin "ex_mod, ^" -----
BigNum BigNum::ex_mod(BigNum e, BigNum n)
{
	BigNum ans;
	return ans;
}

BigNum operator^(BigNum &bn_1, BigNum &bn_2)
{
	BigNum tmp_bn_e;
	int tmp_val_0 = bn_2.val[0] - '0';
	if (tmp_val_0 % 2 == 0)
	{
		//tmp_bn_e = bn_2 / BigNum::two;
		//tmp_bn_e = (bn_1 ^ tmp_bn_e)^BigNum::two;
		return bn_1 ^ BigNum::two;
	}
}
// ----- end "ex_mod, ^" -----

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

// ----- begin "negate" -----
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
// ----- end "negate" -----