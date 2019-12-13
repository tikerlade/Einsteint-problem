#pragma comment(lib, "bdd.lib")

#include "bdd.h"
#include <iostream>
#include <fstream>
#include <vector>

#define N 9			// number of objects
#define M 4			// number of properties
#define K 2			// sum of properties of objects-neighbors
#define row 3
#define col 3
#define LOG_N 4		// log base2
#define N_VAR N * M * LOG_N	// number of boolean variables
#define n1 36

using namespace std;
ofstream out;
bdd p[M][N][N];


int up_neighbour[] = {0, 0, 0,
					  0, 0, 0,
					  0, 0, 1};

int down_neighbour[] = {0, 0, 1,
						0, 0, 0,
						0, 0, 0};

//int constraints1[4][3] = {{2, 2, 6},
//						   {6, 1, 6},
//						   {8, 0, 0},
//						   {7, 2, 1}};

int constraints1[n1][3] = {{0, 0, 4},
						   {0, 1, 2},
						   {0, 2, 0},
						   {0, 3, 1},

						   {1, 0, 7},
						   {1, 1, 4},
						   {1, 2, 8},
						   {1, 3, 5},

						   {2, 0, 6},
						   {2, 1, 3},
						   {2, 2, 6},
						   {2, 3, 4},

						   {3, 0, 5},
						   {3, 1, 1},
						   {3, 2, 2},
						   {3, 3, 2},

						   {4, 0, 3},
						   {4, 1, 0},
						   {4, 2, 3},
						   {4, 3, 0},

						   {5, 0, 2},
						   {5, 1, 8},
						   {5, 2, 5},
						   {5, 3, 7},

						   {6, 0, 8},
						   {6, 1, 6},
						   {6, 2, 7},
						   {6, 3, 6},

						   {7, 0, 1},
						   {7, 1, 5},
						   {7, 2, 1},
						   {7, 3, 3},

						   {8, 0, 0},
						   {8, 1, 7},
						   {8, 2, 4},
						   {8, 3, 8}};


int constraints2[5][4] = {{3, 0, 1, 3},
						  {0, 4, 1, 2},
						  {0, 2, 2, 5},
						  {1, 5, 2, 1},
						  {3, 2, 0, 5}};

// last digit of array: 0 - upper; 1 - downer
int constraints3[4][5] = {{2, 3, 1, 7, 0},
						  {1, 4, 3, 0, 0},
						  {2, 2, 1, 4, 1},
						  {2, 5, 0, 2, 1}};

int constraints4[4][4] = {{3, 6, 1, 0},
						  {0, 7, 1, 8},
						  {0, 3, 1, 3},
						  {3, 2, 3, 3}};

void generate_random();
void fun(char* varset, int size); // function for printing out solution

int* get_scheme(int i);
vector<int> get_neighbours(int* scheme, int rows, int cols, int base, int index);

void set_constraint1(bdd& solution, int object, int prop, int value);
void set_constraint2(bdd& solution, int prop1, int value1, int prop2, int value2);
void set_constraint3(bdd& solution, int prop1, int value1, int prop2, int value2, int sc, int base);
void set_constraint4(bdd& solution, int prop1, int value1, int prop2, int value2);
void set_constraint5(bdd& solution);
void set_constraint6(bdd& solution);
bdd set_constraint7();




void generate_random() {
	for (int i = 0; i < 3; i++) {
		int obj = rand() % 9;
		int prop = rand() % 4;
		int val = rand() % 9;

		constraints1[i][0] = obj;
		constraints1[i][1] = prop;
		constraints1[i][2] = val;
	}
	
	for (int i = 0; i < 5; i++) {
		int prop1 = rand() % 4;
		int val1 = rand() % 9;
		int prop2 = rand() % 4;
		int val2 = rand() % 9;

		constraints2[i][0] = prop1;
		constraints2[i][1] = val1;
		constraints2[i][2] = prop2;
		constraints2[i][3] = val2;
	}


	for (int i = 0; i < 4; i++) {
		int prop1 = rand() % 4;
		int val1 = rand() % 9;
		int prop2 = rand() % 4;
		int val2 = rand() % 9;

		constraints3[i][0] = prop1;
		constraints3[i][1] = val1;
		constraints3[i][2] = prop2;
		constraints3[i][3] = val2;
		constraints3[i][4] = rand() % 2;
	}


	for (int i = 0; i < 4; i++) {
		int prop1 = rand() % 4;
		int val1 = rand() % 9;
		int prop2 = rand() % 4;
		int val2 = rand() % 9;

		constraints4[i][0] = prop1;
		constraints4[i][1] = val1;
		constraints4[i][2] = prop2;
		constraints4[i][3] = val2;
	}

};





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

