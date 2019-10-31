#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <iostream>

using namespace std;

int main() {
	bdd_init(10000, 1000);
	bdd_setvarnum(18);
	
	cout << "Hello" << endl;
	
	bdd_done();

	return 0;
}