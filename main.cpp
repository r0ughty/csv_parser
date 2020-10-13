#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "util_h.h"





int main()
{

	Container data;
	ReadyFileNames finish_names;
	Input input_data;
	
	Inputs(&input_data);
	
	std::ifstream dispoFile(input_data.file_name);
	if (!dispoFile.is_open())
	{
		std::cerr << "\n\nFailed to open the file." << std::endl;
		return 1;
	}
	
	
	int num_of_lines = getFileLength(&dispoFile);
	initData(&data, num_of_lines);

	int counter = 0;

	while(!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, input_data.separator);
		switchParser(&counter, &data, line);
	}
	dispoFile.close();

	statusFiller(&data);
	beaconRangeFormatting(&data);
	affiliateCodeFormatting(&data);
	statusSetter(&data);
	removeDuplicates(&data);
	makeFileNames(&finish_names,input_data.file_type);
	
	std::ofstream emailFile(finish_names.email_name);
	std::ofstream guidFile(finish_names.guid_name);

	fileAssembly(&data, &emailFile, &guidFile);
	emailFile.close();
	guidFile.close();

	std::cout << "WE GUCCI." << std::endl;

	return 0;
}