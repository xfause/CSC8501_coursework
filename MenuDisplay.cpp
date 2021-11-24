#include<string>

template<typename T>
void MenuDisplay(T& p, std::string value = "") {
	p.display(value);
}