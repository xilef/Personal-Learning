// Decimal to octal converter
#include <iostream>

using namespace::std;

int main()
{
	int num, out, mult;

	cout << "Enter number: ";
	cin >> num;

	mult = 1;
	out = 0;
	while (num > 0) {
		out += (num % 8) * mult;
		num /= 8;
		mult *= 10;
	}

	cout << out;
	return 0;
}
