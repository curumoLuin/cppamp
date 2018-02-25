// CppAMP_Task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <string>      
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>


int maxRecords = 0;
double moneyz = 0;
double moneyzAvg = 0;


struct record
{
	std::string name;
	bool calendar[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 }; //jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec;
	double sum = 0;
	std::string status;
};

void addToDb(record db[], record newRecord){
	

}

void printOne(record db[], std::string name) {

	std::cout << "\n--------------------------------------" << std::endl;
	std::cout << "   Name    | Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec |  Paid  |  Status |" << std::endl;
	for (int i = 0; i < maxRecords; i++) {
		if (db[i].name == name) {
			std::cout << db[i].name;
			for (int j = 0; j < 12; ++j)
				if (db[i].calendar[j]==true)
					std::cout << "|  +  ";
				else
					std::cout << "|  -  ";
			std::cout << "|";
			std::cout << db[i].sum;
			std::cout << db[i].status;
			break;
		}
	}
}

void printAll(record db[]) {

	std::cout << "\n--------------------------------------" << std::endl;
	std::cout << "   Name    | Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec |  Paid  |  Status |" << std::endl;
	for (int i = 0; i < sizeof(db); ++i) {
		std::cout << db[i].name;
		std::cout << "  ";
		for (int j = 0; j < 12; ++j)
			if (db[i].calendar[j] == true)
				std::cout << "|  +  ";
			else
				std::cout << "|  -  ";
		std::cout << "| ";
		std::cout << db[i].sum;
		std::cout << " PLN | ";
		std::cout << db[i].status;
		std::cout << " | " << std::endl;
	}
}

void summary(record db[]) {
	double sum = 0;
	double avg = 0;
	for (int i = 0; i < sizeof(db); ++i) {
		sum += db[i].sum;
	}
	avg = sum / sizeof(db);
	std::cout << std::endl;
	std::cout << "Suma wplat: " << sum << " PLN" << std::endl;
	std::cout << "Srednia wplata: " << avg << " PLN"<< std::endl;
}

int isThereAnybodyOutThere(record db[],std::string name) { //returns index in db if record found
	int response = -2;
	for (int i = 0; i < maxRecords; ++i) {
		if (db[i].name == name) {
			return i;
		}
		else {
			response = -1;
		}
	}
	return response;
}

int whichMonth(std::string date) { //returns number 0-11 for specyfic month.
	int nm=0; // number of the month
	std::vector< std::string > vec;
	boost::tokenizer<> tok(date);
	vec.assign(tok.begin(), tok.end());
	nm = std::stoi(*(&vec[1]))-1; //array
	//std::cout << nm;
	return nm;
}

std::string labeller(double paid) {
	if (paid >= moneyzAvg) {
		return "Fandoszlachta";
	}
	else {
		return "Fandoplebs";
	};
}




int main()
{
	using namespace boost;

	std::vector< std::string > vec;
	std::string line;


	//File operations
	std::string data("data.csv");

	std::ifstream init(data.c_str());
	if (!init.is_open()) return 1;

	//Tokenownik
	typedef tokenizer< escaped_list_separator<char> > Tokenizer;

	//Count number of lines for estimated database array size
	while (std::getline(init, line))
	{
		++maxRecords;
	}
	
	//DATABASE 
	record * database = new record[maxRecords];

    //Actual ifstream object
	std::ifstream in(data.c_str());
	
	//First line to be ommited
	int i = 0;
	bool firstPassFlag = true;

	while (getline(in, line))
	{ 
		if (firstPassFlag == false) {
			Tokenizer tok(line);
			vec.assign(tok.begin(), tok.end());
			
			if (vec.size() < 3) continue;
			
			int isThere = isThereAnybodyOutThere(database, *(&vec[0]));
			if (isThere != (-1)) {
				int month = whichMonth(*(&vec[1]));
				database[isThere].calendar[month] = 1;
				database[isThere].sum += std::stod(*(&vec[2]));
				database[isThere].status = labeller(database[isThere].sum);
			}
			else {
				database[i].name = *(&vec[0]);
				int month = whichMonth(*(&vec[1]));
				database[i].calendar[month] = 1;
				database[i].sum = std::stod(*(&vec[2]));
				database[i].status = labeller(database[i].sum);

			}

			copy(vec.begin(), vec.end(),
				std::ostream_iterator<std::string>(std::cout, "|"));
			std::cout << std::endl << database[i].sum;
			std::cout << "\n----------------------" << std::endl;

			i++;
		}
		firstPassFlag = false;
	}

	printAll(database);
	summary(database);

	int cokolwiek;
	std::cin >> cokolwiek;

	delete database;
}