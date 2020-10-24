#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "protect_util_h.h"
#include "viv_util.h"
#include "utility.h"

void menu()
{
	std::cout << "=======BEST MENU YOU'VE EVER SEEN=======\n" << std::endl;
	std::cout << "1. Protect" << std::endl;
	std::cout << "2. Vivint" << std::endl;
	std::cout << "0. Exit" << std::endl;
}



int main()
{
	int choice;
	int loop = 1;


	while (loop)
	{ 
		menu();
		std::cout << "Input: ";
		choice = getIntInput(std::cin);
		while (choice < 0 || choice > 2)
		{
			choice = getIntInput(std::cin);
		}

		switch (choice)
		{
		case 1:
			protectRunDispo();
			break;
		case 2:
			vivRunDispo();
			break;
		case 0:
			loop = 0;
			break;
		default:
			break;
		}	
	}


	return 0;
}