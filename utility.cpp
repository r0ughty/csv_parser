#include "utility.h"

int getIntInput(std::istream& in)
{
	std::string str;
	std::getline(in, str);
	str.erase(str.find_last_not_of("\t\r\n\f\v") + 1);
	
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]) || i > 2)
		{
			std::cout << "Wrong Input" << std::endl;
			return 0;
		}
	}
	
	std::istringstream sstr(str);
	int result;
	sstr >> result;
	if (!sstr.eof()) std::cin.setstate(std::ios::failbit);

	return result;
}

std::string getStrInput(std::istream& in)
{
	std::string str;
	std::getline(in, str);
	str.erase(str.find_last_not_of("\t\r\n\f\v") + 1);

	std::istringstream sstr(str);
	std::string result;
	sstr >> result;
	if (!sstr.eof()) std::cin.setstate(std::ios::failbit);

	return result;
}

bool fileValidity(std::ifstream* file, std::string checker_line)
{
	std::string line;
	getline(*file, line);
	if (line.find(checker_line) != std::string::npos)
	{
		file->clear();
		file->seekg(0, std::ios::beg);
		return true;
	}
	else
	{
		file->clear();
		file->seekg(0, std::ios::beg);
		return false;
	}
}