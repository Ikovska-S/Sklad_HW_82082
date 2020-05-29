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

using namespace std; //zadavame std kato rabotno prostransvo po podrazbirane

#define MAX_ITEMS 100    //konstanta koiato opredelq maksimalniq dopustim broj artikuli
#define MAX_WARELOCS 100 //konstanta koiato opredelq maksimalniq dopustim broj skladovi mesta 
#define MAX_RECS 1000    //konstanta koiato opredelq maksimalniq dopustim broj zapisi na zaprihozdavani i izpisvaniq v sklada 

string date_str(time_t t) {
	/*
	Spomagatelna fukciq za preobrazuvane na predadenata data t v format time_t
	kum tekstova data v format "yyyy-mm-dd".
	*/
	struct tm* ti;
	char buffer[11];

	ti = localtime(&t);
	strftime(buffer, 11, "%Y-%m-%d", ti);
	string s(buffer);

	return s;
}

int date_info(time_t t, struct tm* ti) {
	/*
	Spomagatelna fukciq za preobrazuvane na data sudurzhashta se v time_t promenliva t
	kum struktura v format tm. Tai kato funkciata vrushta samo datata, infromaciata za
	chasa se nulira.

	struct tm {
		tm_sec	int	seconds after the minute	0-61*
		tm_min	int	minutes after the hour		0-59
		tm_hour	int	hours since midnight		0-23
		tm_mday	int	day of the month			1-31
		tm_mon	int	months since January		0-11
		tm_year	int	years since 1900
		tm_wday	int	days since Sunday			0-6
		tm_yday	int	days since January 1		0-365
		tm_isdst	int	Daylight Saving Time flag
	};

	*/
	struct tm* tip;

	tip = localtime(&t);

	ti->tm_sec = 0;
	ti->tm_min = 0;
	ti->tm_hour = 0;
	ti->tm_mday = tip->tm_mday;
	ti->tm_mon = tip->tm_mon;
	ti->tm_year = tip->tm_year;
	ti->tm_wday = tip->tm_wday;
	ti->tm_yday = tip->tm_yday;
	ti->tm_isdst = 0;

	return 0;
}

time_t today() {
	/*
	spomagatelna fukciq, koiato vruzhta dneshna data v format time_t. Tai kato funkciata vrushta
	samo datata, infromaciata za chasa se nulira.
	*/
	time_t t;
	struct tm* tip, ti;
	time(&t);
	tip = localtime(&t);

	ti.tm_sec = 0;
	ti.tm_min = 0;
	ti.tm_hour = 0;
	ti.tm_mday = tip->tm_mday;
	ti.tm_mon = tip->tm_mon;
	ti.tm_year = tip->tm_year;
	ti.tm_wday = tip->tm_wday;
	ti.tm_yday = tip->tm_yday;
	ti.tm_isdst = 0;

	return mktime(&ti);
}


bool IsLeapYear(int year)
{
	/*
	spomagatelna fukciq, koiato opredelia predadenata kato parametar godina dali e
	visokosna ili ne
	*/
	if (year % 4 != 0) return false;
	if (year % 400 == 0) return true;
	if (year % 100 == 0) return false;
	return true;
}

