#include<iostream>
using namespace std;

int items_menu() {
	/*
	Funkcia koiato pokzazva menuto za obrabotka na artikulite v sklada.
	Potrbiteliat triabva da vuvede niakoia ot chetirite tochki na menuto.
	*/
	char c;


	do {
		cout << endl;
		cout << "-------------=Items Menu=-------------" << endl;
		cout << "1.Insert Item" << endl;
		cout << "2.List Items" << endl;
		cout << "3.Edit Item" << endl;
		cout << "4.Back" << endl;

		cin.get(c);
		if (c != '\n') cin.ignore(256, '\n');

		switch (c) {
		case '1':
			break;
		case '2':
			break;
		case '3':
			break;
		}

	} while (c != '4');

	return 0;
}

int records_menu() {
	/*
	Funkcia koiato pokzazva menuto za obrabotka na zapisite za prihod i razhod ot sklada.
	Potrbiteliat triabva da vuvede niakoia ot sheste tochki na menuto.
	*/
	char c;

	do {
		cout << endl;
		cout << "------------=Records Menu=------------" << endl;
		cout << "1.Print inventory" << endl;
		cout << "2.Add inventory" << endl;
		cout << "3.Remove inventory" << endl;
		cout << "4.Records log" << endl;
		cout << "5.Inventory clean" << endl;
		cout << "6.Back" << endl;

		cin.get(c);
		if (c != '\n') cin.ignore(256, '\n');

		switch (c) {
		case '1':
			break;
		case '2':
			break;
		case '3':
			break;
		case '4':
			break;
		case '5':
			break;
		}


	} while (c != '6');

	return 0;
}

int main() {
	                /* Osnovnata funkcya na programata. Pokzazva glavnoto menu na programata.
	                   Potrbitelyat tyaabva da vyvede nyakoya ot trite tochki na menuto. */
	char c;


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
