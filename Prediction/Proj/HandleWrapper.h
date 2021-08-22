#pragma once
#ifndef HANDLEWRAPPER_H
#define HANDLEWRAPPER_H

#include <Windows.h>
class HandleWrapper
{
public:
	HandleWrapper(HANDLE handle = INVALID_HANDLE_VALUE) : m_handle(handle) {};
	HANDLE Get() const { return m_handle; };
	~HandleWrapper()
	{
 		if (m_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}
	HandleWrapper(const HandleWrapper& wrapper) = delete;
	HandleWrapper& operator=(const HandleWrapper& wrapper) = delete;

private:
	HANDLE m_handle;
};
#endif // !HANDLEWRAPPER_H
