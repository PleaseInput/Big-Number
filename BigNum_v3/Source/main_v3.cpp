#include "BigNum_v3.h"
#include <stdlib.h>
#include <time.h>
#include <random>

int main()
{
	BigNum a, b;
	BigNum ans_bn, tmp;

	int x, y;
	int ans_i;

	cin >> a >> b;
	a += b += a;

	/*
	srand(time(NULL));

	for (int i = 0; i < 20000; i++)
	{
		x = rand();
		y = rand();
		//if (rand() % 2)	x = -x;
		//if (rand() % 2)	y = -y;
		a.set_val(to_string(x));
		b.set_val(to_string(y));

		cout << "the " << i << " times\n";
		
		cout << x << " += " << y << " : ";
		x += y;
		cout << x << "\n";

		cout << a << " += " << b << " : ";
		a += b;
		cout << a << "\n";

		tmp.set_val(to_string(x));


		if (!(tmp == a))
		{
			getchar();
		}
	}
	*/

	return 0;
}