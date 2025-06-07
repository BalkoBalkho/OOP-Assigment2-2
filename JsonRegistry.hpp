#include "veryslowjson.hpp" //I kind of hate how absurdly slow json.hpp is, but its very easy to include and relativly easy to use.
#include "Units.hpp"
#include <fstream>
using json = nlohmann::json;

class Monsterjsonregistry {
//json registry gets some data from jibran_ate.json
//most of the data is ignored

	static json data;
	static bool read = false;
public:
	Monsterjsonregistry() { // slow libary will be very slow
		//if this somehow fails it crashes, oh well
		if (!read) {
		std::ifstream f("./data/jibran_ate.json");
		f >> data;
		f.close();

		}

	}

	

public:

};