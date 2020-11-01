#define _CRT_SECURE_NO_WARNINGS
#include "protect_h.h"
#include "utility.h"
#include <unordered_set>


void protectInputs(ProtectInput* input_data)
{
	std::cout << "--------------Please note that the file must be in CSV format--------------\n" << std::endl;
	std::cout << "Enter the file name with extention (Example.csv): ";
	input_data->file_name = getStrInput(std::cin);
	

	std::cout << "\nWhat file it is?" << std::endl;
	std::cout << "1. Open" << std::endl;
	std::cout << "2. Private" << std::endl;
	std::cout << "3. Zeta" << std::endl;
	std::cout << "Input: ";
	input_data->file_type = getIntInput(std::cin);

	while (input_data->file_type < 1 || input_data->file_type > 3)
	{
		std::cout << "Wrong file choice." << std::endl;
		std::cout << "Input: ";
		input_data->file_type = getIntInput(std::cin);
	}



	std::cout << "\nChoose the separator value (for english Excell (comma) \",\" for russian (semicolon) \";\")\n";
	std::cout << "1. Comma\n";
	std::cout << "2. Semicolon\n";
	std::cout << "Input: ";
	input_data->separator_choice = getIntInput(std::cin);

	while (input_data->separator_choice < 1 || input_data->separator_choice > 2 )
	{
		std::cout << "Wrong Input" << std::endl;
		std::cout << "Input: ";
		input_data->separator_choice = getIntInput(std::cin);
	}

	if (input_data->separator_choice == 1)
	{
		input_data->separator = ',';
	}
	else
	{
		input_data->separator = ';';
	}
}

bool protectMakeFileNames(ProtectReadyFileNames* names, int choice)
{
	
	char buff[100];
	time_t calendar = time(NULL);
	struct tm* time_data;
	time_data = localtime(&calendar);

 	switch (choice)
	{
	case 1:
		strftime(buff, 80, "OpenProtectEmail_%d-%m-%Y.csv", time_data);
		names->email_name = buff;
		strftime(buff, 80, "OpenProtectGUID_%d-%m-%Y.csv", time_data);
		names->guid_name = buff;
		break;
	case 2:
		strftime(buff, 80, "PrivateProtectEmail_%d-%m-%Y.csv", time_data);
		names->email_name = buff;
		strftime(buff, 80, "PrivateProtectGUID_%d-%m-%Y.csv", time_data);
		names->guid_name = buff;
		break;
	case 3:
		strftime(buff, 80, "ZetaProtectEmail_%d-%m-%Y.csv", time_data);
		names->email_name = buff;
		strftime(buff, 80, "ZetaProtectGUID_%d-%m-%Y.csv", time_data);
		names->guid_name = buff;
		break;
	default:
		std::cout << "Wrong choice" << std::endl;
		return false;
		break;
	}
	return true;
}

void protectInitData(ProtectContainer* data, int num_of_elems)
{
	num_of_elems--;

	data->affiliate_code.reserve(num_of_elems);
	data->finish_code.reserve(num_of_elems);
	data->is_lead.reserve(num_of_elems);
	data->is_sale.reserve(num_of_elems);
	data->submitted_email.reserve(num_of_elems);
	data->beacon_range.reserve(num_of_elems);
	data->status.reserve(num_of_elems);
}

int protectGetFileLength(std::ifstream* file)
{
	int num_of_lines = 0;
	while (!file->eof())
	{
		std::string line;
		getline(*file, line);
		num_of_lines++;
	}

	file->clear();
	file->seekg(0, std::ios::beg);

	return num_of_lines;
}

void protectSwitchParser(int* num, ProtectContainer* data, std::string line)
{
	enum FileColumnNumbers { affiliate_code = 11, finish_code = 14, is_lead = 27, is_sale = 28, submitted_email = 37, beacon_range = 42 };
	enum TotalNumberOfColumnsInFile {total_columns = 42};

	if (*num >= total_columns)
	{
		*num = 1;
	}

	(*num)++;


	switch (*num)
	{
	case affiliate_code:
		data->affiliate_code.push_back(line);
		break;
	case finish_code:
		data->finish_code.push_back(line);
		break;
	case is_lead:
		data->is_lead.push_back(line);
		break;
	case is_sale:
		data->is_sale.push_back(line);
		break;
	case submitted_email:
		data->submitted_email.push_back(line);
		break;
	case beacon_range:
		data->beacon_range.push_back(line);
	default:
		break;
	}
	
}

void protectStatusFiller(ProtectContainer* data)
{
	for (int i = 0; i < data->affiliate_code.size() - 1; i++)
	{
		if (i == 0)
		{
			data->status.push_back("Status");
		}
		data->status.push_back("0");
	}
}

