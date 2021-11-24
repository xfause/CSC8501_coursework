#pragma once
#include<string>
#include<list>

std::string ListToString(const std::list<int>& x);

std::string DecryptPassword(std::string encrypted, std::string decryptedASCII, int a, int tag, bool compare = true);

std::string EncryptPassword(std::string origin);

int calcSteps(int a);

