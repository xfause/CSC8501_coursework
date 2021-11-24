#include<iostream>
#include<fstream>
#include<cstring>
#include <stdexcept>
#include<vector>
#include<map>
#include"Menu.h"
#include"Encrypt.h"

bool Op1CheckUsername(std::string value, BaseMenu bm) {
    std::map<std::string, std::string> pwdList;

    std::string spaceDelimiter = " ";
    try {
        std::string str;
        std::ifstream passwordFile;
        passwordFile.open("./Result/password.txt");
        if (passwordFile.fail()) {
            throw std::invalid_argument("Unable To Open File: password.txt");
        }
        while (getline(passwordFile, str))
        {
            size_t pos = 0;
            if ((pos = str.find(spaceDelimiter)) == std::string::npos) {
                continue;
            }
            else {
                std::string username = str.substr(0, pos);
                str.erase(0, pos + spaceDelimiter.length());
                std::string password = str;
                pwdList[username] = password;
            }
        }
        if (pwdList.size() == 0) {
            throw std::invalid_argument("Password List Is Empty");
        }
        if (passwordFile.eof()) {
            passwordFile.close();
        }
        else {
            throw std::invalid_argument("Unable To Read File: password.txt");
        }

        bool isUsernameInFile = false;

        for (std::map<std::string, std::string>::iterator iter = pwdList.begin();
            iter != pwdList.end();
            iter++) {
            if ((*iter).first == value) {
                isUsernameInFile = true;
            }
        }
        if (!isUsernameInFile) {
            return true;
        }
        else {
            return false;
        }
    }
    catch (std::exception const& e) {
        std::cout << "There Is An Error: " << e.what() << std::endl;
        MenuDisplay(bm);
        return false;
    }
}

bool Op1CheckPassword(std::string password) {
	if (password.length() < 1) {
		return false;
	}
	bool flag = true;
	for (int i = 0; i < password.length(); i++) {
		if (password[i] < 32 || password[i]>127) {
			flag = false;
		}
	}
	return flag;
}

bool SaveUsernameAndPassword(OptionMenu_1 om1, BaseMenu bm) {
	std::string username, password;

	std::cin.ignore();
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			std::getline(std::cin, username);
		}
		else {
			std::getline(std::cin, password);
		}
	}

	if (!Op1CheckUsername(username, bm)) {
        MenuDisplay(om1, "user_exist");
		return false;
	}

	if (!Op1CheckPassword(password)) {
        MenuDisplay(om1, "password_illegal");
        return false;
	}

	std::ofstream file;
	file.open("./Result/password.txt", std::ios_base::app);
	file << username << " " << EncryptPassword(password) << "\n";
	file.close();
	return true;

}