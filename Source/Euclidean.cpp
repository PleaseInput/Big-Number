#include<iostream>
#define SWAP(x, y, t) (t = x, x = y, y = t)
using namespace std;

int main()
{
	int a, b;
	cin >> a >> b;
	
	int r0 = a, s0 = 1, t0 = 0;
	int r1 = b, s1 = 0, t1 = 1;
	int q2, tmp;
	
	while (1)
	{
		q2 = r0 / r1;

		r0 -= (q2*r1);
		SWAP(r0, r1, tmp);
	
		if (r1 == 0)
			break;

		s0 -= (q2*s1);
		SWAP(s0, s1, tmp);

		t0 -= (q2*t1);
		SWAP(t0, t1, tmp);
	

	}
	
	cout << a << " * " << s1 << " + " << b << " * " << t1 << " = " << r0 << "\n";

	return 0;
}