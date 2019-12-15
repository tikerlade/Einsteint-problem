#pragma comment(lib, "bdd.lib")

#include "bdd.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define N 9			// number of objects
#define M 4			// number of properties
#define K N * M		// sum of properties of objects-neighbors
#define row 3
#define col 3
#define LOG_N 4		// log base2
#define N_VAR N * M * LOG_N	// number of boolean variables
#define n1 12

using namespace std;

void fun(char* varset, int size); // function for printing out solution


int* get_scheme(int i);
vector<int> get_neighbours(int* scheme, int rows, int cols, int base, int index);
int sum_digits(int number);
bdd apply7(vector<int> neighbours, int values);

void set_constraint1(bdd& solution, int object, int prop, int value);
void set_constraint2(bdd& solution, int prop1, int value1, int prop2, int value2);
void set_constraint3(bdd& solution, int prop1, int value1, int prop2, int value2, int sc, int base);
void set_constraint4(bdd& solution, int prop1, int value1, int prop2, int value2);
void set_constraint5(bdd& solution);
void set_constraint6(bdd& solution);
void set_constraint7(bdd& solution);




ofstream out;
bdd p[M][N][N];
char var[N_VAR];

int up_neighbour[] = {0, 0, 0,
					  0, 0, 0,
					  0, 0, 1};

int down_neighbour[] = {0, 0, 1,
						0, 0, 0,
						0, 0, 0};

int scheme[] = {1, 0, 0,
				0, 0, 0,
				1, 0, 0};


int constraints1[n1][3] = {{0, 3, 1},
						   {1, 2, 8},
						   {1, 3, 5},
						   {2, 0, 6},
						   {2, 2, 6},
						   {2, 3, 4},
						   {3, 0, 5},
						   {5, 3, 7},
						   {6, 0, 8},
						   {7, 0, 1},
						   {8, 1, 7},
						   {8, 2, 4}};


int constraints2[5][4] = {{3, 0, 2, 3},
						  {0, 4, 1, 2},
						  {0, 2, 2, 5},
						  {1, 5, 2, 1},
						  {0, 8, 1, 6}};

// last digit of array: 0 - upper; 1 - downer
int constraints3[4][5] = {{0, 3, 1, 7, 0},
						  {2, 0, 3, 0, 0},
						  {2, 2, 1, 4, 1},
						  {1, 5, 0, 2, 1}};

int constraints4[4][4] = {{3, 6, 1, 0},
						  {0, 7, 1, 8},
						  {1, 1, 3, 5},
						  {3, 2, 3, 3}};


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
 
void fun(char* varset, int size) {
    build(varset, size, 0);
}

int* get_scheme(int i) {
	if (i == 0) {
		return up_neighbour;
	} else if (i == 1) {
		return down_neighbour;
	}
};
vector<int> get_neighbours(int* scheme, int rows, int cols, int base, int index) {
	vector<int> neighbors = vector<int>();
	int x = index % cols;
	int y = index / rows;

	for (int i = 0; i < N; i++) {
		if ((scheme[i] == 1)) {
			int dx = (i % 3) - (base % 3);
			int dy = (i / 3) - (base / 3);

			if ((x + dx >= 0) && (y + dy >= 0) && (x + dx < cols) && (y + dy < rows)) {
				neighbors.push_back((x + dx) + (y + dy)*cols);
			}
		}
	}

	return neighbors;
}

int sum_digits(int number) {
	int sum = 0;

	while (number > 10) {
		sum += number % 10;
		number /= 10;
	}

	return sum;
}


