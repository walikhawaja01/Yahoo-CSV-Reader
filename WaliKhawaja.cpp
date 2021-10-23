// Wali Khawaja Yahoo CSV Project
// Sources used: Cracking the Coding Interview for sorting algorithm implementation, Professor C-Harris Notes for computing file name.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;
// Data structure.
struct dataToRead {
	string date;
	double open=0;
	double adjClose=0;
	double percentChange=0;
};

// Used to compute file name.
void Compute_1_Filename(string& filename) {

	WIN32_FIND_DATAA findfiledata;
	HANDLE hfind;
	string file_extension = "*.csv";
	string directory_file_ext = file_extension;
	hfind = FindFirstFileA(directory_file_ext.c_str(), &findfiledata);

	if (hfind != INVALID_HANDLE_VALUE)
		filename = findfiledata.cFileName;
	else
		cout << "No file was present" << endl;
}

//Prints the headers. 
void printHeaders() {
	
	cout << endl;
	cout << setw(15) << left << "Date" << setw(9) << left << "Open" << setw(9) << left << "A.Close  " << setw(8) << left << "Percent Change" << endl;
}

//Outputs data after headers. 
void printDataToRead(dataToRead a) {

	cout << setw(14) << left << a.date;
	cout << " ";

	cout << setw(8) << left << a.open;
	cout << " ";
	cout << setw(8) << left << a.adjClose;
	cout << " ";
	cout << setw(8) << left << a.percentChange << endl;
}

// sorting and printing top increased records. 
void sortAndPrintTopKRecords(vector<dataToRead> a, int k) {
	printHeaders();
	
	for (int i = 0; i < k; i++) {
		int currentMaxIdx = 0;
		for (int j = 1; j < a.size() - i; j++) {
			if (a.at(j).percentChange > a.at(currentMaxIdx).percentChange) {
				currentMaxIdx = j;
			}
		}
		printDataToRead(a[currentMaxIdx]);
		swap(a[currentMaxIdx], a[a.size() - 1 - i]);
	}
}

// Main function. 
void main() {

	// The vector of structs.
	vector<dataToRead> dataInfo;
	string filename, ticker_name;

	unsigned int ticker_length;
	Compute_1_Filename(filename);
	ticker_length = filename.length() - 4;  //4 is the length of ".csv" (Reference to Professor notes)
	ticker_name = filename.substr(0, ticker_length);
	string file_full = filename;
	

	ifstream infile;
	infile.open(filename);
	string data; // for removing 1st row.
	string highPlacehold, lowPlacehold, closePlacehold, volume;

	getline(infile, data) ; // removes row 1
	
	while (infile.good()) {
		dataToRead holder;
		getline(infile, holder.date, ',');
		getline(infile, data, ',');
		holder.open = stod(data);
		getline(infile, highPlacehold, ',');
		getline(infile, lowPlacehold, ',');
		getline(infile, closePlacehold, ',');
		getline(infile, data, ',');
		holder.adjClose = stod(data);
		

		holder.percentChange = ((holder.adjClose - holder.open) / holder.open) * 100;

		dataInfo.push_back(holder);
		getline(infile, data); // move to next line
	}

	dataInfo.pop_back(); //have to remove last element of vector
	cout << "This program will analyze " << ticker_name << " from " << dataInfo.at(0).date<<" to " << dataInfo.at(dataInfo.size()-1).date << endl;
	

	int numOfRecords;
	cout << dataInfo.size() << " trading day(s) exist in the file." << endl;
	bool hasEntered = false;
	while (!hasEntered) {
		cout << "How many increased records do you want to find? ";
		cin >> numOfRecords;
		if (numOfRecords > dataInfo.size() || numOfRecords < 0) {
			cout << endl << "The file does not contain enough trading days" << endl;
			cout << "There are only " << dataInfo.size() << " trading days." << endl;
		}
		else {
			hasEntered = true;
		}
	}

	sortAndPrintTopKRecords(dataInfo, numOfRecords);
}
