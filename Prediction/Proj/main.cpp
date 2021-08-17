#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <vector>

#include "Gamble.h"

/*

Реализовать класс Gamble, который с помощью рандома загадывает простое число.
Класс реализует функцию MakePrediction(std::string userName, int prediction)
С консоли пользователи вводят свои предположения, для этого мы запрашиваем у пользователя его имя и число.
После ввода имени и числа создается поток, который вызывает MakePrediction.
MakePrediction добавляет предсказание в мембер класса Gamble (std::map).
Через 2 минуты после старта приложения,
Gamble определяет победителя для этого находит ставку наиболее близкую к загаданному числу и выводит на экран.

Доп задача: std::map нельзя использовать многопоточно, т.е. доступ к ней должен быть синхронизирован.
Попробуйте синхронизировать доступ используя CriticalSection из Winapi
*/

struct MyErrorException
{
	std::vector<std::string> myError = { "Can't open thread" };
} errorVec;
struct FunctData
{
	FunctData(Gamble& firstP, std::vector<std::pair<std::string, int> >& userD)
		: firstPrediction(firstP), userData(userD) {};
	Gamble& firstPrediction;
	std::vector<std::pair<std::string, int> >& userData;
};
LONG isLocked = FALSE;

CRITICAL_SECTION CriticalSectionTemp;
void InitUserData(std::vector<std::pair<std::string, int> >& userData);
DWORD WINAPI Timer(LPVOID timeStart);
DWORD WINAPI ThreadPrediction(LPVOID userData);

int main()
{
	try
	{
		auto start = std::chrono::steady_clock::now();
		std::cout << " Please enter user name and forecast number between 0 and 999 " << std::endl;
		Gamble firstPrediction;
		DWORD threadId = 0;

		HANDLE thread0 = CreateThread(
			0,
			0,
			Timer,
			(LPVOID)&start,
			0,
			&threadId

		);
		if (thread0 == 0)
		{
			throw errorVec.myError[0];
		}
		std::vector<std::pair<std::string, int> > userData(100, { "", {} });
		InitUserData(userData);

		DWORD threadId2 = 2;
		std::vector<HANDLE> thread2 = {};
		FunctData data(firstPrediction, userData);

		InitializeCriticalSection(&CriticalSectionTemp);

		for (int i = 0; i < 5; ++i)
		{

			thread2.emplace_back(CreateThread(
				NULL,
				0,
				ThreadPrediction,
				(LPVOID)&data,
				0,
				&threadId2
			));
			if (thread2[i] == 0)
			{
				throw errorVec.myError[0];
			}
		}
		WaitForMultipleObjects(thread2.size(), &thread2[0], TRUE, INFINITE);
		std::for_each(thread2.begin(), thread2.end(), [](const auto& val) {
			CloseHandle(val);
			});
		while (!isLocked)
		{
			Sleep(0);
		};
		WaitForSingleObject(thread0, INFINITE);
		CloseHandle(thread0);

		DeleteCriticalSection(&CriticalSectionTemp);
		firstPrediction.FindingWinner();
	}
	catch (const MyErrorException& ex)
	{
		std::cout << &ex << " Error number : " << GetLastError() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << &ex << std::endl;
	}


	return 0;
}

void InitUserData(std::vector<std::pair<std::string, int> >& userData)
{
	int userId = 0;
	for (auto it = userData.begin(); it != userData.end(); ++it, ++userId)
	{
		it->first = std::string("User" + std::to_string(userId) + " : ");
		it->second = rand() % 999;
	}
};

DWORD WINAPI Timer(LPVOID timeStart)
{
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
		*static_cast<std::chrono::steady_clock::time_point*>(timeStart)).count() <= 72000)
	{
		Sleep(0);
	};
	std::cout << "timer end" << std::endl;
	isLocked = TRUE;
	return 0;
}
DWORD WINAPI ThreadPrediction(LPVOID userData)
{
	EnterCriticalSection(&CriticalSectionTemp);
	auto temp = *static_cast<FunctData*>(userData);
	for (auto it : temp.userData)
		temp.firstPrediction.MakePrediction(it.first, it.second);
	LeaveCriticalSection(&CriticalSectionTemp);

	return 0;
}