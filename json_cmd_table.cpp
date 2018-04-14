/*
 * json_table.cpp
 *
 * Benjamin Lanthier and Kevin De-Oliveira
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;

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

	cout<< argc << endl;
	cout << argv[0] << endl;
	cout << argv[1] << endl;
	cout << argv[2] << endl;
	cout << argv[3] << endl;
	cout << argv[4] << endl;

	//exits program if wrong number of input arguments
	if (argc != 5) {
		ofstream err("Error.txt");
		err << "Wrong number of input arguments in the command line";
		err.close();
		return 1;
	}

	//Transforms char* to string for ease of use
	string input = static_cast<string> (argv[3]);
	string output = static_cast<string> (argv[4]);

	//Looks for an input Sample.json
	json jfile = getjFile(input.substr(input.find("Sample.json"), 11));

	//Looks for an output sortbyid.table
	buildTable(jfile, output.substr(output.find("sortbyid.table"), 14), argv[2]);

	return 0;
}



//==================================================================================
//Function definitions



void buildTable(string filename, string outfile, string outSort) {
	buildTable(getjFile(filename), outfile, outSort);
}

void buildTable(json jfile, string outfile, string outSort) {

	//Vector holds sorted json objects
	vector<vector<string>> jsonVector = sortByInput(getVector(jfile), outSort);

	//output stream to file
	ofstream out(outfile);

	if (out.is_open()) {

		if (outSort == "id") {
			out << setw(72) << "|  id  |  type  |  name  |     batter     |           topping          |"<< endl;
			out << setw(72) << "------------------------------------------------------------------------"<< endl;
		}

		else if (outSort == "type") {
			out << setw(72) << "|  type  |  id  |  name  |     batter     |           topping          |"<< endl;
			out << setw(72) << "------------------------------------------------------------------------"<< endl;
		}
		//TODO Create table header depending on type input

		else if (outSort == "name") {
			out << setw(72) << "|  name  |  type  |  id  |     batter     |           topping          |"<< endl;
			out << setw(72) << "------------------------------------------------------------------------"<< endl;
		}
		//TODO

		else if (outSort == "batter") {
			out << setw(72) << "|     batter     |  type  |  name  |  id  |           topping          |"<< endl;
			out << setw(72) << "------------------------------------------------------------------------"<< endl;
		}
		//TODO

		else if (outSort == "topping") {
			out << setw(72) << "|           topping          |  type  |  name  |     batter     |  id  |"<< endl;
			out << setw(72) << "------------------------------------------------------------------------"<< endl;
		}
		//TODO

		else {
			ofstream err("Error.txt");
			err << "Wrong sorting type input argument in the command line";
			exit(1);
		}

		for (unsigned int it = 0; it < jsonVector.size(); it++) {
			out<< "| " << jsonVector[it][0] << " | "
			   << jsonVector[it][1] << " | "
			   << jsonVector[it][2] << " | "
			   << jsonVector[it][3] << " | "
			   << jsonVector[it][4] << " |" << endl
			   << setw(72) << "------------------------------------------------------------------------" << endl;
		}

		/*for (const json &item : jfile["items"]["item"]) {
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
		}*/
		out.close();
	}
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
				jsonVector[i].push_back(batter.value("type", ""));
				jsonVector[i].push_back(topping.value("type", ""));
				i++;
			}
		}
	}
	return jsonVector;
}

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

	else {
		ofstream err("Error.txt");
		err << "Wrong sorting type input argument in the command line";
		exit(1);
	}

	//Swaps the id with the chosen type
	for (unsigned int it = 0; it < jsonVector.size(); it++) {
		temp = jsonVector[it][0];
		jsonVector[it][0] = jsonVector[it][type];
		jsonVector[it][type] = temp;
	}
	return selectionSortVector(jsonVector);
}

vector<vector<string>> selectionSortVector(vector<vector<string>> jsonVector) {

	unsigned int min;

	for (unsigned int size = 0; size < jsonVector.size(); size++) {
		min = size;

		for (unsigned int it = 0; it < jsonVector.size(); it++) {
			if (jsonVector[min][0] > jsonVector[it][0]) {
				min = it;
			}
		}

		vector<string> tempV = jsonVector[size];
		jsonVector[size] = jsonVector[min];
		jsonVector[min] = tempV;

	}

	return jsonVector;
}
