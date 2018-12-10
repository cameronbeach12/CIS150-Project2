#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

//struct that will control the settings of valid users
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
string login(vector<valid_user>);
void checkReserveDiffSeat(bool&);
int makeChoice();

/*
Author - Cameron Beach
Creation Date - 11/20/18
Modification Date - 12/8/18
Purpose - Airplane seat reservation system with altering sizes of planes
*/
int main() {
	vector<valid_user> users; // vector of valid users
	string current_user, chartName; //string of the current user and current chart name
	ifstream inFile; // this will capture the name of the chart file for us
	vector< vector<string> > chart; // vector of the seating chart
	vector<string> reserves; // vector of the current reserved seats
	int choice = -1; // This will store the users choice for what function to use

	// This will grab the chart name for us from the currFile.txt file
	inFile.open("currFile.txt");
	inFile >> chartName;

	// populate the users vector, login to the system, populate the (chart, reserves), and display the newly populated chart respectively
	populateUsers(users);
	current_user = login(users);
	populateChart(chart, reserves, chartName);
	displayChart(chart);

	// executes until the user chooses to quit (option 7), prompts the user for what they want to do and the system will perform their choice
	while (choice != 7) {
		choice = makeChoice();

		cout << endl; //create some space - for design

		if (choice == 1) {
			displayChart(chart); // displays the chart
		}
		else if (choice == 2) {
			reserveSeat(chart, reserves); // reserves a seat
		}
		else if (choice == 3) {
			cancelReserve(chart, reserves); // cancels a seat reservation
		}
		else if (choice == 4) {
			writeNewFile(chart, chartName); // writes the chart to the system
		}
		else if (choice == 5) {
			stats(chart, reserves); // outputs the stats and writes to the stats file
		}
		else if (choice == 6) {
			help(); // outputs the help menu
		}
		else if (choice != 7) {
			cout << "Please input a valid choice." << endl << endl; // outputs if the user gives invalid information
		}
	}

	// executes when the program quits
	cout << "Have a nice day!" << endl;

	system("pause");
	return 0;
}

/*
Author - Cameron Beach
Creation Date - 11/21/18
Modification Date - 11/22/18
Purpose - Populate the users vector
Pre-condition - Must get a vector of valid users passed by address
Post-condition - the vector of valid users will be populated from the systemUser.txt file
*/
void populateUsers(vector<valid_user> &users) {
	ifstream ins; // input file stream
	int count = 0; // will be used to figure out what user we are currently populating

	ins.open("systemUser.txt"); // grab the file of user information

	while (!ins.eof()) {
		// populate the users vector
		users.push_back(valid_user());

		ins >> users[count].id >> users[count].first_name >> users[count].last_name >> users[count].username >> users[count].password;

		count++; // increment the count variable
	}
}

/*
Author - Cameron Beach
Creation Date - 11/22/18
Modification Date - 11/22/18
Purpose - Populate the seating chart
Pre-condition - Must get a vector of vectors of strings passed by reference, must get a vector of strings passed by reference, must get the chart file name passed by value
Post-condition - The two vectors passed in will be populated by reading the file name provided by chartName
*/
void populateChart(vector< vector<string> > &chart, vector<string> &reserves, string chartName) {
	string curr_ele = "1", tmp; // curr_ele holds the current seat value IE. 1A, tmp holds a temporary line
	int cols = 0, rows = 0; // cols holds the number of columns in the file, rows hold the number of rows in the file
	ifstream ins; //input file that will read in the chart

	ins.open(chartName); //open chart file

	//Read each element up to a whitespace in column 1
	//49 is ASCII code for '1'
	while ((int)curr_ele[0] == 49) {
		ins >> curr_ele; // grab the next element while the first character of the current element is 1

		cols++;
	}

	cols--; //while loop above will count one extra row;

	ins.clear();
	ins.seekg(0, ins.beg); //Sets the file pointer back to the beginning of the file

	//count rows
	for (int i = 0; !ins.eof(); i++) {
		getline(ins, tmp); //get the next line 

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
				reserves.push_back(to_string((i + 1)) + char(j + 65)); // if the seat is reserved, add it to our reserves vector
			}
		}
	}
	ins.close(); // close the file
}

/*
Author - Cameron Beach
Creation Date - 11/22/18
Modification Date - 11/22/18
Purpose - Display the seating chart
Pre-condition - Must get a vector of vectors of strings passed by value
Post-condition - The seating chart will be displayed
*/
void displayChart(vector< vector<string> > chart) {
	for (int i = 0; i < chart.size(); i++) { // rows
		for (int j = 0; j < chart[i].size(); j++) { // cols
			cout << chart[i][j] << " ";
		}
		cout << endl;
	}
}

