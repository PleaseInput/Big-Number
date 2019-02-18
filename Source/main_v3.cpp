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

	
	/*cin >> a >> b;
	ans_bn = a - b - b;
	cout << ans_bn;*/
	

	srand(time(NULL));

	for (int i = 0; i < 200000; i++)
	{
		x = rand();
		y = rand();
		if (rand() % 2)	x = -x;
		if (rand() % 2)	y = -y;
		a.set_val(to_string(x));
		b.set_val(to_string(y));

		cout << "the " << i << " times\n";
		
		cout << x << " += " << y << " : ";
		x += y;
		cout << x << "\n";

		cout << a << " += " << b << " : ";
		a += b;
		cout << a << "\n";

		/*ans_i = x - y;
		cout << x << " - " << y << " = " << ans_i << "\n";

		ans_bn = a - b;
		cout << a << " - " << b << " = " << ans_bn << "\n";*/

		tmp.set_val(to_string(x));

		if (!(tmp == a))
		{
			getchar();
		}
	}
	

	return 0;
}