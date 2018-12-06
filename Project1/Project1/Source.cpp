#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>

using namespace std;

struct valid_user {
	int id;
	string first_name;
	string last_name;
	string username;
	string password;
};

void populateUsers(vector<valid_user>&);
void populateChart(vector< vector<string> >&, vector<string>&, string);
void displayChart(vector< vector<string> >);
void reserveSeat(vector< vector<string> >&, vector<string>&);
void cancelReserve(vector< vector<string> >&, vector<string>&);
void writeAtQuit(vector< vector<string> >);
void writeNewFile(vector< vector<string> >, string&);
void stats(vector< vector<string> >, vector<string>);
void help();
bool checkCancel(bool);
string login(vector<valid_user>&);
void checkReserveDiffSeat(bool&);
int makeChoice();

int main() {
	vector<valid_user> users;
	string current_user, chartName;
	ifstream inFile;
	vector< vector<string> > chart;
	vector<string> reserves;
	int choice = -1;

	inFile.open("currFile.txt");
	inFile >> chartName;

	populateUsers(users);
	current_user = login(users);
	populateChart(chart, reserves, chartName);
	displayChart(chart);

	while (choice != 7) {
		choice = makeChoice();

		cout << endl; //create some space - for design

		if (choice == 1) {
			displayChart(chart);
		}
		else if (choice == 2) {
			reserveSeat(chart, reserves);
		}
		else if (choice == 3) {
			cancelReserve(chart, reserves);
		}
		else if (choice == 4) {
			writeNewFile(chart, chartName);
		}
		else if (choice == 5) {
			stats(chart, reserves);
		}
		else if (choice == 6) {
			help();
		}
	}

	cout << "Have a nice day!" << endl;

	system("pause");
	return 0;
}

void populateUsers(vector<valid_user> &users) {
	ifstream ins;
	int count = 0;

	ins.open("systemUser.txt");

	while (!ins.eof()) {
		users.push_back(valid_user());

		ins >> users[count].id >> users[count].first_name >> users[count].last_name >> users[count].username >> users[count].password;

		count++;
	}
}

void populateChart(vector< vector<string> > &chart, vector<string> &reserves, string chartName) {
	string curr_ele = "1", tmp;
	int cols = 0, rows = 0;
	ifstream ins;

	ins.open(chartName); //open chart file

	//Read each element up to a whitespace in column 1
	//49 is ASCII code for '1'
	while ((int)curr_ele[0] == 49) {
		ins >> curr_ele;

		cols++;
	}

	cols--; //while loop will count one extra row;
	
	ins.clear();
	ins.seekg(0, ins.beg); //Sets the file pointer back to the beginning of the file

	//count rows
	for (int i = 0; !ins.eof(); i++) {
		getline(ins, tmp);

		rows++;
	}

	chart.resize(rows); //Resize the chart to match the amount of rows we have
	
	ins.clear();
	ins.seekg(0, ins.beg); //Sets the file pointer back to the beginning of the file
	
	//assign the cols and rows values;
	for (int i = 0; i < chart.size(); i++) {
		chart[i].resize(cols); //Resize the amount of columns in each row
		for (int j = 0; j < chart[i].size(); j++) {
			ins >> chart[i][j]; //Reads each element from the chartIn file based on row and col

			if (chart[i][j] == to_string((i + 1)) + "~") {
				reserves.push_back(to_string((i + 1)) + char(j + 65));
			}
		}
	}
	ins.close();
}

void displayChart(vector< vector<string> > chart) {
	for (int i = 0; i < chart.size(); i++) {
		for (int j = 0; j < chart[i].size(); j++) {
			cout << chart[i][j] << " ";
		}
		cout << endl;
	}
}

