#pragma once
#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

class Process
{
public:
	Process()
protected:

private:
	std::vector<std::unique_ptr<HANDLE> > hProcess;
};

