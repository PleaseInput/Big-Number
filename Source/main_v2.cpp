#include "BigNum.h"
#include <stdlib.h>
#include <time.h>
#include <random>

int main()
{
	//random_device rd;
	//default_random_engine gen = default_random_engine(rd());
	//uniform_int_distribution<int> dis(0, 2147483647);

	BigNum a, b;
	BigNum ans_bn, tmp;

	int x, y;
	int ans_i;

	srand(time(NULL));

	//char c;
	//string t;
	/*
	for (int i = 0; i < 10; i++)
	{
		ans_bn = a.mul_int(i);
		tmp.set_val(to_string(x * i));
		if (ans_bn == tmp)
			cout << i << " ok\n";
		else
			cout << i << "\n";
	}
	*/
	//for(int i=0;i<255;i++)
	//	cout << i << " th is \"" << char(i) << "\"\n";

	cin >> a >> b;
	a = b = a;
	ans_bn = a + b;
	cout << ans_bn << "\n";
	

	/*
	for(int i=0; i < 200000; i++)
	{
		x = rand() * rand();
		y = rand() + 1;
		if (rand() % 2)	x = -x;
		if (rand() % 2)	y = -y;
		a.set_val(to_string(x));
		b.set_val(to_string(y));

		ans_i = x % y;		
		ans_bn = a % b;
		
		tmp.set_val(to_string(ans_i));

		cout << "the " << i << " times\n";
		cout << x << " % " << y << " = " << ans_i << "\n";
		cout << a << " % " << b << " = " << ans_bn << "\n";

		if (!(tmp == ans_bn))
		{
			//cout << x << " / " << y << " = " << ans_i << "\n";
			//cout << a << " / " << b << " = " << ans_bn << "\n";
			getchar();
		}
	}
	*/

	/*
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int p = (i + j) / 10;
			cout << p << ", ";
		}
		cout << "\n";
	}
	cout << "\n";
	*/
	return 0;
}