#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
Config::Config(string filename)
{
	fstream infile(filename);

	std::string line;
	while (getline(infile, line))
	{
		if(line[0] != '/')continue;
		Row row;
		istringstream is(line);
		is >> row.path;
		string param;
		while (is >> param) {
			unsigned int ip;
			sscanf(param.c_str(), "%x", &ip);
			row.params.push_back(ip);
		}
		mRows.push_back(row);
	}

}

Config::~Config()
{

}