void reserveSeat(vector< vector<string> > &chart, vector<string> &reserves) {
	int row, col;
	char colChar;
	bool choice = false;
	char confirm;

	while (choice == false) {
		displayChart(chart);

		cout << "Seat to reserve: ";
		cin >> row >> colChar;

		colChar = toupper(colChar);

		col = int(colChar) - 65;
		row -= 1;

		if (row > chart.size() - 1 || col > chart[row].size() - 1 || row < 0 || col < 0){
			cout << "That seat is not available on this plane" << endl;

			checkReserveDiffSeat(choice);
		}
		else if (chart[row][col] == "1~") {
			cout << "That seat is already reserved" << endl;

			checkReserveDiffSeat(choice);
		}
		else {
			cout << "Would you like to reserve seat " << chart[row][col] << " (y/n)? ";
			cin >> confirm;

			confirm = toupper(confirm);

			if (confirm == 'Y') {
				cout << "Successfully reserved seat " << chart[row][col] << endl << endl;

				reserves.push_back(chart[row][col]);

				chart[row][col] = to_string(row + 1) + "~";

				choice = true;
			}
			else {
				checkReserveDiffSeat(choice);
			}
		}
	}
	displayChart(chart);
}

void cancelReserve(vector< vector<string> > &chart, vector<string> &reserves) {
	string userCancel;
	char confirm;
	int row, col;
	bool choiceMade = false, seatFound = false;

	displayChart(chart);

	cout << endl << "Reserved seats: ";

	//Print out the reserved seats
	for (int i = 0; i < reserves.size(); i++) {
		if (i != reserves.size() - 1) {
			cout << reserves[i] << ", ";
		}
		else {
			cout << reserves[i] << endl << endl;
		}
	}

	while (!choiceMade) {
		cout << "What seat would you like to cancel? ";
		cin >> userCancel;

		for (int i = 0; i < reserves.size(); i++) {
			if (userCancel.size() > 1) {
				userCancel[1] = toupper(userCancel[1]);
			}
			else {
				cout << "That is not a valid seat";
				//Check if they want to try and cancel a different seat
				choiceMade = checkCancel(choiceMade);

				break;
			}

			if (userCancel == reserves[i]) {
				cout << "Would you like to cancel reservation on seat " << userCancel << " (y/n): ";
				cin >> confirm;

				confirm = toupper(confirm);

				if (confirm == 'Y') {
					cout << "Reservation on seat " << userCancel << " was successful!" << endl << endl;
					
					//Get the row and column to be replaced in the array
					row = reserves[i][0] - 49;
					col = reserves[i][1] - 65;

					chart[row][col] = userCancel;

					//Remove the element from the reserves vector
					reserves.erase(reserves.begin() + i);

					choiceMade = checkCancel(choiceMade);

					break;
				}
				else {
					choiceMade = checkCancel(choiceMade);

					break;
				}
			}
			else if (i == reserves.size() - 1) {
				cout << "That seat hasn't been reserved";

				choiceMade = checkCancel(choiceMade);

				break;
			}
		}
	}

	displayChart(chart);
}

void writeNewFile(vector< vector<string> > chart, string &chartFile) {
	string newFileName, tmp = chartFile;
	ofstream outs, changeInFile;

	cout << "File name of new chart file: ";
	cin >> newFileName;

	chartFile = newFileName;

	rename(tmp.c_str(), chartFile.c_str()); //rename function only takes type cstring

	outs.open(chartFile);
	for (int i = 0; i < chart.size(); i++) {
		for (int j = 0; j < chart[i].size(); j++) {
			outs << chart[i][j] << "\t";
		}
		if (i != chart.size() - 1) {
			outs << endl;
		}
		else {
			continue;
		}
	}
	outs.close();

	changeInFile.open("currFile.txt");
	changeInFile << chartFile;
	changeInFile.close();

	cout << "File " << chartFile << " has successfully been written" << endl << endl;
}

