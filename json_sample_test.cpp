//============================================================================
// Name        : json_test.cpp
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

int main() {

	json jfile;
	ifstream Sample("Sample.json");

	if (Sample.is_open()) {

		Sample >> jfile;

		cout << jfile.dump(4) << endl;

		cout << endl << "====================== Next Part ======================"
			 << endl << endl;

		for (const json &item : jfile["items"]["item"]) {

			cout << "id: " << item["id"] << endl;
			cout << "type: " << item["type"] << endl;
			cout << "name: " << item["name"] << endl;
			cout << "ppu: " << item["ppu"] << endl;

			// check if item["batters"]["batter"] exists
			if (item.find("batters") != item.end()&& item["batters"].find("batter")!= item["batters"].end()) {
				cout << endl << "Batters: " << endl;

				for (const json &batter : item["batters"]["batter"]) {
					//check if "batter" if empty
					if (!item["batters"]["batter"].empty()) {
						cout << batter.dump(3) << endl;
					}
				}
			}
			// check if item["topping"] exists
			if (item.find("topping") != item.end()) {
				cout<< endl << "Topping: " << endl;

				for (const auto &topping : item["topping"]) {
					//check if "topping" is empty
					if (!item["topping"].empty()) {
						cout << topping.dump(3) << endl;
					}
				}
			}
		}
		Sample.close();
	}

	else
		cout << "wut\'s not working";

	return 0;

}
