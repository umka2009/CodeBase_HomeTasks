#pragma once
#ifndef GAMBLE_H
#define GAMBLE_H
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
class Gamble
{
public:
	Gamble() : number(rand() % 999), userData({}) {};  
	void MakePrediction(const std::string userName, int prediction);
	void FindingWinner();
 protected:

 private:
	int number;
	std::map<std::string, int> userData;
};

/*
Класс реализует функцию MakePrediction(std::string userName, int prediction)
MakePrediction добавляет предсказание в мембер класса Gamble (std::map).
*/
#endif // !GAMBLE_H
