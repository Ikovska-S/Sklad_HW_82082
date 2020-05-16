
#include <iostream>  // fail v koito sa opisani vsi4ki funkcii i klasove za rabota s konzolata
#include <fstream>   // fail v koito sa opisani vsi4ki funkcii i klasove za s vunshni failove
#include <string>    // fail v koito sa opisani vsi4ki funkcii i klasove za rabota s simvolni nizove

using namespace std;
int clean_inv() {
		/*
		Publichna fukncia na klasa, koiato izpisva vsichki nalichnosti s iztekul srok na godnost ot sklada
		*/
		int i;
		time_t irecdate, iexpdate;
		string inp;
		item* pitem;

		irecdate = today();
		iexpdate = irecdate;

		//generirat se nalichnostite v sklada po artikul i skladovo miasto
		build_inv(MAX_ITEMS);
		//nalichnostite se sortirat po artikul i data na godnost
		sort(vinventory.begin(), vinventory.end(), sortby_item_expiredate);

		//na ekrana se izvezhdat vsichki nalichnosti s iztekul srok na godnost
		cout << endl << "The following items are expired:";
		for (i = 0; i < vinventory.size(); i++)
			if (vinventory[i].qty > 0 && vinventory[i].expdate <= iexpdate) {
				pitem = ic.get_item_by_index(vinventory[i].itemid);
				cout << endl << "Item:" << pitem->name << ", Warehouse Location:" << vinventory[i].wareloc << ", Expire Date:" << date_str(vinventory[i].expdate) << ", Quantity:" << vinventory[i].qty;
			}

		//potrebitelia trabva da potvurdi izpisvaneto
		cout << endl << "Are you sure you want to clear them? [Y/N]:";
		getline(cin, inp);

		//sled kato potrebitelia potvurdi spisuka s izteklite nalichnosti ot vektornia masiv vinventory
		//se izbrozhda i se zapisvat kato razhod v masiva records
		if (inp == "y" || inp == "Y") {
			for (i = 0; i < vinventory.size(); i++)
				if (vinventory[i].qty > 0 && vinventory[i].expdate <= iexpdate) {
					records[num].itemid = vinventory[i].itemid;
					records[num].recdate = irecdate;;
					records[num].expdate = vinventory[i].expdate;
					records[num].producer = "";
					records[num].wareloc = vinventory[i].wareloc;
					records[num].qty = -vinventory[i].qty;
					records[num].notes = "Automatic cleanup of expired items.";
					num++;
				}

		}
		return 0;
	}

	bool save_data() {
		/*
		Publichna funkcia na klasa records_col, koiato zapazva vsichki zapisi ot masiva records vuv vunshniq
		tekstovi fail records.txt. Ako vuznikne greshka vrushta false, inache vrushta true.
		*/
		char sep = '_';
		int i;
		ofstream fs;

		fs.open("records.txt");

		if (fs.fail()) return false;

		for (i = 0; i < num; i++)
			fs << records[i].itemid << sep << records[i].recdate << sep << records[i].expdate << sep << records[i].producer << sep << records[i].wareloc << sep << records[i].qty << sep << records[i].notes << endl;
		fs.close();
		return true;
	}

	bool load_data() {
		/*
		Publichna funkcia na klasa records_col, koiato zarezhda danni za artikulite ot vunshniq fail records.txt.
		Failut se iz4ita liniq po liniq i vsiaka linia se razbiva na sustavnite i poleta.
		Ako vuznikne greshka se izvezhda na ekrana tochno koia linia i pole sa greshni, sled koeto programata spira tai kato
		osnovniq fail records.txt e povreden. Vsichki poleta na zapisite za zaprhiozhdavania i izpisvania se proveriavat.
		Pri uspesh vrushta true inache false.
		*/
		int i, iitemid, iwareloc;
		float iqty;
		time_t irecdate, iexpdate;
		string line, inp, iproducer, inotes;
		ifstream fs;

		fs.open("records.txt");
		if (fs.fail()) return true;
		i = 0;
		while (!fs.eof()) {
			if (num >= MAX_RECS) {
				cout << endl << "Maximum number of records reached at line " << i << " in file records.txt!";
				return false;
			}

			i++;
			getline(fs, line);
			if (line == "") continue;
			istringstream ss(line);

			//proverka na indexa na artikula
			if (getline(ss, inp, '_')) {
				try {
					iitemid = stoi(inp);
				}
				catch (...) {
					cout << endl << "Invalid item id at line " << i << " in file records.txt!";
					return false;
				}

				if (iitemid >= ic.num) {
					cout << endl << "Invalid item id at line " << i << " in file records.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na datata zapisa
			if (getline(ss, inp, '_')) {
				try {
					irecdate = (time_t)stol(inp);
				}
				catch (...) {
					cout << endl << "Invalid record date at line " << i << " in file records.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na datata na godnost
			if (getline(ss, inp, '_')) {
				try {
					iexpdate = (time_t)stol(inp);
				}
				catch (...) {
					cout << endl << "Invalid expire date at line " << i << " in file records.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na ime na porizvoditel
			if (getline(ss, inp, '_')) {
				iproducer = inp;
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na skladovo miasto
			if (getline(ss, inp, '_')) {
				try {
					iwareloc = stoi(inp);
				}
				catch (...) {
					cout << endl << "Invalid warehouse location at line " << i << " in file records.txt!";
					return false;
				}

				if (iwareloc >= MAX_WARELOCS) {
					cout << endl << "Invalid warehouse location at line " << i << " in file records.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na kolichestvoto
			if (getline(ss, inp, '_')) {
				try {
					iqty = stof(inp);
				}
				catch (...) {
					cout << endl << "Invalid quantity at line " << i << " in file records.txt!";
					return false;
				}

				if (iqty == 0) {
					cout << endl << "Invalid quantity location at line " << i << " in file records.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			//proverka na dopulnitelnite zabelzhki kum zapisa
			if (getline(ss, inp, '_')) inotes = inp;
			else inotes = "";

			//liniata ot tekstovia fail records.txt e proverena i otdelntie poleta se zapisvat
			//v masiva records
			records[num].itemid = iitemid;
			records[num].recdate = irecdate;;
			records[num].expdate = iexpdate;
			records[num].producer = iproducer;
			records[num].wareloc = iwareloc;
			records[num].qty = iqty;
			records[num].notes = inotes;
			num++;
		}

		return true;
	}
};

//inicializira se obekt rc ot klas records_col
records_col rc(MAX_RECS);


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
			rc.print_inv();
			break;
		case '2':
			rc.add_inv();
			break;
		case '3':
			rc.remove_inv();
			break;
		case '4':
			rc.show_records();
			break;
		case '5':
			rc.clean_inv();
			break;
		}


	} while (c != '6');

	return 0;
}

int main() {
	/*
	Osnovnata funkcia na programata
	Pokzazva glavnoto menu na programata.
	Potrbiteliat triabva da vuvede niakoia ot trite tochki na menuto.
	*/
	char c;

	//izchitat se dannite ot vunshnia fail items.txt
	if (!ic.load_data()) {
		cout << endl << "Error during loading items.txt! Program stopping!";
		return 0;
	}

	//izchitat se dannite ot vunshnia fail records.txt	
	if (!rc.load_data()) {
		cout << endl << "Error during loading records.txt! Program stopping!";
		return 0;
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

	//zapisvat se dannite vuv vunshnia fail records.txt	
	if (!rc.save_data())
		cout << endl << "Error during saving data to records.txt!";

	//zapisvat se dannite vuv vunshnia fail items.txt
	if (!ic.save_data())
		cout << endl << "Error during saving data to items.txt!";

	return 0;
}