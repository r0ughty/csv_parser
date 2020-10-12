#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>



struct container
{
	std::vector<std::string> affiliate_code;
	std::vector<std::string> finish_code;
	std::vector<std::string> is_lead;
	std::vector<std::string> is_sale;
	std::vector<std::string> submitted_email;
	std::vector<std::string> beacon_range;
	std::vector<std::string> status;
};

struct ready_file_names
{
	std::string email_name;
	std::string guid_name;
};

struct input
{
	std::string file_name;
	char separator = ',';
	int separator_choice = 0;
	int file_type = 0;
};

void inputs(input* input);
bool make_file_name(ready_file_names* names, int choice);
int get_file_length(std::ifstream* file);
void init_data(container* data, int num_of_elems);
void switch_parser(int* num, container* data, std::string line);
void status_filler(container* data);
void beacon_range_formatting(container* data);
void affiliate_code_formatting(container* data);
void status_setter(container* data);
void remove_duplicates(container* data);
void file_assembly(container* data, std::ofstream* emailFile, std::ofstream* guidFile);