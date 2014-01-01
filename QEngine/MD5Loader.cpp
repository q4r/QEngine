#include "MD5Loader.h"

#include "Globals.h"
#include <iostream>
#include <sstream>

#define GET_STRING_RET(str)if ( ! GetString(file, str) ){return;}
#define CHECK_RET(con, msg) if (!(con)) { LOG(msg); return; };

bool GetString(std::ifstream& file, std::string& str){
	str.clear();
	char ch;

	file >> ch;
	if (ch != '"'){
		str = ch;
		return false;
	}

	file.get(ch);
	while (ch != '"'){
		str += ch;
		file.get(ch);
		if (!file.good()){
			return false;
		}
	}

	return true;
}

MD5Loader::MD5Loader(const std::string& fileName){
	std::ifstream file;
	file.open(fileName.c_str());

	if ( ! file.is_open() ){
		return;
	}

	while ( file.good() ){
		std::string element;
		file >> element;

		if (element.compare("MD5Version") == 0){
			file >> version;
			CHECK_RET(version == 10, "Incompatible version of MD5");
			
			continue;
		}

		if (element.compare("commandline") == 0){
			std::string commandline;
			GET_STRING_RET(commandline);
			continue;
		}

		if (element.compare("numJoints") == 0){
			file >> numJoits;
			continue;
		}

		if (element.compare("numMeshes") == 0){
			file >> numMeshes;
			continue;
		}
		
		if (element.compare("joints") == 0){
			CHECK_RET(numJoits == 0, "Joints count is 0");
			CHECK_RET(LoadJoints(file, numJoits), "Joints parsing error");
			
			continue;
		}
	}
}


MD5Loader::~MD5Loader(void){
}

bool MD5Loader::LoadJoints(std::ifstream& file, unsigned int count){
	return true;
}