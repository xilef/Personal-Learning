// Multiplication Table
#include <iostream>

using namespace::std;

int main()
{
	int in, x;

	cout << "Enter number: ";
	cin >> in;

	for (x = 1; x <= 10; x++)
		cout << in << "*" << x << " = " << in * x << endl;

	return 0;
}
