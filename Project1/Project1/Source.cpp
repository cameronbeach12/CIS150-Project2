#include <iostream>
#include <string>
#include <vector>
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
void populateChart(vector< vector<string> >&);
void displayChart(vector < vector<string> >);
void reserveSeat(vector < vector<string> >&);
string login(vector<valid_user>&);
void checkReserveDiffSeat(bool&);
int makeChoice();

int main() {
	vector<valid_user> users;
	string current_user;
	vector< vector<string> > chart;
	int choice = -1;

	populateUsers(users);
	current_user = login(users);
	populateChart(chart);
	displayChart(chart);

	while (choice != 7) {
		choice = makeChoice();

		cout << endl; //create some space - for design

		if (choice == 1) {
			displayChart(chart);
		}
		else if (choice == 2) {
			reserveSeat(chart);
		}
	}

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

void populateChart(vector< vector<string> > &chart) {
	string curr_ele = "1", tmp;
	int cols = 0, rows = 0;
	ifstream ins;

	ins.open("chartIn.txt"); //open chartIn.txt

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

void reserveSeat(vector< vector<string> > &chart) {
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
		else if (chart[row][col] == "X") {
			cout << "That seat is already reserved" << endl;

			checkReserveDiffSeat(choice);
		}
		else {
			cout << "Would you like to reserve seat " << chart[row][col] << " (y/n)? ";
			cin >> confirm;

			confirm = toupper(confirm);

			if (confirm == 'Y') {
				cout << "Successfully reserved seat " << chart[row][col] << endl;

				chart[row][col] = "X";
				choice = true;
			}
			else {
				checkReserveDiffSeat(choice);
			}
		}
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