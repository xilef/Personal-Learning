// Sort in ascending order the letters of a string
#include <iostream>
#include <cstring>

using namespace::std;

int main()
{
	char in[30], temp;
	int x, y, len;
	bool swap = true;
	cout << "Enter string: ";
	cin >> in;

	// Sort
	len = strlen(in);
	while (swap) {
		swap = false;
		for (x = 0; x < len - 1; x++) {
			if (in[x] > in[x + 1]) {
				temp = in[x];
				in[x] = in[x + 1];
				in[x + 1] = temp;
				swap = true;
			}
		}
	}

	cout << "Sorted string is: " << endl;
	cout << in;
	return 0;
}
