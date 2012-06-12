// Binary Triangle
// output a triangle consisting of 1 and 0
#include <iostream>

using namespace::std;

int main()
{
	int in, x, y, z;

	cout << "Input number: ";
	cin >> in;

	z = 1;
	for (x = 0; x < in; x++) {
		for (y = 0; y <= x; y++, z++) {
			cout << z % 2;
		}
		cout << endl;
	}

	return 0;
}
