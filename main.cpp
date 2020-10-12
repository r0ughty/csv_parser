#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "util_h.h"





int main()
{

	container data;
	ready_file_names finish_names;
	input input_data;
	
	inputs(&input_data);
	
	std::ifstream dispoFile(input_data.file_name);
	if (!dispoFile.is_open())
	{
		std::cerr << "\n\nFailed to open the file." << std::endl;
		return 1;
	}
	
	
	int num_of_lines = get_file_length(&dispoFile);
	init_data(&data, num_of_lines);

	int counter = 0;

	while(!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, input_data.separator);
		switch_parser(&counter, &data, line);
	}
	dispoFile.close();

	status_filler(&data);
	beacon_range_formatting(&data);
	affiliate_code_formatting(&data);
	status_setter(&data);
	remove_duplicates(&data);
	make_file_name(&finish_names,input_data.file_type);
	
	std::ofstream emailFile(finish_names.email_name);
	std::ofstream guidFile(finish_names.guid_name);

	file_assembly(&data, &emailFile, &guidFile);
	emailFile.close();
	guidFile.close();

	std::cout << "WE GUCCI." << std::endl;

	return 0;
}