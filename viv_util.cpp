#define _CRT_SECURE_NO_WARNINGS
#include "viv_util.h"
#include "utility.h"
#include <unordered_set>

void vivInputs(VivInput* input_data)
{
	std::cout << "--------------Please note that the file must be in CSV format--------------\n" << std::endl;
	std::cout << "Enter the file name with extention (Example.csv): ";
	input_data->file_name = getStrInput(std::cin);
}

int vivGetFileLength(std::ifstream* file)
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

void vivInitData(VivContainer* data, int num_of_elems)
{
	data->guid.reserve(num_of_elems);
	data->installed.reserve(num_of_elems);
	data->ordered.reserve(num_of_elems);
	data->pass.reserve(num_of_elems);
	data->dp_stat.reserve(num_of_elems);
	data->status.reserve(num_of_elems);
}

bool vivMakeFileNames(VivReadyFileNames* names)
{

	char buff[100];
	time_t calendar = time(NULL);
	struct tm* time_data;
	time_data = localtime(&calendar);

		strftime(buff, 80, "Vivint_%d-%m-%Y.csv", time_data);
		names->name = buff;

	return true;
}

void vivSwitchParser(int* num, VivContainer* data, std::string line)
{
	enum FileColumnNumbers { guid = 8, ordered = 12, install = 13, ran = 14, pass = 15, dp_stat = 16 };
	enum TotalNumberOfColumnsInFile { total_columns = 16 };

	if (*num >= total_columns)
	{
		*num = 1;
	}

	(*num)++;

	switch (*num)
	{
	case guid:
		data->guid.push_back(line);
		break;
	case ordered:
		data->ordered.push_back(line);
		break;
	case install:
		data->installed.push_back(line);
		break;
	case ran:
		data->ran.push_back(line);
		break;
	case pass:
		data->pass.push_back(line);
		break;
	case dp_stat:
		data->dp_stat.push_back(line);
	default:
		break;
	}
}

void vivGuidFormat(VivContainer* data)
{
	for (int i = 0; i < data->guid.size(); i++)
	{
		std::string tmp;
		for (int j = 0; j < data->guid[i].length(); j++)
		{
			if (isalnum(data->guid[i][j]) || data->guid[i][j] == '-')
			{
				tmp += data->guid[i][j];
			}
		}
		data->guid[i] = tmp;
		tmp = "";
	}
}

void vivOrderedFormat(VivContainer* data)
{
	for (int i = 0; i < data->ordered.size(); i++)
	{
		std::string tmp;
		for (int j = 0; j < data->ordered[i].length(); j++)
		{
			if (isalpha(data->ordered[i][j]))
			{
				tmp += data->ordered[i][j];
			}
		}
		data->ordered[i] = tmp;
		tmp = "";
	}
}

void vivInstalledFormat(VivContainer* data)
{
	for (int i = 0; i < data->installed.size(); i++)
	{
		std::string tmp;
		for (int j = 0; j < data->installed[i].length(); j++)
		{
			if (isalpha(data->installed[i][j]))
			{
				tmp += data->installed[i][j];
			}
		}
		data->installed[i] = tmp;
		tmp = "";
	}
}

void vivRanFormat(VivContainer* data)
{
	for (int i = 0; i < data->ran.size(); i++)
	{
		std::string tmp;
		for (int j = 0; j < data->ran[i].length(); j++)
		{
			if (isalpha(data->ran[i][j]))
			{
				tmp += data->ran[i][j];
			}
		}
		data->ran[i] = tmp;
		tmp = "";
	}
}

void vivPassFormat(VivContainer* data)
{
	for (int i = 0; i < data->pass.size(); i++)
	{
		std::string tmp;
		for (int j = 0; j < data->pass[i].length(); j++)
		{
			if (isalpha(data->pass[i][j]))
			{
				tmp += data->pass[i][j];
			}
		}
		data->pass[i] = tmp;
		tmp = "";
	}
}

void vivDpStatFormat(VivContainer* data)
{
	for (int i = 0; i < data->dp_stat.size(); i++)
	{
		data->dp_stat[i].erase(data->dp_stat[i].find('\n'), data->dp_stat[i].length());
		std::string tmp;
		for (int j = 0; j < data->dp_stat[i].length(); j++)
		{
			if (isalpha(data->dp_stat[i][j]) || isspace(data->dp_stat[i][j]))
			{
				tmp += data->dp_stat[i][j];
			}
		}
		data->dp_stat[i] = tmp;
		tmp = "";
	}
}

void vivFieldsFormat(VivContainer* data)
{
	vivGuidFormat(data);
	vivOrderedFormat(data);
	vivInstalledFormat(data);
	vivRanFormat(data);
	vivPassFormat(data);
	vivDpStatFormat(data);
}

void vivEmptyFieldsFiller(VivContainer* data)
{
	for (int i = 0; i < data->guid.size() - 1; i++)
	{
		if (i == 0)
		{
			data->status.push_back("Status");
			data->installs.push_back("Installs");
		}
		data->status.push_back("0");
		data->installs.push_back("0");
	}
}

void vivStatusSetter(VivContainer* data)
{
	for (int i = 1; i < data->guid.size(); i++)
	{
		if (data->installed[i] == "Yes")
		{
			data->status[i] = "Installed";
		}
		else if (data->ordered[i] == "Yes")
		{
			data->status[i] = "Ordered";
		}
		else if (data->pass[i] == "Yes")
		{
			data->status[i] = "Credit Pass";
		}
		else if (data->ran[i] == "Yes")
		{
			data->status[i] = "Credit Ran";
		}
		else
		{
			if (data->dp_stat[i] == "")
			{
				data->status[i] = "0";
			}
			else
			{
				data->status[i] = data->dp_stat[i];
			}
		}
	}
}

void vivInstallsSetter(VivContainer* data)
{
	for (int i = 1; i < data->guid.size(); i++)
	{
		if (data->installed[i] == "Yes")
		{
			data->installs[i] = "1";
		}
	}
}

void vivRemoveDuplicates(VivContainer* data)
{
	std::unordered_set<std::string> s;

	for (int i = 1; i < data->guid.size(); i++)
	{
		if (s.find(data->guid[i]) != s.end())
		{
			data->guid[i] = "";
		}
		else
			s.insert(data->guid[i]);
	}
}

void vivFileAssembly(VivContainer* data, std::ofstream* file)
{

	for (int i = 0; i < data->status.size(); i++)
	{
		if (data->guid[i] != "")
		{
			*file << data->guid[i] + ',' + data->status[i] + ',' + data->installs[i] << std::endl;
		}

	}
}


int vivRunDispo()
{
	VivContainer data;
	VivReadyFileNames finish_names;
	VivInput viv_input_data;

	vivInputs(&viv_input_data);

	std::ifstream dispoFile(viv_input_data.file_name);
	if (!dispoFile.is_open())
	{
		std::cerr << "\n\nFailed to open the file." << std::endl;
		return 1;
	}


	int num_of_lines = vivGetFileLength(&dispoFile);
	vivInitData(&data, num_of_lines);

	int counter = 0;

	while (!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, viv_input_data.separator);
		vivSwitchParser(&counter, &data, line);
	}
	dispoFile.close();

	vivFieldsFormat(&data);
	vivEmptyFieldsFiller(&data);
	vivStatusSetter(&data);
	vivInstallsSetter(&data);
	vivMakeFileNames(&finish_names);

	std::ofstream outFile(finish_names.name);

	vivFileAssembly(&data, &outFile);
	outFile.close();
	system("CLS");

	std::cout << "Vivint is GUCCI." << std::endl;
	
	return 0;
}