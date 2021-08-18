#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Process.h"
unsigned int const APROCSIZE = 1024;

void GetProcesses(DWORD& arrayProcesses);
void InitDataProcessesAndId(std::wstring& nameProcess);
std::wstring GetNameProcess(const DWORD& arrayProcess);
void SelectionOfProcessForKill(const std::vector<std::pair<std::wstring, DWORD> >& foundProcess);
void KillProcess(DWORD selected);
DWORD SelectProcess(const std::vector<std::pair<std::wstring, DWORD> >& foundProcess);
struct MyFormatException
{
	MyFormatException(const wchar_t* str_) : exception(str_) {};
	void SetException(const wchar_t* str_)
	{
		exception = str_;
	};
	std::wstring exception;
};
std::vector<MyFormatException> myException = 
{ 
L"Error Can not load Processes",         // 0
L"Error Such a process does not exist",  // 1
L"Error Open Process",					// 2
L"Error Get Priority Class",			// 3
L"Error Close Process"					// 4
};


int main()
{
	try
	{
		std::wstring nameProcess;
		InitDataProcessesAndId(nameProcess);
		DWORD arrayProcesses[APROCSIZE];
		GetProcesses(*arrayProcesses);
		std::vector<std::pair<std::wstring, DWORD> > foundProcess;
		auto selection = [arrayProcesses, nameProcess, &foundProcess](auto it)
		{
			std::wstring namePTemp = GetNameProcess(it);
			if (namePTemp == nameProcess)
				foundProcess.emplace_back(namePTemp, it);
			return &it == arrayProcesses + APROCSIZE;
		};
		std::find_if(arrayProcesses, arrayProcesses + APROCSIZE, selection);	
		SelectionOfProcessForKill(foundProcess);
	}
	catch(const MyFormatException& ex)
	{
		std::wcout << ex.exception;
	}
	return 0;
}
void GetProcesses(DWORD& arrayProcesses)
{
	DWORD cbNeeded;
	if (!EnumProcesses(&arrayProcesses, sizeof(arrayProcesses) * APROCSIZE, &cbNeeded))
	{
		throw myException[0];
	}
}
void InitDataProcessesAndId(std::wstring& nameProcess)
{
	std::cout << "Enter the name of the desired process to kill/stop" << std::endl;
	std::cout << "name : ";
	std::wcin >> nameProcess;
}
std::wstring GetNameProcess(const DWORD& arrayProcess)
{
	wchar_t szProcessName[MAX_PATH] = TEXT("");
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, arrayProcess);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),
			&cbNeeded, 0x03))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(wchar_t));
		}
		CloseHandle(hProcess);
	}

	return szProcessName;
}
DWORD SelectProcess(const std::vector<std::pair<std::wstring, DWORD> >& foundProcess)
{
	for (const auto& it : foundProcess)
		std::wcout << "Process : " << it.first << "ID : " << it.second << std::endl;
	
	DWORD select;
	std::cout << "Please enter an existing process ID : " ;
	bool test = true;
	auto selection = [select](auto it){ return it.second == select;};
	do {
		if(!test)
			std::cout << " \nThe ID is incorrect\n Please enter an existing process ID : ";
		std::cin >> select;
		test = false;
	} while (find_if(begin(foundProcess), end(foundProcess), selection) != end(foundProcess));
	return select;
};
void SelectionOfProcessForKill(const std::vector<std::pair<std::wstring, DWORD> >& foundProcess)
{
	if (foundProcess.size() == 1)
	{
		KillProcess(foundProcess.begin()->second);
	}
	else if (foundProcess.size() > 1)
	{
		KillProcess(SelectProcess(foundProcess));
	}
	else
	{
		throw myException[1];
	}
};
void KillProcess(DWORD selected)
{
	DWORD dwPriorityClass = 0;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,
		FALSE, selected);
	if (hProcess == NULL)
		throw myException[2];
	else
	{
		if (TerminateProcess(hProcess, 0)) 
			std::cout << "Ok ";
		else
			throw myException[3];
	}
};