/*
Author - Cameron Beach
Creation Date - 11/23/18
Modification Date - 12/09/18
Purpose - Reserve a seat
Pre-condition - Must get a vector of vectors of strings passed by reference, must get a vector of strings passed by reference
Post-condition - The seat selected is reserved, or the user will choose not to reserve a seat in which case no seats are reserved
*/
void reserveSeat(vector< vector<string> > &chart, vector<string> &reserves) {
	int row, col, testMid; // row and col will hold their respective row and column value, testMid will hold the middle value of the column EX 8 cols / 2 = 4;
	string seatType; // holds the seat type that the user is trying to reserve
	char colChar; // gets the column character initially, later converted into the integer value row (declared above)
	bool choice = false; // will control the while loop
	char confirm; // will control areas where we need user confirmation

	while (choice == false) {
		displayChart(chart); // displays the current seating chart to show the available seats

		cout << "Seat to reserve: "; 
		cin >> row >> colChar; // get input for the row and column char

		colChar = toupper(colChar); // convert colChar to uppercase

		col = int(colChar) - 65; // set the column to the ascii code of colChar - 65, which will get us the index column number
		row -= 1; // gets the index row number

		//checks the ranges of the chart to make sure we are not outside the plane seating boundaries
		if (row > chart.size() - 1 || col > chart[row].size() - 1 || row < 0 || col < 0){
			cout << "That seat is not available on this plane" << endl;

			checkReserveDiffSeat(choice); // checks to see if they want to try reserving a different seat
		} // checks if the seat is already reserved
		else if (chart[row][col] == to_string(row + 1) + "~") {
			cout << "That seat is already reserved" << endl;

			checkReserveDiffSeat(choice);
		}
		else {
			// gets the mid point of the chart row sizes
			testMid = chart[row].size() / 2;

			// if row sizes are even...
			if (chart[row].size() % 2 == 0) {
				// if index col + 1 (because testMid will be one more that we want) is testMid or index col + 1 is testMid + 1
				if (col + 1 == testMid || col + 1 == testMid + 1) {
					seatType = "Aisle";
				}
			} // if row sizes are odd...
			else if (chart[row].size() % 2 == 1) {
				// if the seat if the middle 3 seats
				if (col == testMid || col == testMid - 1 || col == testMid + 1) {
					seatType = "Aisle";
				}
			}
			
			// if the seats are on the edges...
			if (col == 0 || col == chart[row].size() - 1) {
				seatType = "Window";
			} // if we haven't assigns a seat type yet...
			else if (seatType != "Aisle" && seatType != "Window") {
				seatType = "Center";
			}

			// gets user confirmation
			cout << "Would you like to reserve seat " << chart[row][col] << " - " << seatType << " (y/n)? ";
			cin >> confirm;

			confirm = toupper(confirm);

			// if user confirms choice...
			if (confirm == 'Y') {
				cout << "Successfully reserved seat " << chart[row][col] << " - " << seatType << endl << endl;

				reserves.push_back(chart[row][col]); // add the seat to the reserves vector

				chart[row][col] = to_string(row + 1) + "~"; // set the seat to the reserved format

				choice = true; // user has made their choice
			}
			else {
				checkReserveDiffSeat(choice);
			}
		}
	}
	displayChart(chart); // display the new chart
}

