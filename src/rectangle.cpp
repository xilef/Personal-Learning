// Print rectangle
#include <iostream>

using namespace::std;

int main()
{
	int width, height, x, y;

	cout << "Enter width: ";
	cin >> width;
	cout << "Enter height: ";
	cin >> height;

	for (x = 0; x < height; x++) {
		for (y = 0; y < width; y++) {
			if (y == 0 || y == width - 1)
				cout << "*";
			else if (x == 0 || x == height - 1)
				cout << "*";
			else
				cout << " ";
		}
		cout << endl;
	}
	return 0;
}
