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
string login(vector<valid_user>&);
void populateChart(vector< vector<string> >&);

int main() {
	vector<valid_user> users;
	string current_user;
	vector< vector<string> > chart;

	populateUsers(users);
	current_user = login(users);
	populateChart(chart);

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

	ins.open("chartIn.txt");

	//49 is ASCII code for '1'
	while ((int)curr_ele[0] == 49) {
		ins >> curr_ele;

		cols++;
	}

	cols -= 1; //while loop will count one extra row;
	
	ins.clear();
	ins.seekg(0, ins.beg);

	//count rows
	for (int i = 0; !ins.eof(); i++) {
		getline(ins, tmp);

		rows++;
	}

	chart.resize(rows);
	
	ins.clear();
	ins.seekg(0, ins.beg);
	
	//assign the cols and rows values;
	for (int i = 0; i < chart.size(); i++) {
		chart[i].resize(cols);
		for (int j = 0; j < chart[i].size(); j++) {
			ins >> chart[i][j];
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