/*
Author - Cameron Beach
Creation Date - 11/23/18
Modification Date - 12/09/18
Purpose - Cancel a seat reservation
Pre-condition - Must get a vector of vectors of strings passed by reference, must get a vector of strings passed by reference
Post-condition - The seat selected is cancelled, if no seat is selected then no seat is cancelled
*/
void cancelReserve(vector< vector<string> > &chart, vector<string> &reserves) {
	string userCancel, rowString; // userCancel will hold the value of the seat the user wants to cancel, rowString will hold the row number as a string
	char confirm; // used for user confirmation purposes
	int row, col; // holds the index row and index column
	stringstream toInt; // string stream to convert our rowString to the int row
	bool choiceMade = false; // choiceMade will control our while loop

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
				// convert the row letter to uppercase
				for (int j = 0; j < reserves[i].size(); j++) {
					if (j != reserves[i].size() - 1) {
						continue;
					}
					else {
						userCancel[j] = toupper(userCancel[j]); // convert the row letter to uppercase
					}
				}
			}
			else { // if there is not more than 1 character the seat is invalid, so this message is output
				cout << "That is not a valid seat";
				//Check if they want to try and cancel a different seat
				choiceMade = checkCancel(choiceMade);

				break;
			}

			// If we found the seat in the reserves vector...
			if (userCancel == reserves[i]) {
				cout << "Would you like to cancel reservation on seat " << userCancel << " (y/n): ";
				cin >> confirm; // get confirmation to cancel the reservation

				confirm = toupper(confirm); // convert to uppercase

				if (confirm == 'Y') { // this will execute if the user confirms the cancellation
					cout << "Reservation cancellation on seat " << userCancel << " was successful!" << endl << endl;
					
					//Get the row and column to be replaced in the array
					for (int j = 0; j < reserves[i].size(); j++) {
						if (j != reserves[i].size() - 1) {
							rowString += reserves[i][j];
						}
						else {
							col = reserves[i][j] - 65;
						}
					}
					toInt << rowString; // the rowString into the output stream of toInt (string stream)

					toInt >> row; // assign row to rowString (because rowString is the only thing in the output stream of toInt)

					row--; // get the index row

					chart[row][col] = userCancel; // put the value back in the chart

					//Remove the element from the reserves vector
					reserves.erase(reserves.begin() + i);

					choiceMade = true;

					break;
				}
				else {
					choiceMade = checkCancel(choiceMade); // checks if the user wants to cancel a different seat

					break;
				}
			} // outputs if the user input a seat that hasn't been reserved yet
			else if (i == reserves.size() - 1) {
				cout << "That seat hasn't been reserved";

				choiceMade = checkCancel(choiceMade);

				break;
			}
		}
	}

	displayChart(chart); // displays the new chart
}

/*
Author - Cameron Beach
Creation Date - 12/03/18
Modification Date - 12/05/18
Purpose - Write the chart to a file of the users choice, replaces the old chart file
Pre-condition - Must get a vector of vectors of strings passed by value, must get a string holding the chart file name passed by reference
Post-condition - The new file replaces the old chart file and saves changes
*/
void writeNewFile(vector< vector<string> > chart, string &chartFile) {
	string newFileName, tmp = chartFile; // newFileName will hold the name of the new chart file, tmp holds the current name of the chart file
	ofstream changeInFile; // changeInFile will write the new chart file name to our currFile.txt file

	cout << "File name of new chart file (WITH .txt EXTENSION): ";
	cin >> newFileName;

	chartFile = newFileName;

	rename(tmp.c_str(), chartFile.c_str()); //rename function only takes type cstring

	changeInFile.open("currFile.txt");
	changeInFile << chartFile;
	changeInFile.close();

	cout << "File " << chartFile << " has successfully been written" << endl << endl; // lets us know that the file has successfully been written
}

/*
Author - Cameron Beach
Creation Date - 12/05/18
Modification Date - 12/05/18
Purpose - Calculates the stats
Pre-condition - Must get a vector of vectors of strings passed by value, must get a vector of strings passed by reference
Post-condition - outputs and calculates the statistics of the current seating chart
*/
void stats(vector< vector<string> > chart, vector<string> reserves) {
	float percentAvail = ((chart.size() * chart[0].size()) - reserves.size()) / float(chart.size() * chart[0].size()); // calculates percent of available seats
	float percentReserved = reserves.size() / float(chart.size() * chart[0].size()); // calculates percent of reserved seats
	int avail = (chart.size() * chart[0].size()) - reserves.size(), reserved = reserves.size(); // calculates the amount of available seats and reserved seats respectively
	ofstream outs; // will write to the statistics.txt file

	outs.open("statistics.txt"); // open the statistics.txt file

	outs << "Number of Available Seats: " << avail << endl << "Number of Reserved Seats: " << reserved << endl;
	cout << "Number of Available Seats: " << avail << endl << "Number of Reserved Seats: " << reserved << endl; // write and output the # of avail seats and # of reserved seats
	outs << "Percentage of Available Seats: " << percentAvail * 100 << endl << "Percentage of Reserved Seats: " << percentReserved * 100 << endl;
	cout << "Percentage of Available Seats: " << percentAvail * 100 << endl << "Percentage of Reserved Seats: " << percentReserved * 100 << endl; // write and output the % of avail seats and % of reserved seats

	outs << "List of Available Seats: ";
	cout << "List of Available Seats: ";
	for (int i = 0; i < chart.size(); i++) {
		for (int j = 0; j < chart[i].size(); j++) {
			if (chart[i][j] == to_string((i + 1)) + "~") {
				continue;
			}
			else {
				outs << chart[i][j] << ", ";
				cout << chart[i][j] << ", "; // write and output list of available seats
			}
		}
	}
	outs << endl;
	cout << endl;

	outs << "List of Reserved Seats: ";
	cout << "List of Reserved Seats: ";
	for (int i = 0; i < reserves.size(); i++) {
		outs << reserves[i] << ", ";
		cout << reserves[i] << ", "; // write and output list of reserved seats
	}

	cout << endl << endl;
	outs.close(); // close the statistics.txt file
}

