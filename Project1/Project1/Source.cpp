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
	ifstream ins;
	string line, curr_element;
	int line_number = 0, element_num = 0;

	ins.open("chartIn.txt");
	//Get the number of rows on the plane
	while (!ins.eof()) {
		getline(ins, line);

		line_number++;
	}

	//since each line is the same length, and I know there are 3 chars for each column except the last
	//where there are 2, I added one to make up for the missing char at the end and divided by 3
	//because 3 is the amount of chars each column takes up.
	element_num = (line.size() + 1) / 3;

	//resize the amount of rows to the line_number
	chart.resize(line_number);

	//send the files point back to the beginning
	ins.clear();
	ins.seekg(0, ins.beg); //the first parameter represents the offset
						   //the second parameter represents where we want to go in our file.

	//populate the chart
	for (int i = 0; i < chart.size(); i++) {
		chart[i].resize(element_num);

		for (int j = 0; j < chart[i].size(); j++) {
			ins >> chart[i][j];
			cout << chart[i][j] << " ";
		}

		cout << endl;
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
					logged_in == true;
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