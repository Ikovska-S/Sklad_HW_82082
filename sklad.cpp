#include <iostream>
using namaspece std;
#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream> 
#include <fstream>   
#include <string>    
#include <ctime>    
#include <iomanip>   
#include <vector>    
#include <algorithm>  


bool IsLeapYear(int year)
{
	
	if (year % 4 != 0) return false;
	if (year % 400 == 0) return true;
	if (year % 100 == 0) return false;
	return true;
}

time_t build_expdate(time_t irecdate, int experiod) {
	
	struct tm ti;
	int d, m, y;
	int daysInMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	date_info(irecdate, &ti);

	d = ti.tm_mday;
	m = ti.tm_mon;
	y = ti.tm_year;

	y = y + experiod / 12;
	m = m + experiod % 12;

	if (m > 11)
	{
		y += 1;
		m -= 12;
	}

	int days = daysInMonths[m];

	if (m == 1 && IsLeapYear(y)) days += 1;

	if (d > days) {
		m++;
		if (m > 11) m = 0;
		d = d - days;
	}

	ti.tm_mday = d;
	ti.tm_mon = m;
	ti.tm_year = y;
	return mktime(&ti);
}


struct item {
	string name;
	string um;
	int defexperiod;
};


class items_col {

	
private:
	item* items; 


public:
	int cap; 
	int num; 

	items_col(int pcap) {
		
		cap = pcap;
		num = 0;
		items = new item[cap]; 
	}

	~items_col() {
		

		
		delete[] items;
	}

	int input_item() {
		
		string inp, iname, ium;
		int i, iexp;
		bool repeat;
		string um[] = { "pcs","kg","g","m","cm" };

		if (num >= cap) {
			cout << endl << "Maximum number of items already entered!";
			return 0;
		}

		repeat = true;

		do {
			cout << endl << "Item name[. cancel, < back]: ";
			getline(cin, inp);

			if (inp == ".") return 0;
			else if (inp == "<") break;
			else if (inp == "") {
				cout << endl << "Item name cannot be empty!";
				continue;
			}

			for (i = 0; i < num; i++)
				if (items[i].name == inp)
					break;

			if (i < num) {
				cout << endl << "Item name already exist!";
				continue;
			}

			iname = inp;

			
			do {
				cout << endl << "Unit of measurement[. cancel, < back]: ";
				getline(cin, inp);

				if (inp == ".") return 0;
				else if (inp == "<") break;

				for (i = 0; i < sizeof(um) / sizeof(um[0]); i++) if (inp == um[i]) break;
				if (i >= sizeof(um) / sizeof(um[0])) {
					cout << endl << "Wrong unit of measurement!";
					continue;
				}

				ium = um[i];

				
				do {
					cout << endl << "Default expire period in months[. cancel, < back]: ";
					getline(cin, inp);

					if (inp == ".") return 0;
					else if (inp == "<") break;

					try {
						iexp = stoi(inp);
					}
					catch (...) {
						cout << endl << "Invalid default expire period!";
						continue;
					}

					
					items[num].name = iname;
					items[num].um = ium;
					items[num].defexperiod = iexp;

					cout << endl << "Index: " << num << ", Item name: " << iname << ", Unit of measurement: " << ium << ", Expire period in months: " << iexp;
					cout << endl << "Successfully saved!";

					num++;

					getline(cin, inp);
					repeat = false;

				} while (repeat);
			} while (repeat);
		} while (repeat);

		return 0;

	}

	int edit_item() {
	
		bool repeat;
		int i, iitemid, iexp;
		string inp, iname, ium;
		string um[] = { "pcs","kg","g","m","cm" };

		repeat = true;

		
		do {
			cout << endl << "Enter item index to edit[. cancel, < back, ? list of items]: ";
			getline(cin, inp);

			if (inp == ".") return 0;
			else if (inp == "<") break;
			else if (inp == "?") {
				show_items();
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

				if (iitemid >= num) {
					cout << endl << "Invalid item index!";
					continue;
				}
			}

							
			do {
				cout << endl << "Enter new item name[. cancel, < back, ENTER remains " << items[iitemid].name << "]: ";
				getline(cin, inp);

				if (inp == ".") return 0;
				else if (inp == "<") break;
				else if (inp == "") iname = items[iitemid].name;
				else {
					for (i = 0; i < num; i++)
						if (items[i].name == inp)
							break;

					if (i < num) {
						cout << endl << "Item name already exist!";
						continue;
					}

					iname = inp;
				}
				
				do {
					cout << endl << "Enter new unit of measurement[. cancel, < back, ENTER remains " << items[iitemid].um << "]: ";
					getline(cin, inp);

					if (inp == ".") return 0;
					else if (inp == "<") break;
					else if (inp == "") ium = items[iitemid].um;
					else {
						for (i = 0; i < sizeof(um) / sizeof(um[0]); i++) if (inp == um[i]) break;
						if (i >= sizeof(um) / sizeof(um[0])) {
							cout << endl << "Wrong unit of measurement!";
							continue;
						}
						ium = inp;
					}

					 
					do {
						cout << endl << "Enter new default expire period[. cancel, < back, ENTER remains " << items[iitemid].defexperiod << " months]: ";
						getline(cin, inp);

						if (inp == ".") return 0;
						else if (inp == "<") break;
						else if (inp == "") iexp = items[iitemid].defexperiod;
						else {
							try {
								iexp = stoi(inp);
							}
							catch (...) {
								cout << endl << "Invalid default expire period!";
								continue;
							}
						}

						
						items[iitemid].name = iname;
						items[iitemid].um = ium;
						items[iitemid].defexperiod = iexp;

						cout << endl << "Index:" << iitemid << ",Item name:" << iname << ",Unit of measurement:" << ium << ",Expire period in months:" << iexp;
						cout << endl << "Successfully updated!";

						getline(cin, inp);

						repeat = false;

					} while (repeat);
				} while (repeat);
			} while (repeat);
		} while (repeat);

		return 0;
	}

