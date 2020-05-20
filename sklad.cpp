#include <iostream>
using namaspece std;
#define _CRT_SECURE_NO_WARNINGS

//vkliuchvame slednite dopulnitelni header failove
#include <sstream>
#include <iostream>  // fail v koito sa opisani vsi4ki funkcii i klasove za rabota s konzolata
#include <fstream>   // fail v koito sa opisani vsi4ki funkcii i klasove za s vunshni failove
#include <string>    // fail v koito sa opisani vsi4ki funkcii i klasove za rabota s simvolni nizove
#include <ctime>     // fail v koito sa opisani vsi4ki funkcii i klasove za obrabotka na data i vreme
#include <iomanip>   // izpolzvame samo edna funkcia get_time ot tozi header fail, za vuvezhdane na data ot potrebitelq
#include <vector>    // fail v koito sa opisani vsi4ki funkcii i klasove za rabota s vector klasa
#include <algorithm> // izpolzvame samo edna funkcia sort ot tozi header fail, za sortirane na vector masiv po opredeleno pole

int add_inv() {
		/*
		Publichna fukncia na klasa koiato dobavia nov zapis za zaprihozhdavane na artikuli v sklada.
		*/
		item* pitem;
		bool repeat, warelocavail;
		int i, j, iitemid, iexperiod, iwareloc, idefwareloc;
		time_t irecdate, iexpdate;
		string inp, iproducer, inotes;
		float iqty;

		//proverka dali veche ne dostignat maksimalnia broi zapisi opredelen ot konstantata MAX_RECS
		if (num >= cap) {
			cout << endl << "Maximum number of records already entered!";
			return 0;
		}

		repeat = true;
		//vuvezhdane ot potrebitelia i proverka na indeks na artikul
		//potrebitelia mozhe da vidi spisak na vuvedenite artikuli s ?
		do {
			cout << endl << "Enter item index[. cancel, < back, ? list of items]: ";
			getline(cin, inp);

			if (inp == ".") return 0;
			else if (inp == "<") break;
			else if (inp == "") continue;
			else if (inp == "?") {
				ic.show_items();
				continue;
			}
			else {
				try {
					iitemid = stoi(inp);
				}
				catch (...) {
					cout << endl << "Invalid item index!";
					continue;
				}

				if (iitemid >= ic.num) {
					cout << endl << "Invalid item index!";
					continue;
				}
			}

			build_inv(iitemid);
			pitem = ic.get_item_by_index(iitemid);
			cout << endl << "Item: " << pitem->name << " selected.";
			irecdate = today();
			//vuvezhdane ot potrebitelia i proverka na srok na godnost
			//po podrazbirane se predlaga sroka na godnost vuveden za dadenia artikul
			do {
				cout << endl << "Enter expire period in months[. cancel, < back, <ENTER> default " << pitem->defexperiod << " months]: ";
				getline(cin, inp);

				if (inp == ".") return 0;
				else if (inp == "<") break;
				else if (inp == "") iexperiod = pitem->defexperiod;
				else {
					try {
						iexperiod = stoi(inp);
					}
					catch (...) {
						cout << endl << "Invalid expire period!";
						continue;
					}
				}

				iexpdate = build_expdate(irecdate, iexperiod);
				//vuvezhdane ot potrebitelia i proverka na ime na proizvoditel
				do {
					cout << endl << "Enter producer name[. cancel, < back]: ";
					getline(cin, inp);

					if (inp == ".") return 0;
					else if (inp == "<") break;
					else iproducer = inp;

					//vuvezhdane ot potrebitelia i proverka na kolichestvo za zaprihozhdavane
					do {
						cout << endl << "Enter quantity to add[. cancel, < back]: ";
						getline(cin, inp);

						if (inp == ".") return 0;
						else if (inp == "<") break;
						else {
							try {
								iqty = stof(inp);
							}
							catch (...) {
								cout << endl << "Invalid quantity!";
								continue;
							}

							if (iqty <= 0) {
								cout << endl << "Quantity cannot be <=0.00!";
								continue;
							}
						}

						for (i = 0; i < MAX_WARELOCS; i++) {
							warelocavail = true;
							for (j = 0; j < vinventory.size(); j++) {
								if (vinventory[j].expdate == iexpdate) break;
								if (vinventory[j].wareloc == i) warelocavail = false;
							}
							if (j < vinventory.size()) {
								idefwareloc = vinventory[j].wareloc;
								break;
							}
							else if (warelocavail) {
								idefwareloc = i;
								break;
							}
						}

						if (i >= MAX_WARELOCS) {
							cout << endl << "There is no available warehouse location to load this item!";
							continue;
						}

						//vuvezhdane ot potrebitelia i proverka na skladovo miasto
						//na ekrana se izvezhdat vskichki skladovi lokacii s podobna data na godnost
						//po podrazbirane se predlaga skladovo miasto s nai nisuk indeks
						do {
							cout << endl << "Available warehouse locations with same expiration date:";
							for (i = 0; i < vinventory.size(); i++)
								if (vinventory[j].expdate == iexpdate)
									cout << endl << "Warehouse Location:" << vinventory[i].wareloc << ", Expiration date:" << date_str(vinventory[i].expdate) << ", Quantity:" << vinventory[i].qty;


							cout << endl << "Enter warehouse location[. cancel, < back, ENTER for avail. loc. " << idefwareloc << "]: ";
							getline(cin, inp);

							if (inp == ".") return 0;
							else if (inp == "<") break;
							else if (inp == "") iwareloc = idefwareloc;
							else {
								try {
									iwareloc = stoi(inp);
								}
								catch (...) {
									cout << endl << "Invalid warehouse location!";
									continue;
								}

								if (iwareloc < 0 || iwareloc >= MAX_WARELOCS) {
									cout << endl << "Invalid warehouse location!";
									continue;
								}

								for (i = 0; i < vinventory.size(); i++)
									if (vinventory[i].wareloc == iwareloc && vinventory[i].expdate != iexpdate)
										break;

								if (i < vinventory.size()) {
									cout << endl << "Same item with different expire date already present at this location!";
									continue;
								}
							}
							//vuvezhdane ot potrebitelia i proverka na dopulnitelni zabelezhki kum zapisa
							do {
								cout << endl << "Enter additional notes for the record[. cancel, < back]: ";
								getline(cin, inp);

								if (inp == ".") return 0;
								else if (inp == "<") break;
								else inotes = inp;

								//sled kato vsichki parametri na zapisa sa vuvedeni i provereni
								//te se zapisvat kato novo zaprihozhdavane v masiva s skladovi zapisi records;
								//potrebitelia se informira za uspeshno napravenia zapis
								records[num].itemid = iitemid;
								records[num].recdate = irecdate;
								records[num].expdate = iexpdate;
								records[num].producer = iproducer;
								records[num].wareloc = iwareloc;
								records[num].qty = iqty;
								records[num].notes = inotes;
								num++;

								cout << endl << "Item: " << pitem->name << ", Record Date: " << date_str(irecdate) << ", Expire Date: " << date_str(iexpdate);
								cout << endl << "Producer: " << iproducer << ", Ware. Loc.: " << iwareloc << ", Quantity: " << iqty;
								cout << endl << "Notes: " << inotes;
								cout << endl << "Successfully added to warehouse!";

								getline(cin, inp);

								repeat = false;
							} while (repeat);
						} while (repeat);
					} while (repeat);
				} while (repeat);
			} while (repeat);
		} while (repeat);

		return 0;
	}

	int remove_inv() {
		/*
		Publichna fukncia na klasa koiato dobavia nov zapis za izpisvane na artikuli ot sklada.
		*/
		item* pitem;
		bool repeat;
		int i, iitemid, iwareloc, iunloadid;
		time_t irecdate;
		string inp, iproducer, inotes;
		float iqty;

		//proverka dali veche ne dostignat maksimalnia broi zapisi opredelen ot konstantata MAX_RECS
		if (num >= cap) {
			cout << endl << "Maximum number of records already entered!";
			return 0;
		}

		repeat = true;
		//vuvezhdane ot potrebitelia i proverka na indeks na artikul
		//potrebitelia mozhe da vidi spisak na vuvedenite artikuli s ?			
		do {
			cout << endl << "Enter item index[. cancel, < back, ? list of items]: ";
			getline(cin, inp);

			if (inp == ".") return 0;
			else if (inp == "<") break;
			else if (inp == "") continue;
			else if (inp == "?") {
				ic.show_items();
				continue;
			}
			else {
				try {
					iitemid = stoi(inp);
				}
				catch (...) {
					cout << endl << "Invalid item index!";
					continue;
				}

				if (iitemid >= ic.num) {
					cout << endl << "Invalid item index!";
					continue;
				}
			}

			pitem = ic.get_item_by_index(iitemid);
			cout << endl << "Item: " << pitem->name << " selected.";
			build_inv(iitemid);
			sort(vinventory.begin(), vinventory.end(), sortby_item_expiredate);
			if (vinventory.size() == 0) {
				cout << endl << "There is no available quantity for this item into the warehouse!";
				break;
			}

			irecdate = today();

			//vuvezhdane ot potrebitelia i proverka na skladovo miasto
			//na ekrana se izvezhdat vskichki skladovi lokacii s polozhitelni nalichnosti
			//po podrazbirane se predlaga skladovo miasto s nalichnost chiato data na godnost izticha nai skoro
			do {
				cout << endl << "Available quantities in different warehouse locations:";
				for (i = 0; i < vinventory.size(); i++)
					cout << endl << "Warehouse Location:" << vinventory[i].wareloc << ", Expiration date:" << date_str(vinventory[i].expdate) << ", Quantity:" << vinventory[i].qty;
				iunloadid = 0;
				cout << endl << "Select warehouse location from the list[. cancel, < back, ENTER default " << vinventory[iunloadid].wareloc << "]: ";
				getline(cin, inp);

				if (inp == ".") return 0;
				else if (inp == "<") break;
				else if (inp == "") {}
				else {
					try {
						iwareloc = stoi(inp);
					}
					catch (...) {
						cout << endl << "Invalid warehouse location!";
						continue;
					}

					if (iwareloc < 0 || iwareloc >= MAX_WARELOCS) {
						cout << endl << "Invalid warehouse location!";
						continue;
					}

					for (i = 0; i < vinventory.size(); i++)
						if (iwareloc == vinventory[i].wareloc)
							break;

					if (i >= vinventory.size()) {
						cout << endl << "Warehouse location not into the list of available locations!";
						continue;
					}

					iunloadid = i;
				}
				//vuvezhdane ot potrebitelia i proverka na kolichestvo za izpisvane, to ne triabva da e po goliamo ot 
				//nalichnoto v momenta na goreopredelenoto skladovo miasto
				do {
					cout << endl << "Enter quantity to remove from location[. cancel, < back]: ";
					getline(cin, inp);

					if (inp == ".") return 0;
					else if (inp == "<") break;
					else {
						try {
							iqty = stof(inp);
						}
						catch (...) {
							cout << endl << "Invalid quantity!";
							continue;
						}

						if (iqty <= 0) {
							cout << endl << "Quantity cannot be <=0.00!";
							continue;
						}

						if (iqty > vinventory[iunloadid].qty) {
							cout << endl << "Quantity cannot be greater than location quantity[" << vinventory[iunloadid].qty << "]!";
							continue;
						}
					}

					//vuvezhdane ot potrebitelia i proverka na dopulnitelni zabelezhki kum zapisa
					do {
						cout << endl << "Enter additional notes for the record[. cancel, < back]: ";
						getline(cin, inp);

						if (inp == ".") return 0;
						else if (inp == "<") break;
						else inotes = inp;

						//sled kato vsichki parametri na zapisa sa vuvedeni i provereni
						//te se zapisvat kato novo izpisvane v masiva s skladovi zapisi records;
						//potrebitelia se informira za uspeshno napravenia zapis
						records[num].itemid = iitemid;
						records[num].recdate = irecdate;
						records[num].expdate = vinventory[iunloadid].expdate;
						records[num].producer = "";
						records[num].wareloc = vinventory[iunloadid].wareloc;
						records[num].qty = -iqty;
						records[num].notes = inotes;
						num++;

						cout << endl << "Item: " << pitem->name << ", Record Date: " << date_str(irecdate) << ", Expire Date: " << date_str(vinventory[iunloadid].expdate);
						cout << endl << "Producer: , Ware. Loc.: " << vinventory[iunloadid].wareloc << ", Quantity: " << iqty;
						cout << endl << "Notes: " << inotes;
						cout << endl << "Successfully removed from warehouse!";

						getline(cin, inp);

						repeat = false;

					} while (repeat);
				} while (repeat);
			} while (repeat);
		} while (repeat);

		return 0;
	}

	int show_records() {
		/*
		Publichna fukncia na klasa koiato pokzava vsizhki zapisi ot masiva records ,koito imat data
		na zapis v period s nachalna i kraina data opredeleni ot potrebitelia.
		*/
		int i;
		time_t istart, iend;
		struct tm ti_start, ti_end;
		item* pitem;
		string inp;
		bool repeat;

		//vuvezhdane i proverka na nachalnata data na perioda spravkata
		do {
			istart = time(0);
			date_info(istart, &ti_start);

			cout << endl << "Enter start record date[. cancel, < back, ENTER for " << date_str(istart) << "]:";
			getline(cin, inp);

			if (inp == ".") return 0;
			else if (inp == "<") break;
			else if (inp == "") istart = mktime(&ti_start);
			else {
				if (inp.size() != 10) {
					cout << endl << "Invalid start record date!";
					continue;
				}

				istringstream ss(inp);

				ss >> get_time(&ti_start, "%Y-%m-%d");
				if (ss.fail()) {
					cout << endl << "Invalid start record date!";
					continue;
				}

				istart = mktime(&ti_start);
			}

			//vuvezhdane i proverka na krainata data na perioda spravkata				
			do {
				iend = time(0);
				date_info(iend, &ti_end);

				cout << endl << "Enter end record date[. cancel, < back, ENTER for " << date_str(iend) << "]:";
				getline(cin, inp);

				if (inp == ".") return 0;
				else if (inp == "<") break;
				else if (inp == "") iend = mktime(&ti_end);
				else {
					if (inp.size() != 10) {
						cout << endl << "Invalid end record date!";
						continue;
					}

					istringstream ss(inp);

					ss >> get_time(&ti_end, "%Y-%m-%d");
					if (ss.fail()) {
						cout << endl << "Invalid end record date!";
						continue;
					}

					iend = mktime(&ti_end);

					if (iend < istart) {
						cout << endl << "End Record Date cannot be earlier than Start Record Date!";
						continue;
					}
				}

				//na ekrana se izvezhdat vsichki zapisi v tozi period
				cout << endl << "Records list from:" << date_str(istart) << " to:" << date_str(iend);
				cout << endl << "Legend:I-Item, RD-Record Date, ED-Expire Date, L-warehouse Location, Q-Quantity, P-Producer, N-Notes" << endl;
				for (i = 0; i < num; i++) {
					if (records[i].recdate<istart || records[i].recdate>iend) continue;

					pitem = ic.get_item_by_index(records[i].itemid);
					cout << endl << "I:" << pitem->name << ",RD:" << date_str(records[i].recdate) << ",ED:" << date_str(records[i].expdate);
					cout << ",L:" << records[i].wareloc << ",Q:" << records[i].qty << ",P:" << records[i].producer << ",N:" << records[i].notes;
				}

				getline(cin, inp);

				repeat = false;
			} while (repeat);
		} while (repeat);



		return 0;
	}

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
