#include <iostream>
#include "gradient.h"
using namespace std;
int main() {
	char* yellow =  hex(YELLOW);
	cout << yellow << endl;
	delete yellow;
}
