/*
 * json_table.cpp
 *
 * Benjamin Lanthier and Kevin De-Oliveira
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

json getjFile(string);
void buildTable(string, string);
void buildTable(json, string);

//path C:\Users\benji\Desktop\Programming\Assignments\Assignment 5\json_test\src\json_table.cpp
int main(int argc, char *argv[]) {

	//exits program if wrong number of input arguments
	if (argc != 5) {
		ofstream err("Error.txt");
		err << "Wrong number of input arguments in the command line";
		return 1;
	}

	//Transforms char* to string for ease of use
	string input = static_cast<string> (*argv[4]);
	string output = static_cast<string> (*argv[5]);

	//Looks for an input Sample.json
	json jfile = getjFile(input.substr(input.find("Sample.json"), 11));

	//Looks for an output sortbyid.table
	buildTable(jfile, output.substr(output.find("sortbyid.table"), 14));

	return 0;
}

json getjFile(std::string filename) {

	json jfile;

	//checks if .json file is given
	if (filename.find(".json") < filename.size()) {

		ifstream Sample(filename);

		if (Sample.is_open()) {
			Sample >> jfile;

			Sample.close();
		}
		else cout<< "File could not be opened" <<endl;
	}
	else cout<< "File could not be opened" <<endl;

	return jfile;
}

void buildTable(string filename, string outfile) {
	buildTable(getjFile(filename), outfile);
}

void buildTable(json jfile, string outfile) {

	//output stream to file
	ofstream out(outfile);

	if (out.is_open()) {

		out<< setw(72) << "|  id  |  type  |  name  |     batter     |           topping          |" << endl;
		out<< setw(72) << "------------------------------------------------------------------------" << endl;

		for (const json &item : jfile["items"]["item"]) {
			for (const json &batter : item["batters"]["batter"]) {
				for (const json &topping : item["topping"]) {

					out<< "|" << item["id"] << "|"
					   << setw(9) << item["type"] << " | "
					   << setw(9) << item["name"] << " | "
					   << setw(17) << batter["type"] << "|"
					   << setw(28) << topping["type"]
					   << "|" << endl
				       << setw(72) << "------------------------------------------------------------------------" << endl;
				}
			}
		}
		out.close();
	}
}