/*
Author - Cameron Beach
Creation Date - 12/05/18
Modification Date - 12/05/18
Purpose - Help menu
Pre-condition - no pre conditions
Post-condition - outputs the help menu
*/
void help() { // help menu for each functionality the system has
	cout << endl << "1. Display Chart - Displays the updated chart of reserved and available seats." << endl;
	cout << "2. Reserve Seat - Displays the updated chart of reserved and available seats, prompts for a valid seat selection." << endl << "Input the row number and column letter.";
	cout << "The system will tell you whether the seat is still available, if it is, the system will prompt for confirmation on your choice" << endl;
	cout << "3. Cancel Reservation - Displays the updated chart of reserved and available seats, prompts for a valid seat selection." << endl << "Input the row number and column letter of the seat you would like cancelled.";
	cout << endl << "If the seat is able to be cancelled, the system will prompt for confirmation." << endl;
	cout << "4. Save Seat Chart to File - Saves the seat chart to the file with a name of your choosing. Replaces the original file." << endl;
	cout << "5. Statistics - Saves the statistics of the system to a file name 'statistics'." << endl << endl;
}

/*
Author - Cameron Beach
Creation Date - 12/05/18
Modification Date - 12/05/18
Purpose - Checks if the user wants to cancel a different reserved seat or continue with a different seat
Pre-condition - must get a bool check value passed in
Post-condition - returns the users choice
*/
bool checkCancel(bool check) {
	char userChoice; // will check if the user wants to cancel a reservation

	cout << endl << "Would you like to cancel a different seat (y/n)? ";
	cin >> userChoice; // get a yes or no input from the user

	userChoice = toupper(userChoice); // convert to uppercase

	if (userChoice == 'Y') { // if the user enters yes...
		return false;
	}
	else { // otherwise...
		return true;
	}
}

/*
Author - Cameron Beach
Creation Date - 11/22/18
Modification Date - 11/22/18
Purpose - Log in to the system
Pre-condition - Must get a vector of valid users passed by value
Post-condition - The current user will be returned
*/
string login(vector<valid_user> users) {
	string userInput; // username input
	string passInput; // password input
	bool logged_in = false; // check if the user has successfully logged in.

	cout << "==========================================" << endl;
	cout << "       WELCOME TO AIR TICKET SYSTEM       " << endl;
	cout << "==========================================" << endl << endl;

	while (!logged_in) {
		cout << "Username: ";
		cin >> userInput; // get username input

		cout << "Password: ";
		cin >> passInput; // get password input

		for (int i = 0; i < users.size(); i++) {
			if (users[i].username == userInput) { // if we found the username
				if (users[i].password == passInput) { // check if the password is valid
					cout << "Successfully logged in as " << users[i].username << endl;
					cout << "==========================================" << endl;
					logged_in = true; // if it is then log in
					return users[i].username;
				}
				else {
					cout << "Incorrect username or password" << endl; // if not warn the user
					break;
				}
			}
			else if (i == users.size() - 1) {
				cout << "Incorrect username or password" << endl; // if username not found, warn the user.
			}
		}
	}
}

/*
Author - Cameron Beach
Creation Date - 12/05/18
Modification Date - 12/05/18
Purpose - Checks if the user wants to reserve a different seat or to exit the reservation menu
Pre-condition - must get a bool check value passed in by reference
Post-condition - changes the value passed in to the users choice
*/
void checkReserveDiffSeat(bool &choice) {
	char confirm; // used for user confirmation
	
	cout << "Would you like to reserve a different seat(y/n)? ";
	cin >> confirm; // gets input for user confirmation

	confirm = toupper(confirm); // converts to uppercase

	if (confirm == 'Y') { // if the user answers yes...
		choice = false;
		}
	else { // otherwise...
		choice = true;
	}
}

/*
Author - Cameron Beach
Creation Date - 11/22/18
Modification Date - 11/22/18
Purpose - Gets the user input for their choice
Pre-condition - no pre conditions
Post-condition - returns the users choice
*/
int makeChoice() {
	int choice; // controls the user choice

	// output the menu
	cout << "1. Display Chart" << endl;
	cout << "2. Reserve Seat" << endl;
	cout << "3. Cancel Reservation" << endl;
	cout << "4. Save Seat Chart to File" << endl;
	cout << "5. Statistics" << endl;
	cout << "6. Help" << endl;
	cout << "7. Quit" << endl << endl;

	cout << "What would you like to do? ";
	cin >> choice; // get the user choice

	return choice;
}