time_t build_expdate(time_t irecdate, int experiod) {
	/*
	spomagatelna fukciq, koiato dobavia opredelen broi meseci predadeni v parametura experiod
	kum datata irecdate v format time_t i vrushta izchislenata data v format time_t. Fukciata se
	izpolzva za izchisliavane na sroka na godnost na daden artikul postupil v sklada.
	*/
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

/*
Struktura sudurzhashta danni za vseki edin artikul v sklada. V sluchaia se
zapisvat naimenovanie, merna edinica i srok na godnost po podrazbirane
na artikula.
*/
struct item {
	string name;
	string um;
	int defexperiod;
};

/*
Klasut items_col predostavia razlichni funkcii za upravlenie(dobaviane, redaktirane, printirane)
na vsi4ki artikuli na sklada.
*/
class items_col {

	//chastni funkcii i promenlivi
private:
	item* items; //ukazatel kum masiv ot strukturi ot tip item

//publichni funkcii i promenlivi
public:
	int cap; //promenliva tip int, koiato sudurzha maksimalniq broi elementi na masiva items
	int num; //promenliva tip int, koiato sudurzha tekushtiq broi elementi na masiva items

	items_col(int pcap) {
		/*
		konstruktor na klasut items_col. Ima edin parametur pcap koito opredelia razmera na masiva ot item
		koito shte se inicializira v pametta
		*/

		//inicializirat se vsichki promenlivi
		cap = pcap;
		num = 0;
		items = new item[cap]; //inicializira se masiv ot cap elementa tip item v pameta na programata
							   //i se prisvoiavat na ukazatelia items
	}

	~items_col() {
		/*
		destruktor na klasut items_col
		*/

		//masivat items se iztriva ot pametta na programata
		delete[] items;
	}

	int input_item() {
		/*
		Publichna funkcia na klasa items_col, chrez koiato potrebitelia vuvezhda parametrite na nov
		artikul. Vseki parametur se proveriava za validnost.
		*/

		//inicializirane na promenlivite
		string inp, iname, ium;
		int i, iexp;
		bool repeat;
		string um[] = { "pcs","kg","g","m","cm" };

		if (num >= cap) {
			cout << endl << "Maximum number of items already entered!";
			return 0;
		}

		repeat = true;

		//purvo se vuvezhda naimenovanie na novia artikul, kato to triabva da e unikalno
		//potrebitelia mozhe da vuvede . za otkaz ot procedurata ili < za vrushtane stupka nazad
		//imeto na artikula se sravniava sus vsichki veche vuvdeni artikuli v masiva items
		//i ako se otkrie artikul sus sushtoto ime se izvezhda greshka
		//potrebitelia vuvezhda danni dokato ne vuvede pravilno ime ili se otkazhe
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

			//sled tova potrbiteliat triabva da vuvede merna edinica na artikula
			//mernata edinica triabva da suvpada s niakoi ot predvaritelno izbroenite merni edinici v
			//masiva um[]
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

				//posledno potrebiteliat vuvezhda srok na godnost po podrazbirane v meseci
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

					//nakraia na funkciata dannite se zapazvat v masiva items i se izvezhda saobshtenie za uspeshen zapis
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
		/*
		Publichna funkcia na klasa items_col, chrez koiato potrebitelia redaktira opredelen ot nego
		artikul. Vseki parametur se proveriava za validnost. Pri vuvezhdane na prazen niz ili klavish enter
		dadeniq parametur se zapazva.
		*/

		bool repeat;
		int i, iitemid, iexp;
		string inp, iname, ium;
		string um[] = { "pcs","kg","g","m","cm" };

		repeat = true;

		// purvo se iziskva ot potrebitelia da vuvede index na artikul koito shte se redaktira
		// potrebitelia mozhe da vidi vsichi artikuli kato vuvede ?
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

			//sled kato e opredelen artikulat ot potrebitelia se iska da vuvede novoto ime				
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
				//potrebitelia triabva da vuvede nova merna edinica za artikula 
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

					//potrebitelia triabva da vuvede nov srok na godnost na artikula 
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

						//sled kato vsichki novi parametri na artikula sa opredeleni to se obnoviava v masiva items
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
		/*
		Publichna funkcia na klasa items_col, chrez koiato se izvezhda spisuk na vsichki vuvedeni artikuli zaedno s tehnite parametri
		*/


		cout << endl << "List of items:";

		//izbrozhda se vuvedenite elementi na masiva items
		for (int i = 0; i < num; i++)
			cout << endl << "Index:" << i << ",Item name:" << items[i].name << ",Unit of measurement:" << items[i].um << ",Default expire period:" << items[i].defexperiod << " months";

		return 0;

	}

	item* get_item_by_index(int idx) {
		/*
		Publichna funkcia na klasa items_col, koiato vrushta ukazatel kum artikul s index raven na parametura idx
		*/
		return &items[idx];
	}

	bool save_data() {
		/*
		Publichna funkcia na klasa items_col, koiato zapazva vsichki vuvedeni artikuli ot masiva items vuv vunshniq
		tekstovi fail items.txt. Ako vuznikne greshka vrushta false, inache vrushta true.
		*/

		char sep = '_';
		int i;
		ofstream fs;

		//otvariane na faila items.txt za zapis
		fs.open("items.txt");

		if (fs.fail()) return false;

		for (i = 0; i < num; i++)
			fs << i << sep << items[i].name << sep << items[i].um << sep << items[i].defexperiod << endl;
		fs.close();
		return true;
	}

	bool load_data() {
		/*
		Publichna funkcia na klasa items_col, koiato zarezhda danni za artikulite ot vunshniq fail items.txt.
		Failut se iz4ita liniq po liniq i vsiaka linia se razbiva na sustavnite i poleta.
		Ako vuznikne greshka se izvezhda na ekrana tochno koia linia i pole sa greshni, sled koeto programata spira tai kato
		osnovniq fail items.txt e povreden. Vsichki poleta na artikulite se proveriavat. Pri uspesh vrushta true inache false.
		*/
		int i, j, iitemid, iexp;
		string line, inp, iname, ium;
		string um[] = { "pcs","kg","g","m","cm" };
		ifstream fs;

		//otvariane na faila items.txt za chetene
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

			//proverka na mernata edinica
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

			//proverka na sroka na gosnost po podrazbirane
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

//spogatelna struktura inventory v koiato se grupira informacia za nalichnostite v sklada
//itemid - index na artikul
//wareloc - skladovo miasto
//expdate - data na godnost na artikula
//wareloc - sumarno kolichestvo za dadenia artikul i skladovo miasto
struct inventory {
	int itemid;
	int wareloc;
	time_t expdate;
	float qty;
};

//spomagatelna funkcia koeto se izpolzva ot fumckiata sort za sortirane na vektornia masiv sudurzhasht nalichnostite v sklada
//v sluchaia dannite se sortirat po indeks na artikul i data na godnost v vuzhodiash red
bool sortby_item_expiredate(inventory a, inventory b)
{
	if (a.itemid == b.itemid) return a.expdate < b.expdate;
	else return a.itemid < b.itemid;
}

//vtoria osnoven klas v programata records_col, koito sudurzha funkcii za upravlenieto 
//na zapisite za zaprihozhdavania i izpisvania ot sklada
class records_col {
	//chastni funkcii i promenlivi v klasa
private:
	//masiv ot goreopisanata struktura record, tuk se zapisvat cialata hronologia na dvizheniata v sklada
	record* records;

	//spomagatelen vektoren masiv koito se generira na baza vsichki zapisi ot records
	vector<inventory> vinventory;

	int build_inv(int itemid) {
		/*
		Spomagatelna fukncia chastna za klasa s koiato se generira vektorniq masiv vinventory.
		V nego se izchisliavat vsichki nalichnosti ili za opredelen artikul v sklada, kato dannite se
		bazirat na hronologichnite zapisi na prihod i razhod v masiva records.
		Nalichnostite se grupirat po indeks na artikul i skladovo miasto.
		*/
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

	//publichni funkcii i promenlivi v klasa	
public:
	//maksimalniqt broi zapisi za dvizhenia v sklada
	int cap;
	//tekushtiq broi zapisi za dvizhenia v sklada
	int num;

	//konstruktor na klasa,v koito se generira masiva records sudurzhasht vsichkite zapisi na sklada
	records_col(int pcap) {
		cap = pcap;
		num = 0;
		records = new record[cap];
	}

	//destruktor na klasa, koito osvobozhdava ot pameta masiva s zapisite records
	~records_col() {
		delete[] records;
	}

	int print_inv() {
		/*
		Publichna fukncia na klasa koiato izvezhda danni za nalichnostite v sklada.
		Za celta se izpolzva chastnata spomagatelna funkcia na klasa build_inv s parametur vsichki artikuli
		*/
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
		Publichna funkcia na klasa records_col, koiato zarezhda danni za zapisite v sklada ot vunshniq fail records.txt.
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

