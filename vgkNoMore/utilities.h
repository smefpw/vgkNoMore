#pragma once
#include <chrono>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <fstream>

#include "termcolor.h"

namespace utilities
{	
	inline BOOL is_process_elevated()
	{
		HANDLE h_token = nullptr;

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
		{
			TOKEN_ELEVATION elevation;
			DWORD dw_size = 0;

			if (GetTokenInformation(h_token, TokenElevation, &elevation, sizeof elevation, &dw_size))
			{
				CloseHandle(h_token);
				return elevation.TokenIsElevated;
			}
			
			MessageBox(nullptr, "GetTokenInformation failed. Program will close.", "vgkNoMore", MB_ICONERROR | MB_OK);
			return FALSE;
		}
		
		MessageBox(nullptr, "OpenProcessToken failed. Program will close.", "vgkNoMore", MB_ICONERROR | MB_OK);
		return FALSE;
	}

	inline void log_type(const int type)
	{
		/*
		type 0, general logging
		type 1, warning logging
		type 2, error logging
		type 3, success logging
		*/
		switch (type)
		{
		case 0: std::cout << termcolor::cyan << "[notice]" << termcolor::white << ' ';
			break;
		case 1: std::cout << termcolor::yellow << "[warning]" << termcolor::white << ' ';
			break;
		case 2: std::cout << termcolor::red << "[error]" << termcolor::white << ' ';
			break;
		case 3: std::cout << termcolor::green << "[success]" << termcolor::white << ' ';
			break;
		default: break;
		}
	}

	inline void elevate_privileges()
	{
		HANDLE h_token = nullptr;
		TOKEN_PRIVILEGES tp;

		if (OpenProcessToken(HANDLE(-1), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token))
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			tp.Privileges[0].Luid.LowPart = 20;
			tp.Privileges[0].Luid.HighPart = 0;

			LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);

			AdjustTokenPrivileges(h_token, FALSE, &tp, 0, nullptr, nullptr);
			CloseHandle(h_token);
		}
	}
}