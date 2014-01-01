#include "Globals.h"
std::ofstream _log_file_;
void Split(std::string data, char c, Strings& strings){
	std::size_t found = data.find(c);
	while ( found != std::string::npos ) {
		strings.push_back(data.substr(0, found));
		data.erase(0, found + 1);
		found = data.find(c);
	}
	strings.push_back(data);
}