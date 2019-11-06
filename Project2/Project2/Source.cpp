#pragma comment(lib, "bdd.lib")

#include "bdd.h"
#include <iostream>
#include <fstream>

#define N 9			// number of objects
#define M 4			// number of properties
#define K 2			// sum of properties of objects-neighbors
#define LOG_N 4		// log base2
#define N_VAR N * M * LOG_N	// number of boolean variables

using namespace std;
ofstream out;

void fun(char* varset, int size); // function for printing out solution

// Question - "How to scale program if objects in square ?"
// "Can I use 3D array"
int main() {

	// Initialization
	bdd_init(10000, 1000);
	bdd_setvarnum(N_VAR);

	// Input boolean fucntion
	// pk[i][j]
	// k - number of property
	// i - number of object
	// j - value of property
	bdd p1[N][N];
	bdd p2[N][N];
	bdd p3[N][N];
	bdd p4[N][N];

	unsigned I = 0;
	for (unsigned i = 0; i < N; i++) {
		for (unsigned j = 0; j < N; j++) {
			p1[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++)
				p1[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);

			p2[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++)
				p2[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k) ;
			
			p3[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++)
				p3[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);

			p4[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++)
				p4[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 3 + k) : bdd_nithvar(I + LOG_N * 3 + k);

			}

		I += LOG_N * M;
	}
	

	// Computation
	bdd solution = bddtrue; // result boolean function, intitiallly true


	// Constraints #1 (3 constraints)
	solution &= p1[7][6];
	solution &= p2[0][5];
	solution &= p3[6][7];



	// Constraints #2 (5 constraints)
	for (int i = 0; i < N; i++) solution &= !(p4[i][8] ^ p1[i][2]);
	for (int i = 0; i < N; i++) solution &= !(p2[i][4] ^ p4[i][7]);
	for (int i = 0; i < N; i++) solution &= !(p4[i][6] ^ p3[i][4]);
	for (int i = 0; i < N; i++) solution &= !(p2[i][2] ^ p4[i][7]);
	for (int i = 0; i < N; i++) solution &= !(p3[i][5] ^ p3[i][6]);



	// Constraints #3 (4 constraints)
	for (int i = 0; i < N; i++) solution &= !p4[i][2] & !p3[i*3][4];					// validate objects
	for (int i = 1; i < 3; i++)
		for (int j = 1; j < 3; j++) solution &= !(p3[i*3 + j][4] ^ p4[i*3 + j - 4][2]);	// apply constraints
	
	for (int i = 0; i < 3; i++) solution &= !p1[i+6][8] & !p4[i*3][1];
	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 3; j++) solution &= !(p4[i*3 + j][1] ^ p1[i*3 + j + 2][8]);

	for (int i = 0; i < 3; i++) solution &= !p2[i][3] & !p3[i*3][0];
	for (int i = 1; i < 3; i++)
		for (int j = 1; j < 3; j++) solution &= !(p3[i*3 + j][0] ^ p2[i*3 + j - 4][3]);

	for (int i = 0; i < 3; i++) solution &= !p1[i+6][5] & !p2[i*3][7];
	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 3; j++) solution &= !(p2[i*3 + j][7] ^ p1[i*3 + j + 2][5]);



	// Constraints #4 (4 constraints)


	// Constraints #5 (? constraints)
	

	// Constraints #6 (? constraints)


	// Constraints #7 (? constraints)

	
	cout << "Hello" << endl;
	bdd_done(); // Closing bdd

	return 0;
}


char var[N_VAR];
 
void print(void) {
    for (unsigned i = 0; i < N; i++) {
        out << i<< ": ";

        for (unsigned j = 0; j < M; j++) {
            int J = i*M*LOG_N + j*LOG_N;
            int num = 0;

            for (unsigned k = 0; k < LOG_N; k++)
				num += (unsigned)(var[J + k] << k);
            
			out << num << ' ';
        }
        out << endl;
    }
    out << endl;
}
 
void build(char* varset, unsigned n, unsigned I) {
    if (I == n - 1) {
        if (varset[I] >= 0) {
            var[I] = varset[I];
            print();
            return;
        }
        var[I] = 0;
        print();
        var[I] = 1;
        print();
        return;
    }
    if (varset[I] >= 0) {
        var[I] = varset[I];
        build(varset, n, I + 1);
        return;
    }
    var[I] = 0;
    build(varset, n, I + 1);
    var[I] = 1;
    build(varset, n, I + 1);
}
 
void fun(char* varset, int size)
{
    build(varset, size, 0);
}