	int show_items() {
		


		cout << endl << "List of items:";

		
		for (int i = 0; i < num; i++)
			cout << endl << "Index:" << i << ",Item name:" << items[i].name << ",Unit of measurement:" << items[i].um << ",Default expire period:" << items[i].defexperiod << " months";

		return 0;

	}

	item* get_item_by_index(int idx) {
		blichna funkcia na klasa items_col, koiato vrushta ukazatel kum artikul s index raven na parametura idx
		
		return &items[idx];
	}

	bool save_data() {
*/

		char sep = '_';
		int i;
		ofstream fs;

		
		fs.open("items.txt");

		if (fs.fail()) return false;

		for (i = 0; i < num; i++)
			fs << i << sep << items[i].name << sep << items[i].um << sep << items[i].defexperiod << endl;
		fs.close();
		return true;
	}

	bool load_data() {
		
		int i, j, iitemid, iexp;
		string line, inp, iname, ium;
		string um[] = { "pcs","kg","g","m","cm" };
		ifstream fs;

		
		fs.open("items.txt");
		if (fs.fail()) return true;
		i = 0;
		while (!fs.eof()) {
			if (num >= MAX_ITEMS) {
				cout << endl << "Maximum number of items reached at line " << i << " in file items.txt!";
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
					cout << endl << "Invalid item id at line " << i << " in file items.txt!";
					return false;
				}
				if (iitemid != num) {
					cout << endl << "Invalid item id at line " << i << " in file items.txt! Found " << iitemid << " expected " << num;
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file items.txt!";
				return false;
			}

			//proverka na imeto na artikula
			if (getline(ss, inp, '_')) {
				if (inp == "") {
					cout << endl << "Invalid item name at line " << i << " in file items.txt!";
					return false;
				}

				for (j = 0; j < num; j++)
					if (inp == items[j].name)
						break;

				if (j < 0) {
					cout << endl << "Duplicate item name at line " << i << " in file items.txt!";
					return false;
				}

				iname = inp;
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file items.txt!";
				return false;
			}

			
			if (getline(ss, inp, '_')) {
				if (inp == "") {
					cout << endl << "Invalid item unit of measurement at line " << i << " in file items.txt!";
					return false;
				}

				for (j = 0; j < sizeof(um) / sizeof(um[0]); j++)
					if (inp == um[j])
						break;

				if (j >= sizeof(um) / sizeof(um[0])) {
					cout << endl << "Invalid item unit of measurement at line " << i << " in file items.txt!";
					return false;
				}

				ium = inp;
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file items.txt!";
				return false;
			}

			
			if (getline(ss, inp, '_')) {
				try {
					iexp = stoi(inp);
				}
				catch (...) {
					cout << endl << "Invalid default expire period at line " << i << " in file items.txt!";
					return false;
				}
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file items.txt!";
				return false;
			}

			//sled kato poletata sa provereni se zapisvat v masiva items
			items[num].name = iname;
			items[num].um = ium;
			items[num].defexperiod = iexp;
			num++;
		}

		fs.close();
		return true;
	}


};

//inicializira se obekt ic ot klas items_col
items_col ic(MAX_ITEMS);

//struktura koiato sudurzha dannite za otdelnite zaprihozhdavania i izpisvania ot sklada
//dannite vkliuchvat itemid - indeks na artikul s dannite se svurzvat s drugiat osnoven masiv items
//recdate - data na zapisa
//expdate - data na godnost na artikula
//producer - proizvoditel na artikula
//wareloc - skladovo miasto
//qty - kolichestvo ot artikula koeto e zaprihodeno ili izpisano saotvetno mozh da bude polozhitelno ili tricatelno
//notes - zabelezhki kym zapisa
struct record {
	int itemid;
	time_t recdate;
	time_t expdate;
	string producer;
	int wareloc;
	float qty;
	string notes;
};


struct inventory {
	int itemid;
	int wareloc;
	time_t expdate;
	float qty;
};


bool sortby_item_expiredate(inventory a, inventory b)
{
	if (a.itemid == b.itemid) return a.expdate < b.expdate;
	else return a.itemid < b.itemid;
}

