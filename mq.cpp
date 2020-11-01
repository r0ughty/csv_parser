#define _CRT_SECURE_NO_WARNINGS
#include "mq_h.h"
#include "utility.h"
#include <unordered_set>

void mqInputs(MqInput* input_data)
{
	std::cout << "--------------Please note that the file must be in CSV format--------------\n" << std::endl;
	std::cout << "Enter the file name with extention (Example.csv): ";
	input_data->file_name = getStrInput(std::cin);
}

int mqGetFileLength(std::ifstream* file)
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

bool mqMakeFileNames(MqReadyFileNames* names)
{

	char buff[100];
	time_t calendar = time(NULL);
	struct tm* time_data;
	time_data = localtime(&calendar);

	strftime(buff, 80, "MQ_%d-%m-%Y.csv", time_data);
	names->name = buff;

	return true;
}

void mqInitData(MqContainer* data, int num_of_elems)
{
	data->id.reserve(num_of_elems);
	data->sms_valid.reserve(num_of_elems);
	data->email_valid.reserve(num_of_elems);
	data->sms_click.reserve(num_of_elems);
	data->email_click.reserve(num_of_elems);
	data->buy.reserve(num_of_elems);
	data->out.reserve(num_of_elems);
	data->delivered.reserve(num_of_elems);
	data->status.reserve(num_of_elems);
}


void mqSwitchParser(int* num, MqContainer* data, std::string line)
{
	enum FileColumnNumbers { id = 1, sms_valid = 4, sms_click = 6, email_valid = 8,  email_click = 10, buy = 11, out = 12};
	enum TotalNumberOfColumnsInFile { total_columns = 12 };

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
	case sms_valid:
		data->sms_valid.push_back(line);
		break;
	case sms_click:
		data->sms_click.push_back(line);
		break;
	case email_valid:
		data->email_valid.push_back(line);
		break;
	case email_click:
		data->email_click.push_back(line);
		break;
	case buy:
		data->buy.push_back(line);
		break;
	case out:
		data->out.push_back(line);
		break;
	default:
		break;
	}
}

void mqIdSetter(MqContainer* data)
{
	for (int i = 0; i < data->out.size(); i++)
	{
		std::string id = data->out[i].substr(data->out[i].find("\n") + 1, data->out[i].length());
		data->out[i].erase(data->out[i].find("\n"), data->out[i].length());
		data->id.push_back(id);
	}
}

void mqSmsClickSetter(MqContainer* data)
{
	data->sms_valid[0] = "SMS Valid";
	data->email_valid[0] = "Email Valid";
	for (int i = 1; i < data->email_valid.size(); i++)
	{
		if (data->email_valid[i] == "1")
		{
			data->email_valid[i] = "100";
		}
		else
		{
			data->email_valid[i] = "0.01";
		}

		if (data->sms_valid[i] == "1")
		{
			data->sms_valid[i] = "100";
		}
		else
		{
			data->sms_valid[i] = "0.01";
		}
	}
}

void mqDeliverSetter(MqContainer* data)
{
	data->delivered.push_back("Delivered");

	for (int i = 1; i < data->email_valid.size(); i++)
	{
		int sms = stoi(data->sms_valid[i]);
		int email = stoi(data->email_valid[i]);

		if (sms > 0 || email > 1)
		{
			data->delivered.push_back("Delivered");
		}
		else
		{
			data->delivered.push_back("reset");
		}
	}
}

void mqStatusSetter(MqContainer* data)
{
	data->status.push_back("Status");

	for (int i = 1; i < data->email_valid.size(); i++)
	{
		if (data->out[i] == "1")
		{
			data->status.push_back("call out");
		}
		else if (data->buy[i] != "")
		{
			data->status.push_back("buy click");
		}
		else if (data->email_click[i] != "" || data->sms_click[i] != "")
		{
			data->status.push_back("URL click");
		}
		else
		{
			data->status.push_back(data->delivered[i]);
		}
	}
}

void mqRemoveDuplicates(MqContainer* data)
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
}

void mqFileAssembly(MqContainer* data, std::ofstream* file)
{
	for (int i = 0; i < data->status.size(); i++)
	{
		*file << data->id[i] + ',' << data->status[i] + ',' << data->sms_valid[i] + ',' << data->email_valid[i] << std::endl;
	}

	file->close();
}

int mqRunDispo()
{
	MqContainer data;
	MqReadyFileNames finish_names;
	MqInput mq_input_data;

	mqInputs(&mq_input_data);

	std::ifstream dispoFile(mq_input_data.file_name);
	if (!dispoFile.is_open())
	{
		system("CLS");
		std::cerr << "\n\nFailed to open the file.\n" << std::endl;
		return 1;
	}

	if (!fileValidity(&dispoFile, "sms_error_code"))
	{
		system("CLS");
		std::cerr << "\n\nLooks like you've opened a wrong file.\n" << std::endl;
		return 1;
	}

	int num_of_lines = mqGetFileLength(&dispoFile);
	mqInitData(&data, num_of_lines);

	int counter = 0;

	while (!dispoFile.eof())
	{
		std::string line;
		getline(dispoFile, line, mq_input_data.separator);
		mqSwitchParser(&counter, &data, line);
	}
	dispoFile.close();

	mqMakeFileNames(&finish_names);
	mqIdSetter(&data);
	mqDeliverSetter(&data);
	mqSmsClickSetter(&data);
	mqStatusSetter(&data);

	std::ofstream outFile(finish_names.name);
	
	mqFileAssembly(&data, &outFile);
	system("CLS");
	std::cout << "\nMQ is GUCCI.\n" << std::endl;
}
