// Ulam sequence
// A mathematician named Ulam proposed generating a sequence of numbers from any positive integer n (n > 0) as follows:
//
// If n is 1, stop.
// If n is even, the next number is n/2.
// If n is odd, the next number is 3*n + 1.
// Continue with this process until reaching 1.
#include <iostream>

using namespace::std;

int main()
{
	int n;

	cout << "Enter number: ";
	cin >> n;

	while (n != 1) {
		if (n % 2 == 0)
			n = n / 2;
		else 
			n = 3 * n + 1;
		if (n != 1)
			cout << n << " -> ";
	}

	cout << n;
	return 0;
}
