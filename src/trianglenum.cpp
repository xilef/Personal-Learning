// Triangular numbers
// Output the sequence of numbers in a triangle
#include <iostream>

using namespace::std;

int main()
{
	int num, x, y, z;

	cout << "Enter number: ";
	cin >> num;

	z = 1;
	for (x = 0; x < num; x++) {
		for (y = 0; y <= x; y++, z++)
			cout << z;
		cout << endl;
	}
	return 0;
}
