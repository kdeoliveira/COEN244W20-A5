/*
 * json_table.cpp
 *
 * Benjamin Lanthier and Kevin De Oliveira
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include "json_exception.h"


using namespace std;
using namespace JException;	//end of file json_exception.h

using json = nlohmann::json;

//==================================================================================
//Function prototypes
void buildTable(string, string, string);
void buildTable(json, string, string);
json getjFile(string);
vector<vector<string>> getVector(json);
vector<vector<string>> sortByInput(vector<vector<string>>, string);
vector<vector<string>> selectionSortVector(vector<vector<string>>);
//==================================================================================
//Main function

int main(int argc, char *argv[]) {

	try{
		//exits program if wrong number of input arguments
		if (argc != 5)
			throw InvalidArgumentsException();


		cout<< "Number of Arguments: " << argc << endl
			<< "List of Arguments:\n"
			<< argv[0] << endl
			<< argv[1] << endl
			<< argv[2] << endl
			<< argv[3] << endl
			<< argv[4] << endl;

		json jfile;

		//Transforms char* to string for ease of use
		string input = static_cast<string> (argv[3]);
		string output = static_cast<string> (argv[4]);

		//Looks for an input {Sample_name}.json
		if (input.find(".json") != std::string::npos)
		jfile = getjFile(input.substr(input.find_last_of("\\") + 1));

		else
			throw FilePathException();

		//Looks for an output sortby{input_name}.table
		if (output.find(".table") != std::string::npos)
		buildTable(jfile, output.substr(output.find_last_of("\\") + 1), argv[2]);

		else
			throw FilePathException();

	}catch(exception &e){
		cout<<"Exception has been caught: "<<e.what()<<endl;
		return 1;
	}
	return 0;
}

//==================================================================================
//Function definitions


//Overloaded function. Passes the filename to the getfile function. Calls the other buildTable function
void buildTable(string filename, string outfile, string outSort) {
	buildTable(getjFile(filename), outfile, outSort);
}

//Builds the table which is output in the output file
void buildTable(json jfile, string outfile, string outSort) {

	//Vector holds sorted json objects
	vector<vector<string>> jsonVector = sortByInput(getVector(jfile), outSort);

	//output stream to file
	ofstream out(outfile);

	if (out.is_open()) {


	//Each sort type has an output loop such that they are well formatted
		if (outSort == "id") {
			out << setw(80) << "|  id  |    type    |    name    |     batter     |           topping          |"<< endl;
			out << setw(80) << "--------------------------------------------------------------------------------"<< endl;

			for (unsigned int it = 0; it < jsonVector.size(); it++) {
				out << "| " << setw(4) << left << jsonVector[it][0] << " |"
					<< setw(12) << right << jsonVector[it][1] << "|"
					<< setw(12) << right << jsonVector[it][2] << "|"
					<< setw(16) << right << jsonVector[it][3] << "|"
					<< setw(28) << right << jsonVector[it][4] << "|" << endl
					<< setw(80) << "--------------------------------------------------------------------------------" << endl;
			}
		}

		else if (outSort == "type") {
			out << setw(80) << "|    type    |  id  |    name    |     batter     |           topping          |"<< endl;
			out << setw(80) << "--------------------------------------------------------------------------------"<< endl;

			for (unsigned int it = 0; it < jsonVector.size(); it++) {
				out << "|" << setw(12) << right << jsonVector[it][0]
					<< "| " << setw(4) << left << jsonVector[it][1] << " |"
					<< setw(12) << right << jsonVector[it][2] << "|"
					<< setw(16) << right << jsonVector[it][3] << "|"
					<< setw(28) << right << jsonVector[it][4] << "|" << endl
					<< setw(80) << "--------------------------------------------------------------------------------" << endl;
			}
		}

		else if (outSort == "name") {
			out << setw(80) << "|    name    |    type    |  id  |     batter     |           topping          |"<< endl;
			out << setw(80) << "--------------------------------------------------------------------------------"<< endl;

			for (unsigned int it = 0; it < jsonVector.size(); it++) {
				out << "|" << setw(12) << right << jsonVector[it][0] << "|"
					<< setw(12) << right << jsonVector[it][1]
					<< "| " << setw(4) << left << jsonVector[it][2] << " |"
					<< setw(16) << right << jsonVector[it][3] << "|"
					<< setw(28) << right << jsonVector[it][4] << "|" << endl
					<< setw(80) << "--------------------------------------------------------------------------------" << endl;
			}
		}

		else if (outSort == "batter") {
			out << setw(80) << "|     batter     |    type    |    name    |  id  |           topping          |"<< endl;
			out << setw(80) << "--------------------------------------------------------------------------------"<< endl;

			for (unsigned int it = 0; it < jsonVector.size(); it++) {
				out << "|" << setw(16) << right << jsonVector[it][0] << "|"
					<< setw(12) << right << jsonVector[it][1] << "|"
					<< setw(12) << right << jsonVector[it][2]
					<< "| " << setw(4) << left << jsonVector[it][3] << " |"
					<< setw(28) << right << jsonVector[it][4] << "|" << endl
					<< setw(80) << "--------------------------------------------------------------------------------" << endl;
			}
		}

		else if (outSort == "topping") {
			out << setw(80) << "|           topping          |    type    |    name    |     batter     |  id  |"<< endl;
			out << setw(80) << "--------------------------------------------------------------------------------"<< endl;

			for (unsigned int it = 0; it < jsonVector.size(); it++) {
				out << "|" << setw(28) << right << jsonVector[it][0] << "|"
					<< setw(12) << right << jsonVector[it][1] << "|"
					<< setw(12) << right << jsonVector[it][2] << "|"
					<< setw(16) << right << jsonVector[it][3]
					<< "| " << setw(4) << left << jsonVector[it][4] << " |" << endl
					<< setw(80) << "--------------------------------------------------------------------------------" << endl;
			}
		}

		else
			throw InvalidSortArgumentException();
		out.close();
	}
}

//Gets the information from the file, puts it in a json object
json getjFile(std::string filename) {

	json jfile;

	//checks if .json file is given
	if (filename.find(".json") < filename.size()) {

		ifstream Sample(filename);

		if (Sample.is_open()) {
			Sample >> jfile;

			Sample.close();
		}
		else
			throw OpenFileException();
	}
	else
		throw OpenFileException();

	return jfile;
}

//Gets the information in the json object, puts it in a vector
vector<vector<string>> getVector(json jfile) {

	vector<vector<string>> jsonVector;

	unsigned i = 0;

	for (const json &item : jfile["items"]["item"]) {
		for (const json &batter : item["batters"]["batter"]) {
			for (const json &topping : item["topping"]) {
				jsonVector.push_back(vector<string>());
				jsonVector[i].push_back(item["id"]);
				jsonVector[i].push_back(item["type"]);
				jsonVector[i].push_back(item["name"]);
				jsonVector[i].push_back(batter["type"]);
				jsonVector[i].push_back(topping["type"]);
				i++;
			}
		}
	}
	return jsonVector;
}

//Sorts the vector by the requested user inputed argument
vector<vector<string>> sortByInput(vector<vector<string>> jsonVector, string arg) {

	unsigned int type = 0;
	string temp;

	if (arg == "id") {
		return jsonVector;
	}

	else if (arg == "type")
		type = 1;

	else if (arg == "name")
		type = 2;

	else if (arg == "batter")
		type = 3;

	else if (arg == "topping")
		type = 4;

	else
		throw InvalidSortArgumentException();

	//Swaps the id with the chosen type
	for (unsigned int it = 0; it < jsonVector.size(); it++) {
		temp = jsonVector[it][0];
		jsonVector[it][0] = jsonVector[it][type];
		jsonVector[it][type] = temp;
	}
	return selectionSortVector(jsonVector);
}

//Sorts the vectors inside of the vector by order of smallest to biggest value of string with the selection sort algorithm
vector<vector<string>> selectionSortVector(vector<vector<string>> jsonVector) {

	unsigned int min;

	for (unsigned int pos = 0; pos < jsonVector.size(); pos++) {
		min = pos;

		for (unsigned int it = pos; it < jsonVector.size(); it++) {
			if (jsonVector[min][0] > jsonVector[it][0]) {
				min = it;
			}
		}

		vector<string> tempV = jsonVector[pos];
		jsonVector[pos] = jsonVector[min];
		jsonVector[min] = tempV;

	}
	return jsonVector;
}