void stats(vector< vector<string> > chart, vector<string> reserves) {
	float percentAvail = ((chart.size() * chart[0].size()) - reserves.size()) / float(chart.size() * chart[0].size());
	float percentReserved = reserves.size() / float(chart.size() * chart[0].size());
	int avail = (chart.size() * chart[0].size()) - reserves.size(), reserved = reserves.size();
	ofstream outs;

	outs.open("statistics.txt");

	outs << "Number of Available Seats: " << avail << endl << "Number of Reserved Seats: " << reserved << endl;
	cout << "Number of Available Seats: " << avail << endl << "Number of Reserved Seats: " << reserved << endl;
	outs << "Percentage of Available Seats: " << percentAvail * 100 << endl << "Percentage of Reserved Seats: " << percentReserved * 100 << endl;
	cout << "Percentage of Available Seats: " << percentAvail * 100 << endl << "Percentage of Reserved Seats: " << percentReserved * 100 << endl;

	outs << "List of Available Seats: ";
	cout << "List of Available Seats: ";
	for (int i = 0; i < chart.size(); i++) {
		for (int j = 0; j < chart[i].size(); j++) {
			if (chart[i][j] == to_string((i + 1)) + "~") {
				continue;
			}
			else {
				outs << chart[i][j] << ", ";
				cout << chart[i][j] << ", ";
			}
		}
	}
	outs << endl;
	cout << endl;

	outs << "List of Reserved Seats: ";
	cout << "List of Reserved Seats: ";
	for (int i = 0; i < reserves.size(); i++) {
		outs << reserves[i] << ", ";
		cout << reserves[i] << ", ";
	}

	cout << endl << endl;
	outs.close();
}

void help() {
	cout << endl << "1. Display Chart - Displays the updated chart of reserved and available seats." << endl;
	cout << "2. Reserve Seat - Displays the updated chart of reserved and available seats, prompts for a valid seat selection." << endl << "Input the row number and column letter.";
	cout << "The system will tell you whether the seat is still available, if it is, the system will prompt for confirmation on your choice" << endl;
	cout << "3. Cancel Reservation - Displays the updated chart of reserved and available seats, prompts for a valid seat selection." << endl << "Input the row number and column letter of the seat you would like cancelled.";
	cout << endl << "If the seat is able to be cancelled, the system will prompt for confirmation." << endl;
	cout << "4. Save Seat Chart to File - Saves the seat chart to the file with a name of your choosing. Replaces the original file." << endl;
	cout << "5. Statistics - Saves the statistics of the system to a file name 'statistics'." << endl << endl;
}

bool checkCancel(bool check) {
	char userChoice;

	cout << endl << "Would you like to cancel a different seat (y/n)? ";
	cin >> userChoice;

	userChoice = toupper(userChoice);

	if (userChoice == 'Y') {
		return false;
	}
	else {
		return true;
	}
}

string login(vector<valid_user> &users) {
	string userInput;
	string passInput;
	bool logged_in = false;

	cout << "==========================================" << endl;
	cout << "       WELCOME TO AIR TICKET SYSTEM       " << endl;
	cout << "==========================================" << endl << endl;

	while (!logged_in) {
		cout << "Username: ";
		cin >> userInput;

		cout << "Password: ";
		cin >> passInput;

		for (int i = 0; i < users.size(); i++) {
			if (users[i].username == userInput) {
				if (users[i].password == passInput) {
					cout << "Successfully logged in as " << users[i].username << endl;
					cout << "==========================================" << endl;
					logged_in = true;
					return users[i].username;
				}
				else {
					cout << "Incorrect username or password" << endl;
					break;
				}
			}
			else if (i == users.size() - 1) {
				cout << "Incorrect username or password" << endl;
			}
		}
	}
}

void checkReserveDiffSeat(bool &choice) {
	char confirm;
	
	cout << "Would you like to reserve a different seat(y/n)? ";
	cin >> confirm;

	confirm = toupper(confirm);

	if (confirm == 'Y') {
		choice = false;
		}
	else {
		choice = true;
	}
}

int makeChoice() {
	int choice;

	cout << "1. Display Chart" << endl;
	cout << "2. Reserve Seat" << endl;
	cout << "3. Cancel Reservation" << endl;
	cout << "4. Save Seat Chart to File" << endl;
	cout << "5. Statistics" << endl;
	cout << "6. Help" << endl;
	cout << "7. Quit" << endl << endl;

	cout << "What would you like to do? ";
	cin >> choice;

	return choice;
}