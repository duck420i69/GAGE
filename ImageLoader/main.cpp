
#include <fstream>
#include <vector>
#include <iostream>

int main() {
	using namespace std;
	std::string file_name;
	std::cin >> file_name;


	ifstream ifs(file_name, ios::binary | ios::ate);
	if (!ifs.is_open()) {
		return -1;
	}

	ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);
	


	return 0;
}