void set_constraint1(bdd& solution, int object, int prop, int value) {
	solution &= p[prop][object][value];
}
void set_constraint2(bdd& solution, int prop1, int value1, int prop2, int value2) {
	for (int i = 0; i < N; i++) {
		solution &= !(p[prop1][i][value1] ^ p[prop2][i][value2]);
	}
}
void set_constraint3(bdd& solution, int prop1, int value1, int prop2, int value2, int sc, int base = 4) {
	// first element - in corner
	// Объект с значением value1 св-ва prop1 находится слева от value2 prop2.
	// schema - задает отношение, которое ищем
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
	for (unsigned j = 0; j < N; j++) {
		for (unsigned i = 0; i < N - 1; i++) {
			for (unsigned k = i + 1; k < N; k++) {
				for (int prop = 0; i < M; i++) {
					solution &= (p[prop][i][j] >> !p[prop][k][j]);
				}
			}
		}
	}
}
void set_constraint6(bdd& solution) {
	for (int i = 0; i < N; i++) {
		bdd temp[4] = {bddfalse, bddfalse, bddfalse, bddfalse};

		for (int j = 0; j < N; j++) {
			for (int k = 0; k < M; k++) {
				temp[k] |= p[k][i][j];
			}
		}
	
		// ATTENTION ! PLACE OF ERROR might be
		for (int k = 0; k < M; k++)
			solution |= temp[k];
	}
}

// MAIN function

int main() {
	int scheme[] = {1, 0, 0,
					0, 0, 0,
					1, 0, 0};

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

	bdd solution = bddtrue;

	// Set constraints
	for (int i = 0; i < n1; i++) {
		set_constraint1(solution, constraints1[i][0], constraints1[i][1], constraints1[i][2]);
	}

	for (int i = 0; i < 5; i++) {
		set_constraint2(solution, constraints2[i][0], constraints2[i][1], constraints2[i][2], constraints2[i][3]);
	}
	/*
	for (int i = 0; i < 4; i++) {
		set_constraint3(solution, constraints3[i][0], constraints3[i][1], constraints3[i][2], constraints3[i][3], constraints3[i][4]);
	}
	
	for (int i = 0; i < 4; i++) {
		set_constraint4(solution, constraints4[i][0], constraints4[i][1], constraints4[i][2], constraints4[i][3]);
	}
	
	set_constraint5(solution);
	set_constraint6(solution);*/

	// Output results
	out.open("out.txt");
	unsigned satcount = (unsigned)bdd_satcount(solution);
	out << satcount << " Solutions:\n" << endl;
	if (satcount) bdd_allsat(solution, fun);
	out.close();

	bdd_done(); // Closing bdd

}
	
	
	
	
	
	
	//
