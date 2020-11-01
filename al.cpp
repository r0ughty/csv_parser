#define _CRT_SECURE_NO_WARNINGS
#include "al_h.h"
#include "utility.h"
#include <unordered_set>

void aldInputs(AldInput* input_data)
{
	std::cout << "--------------Please note that the file must be in CSV format--------------\n" << std::endl;
	std::cout << "Enter the file name with extention (Example.csv): ";
	input_data->file_name = getStrInput(std::cin);
}

int aldGetFileLength(std::ifstream* file)
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

void aldInitData(AldContainer* data, int num_of_elems)
{
	data->id.reserve(num_of_elems);
	data->progress.reserve(num_of_elems);
	data->tier.reserve(num_of_elems);
	data->phone.reserve(num_of_elems);
	data->c250.reserve(num_of_elems);
	data->c500.reserve(num_of_elems);
	data->c700.reserve(num_of_elems);
}

bool aldMakeFileNames(AldReadyFileNames* names)
{

	char buff[100];
	time_t calendar = time(NULL);
	struct tm* time_data;
	time_data = localtime(&calendar);

	strftime(buff, 80, "LeadAlder_%d-%m-%Y.csv", time_data);
	names->guid_name = buff;

	strftime(buff, 80, "PhoneAlder_%d-%m-%Y.csv", time_data);
	names->phone_name = buff;

	return true;
}

void aldSwitchParser(int* num, AldContainer* data, std::string line)
{
	enum FileColumnNumbers { id = 4, progress = 5, tier = 6, phone = 7 };
	enum TotalNumberOfColumnsInFile { total_columns = 9 };

	if (*num >= total_columns)
	{
		*num = 1;
	}

	(*num)++;


	switch (*num)
	{
	case id:
		data->id.push_back(line);
		break;
	case progress:
		data->progress.push_back(line);
		break;
	case tier:
		data->tier.push_back(line);
		break;
	case phone:
		data->phone.push_back(line);
		break;
	default:
		break;
	}
}

void aldPhoneFiller(AldContainer* data)
{
	data->phone[0] = "Caller ID";

	for (int i = 1; i < data->id.size(); i++)
	{
		std::string s = "+1";
		data->phone[i].insert(0, s);
	}
}

void aldInitTier(AldContainer* data)
{
	for (int i = 0; i < data->id.size(); i++)
	{
		data->c250.push_back("");
		data->c500.push_back("");
		data->c700.push_back("");
	}
}

void aldTierSetter(AldContainer* data)
{
	for (int i = 0; i < data->tier.size(); i++)
	{
		if (i == 0)
		{
			data->c250[i] = "250";
			data->c500[i] = "500";
			data->c700[i] = "700";
		}
		else if (data->tier[i] == "250")
		{
			data->c250[i] = "1";
		}
		else if (data->tier[i] == "500")
		{
			data->c500[i] = "1";
		}
		else if (data->tier[i] == "700")
		{
			data->c700[i] = "1";
		}
	}
}

void aldRemoveDuplicates(AldContainer* data)
{
	std::unordered_set<std::string> s;

	for (int i = 1; i < data->id.size(); i++)
	{
		if (s.find(data->id[i]) != s.end())
		{
			data->id[i] = "";
		}
		else
			s.insert(data->id[i]);
	}

	s.clear();

	for (int i = 1; i < data->phone.size(); i++)
	{
		if (s.find(data->phone[i]) != s.end())
		{
			data->phone[i] = "";
		}
		else
			s.insert(data->phone[i]);
	}
}

void aldFileAssembly(AldContainer* data, std::ofstream* idFile, std::ofstream* phoneFile)
{
	for (int i = 0; i < data->progress.size(); i++)
	{
		if (data->id[i] != "")
		{
			*idFile << data->id[i] + ',' + data->progress[i] + ',' + data->c250[i] + ',' + data->c500[i] + ',' + data->c700[i] << std::endl;
		}

		if (data->phone[i] != "")
		{
			*phoneFile << data->phone[i] + ',' + data->progress[i] + ',' + data->c250[i] + ',' + data->c500[i] + ',' + data->c700[i] << std::endl;
		}
	}

	idFile->close();
	phoneFile->close();
}

int aldRunDispo()
{
	AldContainer data;
	AldReadyFileNames finish_names;
	AldInput ald_input_data;

	aldInputs(&ald_input_data);

	std::ifstream dispoFile(ald_input_data.file_name);
	if (!dispoFile.is_open())
	{
		system("CLS");
		std::cerr << "\n\nFailed to open the file.\n" << std::endl;
		return 1;
	}

	if (!fileValidity(&dispoFile, "Progress"))
	{
		system("CLS");
		std::cerr << "\n\nLooks like you've opened a wrong file.\n" << std::endl;
		return 1;
	}

	int num_of_lines = aldGetFileLength(&dispoFile);
	aldInitData(&data, num_of_lines);

	int counter = 0;

	while (!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, ald_input_data.separator);
		aldSwitchParser(&counter, &data, line);
	}
	dispoFile.close();
	
	aldPhoneFiller(&data);
	aldInitTier(&data);
	aldTierSetter(&data);
	aldRemoveDuplicates(&data);
	aldMakeFileNames(&finish_names);

	std::ofstream idFile(finish_names.guid_name);
	std::ofstream phoneFile(finish_names.phone_name);

	aldFileAssembly(&data, &idFile, &phoneFile);
	system("CLS");
	std::cout << "Alder is GUCCI.\n" << std::endl;
}