#define _CRT_SECURE_NO_WARNINGS
#include "util_h.h"
#include <unordered_set>

void Inputs(Input* input)
{
	std::cout << "--------------Please note that the file must be in CSV format--------------\n" << std::endl;
	std::cout << "Enter the file name with extention (Example.csv): ";
	std::cin >> input->file_name;


	std::cout << "\nWhat file it is?" << std::endl;
	std::cout << "1. Open" << std::endl;
	std::cout << "2. Private" << std::endl;
	std::cout << "3. Zeta" << std::endl;
	std::cout << "Input: ";
	std::cin >> std::setw(1) >> input->file_type;

	while (!std::cin.good())
	{
		std::cout << "Wrong Input!\n";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Input: ";
		std::cin >> std::setw(1) >> input->file_type;
	}

	while (input->file_type < 1 || input->file_type > 3)
	{
		std::cout << "Wrong Input!\n";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Input: ";
		std::cin >> std::setw(1) >> input->file_type;
	}


	std::cout << "\nChoose the separator value (for english Excell (comma) \",\" for russian (semicolon) \";\")\n";
	std::cout << "1. Comma\n";
	std::cout << "2. Semicolon\n";
	std::cout << "Input: ";
	std::cin >> input->separator_choice;

	while (input->separator_choice < 1 || input->separator_choice > 2)
	{
		std::cout << "Wrong Input!\n";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Input: ";
		std::cin >> std::setw(1) >> input->file_type;
	}
	
	if (input->separator_choice == 1)
	{
		input->separator = ',';
	}
	else
	{
		input->separator = ';';
	}
}

bool makeFileNames(ReadyFileNames* names, int choice)
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

void initData(Container* data, int num_of_elems)
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

int getFileLength(std::ifstream* file)
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

void switchParser(int* num, Container* data, std::string line)
{
	enum FileColumnNumbers { affiliate_code = 11, finish_code = 14, is_lead = 27, is_sale = 28, submitted_email = 36, beacon_range = 41 };
	enum TotalNumberOfColumnsInFile {total_columns = 41};

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

void statusFiller(Container* data)
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

void beaconRangeFormatting(Container* data)
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

void affiliateCodeFormatting(Container* data)
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

void statusSetter(Container* data)
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

void removeDuplicates(Container* data)
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

void fileAssembly(Container* data, std::ofstream* emailFile, std::ofstream* guidFile)
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
}

	