//	//// Initialization
//	//bdd_init(10000, 1000);
//	//bdd_setvarnum(N_VAR);
//
//	//// Input boolean fucntion
//	//// pk[i][j]
//	//// k - number of property
//	//// i - number of object
//	//// j - value of property
//	//bdd p1[N][N];
//	//bdd p2[N][N];
//	//bdd p3[N][N];
//
//	//unsigned I = 0;
//	//for (unsigned i = 0; i < N; i++) {
//	//	for (unsigned j = 0; j < N; j++) {
//	//		p1[i][j] = bddtrue;
//	//		for (unsigned k = 0; k < LOG_N; k++)
//	//			p1[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);
//
//	//		p2[i][j] = bddtrue;
//	//		for (unsigned k = 0; k < LOG_N; k++)
//	//			p2[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k) ;
//	//		
//	//		p3[i][j] = bddtrue;
//	//		for (unsigned k = 0; k < LOG_N; k++)
//	//			p3[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);
//	//	}
//	//	I += LOG_N * M;
//	//}
//	//
//
//	//// Computation
//	//bdd solution = bddtrue; // result boolean function, intitially true
//
//
//	//// Constraints #1 (3 constraints)
//	//solution &= p1[7][7];
//	//solution &= p2[0][0];
//	//solution &= p3[6][6];
//
//
//	//// Constraints #2 (5 constraints)
//	//for (int i = 0; i < N; i++) solution &= !(p1[i][1] ^ p3[i][2]);
//	//for (int i = 0; i < N; i++) solution &= !(p2[i][4] ^ p3[i][7]);
//	//for (int i = 0; i < N; i++) solution &= !(p1[i][6] ^ p3[i][4]);
//	//for (int i = 0; i < N; i++) solution &= !(p2[i][2] ^ p3[i][1]);
//	//for (int i = 0; i < N; i++) solution &= !(p1[i][5] ^ p2[i][2]);
//
//
//	////сверху-слева
//	////25678 
//	////01236 - не может быть соседом
//
//	////снизу-слева
//	////03678
//	////01258 - не может быть соседом
//
//	//// Constraints #3 (4 constraints)
//	//// Объект, у которого свойство №3 равно 4 находится слева сверху от объекта, у которого свойство №2 равно 2
//	//solution &= (!p3[2][4] & !p3[5][4] & !p3[6][4] & !p3[7][4] & !p3[8][4]);
//	//solution &= (!p3[0][2] & !p3[1][2] & !p3[2][2] & !p3[3][2] & !p3[6][2]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) solution &= !(p3[i*3 + j][4] ^ p3[i*3 + j - 4][2]);
//	//
//	//// Объект, у которого свойство №1 равно 1 находится слева снизу от объекта, у которого свойство №1 равно 8
//	//solution &= (!p1[0][1] & !p1[1][1] & !p1[2][1] & !p1[5][1] & !p1[8][1]);
//	//solution &= (!p1[0][8] & !p1[3][8] & !p1[6][8] & !p1[7][8] & !p1[8][8]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) solution &= !(p1[i*3 + j][1] ^ p1[i*3 + j + 2][8]);
//
//	//// Объект, у которого свойство №3 равно 0 находится слева сверху от объекта, у которого свойство №2 равно 3
//	//solution &= (!p2[2][3] & !p2[5][3] & !p2[6][3] & !p2[7][3] & !p2[8][3]);
//	//solution &= (!p3[0][0] & !p3[1][0] & !p3[2][0] & !p3[3][0] & !p3[6][0]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) solution &= !(p2[i*3 + j][3] ^ p3[i*3 + j - 4][0]);
//
//	//// Объект, у которого свойство №2 равно 6 находится слева снизу от объекта, у которого свойство №1 равно 5
//	//solution &= (!p2[0][6] & !p2[1][6] & !p2[2][6] & !p2[5][6] & !p2[8][6]);
//	//solution &= (!p1[0][5] & !p1[3][5] & !p1[6][5] & !p1[7][5] & !p1[8][5]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) solution &= !(p2[i*3 + j][6] ^ p1[i*3 + j + 2][5]);
//
//
//
//	//// Constraints #4 (4 constraints)
//	//// Объект, у которого свойство №1 равно 8 является соседом объекту, у которого свойство №3 равно 0
//	//bdd up = bddtrue;
//	//bdd down = bddtrue;
//	//up &= (!p1[2][8] & !p1[5][8] & !p1[6][8] & !p1[7][8] & !p1[8][8]);
//	//up &= (!p3[0][0] & !p3[1][0] & !p3[2][0] & !p3[3][0] & !p3[6][0]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) up &= !(p2[i*3 + j][3] ^ p3[i*3 + j - 4][0]);
//
//	//down &= (!p1[0][4] & !p1[1][4] & !p1[2][4] & !p1[5][4] & !p1[8][4]);
//	//down &= (!p3[0][0] & !p3[3][0] & !p3[6][0] & !p3[7][0] & !p3[8][0]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) down &= !(p1[i*3 + j][4] ^ p3[i*3 + j + 2][0]);
//	//solution &= up | down;
//
//
//	//// Объект, у которого свойство №2 равно 4 является соседом объекту, у которого свойство №2 равно 8
//	//up = bddtrue;
//	//down = bddtrue;
//	//up &= (!p2[2][4] & !p2[5][4] & !p2[6][4] & !p2[7][4] & !p2[8][4]);
//	//up &= (!p2[0][8] & !p2[1][8] & !p2[2][8] & !p2[3][8] & !p2[6][8]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) up &= !(p2[i*3 + j][4] ^ p2[i*3 + j - 4][8]);
//
//	//down &= (!p2[0][4] & !p2[1][4] & !p2[2][4] & !p2[5][4] & !p2[8][4]);
//	//down &= (!p2[0][8] & !p2[3][8] & !p2[6][8] & !p2[7][8] & !p2[8][8]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) down &= !(p2[i*3 + j][4] ^ p2[i*3 + j + 2][8]);
//	//solution &= (up | down);
//
//
//	//// Объект, у которого свойство №3 равно 0 является соседом объекту, у которого свойство №1 равно 3
//	//up = bddtrue;
//	//down = bddtrue;
//	//up &= (!p3[2][0] & !p3[5][0] & !p3[6][0] & !p3[7][0] & !p3[8][0]);
//	//up &= (!p1[0][3] & !p1[1][3] & !p1[2][3] & !p1[3][3] & !p1[6][3]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) up &= !(p3[i*3 + j][0] ^ p1[i*3 + j - 4][3]);
//
//	//down &= (!p3[0][0] & !p3[1][0] & !p3[2][0] & !p3[5][0] & !p3[8][0]);
//	//down &= (!p1[0][3] & !p1[3][3] & !p1[6][3] & !p1[7][3] & !p1[8][3]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) down &= !(p3[i*3 + j][0] ^ p1[i*3 + j + 2][3]);
//	//solution &= (up | down);
//
//
//	//// Объект, у которого свойство №2 равно 6 является соседом объекту, у которого свойство №1 равно 1
//	//up = bddtrue;
//	//down = bddtrue;
//	//up &= (!p2[2][6] & !p2[5][6] & !p2[6][6] & !p2[7][6] & !p2[8][6]);
//	//up &= (!p1[0][1] & !p1[1][1] & !p1[2][1] & !p1[3][1] & !p1[6][1]);
//	//for (int i = 1; i < 3; i++)
//	//	for (int j = 1; j < 3; j++) up &= !(p2[i*3 + j][6] ^ p1[i*3 + j - 4][1]);
//
//	//down &= (!p2[0][6] & !p2[1][6] & !p2[2][6] & !p2[5][6] & !p2[8][6]);
//	//down &= (!p1[0][1] & !p1[3][1] & !p1[6][1] & !p1[7][1] & !p1[8][1]);
//	//for (int i = 0; i < 2; i++)
//	//	for (int j = 1; j < 3; j++) down &= !(p1[i*3 + j][4] ^ p3[i*3 + j + 2][0]);
//	//solution &= (up | down);
//
//
//
//	//// Constraints #5 (? constraints)
//	//for (unsigned j = 0; j < N; j++) {
//	//	for (unsigned i = 0; i < N - 1; i++) {
//	//		cout << j << " " << i << endl;
//	//		for (unsigned k = i + 1; k < N; k++) {
//	//			solution &= !p1[i][j] | !p1[k][j];
//	//			solution &= !p2[i][j] | !p2[k][j];
//	//			solution &= !p3[i][j] | !p3[k][j];
//	//		}
//	//	}
//	//}
//
//	//// Constraints #6 (? constraints)
//
//
//	//// Constraints #7 (? constraints)
//
//
//	//// вывод результатов
//	//out.open("out.txt");
//	//unsigned satcount = (unsigned)bdd_satcount(solution);
//	//out << satcount << " solutions:\n" << endl;
//	//if (satcount) bdd_allsat(solution, fun);
//	//out.close();
//
//	////cout << "Hello" << endl;
//	//bdd_done(); // Closing bdd
//
//	//return 0;
//}
//
//
//




















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
 
