#ifndef CONFIG_H
#define CONFIG_H 
#include <vector> 
#include <string>
using std::string;

class Config
{

public:
	struct Row
	{
		string path;
		std::vector<unsigned int> params;
	};
	Config(string filename);
	~Config();
	std::vector<Row> mRows;

};
#endif