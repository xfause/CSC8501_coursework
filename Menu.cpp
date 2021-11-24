#include<iostream>
#include<cstring>
#include "Menu.h"

BaseMenu::BaseMenu() { }
BaseMenu::~BaseMenu(){ }
void BaseMenu::display(std::string param) {
	std::cout << "Select Option:" << std::endl;
	std::cout << "1.Create username / password" << std::endl;
	std::cout << "2.Check username and password" << std::endl;
	std::cout << "3.Generate password strength analysis file" << std::endl;
	std::cout << "4.Analys password strength analysis file" << std::endl;
	std::cout << "5.Decrypt specfic password" << std::endl;
	std::cout << "6.Quit" << std::endl;
	std::cout << "Input Option Code:";
}

OptionMenu_1::OptionMenu_1() { }
OptionMenu_1::~OptionMenu_1() { }
void OptionMenu_1::display(std::string param) {
	if (param == "") {
		std::cout << "Please Input Username and Password in 2 new line:" << std::endl;
	}
	if (param == "success") {
		std::cout << "Write To File Success." << std::endl;
	}
	if (param == "fail") {
		std::cout << "Write To File Failed." << std::endl;
	}
	if (param == "user_exist") {
		std::cout << "User Exist." << std::endl;
	}
	if (param == "password_illegal") {
		std::cout << "Password Illegal." << std::endl;
	}
}

OptionMenu_2::OptionMenu_2() { }
OptionMenu_2::~OptionMenu_2() { }

void OptionMenu_2::display(std::string param) {
	if (param == "") {
		std::cout << "Please Input Username first:" << std::endl;
	}
	if (param == "username_not_exist") {
		std::cout << "Username Not In File." << std::endl;
	}
	if (param == "after") {
		std::cout << "Please Input Password:" << std::endl;
	}
	if (param == "retry") {
		std::cout << "Password Error, Please Retry:" << std::endl;
	}
	if (param == "retry_end") {
		std::cout << "You have tried 3 times." << std::endl;
	}
	if (param == "check_success") {
		std::cout << "success!" << std::endl;
	}
	if (param == "check_failed") {
		std::cout << "failure!" << std::endl;
	}
}

OptionMenu_3::OptionMenu_3() { }
OptionMenu_3::~OptionMenu_3() { }
void OptionMenu_3::display(std::string param) {
	if (param == "") {
		std::cout << "Generating...." << std::endl;
	}
	if (param == "success") {
		std::cout << "Generating Success." << std::endl;
	}
}

OptionMenu_4::OptionMenu_4() { }
OptionMenu_4::~OptionMenu_4() { }
void OptionMenu_4::display(std::string param) {
	if (param == "") {
		std::cout << "Analysing...." << std::endl;
	}
	if (param == "success") {
		std::cout << "Analysing Success." << std::endl;
	}
}
