#pragma once
#ifndef ARRAYHANDLEWRAPPER_H
#define ARRAYHANDLEWRAPPER_H
#include <vector>
#include "HandleWrapper.h"
class ArrayHandleWrapper : public HandleWrapper
{
public:
	ArrayHandleWrapper() {};
	void AddHendle(HANDLE handle );
	HANDLE Get(int iter) const ;
	int Size() const ;
	const HANDLE& operator[] (size_t iter) const ;
	~ArrayHandleWrapper();

	ArrayHandleWrapper(const ArrayHandleWrapper& wrapper) = delete;
	ArrayHandleWrapper& operator=(const ArrayHandleWrapper& wrapper) = delete;
private:
	std::vector<HANDLE> thread2 = { INVALID_HANDLE_VALUE };
};
#endif // !ARRAYHANDLEWRAPPER_H
