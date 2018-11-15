#include <iostream>
#include <array>
#include <iomanip>
#include <fstream>
using namespace std;

int main() {
	const int RANGE = 10;
	const double INIT_ON = 100.000;
	const double INIT_OFF = 0.000;
	const int SET_PREC = 3;
	const int SET_WIDTH = 9;
	double tempMapInit[RANGE][RANGE];
	double tempMapNew[RANGE][RANGE];
	int y;
	int x;
	int i;

	//Print initial tempMap
	cout << "Hotplate simulator\n\nPrinting initial plate..." << endl;
	cout << fixed << setprecision(SET_PREC);
	for (y = 0; y < RANGE; y++) {
		for (x = 0; x < RANGE; x++) {
			if (y == 0 && x>0 && x<RANGE - 1 || y == RANGE - 1 && x>0 && x<RANGE - 1) {
				tempMapInit[y][x] = INIT_ON;
				tempMapNew[y][x] = INIT_ON;
				cout << setw(SET_WIDTH) << tempMapInit[y][x];
			}
			else {
				tempMapInit[y][x] = INIT_OFF;
				tempMapNew[y][x] = INIT_OFF;
				cout << setw(SET_WIDTH) << tempMapInit[y][x];
			}
			if (x<RANGE - 1) cout << ",";
		}
		cout << endl;
	}

	//calculate first stage
	for (y = 1; y < RANGE -1; y++) {
		for (x = 1; x < RANGE -1; x++) {
			tempMapNew[y][x] = (tempMapInit[y - 1][x] +
							tempMapInit[y + 1][x] +
							tempMapInit[y][x - 1] +
							tempMapInit[y][x + 1]) / 4;
		}
	}

	//print first stage
	cout << "\n\nPrinting plate after one iteration..." << endl;
	for (y = 0; y < RANGE; y++) {
		for (x = 0; x < RANGE; x++) {
			cout << setw(SET_WIDTH) << tempMapNew[y][x];
			if (x<RANGE - 1) cout << ",";
		}
		cout << endl;
	}


	//iterate until values stabilize
	double maxChangeMargin;
	do {
		maxChangeMargin = 0.00;
		for (y = 1; y < RANGE - 1; y++) {
			for (x = 1; x < RANGE - 1; x++) {
				tempMapNew[y][x] = (tempMapInit[y - 1][x] +
					tempMapInit[y + 1][x] +
					tempMapInit[y][x - 1] +
					tempMapInit[y][x + 1]) / 4;
			}
		}
		//swap values between tables to preserve last iteration until current is complete.
		for (y = 1; y < RANGE - 1; y++) {
			for (x = 1; x < RANGE - 1; x++) {
				tempMapInit[y][x] = (tempMapNew[y - 1][x] +
					tempMapNew[y + 1][x] +
					tempMapNew[y][x - 1] +
					tempMapNew[y][x + 1]) / 4;
				//evaluate greatest change between cell values
				double localChange = tempMapInit[y][x] - tempMapNew[y][x];
				if (localChange > maxChangeMargin) {
					maxChangeMargin = localChange;
				}
			}
		}
	} while (maxChangeMargin >= 0.1); //stop when greatest change margin < 0.1


	// create and open Hotplate.cvs
	ofstream outFS;
	outFS.open("Hotplate.csv");
	if (!outFS.is_open()) {
		cout << "Could not open file myoutfile.txt." << endl;
		return 1;
	}


	//print final plate AND output to Hotplate.cvs to save writing a whole other loop.
	cout << "\n\nPrinting final plate..." << endl;
	for (y = 0; y < RANGE; y++) {
		for (x = 0; x < RANGE; x++) {
            cout << fixed << setprecision(SET_PREC);
			cout << setw(SET_WIDTH) << tempMapNew[y][x];
			outFS << fixed << setprecision(SET_PREC);
			outFS << setw(SET_WIDTH) << tempMapNew[y][x];
			if (x < RANGE - 1) {
				cout << ",";
				outFS << ",";
			}
		}
		cout << endl;
		outFS << endl;
	}

	cout << "\nOutputting final plate to file \"Hotplate.csv\"..." << endl << endl;


	ifstream inFS;
	inFS.open("Inputplate.txt");
	if (!inFS.is_open()) {
		cout << "Could not open file myinfile.txt." << endl;
		system("pause");
		return 1;
	}

	double loadTempMapInit[RANGE][RANGE];
	double loadTempMapNew[RANGE][RANGE];
	for (y = 0; y < RANGE; y++) {
		for (x = 0; x < RANGE; x++) {
			inFS >> loadTempMapInit[y][x];
			loadTempMapNew[y][x] = loadTempMapInit[y][x];
		}
	}




	//ITERATE NEW INFO
	for (i = 0; i < 2; i ++) {
		maxChangeMargin = 0.00;
		for (y = 1; y < RANGE - 1; y++) {
			for (x = 1; x < RANGE - 1; x++) {
				loadTempMapNew[y][x] = (loadTempMapInit[y - 1][x] +
					loadTempMapInit[y + 1][x] +
					loadTempMapInit[y][x - 1] +
					loadTempMapInit[y][x + 1]) / 4;
			}
		}

		//swap values between tables to preserve last iteration until current is complete.
		for (y = 1; y < RANGE - 1; y++) {
			for (x = 1; x < RANGE - 1; x++) {
				loadTempMapInit[y][x] = (loadTempMapNew[y - 1][x] +
					loadTempMapNew[y + 1][x] +
					loadTempMapNew[y][x - 1] +
					loadTempMapNew[y][x + 1]) / 4;
			}
		}
	}

	//PRINT NEW TABLE
	cout << "Printing input plate after 3 updates..." << endl;
	for (y = 0; y < RANGE; y++) {
		for (x = 0; x < RANGE; x++) {
			cout << setw(SET_WIDTH) << loadTempMapNew[y][x];
			if (x < RANGE - 1) {
				cout << ",";
			}
		}
		cout << endl;
	}

	// Done with file, so close it
	outFS.close();
	inFS.close();

	//system("pause");
	return 0;
}
