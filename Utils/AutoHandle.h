#pragma once
#include "pch.h"

struct HandleDeleter
{
	void operator()(const HANDLE& handle)
	{
		if (handle)
		{
			CloseHandle(handle);
		}
	}
};

struct WinHttpHandleDeleter
{
	void operator()(const HINTERNET& handle)
	{
		if (handle)
		{
			WinHttpCloseHandle(handle);
		}
	}
};

/* RAII wrapper for HANDLEs. */
using AutoHandle = std::unique_ptr<void, HandleDeleter>;

/* RAII wrapper for HINTERNET handle. */
using AutoHttpHandle = std::unique_ptr<void, WinHttpHandleDeleter>;