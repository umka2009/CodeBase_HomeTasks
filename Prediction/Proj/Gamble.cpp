#include "Gamble.h"

void Gamble::MakePrediction(const std::string userName, int prediction)
{
		userData.emplace(userName, prediction);
};
void Gamble::FindingWinner()
{
	std::pair<std::string, int> searchUser("", {});
	int max = abs(number - userData.begin()->second);
	std::for_each(userData.begin(), userData.end(), [&searchUser, &max, number = number](auto checkNum) {
		if (abs(number - checkNum.second) <= max)
		{
			max = abs(number - checkNum.second);
			searchUser = checkNum;
		}
		});
	std::cout << "Randomly selected number : " << number << std::endl;
	std::cout << "Winner : " << searchUser.first << " number : " << searchUser.second << std::endl;
};