void set_constraint1(bdd& solution, int object, int prop, int value) {
	solution &= p[prop][object][value];
}
void set_constraint2(bdd& solution, int prop1, int value1, int prop2, int value2) {
	for (int i = 0; i < N; i++) {
		solution &= !(p[prop1][i][value1] ^ p[prop2][i][value2]);
	}
}
void set_constraint3(bdd& solution, int prop1, int value1, int prop2, int value2, int sc, int base = 4) {
	int* scheme = get_scheme(sc);

	for (int i = 0; i < N; i++) {
		vector<int> neighbours = get_neighbours(scheme, row, col, base, i);

		if (!neighbours.empty()) {
			for each (int neighbour in neighbours) {
				solution &= !(p[prop1][i][value1] ^ p[prop2][neighbour][value2]);
			}
		} else {
			solution &= !p[prop1][i][value1];
		}
	}
}
void set_constraint4(bdd& solution, int prop1, int value1, int prop2, int value2) {
	bdd up = bddtrue;
	bdd down = bddtrue;

	set_constraint3(up, prop1, value1, prop2, value2, 0);
	set_constraint3(down, prop1, value1, prop2, value2, 1);

	solution &= (up | down);
}
void set_constraint5(bdd& solution) {
	for (int j = 0; j < N; j++) {
		for (int  i = 0; i < N - 1; i++) {
			for (int k = i + 1; k < N; k++) {
				for (int pr = 0; pr < M; pr++) {
					solution &= p[pr][i][j] >> !p[pr][k][j];
				}
			}
		}
	}
}
void set_constraint6(bdd& solution) {
	for (int i = 0; i < N; i++) {
		bdd temp[M] = {bddfalse, bddfalse, bddfalse, bddfalse};

		for (int j = 0; j < N; j++) {
			for (int k = 0; k < M; k++) {
				temp[k] |= p[k][i][j];
			}
		}

		for (int k = 0; k < M; k++)
			solution &= temp[k];
	}
}
void set_constraint7(bdd& solution, int* scheme) {
	for (int i = 0; i < N; i++) {
		vector<int> neighbours = get_neighbours(scheme, row, col, 4, i);
		neighbours.push_back(i);
		bdd temp = bddfalse;

		for (int props = (int) pow(M+1, N); props < (int) pow(M+1, N+1); props++) {
			if (sum_digits(props) > K * neighbours.size()) {
				temp = apply7(neighbours, props);
			} else {
				break;
			}
		}

		solution &= !temp;
	}
}

bdd apply7(vector<int> neighbours, int properties) {
	bdd temp = bddfalse;

	for (int i = 0; i < neighbours.size(); i++) {
		int obj = neighbours[i];

		bdd temp2 = bddtrue;
		for (int prop = 0; prop < M; prop++) {
			int mod = (int) pow(N + 1, prop + 1);
			int div = (int) pow(N + 1, prop);
			int value = (properties % mod) / div;

			temp2 &= p[M - prop - 1][obj][value];
		}
		temp |= temp2;
	}

	return temp;
}


// MAIN function
int main() {
	cout << N_VAR << endl;

	//// Initialization
	bdd_init(10000, 1000);
	bdd_setvarnum(N_VAR);

	unsigned I = 0;
	for (unsigned i = 0; i < N; i++) {
		for (unsigned j = 0; j < N; j++) {
			for (int pr = 0; pr < M; pr++) {
				p[pr][i][j] = bddtrue;

				for (int k = 0; k < LOG_N; k++) {
					p[pr][i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * pr + k) : bdd_nithvar(I + LOG_N * pr + k);
				}
			}
		}
		I += LOG_N * M;
	}


	// Find solution
	bdd solution = bddtrue;
	
	set_constraint6(solution);

	// Set constraints#1
	for (int i = 0; i < n1; i++) {
		set_constraint1(solution, constraints1[i][0], constraints1[i][1], constraints1[i][2]);
	}
	
	// Set constraints#2
	for (int i = 0; i < 5; i++) {
		set_constraint2(solution, constraints2[i][0], constraints2[i][1], constraints2[i][2], constraints2[i][3]);
	}
	
	// Set constraints#3
	for (int i = 0; i < 4; i++) {
		set_constraint3(solution, constraints3[i][0], constraints3[i][1], constraints3[i][2], constraints3[i][3], constraints3[i][4]);
	}
	
	// Set constraints#4
	for (int i = 0; i < 4; i++) {
		set_constraint4(solution, constraints4[i][0], constraints4[i][1], constraints4[i][2], constraints4[i][3]);
	}
	
	// Set constraints#5 & #6
	set_constraint5(solution);
	set_constraint7(solution, scheme);
	
	// Output results
	out.open("out.txt");
	unsigned satcount = (unsigned)bdd_satcount(solution);
	out << satcount << " Solutions:\n" << endl;
	if (satcount) bdd_allsat(solution, fun);
	out.close();
	bdd_done(); // Closing bdd

 	return 0;
}
