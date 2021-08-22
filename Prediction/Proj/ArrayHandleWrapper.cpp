#include "ArrayHandleWrapper.h"

void ArrayHandleWrapper::AddHendle(HANDLE handle = INVALID_HANDLE_VALUE)
{
	thread2.emplace_back(handle);
};
HANDLE ArrayHandleWrapper::Get(int iter) const 
{
	return thread2[iter]; 
};
int ArrayHandleWrapper::Size() const 
{ 
	return thread2.size(); 
};
const HANDLE& ArrayHandleWrapper::operator[] (size_t iter) const 
{
	return thread2[iter]; 
};
ArrayHandleWrapper::~ArrayHandleWrapper()
{
	for (auto it : thread2)
	{
		if (it != INVALID_HANDLE_VALUE)
		{
			CloseHandle(it);
			it = INVALID_HANDLE_VALUE;
		}
	}
};