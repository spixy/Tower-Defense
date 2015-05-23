#include "GameData.h"

std::string Append(std::string str1, int n, std::string str2)
{
	std::ostringstream oss;
	oss << str1 << n << str2;
	return oss.str();
}