void protectBeaconRangeFormatting(ProtectContainer* data)
{
	for (int i = 0; i < data->beacon_range.size(); i++)
	{
		if (data->beacon_range[i].find('-') != std::string::npos)
		{
			data->beacon_range[i].erase(data->beacon_range[i].find('-'));
		}
		else if (data->beacon_range[i].find('\n') != std::string::npos)
		{
			data->beacon_range[i].erase(data->beacon_range[i].find('\n'));
		}

		if (i == 0)
		{
			data->beacon_range[i] = "Credit";
		}
	}
}

void protectAffiliateCodeFormatting(ProtectContainer* data)
{
	data->affiliate_code[0] = "LeadGUID";
	
	for (int i = 1; i < data->affiliate_code.size(); i++)
	{
		if (data->affiliate_code[i] != "")
		{
			data->affiliate_code[i].erase(0, (data->affiliate_code[i].find('|') + 1));
		}
	}
}

void protectStatusSetter(ProtectContainer* data)
{
	for (int i = 1; i < data->affiliate_code.size(); i++)
	{
		int beacon_range;
		if (data->beacon_range[i] != "")
		{
			beacon_range = std::stoi(data->beacon_range[i]);
		}
		else
		{
			beacon_range = 0;
		}


		if (data->finish_code[i] == "")
		{
			data->finish_code[i] = "0";
		}


		if (data->is_lead[i] == "0" && data->is_sale[i] == "0")
		{
			data->status[i] = data->finish_code[i];
		}
		else if (data->is_lead[i] == "1" && data->is_sale[i] == "0")
		{
			data->status[i] = "Interested";
		}
		else if (data->is_sale[i] == "1" && beacon_range > 599)
		{
			data->status[i] = "Sale";
		}
		else if (data->is_lead[i] == "1" && data->is_sale[i] == "1" && beacon_range < 600)
		{
			data->status[i] = "Interested";
		}
		else
		{
				data->status[i] = data->finish_code[i];
		}
	}
}

void protectRemoveDuplicates(ProtectContainer* data)
{
	std::unordered_set<std::string> s;

	for (int i = 1; i < data->affiliate_code.size(); i++)
	{
		if (s.find(data->affiliate_code[i]) != s.end())
		{
			data->affiliate_code[i] = "";
		}
		else
			s.insert(data->affiliate_code[i]);
	}

	s.clear();

	for (int i = 1; i < data->submitted_email.size(); i++)
	{
		if (s.find(data->submitted_email[i]) != s.end())
		{
			data->submitted_email[i] = "";
		}
		else
			s.insert(data->submitted_email[i]);
	}
}

void protectFileAssembly(ProtectContainer* data, std::ofstream* emailFile, std::ofstream* guidFile)
{

	for (int i = 0; i < data->status.size(); i++)
	{
		if (data->affiliate_code[i] != "")
		{
			*guidFile << data->beacon_range[i] + ',' + data->status[i] + ',' + data->affiliate_code[i] << std::endl;
		}

		if (data->submitted_email[i] != "")
		{
			*emailFile << data->beacon_range[i] + ',' + data->status[i] + ',' + data->submitted_email[i] << std::endl;
		}
	}
	emailFile->close();
	guidFile->close();
}

int protectRunDispo()
{
	ProtectContainer data;
	ProtectReadyFileNames finish_names;
	ProtectInput protect_input_data;

	protectInputs(&protect_input_data);

	std::ifstream dispoFile(protect_input_data.file_name);
	if (!dispoFile.is_open())
	{
		system("CLS");
		std::cerr << "\n\nFailed to open the file.\n" << std::endl;
		return 1;
	}

	if (!fileValidity(&dispoFile, "Submitted Email"))
	{
		system("CLS");
		std::cerr << "\n\nLooks like you've opened a wrong file.\n" << std::endl;
		return 1;
	}

	int num_of_lines = protectGetFileLength(&dispoFile);
	protectInitData(&data, num_of_lines);

	int counter = 0;

	while (!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, protect_input_data.separator);
		protectSwitchParser(&counter, &data, line);
	}
	dispoFile.close();

	protectStatusFiller(&data);
	protectBeaconRangeFormatting(&data);
	protectAffiliateCodeFormatting(&data);
	protectStatusSetter(&data);
	protectRemoveDuplicates(&data);
	protectMakeFileNames(&finish_names, protect_input_data.file_type);

	std::ofstream emailFile(finish_names.email_name);
	std::ofstream guidFile(finish_names.guid_name);

	protectFileAssembly(&data, &emailFile, &guidFile);
	system("CLS");
	std::cout << "Protect is GUCCI.\n" << std::endl;
	return 0;
}