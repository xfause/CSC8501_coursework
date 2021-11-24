#include <fstream>
#include <iostream>
#include <stdexcept>
#include<vector>
#include"Menu.h"
#include"Encrypt.h"

bool Op2CheckPasswordIsCorrect(std::string pInFile, std::string inputPassword) {
    std::string encryptedPassword = EncryptPassword(inputPassword);
    if (encryptedPassword == pInFile) {
        return true;
    }
    else {
        return false;
    }
}

void Op2RetryLoop(OptionMenu_2 om2, int times, std::string passwordInFile) {
    if (times <= 3) {
        MenuDisplay(om2, "after");
        std::string inputPassword;
        if (times == 1) {
            std::getline(std::cin, inputPassword);
        }
        std::getline(std::cin, inputPassword);
        if (Op2CheckPasswordIsCorrect(passwordInFile, inputPassword)) {
            MenuDisplay(om2, "check_success");
            return;
        }
        else {
            MenuDisplay(om2, "check_failed");
            Op2RetryLoop(om2, times + 1 , passwordInFile);
            return;
        }
    }
    else {
        MenuDisplay(om2, "retry_end");
        return;
    }

    
}

void Op2CheckPasswordOfUser(OptionMenu_2 om2, BaseMenu bm) {

    auto passwordListSharedPtr = std::make_shared<std::vector<std::pair<std::string, std::string>>>();

    std::string spaceDelimiter =  " ";

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
                passwordListSharedPtr->push_back(std::make_pair(username, password));
            }
        }
        if (passwordListSharedPtr->size() == 0) {
            throw std::invalid_argument("Password List Is Empty");
        }
        if (passwordFile.eof()) {
            passwordFile.close();
        }
        else {
            throw std::invalid_argument("Unable To Read File: password.txt");
        }

        MenuDisplay(om2);

        std::string inputUsername, passwordInFile;
        bool isUsernameInFile = false;
        std::cin >> inputUsername;

        for (std::vector<std::pair<std::string, std::string>>::iterator iter = passwordListSharedPtr->begin();
            iter != passwordListSharedPtr->end();
            iter++){
            if ((*iter).first == inputUsername) {
                isUsernameInFile = true;
                passwordInFile = (*iter).second;
            }
        }

        if (!isUsernameInFile) {
            MenuDisplay(om2, "username_not_exist");
            MenuDisplay(bm);
            return;
        }
        else {
            Op2RetryLoop(om2, 1, passwordInFile);
			MenuDisplay(bm);
            return;
        }

    }
    catch (std::exception const& e) {
        std::cout << "There Is An Error: " << e.what() << std::endl;
        MenuDisplay(bm);
    }
}