void fun(char* varset, int size) {
    build(varset, size, 0);
}

//
//
//#include <fstream>
//
//#define N 9		// число объектов
//#define M 3			// число свойств
//#define LOG_N 4
//#define N_VAR N * M * LOG_N	// число булевых переменных
//
//void fun(char* varset, int size); // функция, используемая для вывода решений
//
//int main(void)
//{
//	// инициализация
// bdd_init(10000000, 1000000);
//	bdd_setvarnum(N_VAR);
//	
//	// ->--- вводим функцию p(k, i, j) следующим образом ( pk[i][j] ):
//	bdd p1[N][N];
//	bdd p2[N][N];
//	bdd p3[N][N];
//
//	unsigned I = 0;
//	for (unsigned i = 0; i < N; i++)
//	{
//		for (unsigned j = 0; j < N; j++)
//		{
//			p1[i][j] = bddtrue;
//			for (unsigned k = 0; k < LOG_N; k++) p1[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k) ;
//			p2[i][j] = bddtrue;
//			for (unsigned k = 0; k < LOG_N; k++) p2[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k) ;
//			p3[i][j] = bddtrue;
//			for (unsigned k = 0; k < LOG_N; k++) p3[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N*2 + k) : bdd_nithvar(I + LOG_N*2 + k) ;
//		}
//		I += LOG_N*M;
//	}
//	// -<---
//
//	bdd task = bddtrue; // булева функция, определяющая решение, начальное значение true
//
//	// ->--- ограничение по-умолчанию 6
//	for (unsigned i = 0; i < N; i++)
//	{
//		bdd temp1 = bddfalse;
//		bdd temp2 = bddfalse;
//		bdd temp3 = bddfalse;
//		for (unsigned j = 0; j < N; j++)
//		{
//			temp1 |= p1[i][j];
//			temp2 |= p2[i][j];
//			temp3 |= p3[i][j];
//		}
//		task &= temp1 & temp2 & temp3;
//	}
//	// -<---
//
// // ограничение типа 1
// task &= p1[8][8];
// task &= p3[1][7];
// task &= p2[5][5];
// task &= p1[7][7];
//task &= p1[1][1];
//task&=p1[5][5];
//
//
// // ограничение типа 2
//for (unsigned i = 0; i < N; i++) task &= !(p1[i][4] ^ p2[i][4]);
//for (unsigned i = 0; i < N; i++) task &= !(p1[i][2] ^ p3[i][6]);
// for (unsigned i = 0; i < N; i++) task &= !(p1[i][8] ^ p3[i][0]);
// for (unsigned i = 0; i < N; i++) task &= !(p2[i][0] ^ p3[i][8]);
// for (unsigned i = 0; i < N; i++) task &= !(p1[i][0] ^ p3[i][8]);
//
// // !(a ^ b) = (a <=> b)
// // ->--- ограничение типа 3
//	// объект с первым свойством 0 слева-вверху от объекта с первым свойством 4
//	task &= (!p1[6][4] & !p1[3][4] & !p1[0][4] & !p1[1][4] & !p1[2][4] & !p1[2][0] & !p1[5][0] & !p1[8][0]  & !p1[7][0] & !p1[6][0]);
//	task &= !(p1[0][0] ^ p1[4][4]) & !(p1[3][0] ^ p1[7][4]) & !(p1[1][0] ^ p1[5][4]) & !(p1[4][0] ^ p1[8][4]);
//
//	// объект со вторым свойством 1 слева-вверху от объекта со вторым свойством 5
//	task &= (!p2[6][5] & !p2[3][5] & !p2[0][5] & !p2[1][5] & !p2[2][5] & !p2[2][1] & !p2[5][1] & !p2[8][1]  & !p2[7][1] & !p2[6][1]);
//	task &= !(p2[0][1] ^ p2[4][5]) & !(p2[3][1] ^ p2[7][5]) & !(p2[1][1] ^ p2[5][5]) & !(p2[4][1] ^ p2[8][5]);
//	
//	// объект с третьим свойством 5 слева-вверху от объекта с третьим свойством 1
//	task &= (!p3[6][1] & !p3[3][1] & !p3[0][1] & !p3[1][1] & !p3[2][1] & !p3[2][5] & !p3[5][5] & !p3[8][5]  & !p3[7][5] & !p3[6][5]);
//	task &= !(p3[0][5] ^ p3[4][1]) & !(p3[3][5] ^ p3[7][1]) & !(p3[1][5] ^ p3[5][1]) & !(p3[4][5] ^ p3[8][1]);
//	
//	// объект со третьим свойством 4 слева-внизу от объекта со вторым свойством 2
//	task &= (!p2[6][2] & !p2[3][2] & !p2[0][2] & !p2[7][2] & !p2[8][2] & !p3[2][4] & !p3[5][4] & !p3[8][4]  & !p3[0][4] & !p3[1][4]);
//	task &= !(p3[3][4] ^ p2[1][2]) & !(p3[6][4] ^ p2[4][2]) & !(p3[4][4] ^ p2[2][2]) & !(p3[7][4] ^ p2[5][2]);
//	
//	// объект с первым свойством 3 слева-внизу от объекта со вторым свойством 1
//	task &= (!p2[6][1] & !p2[3][1] & !p2[0][1] & !p2[7][1] & !p2[8][1] & !p1[2][3] & !p1[5][3] & !p1[8][3]  & !p1[0][3] & !p1[1][3]);
//	task &= !(p1[3][3] ^ p2[1][1]) & !(p1[6][3] ^ p2[4][1]) & !(p1[4][3] ^ p2[2][1]) & !(p1[7][3] ^ p2[5][1]);
//	// -<---
//
//
//
//	//// ->--- ограничение типа 4
//	bdd temp1 = bddtrue;
//	bdd temp2 = bddtrue;
//	// объект с первым свойством 4 рядом с объектом со вторым свойством 8 
//	//слева-внизу
//	int m=8;
//	int n=4;
//	temp1 &= (!p2[6][m] & !p2[3][m] & !p2[0][m] & !p2[7][m] & !p2[8][m] & !p1[2][n] & !p1[5][n] & !p1[8][n]  & !p1[0][n] & !p1[1][n]);
//	temp1 &= !(p1[3][n] ^ p2[1][m]) & !(p1[6][n] ^ p2[4][m]) & !(p1[4][n] ^ p2[2][m]) & !(p1[7][n] ^ p2[5][m]);
//	//слева-вверху
//	temp2= (!p2[6][m] & !p2[3][m] & !p2[0][m] & !p2[1][m] & !p2[2][m] & !p1[2][n] & !p1[5][n] & !p1[8][n]  & !p1[7][n] & !p1[6][n]);
//	temp2= !(p1[0][n] ^ p2[4][m]) & !(p1[3][n] ^ p2[7][m]) & !(p1[1][n] ^ p2[5][m]) & !(p1[4][n] ^ p2[8][m]);
//	task &= (temp1 | temp2);
//
//
//
//	temp1 = bddtrue;
//	temp2 = bddtrue;
//	//слева-внизу
//	m=7;
//	n=5;
//	temp1 &= (!p2[6][m] & !p2[3][m] & !p2[0][m] & !p2[7][m] & !p2[8][m] & !p3[2][n] & !p3[5][n] & !p3[8][n]  & !p3[0][n] & !p3[1][n]);
//	temp1 &= !(p3[3][n] ^ p2[1][m]) & !(p3[6][n] ^ p2[4][m]) & !(p3[4][n] ^ p2[2][m]) & !(p3[7][n] ^ p2[5][m]);
//	//слева-вверху
//	temp2= (!p2[6][m] & !p2[3][m] & !p2[0][m] & !p2[1][m] & !p2[2][m] & !p3[2][n] & !p3[5][n] & !p3[8][n]  & !p3[7][n] & !p3[6][n]);
//	temp2= !(p3[0][n] ^ p2[4][m]) & !(p3[3][n] ^ p2[7][m]) & !(p3[1][n] ^ p2[5][m]) & !(p3[4][n] ^ p2[8][m]);
//	task &= (temp1 | temp2);
//
//
//	temp1 = bddtrue;
//	temp2 = bddtrue;
//	// объект с третьим свойством 0 рядом с объектом с первым свойством 6 
//	//слева-внизу
//	m=4;
//	n=2;
//	temp1 &= (!p1[6][m] & !p1[3][m] & !p1[0][m] & !p1[7][m] & !p1[8][m] & !p3[2][n] & !p3[5][n] & !p3[8][n]  & !p3[0][n] & !p3[1][n]);
//	temp1 &= !(p3[3][n] ^ p1[1][m]) & !(p3[6][n] ^ p1[4][m]) & !(p3[4][n] ^ p1[2][m]) & !(p3[7][n] ^ p1[5][m]);
//	//слева-вверху
//	temp2= (!p1[6][m] & !p1[3][m] & !p1[0][m] & !p1[1][m] & !p1[2][m] & !p3[2][n] & !p3[5][n] & !p3[8][n]  & !p3[7][n] & !p3[6][n]);
//	temp2= !(p3[0][n] ^ p1[4][m]) & !(p3[3][n] ^ p1[7][m]) & !(p1[1][n] ^ p1[5][m]) & !(p3[4][n] ^ p1[8][m]);
//	task &= (temp1 | temp2);
//
//
//	temp1 = bddtrue;
//	temp2 = bddtrue;
//	// объект с третьим свойством 3 рядом с объектом с первым свойством 6 
//	//слева-внизу
//	m=4;
//	n=6;
//	temp1 &= (!p3[6][m] & !p3[3][m] & !p3[0][m] & !p3[7][m] & !p3[8][m] & !p1[2][n] & !p1[5][n] & !p1[8][n]  & !p1[0][n] & !p1[1][n]);
//	temp1 &= !(p1[3][n] ^ p3[1][m]) & !(p1[6][n] ^ p3[4][m]) & !(p1[4][n] ^ p3[2][m]) & !(p1[7][n] ^ p3[5][m]);
//	//слева-вверху
//	temp2= (!p3[6][m] & !p3[3][m] & !p3[0][m] & !p3[1][m] & !p3[2][m] & !p1[2][n] & !p1[5][n] & !p1[8][n]  & !p1[7][n] & !p1[6][n]);
//	temp2= !(p1[0][n] ^ p3[4][m]) & !(p1[3][n] ^ p3[7][m]) & !(p3[1][n] ^ p3[5][m]) & !(p1[4][n] ^ p3[8][m]);
//	task &= (temp1 | temp2);
//
//
//	temp1 = bddtrue;
//	temp2 = bddtrue;
//	// объект с третьим свойством 3 рядом с объектом с третьим свойством 6 
//	//слева-внизу
//	m=1;
//	n=3;
//	temp1 &= (!p3[6][m] & !p3[3][m] & !p3[0][m] & !p3[7][m] & !p3[8][m] & !p2[2][n] & !p2[5][n] & !p2[8][n]  & !p2[0][n] & !p2[1][n]);
//	temp1 &= !(p2[3][n] ^ p3[1][m]) & !(p2[6][n] ^ p3[4][m]) & !(p2[4][n] ^ p3[2][m]) & !(p2[7][n] ^ p3[5][m]);
//	//слева-вверху
//	temp2= (!p3[6][m] & !p3[3][m] & !p3[0][m] & !p3[1][m] & !p3[2][m] & !p2[2][n] & !p2[5][n] & !p2[8][n]  & !p2[7][n] & !p2[6][n]);
//	temp2= !(p2[0][n] ^ p3[4][m]) & !(p2[3][n] ^ p3[7][m]) & !(p2[1][n] ^ p3[5][m]) & !(p2[4][n] ^ p3[8][m]);
//	task &= (temp1 | temp2);
//
//
//
//
//
//
//
//
//
//	
//	// ->--- ограничение по-умолчанию 5
//	for (unsigned j = 0; j < N; j++)
//		for (unsigned i = 0; i < N - 1; i++)
//			for (unsigned k = i + 1; k < N; k++)
//			{
//				task &= p1[i][j] >> !p1[k][j];
//				task &= p2[i][j] >> !p2[k][j];
//				task &= p3[i][j] >> !p3[k][j];
//				cout<<j<<" "<<i<<" "<<k<< endl;
//			}
//	// -<---
//
//
//
//	// вывод результатов
//	out.open("out.txt");
//	unsigned satcount = (unsigned)bdd_satcount(task);
//	out<<satcount<<" solutions:\n"<<endl;
//	if (satcount) bdd_allsat(task, fun);
//	out.close();
//
//	bdd_done(); // завершение работы библиотеки
//	return 0;
//}
//
////// Ниже реализация функций, управляющих выводом результатов.
////// Рекомендуется самостоятельно с ними ознакомиться.
////// В собственных заданиях следует использовать эти функции
////// или придумать собственные.
////
////char var[N_VAR];
////
////void print(void)
////{
////	for (unsigned i = 0; i < N; i++)
////	{
////		out<<i<<": ";
////		for (unsigned j = 0; j < M; j++)
////		{
////			int J = i*M*LOG_N + j*LOG_N;
////		    int num = 0;
////			for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
////			out<<num<<' ';
////		}
////		out<<endl;
////	}
////	out<<endl;
////}
////
////void build(char* varset, unsigned n, unsigned I)
////{
////	if (I == n - 1)
////	{
////		if (varset[I] >= 0)
////		{
////			var[I] = varset[I];
////			print();
////			return;
////		}
////		var[I] = 0;
////		print();
////		var[I] = 1;
////		print();
////		return;
////	}
////	if (varset[I] >= 0) 
////	{
////		var[I] = varset[I];
////		build(varset, n, I + 1);
////		return;
////	}
////	var[I] = 0;
////	build(varset, n, I + 1);
////	var[I] = 1;
////	build(varset, n, I + 1);
////}
////
////void fun(char* varset, int size)
////{
////	build(varset, size, 0);
////}