/
class records_col {
	
private:
	
	record* records;

	
	vector<inventory> vinventory;

	int build_inv(int itemid) {
		
		int i, j;
		inventory rinventory;

		vinventory.clear();

		for (i = 0; i < num; i++) {
			if (itemid < MAX_ITEMS && itemid != records[i].itemid) continue;

			rinventory.itemid = records[i].itemid;
			rinventory.expdate = records[i].expdate;
			rinventory.wareloc = records[i].wareloc;
			rinventory.qty = records[i].qty;


			for (j = 0; j < vinventory.size(); j++) {
				if (rinventory.itemid < vinventory[j].itemid) break;
				if (rinventory.itemid == vinventory[j].itemid && rinventory.wareloc <= vinventory[j].wareloc) break;
			}

			if (j < vinventory.size()) {
				if (rinventory.itemid == vinventory[j].itemid && rinventory.wareloc == vinventory[j].wareloc)
					vinventory[j].qty += rinventory.qty;
				else
					vinventory.insert(vinventory.begin() + j, rinventory);

			}
			else
				vinventory.push_back(rinventory);

		}
		return 0;
	}

		
public:
	
	int cap;
	
	int num;

	
	records_col(int pcap) {
		cap = pcap;
		num = 0;
		records = new record[cap];
	}

	
	~records_col() {
		delete[] records;
	}

	int print_inv() {
		
		int i;
		item* pitem;

		build_inv(MAX_ITEMS);

		cout << endl << "Warehouse inventory:";
		for (i = 0; i < vinventory.size(); i++)
			if (vinventory[i].qty != 0) {
				pitem = ic.get_item_by_index(vinventory[i].itemid);
				cout << endl << "Item:" << pitem->name << ", Warehouse Location:" << vinventory[i].wareloc << ", Expire Date:" << date_str(vinventory[i].expdate) << ", Quantity:" << vinventory[i].qty;
			}

		return 0;
	}

	int add_inv() {
		
		item* pitem;
		bool repeat, warelocavail;
		int i, j, iitemid, iexperiod, iwareloc, idefwareloc;
		time_t irecdate, iexpdate;
		string inp, iproducer, inotes;
		float iqty;

		
		if (num >= cap) {
			cout << endl << "Maximum number of records already entered!";
			return 0;
		}

		repeat = true;
		
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
				
					cout << endl << "Enter producer name[. cancel, < back]: ";
					getline(cin, inp);

					if (inp == ".") return 0;
					else if (inp == "<") break;
					else iproducer = inp;

					
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
		
		item* pitem;
		bool repeat;
		int i, iitemid, iwareloc, iunloadid;
		time_t irecdate;
		string inp, iproducer, inotes;
		float iqty;

		
		if (num >= cap) {
			cout << endl << "Maximum number of records already entered!";
			return 0;
		}

		repeat = true;
					
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

			/
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

				
					do {
						cout << endl << "Enter additional notes for the record[. cancel, < back]: ";
						getline(cin, inp);

						if (inp == ".") return 0;
						else if (inp == "<") break;
						else inotes = inp;

					
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
		
		int i;
		time_t istart, iend;
		struct tm ti_start, ti_end;
		item* pitem;
		string inp;
		bool repeat;

		
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
	
		int i;
		time_t irecdate, iexpdate;
		string inp;
		item* pitem;

		irecdate = today();
		iexpdate = irecdate;

		
		build_inv(MAX_ITEMS);
		
		sort(vinventory.begin(), vinventory.end(), sortby_item_expiredate);
		cout << endl << "The following items are expired:";
		for (i = 0; i < vinventory.size(); i++)
			if (vinventory[i].qty > 0 && vinventory[i].expdate <= iexpdate) {
				pitem = ic.get_item_by_index(vinventory[i].itemid);
				cout << endl << "Item:" << pitem->name << ", Warehouse Location:" << vinventory[i].wareloc << ", Expire Date:" << date_str(vinventory[i].expdate) << ", Quantity:" << vinventory[i].qty;
			}

		cout << endl << "Are you sure you want to clear them? [Y/N]:";
		getline(cin, inp);

		
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

			
			if (getline(ss, inp, '_')) {
				iproducer = inp;
			}
			else {
				cout << endl << "Wrong number of fields at line " << i << " in file records.txt!";
				return false;
			}

			
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

			
			if (getline(ss, inp, '_')) inotes = inp;
			else inotes = "";

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


records_col rc(MAX_RECS);

int records_menu() {
	
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

int items_menu() {
	
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
			ic.input_item();
			break;
		case '2':
			ic.show_items();
			break;
		case '3':
			ic.edit_item();
			break;
		}

	} while (c != '4');

	return 0;
}

int main() {
	
	char c;

	if (!ic.load_data()) {
		cout << endl << "Error during loading items.txt! Program stopping!";
		return 0;
	}

	
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

	if (!rc.save_data())
		cout << endl << "Error during saving data to records.txt!";


	if (!ic.save_data())
		cout << endl << "Error during saving data to items.txt!";

	return 0;
}		
