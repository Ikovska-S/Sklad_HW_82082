#include<iostream>
using namespace std;

int main() {
	                /* Osnovnata funkcya na programata. Pokzazva glavnoto menu na programata.
	                   Potrbitelyat tyaabva da vyvede nyakoya ot trite tochki na menuto. */
	char c;

	}

	do {
		cout << endl;
		cout << "-------------=MAIN MENU=-------------" << endl;
		cout << "1.Items" << endl;
		cout << "2.Warehouse" << endl;
		cout << "3.Exit" << endl;

		cin.get(c);
		if (c != '\n') cin.ignore(256, '\n');

		switch (c) {
		case '1':
			items_menu();
			break;
		case '2':
			records_menu();
			break;
		}

	} while (c != '3');

	

	return 0;
}
