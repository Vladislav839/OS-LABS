#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>
#include <comdef.h>
#include <string>

#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
#include <algorithm>


void StartKillerWithId(int id)
{
	std::cout << "Process list with current id before killing\n";
	std::string cmd_tasks = "tasklist /fi \"pid eq ";
	cmd_tasks += std::to_string(id) + "\"";
	system(cmd_tasks.c_str());
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));
	std::string cmd = "--id ";
	cmd += std::to_string(id);
	BSTR b = _com_util::ConvertStringToBSTR(cmd.c_str());
	CreateProcess(L"..\\Debug\\Killer.exe", b, NULL, NULL,
		FALSE, NULL, NULL, NULL, &startupInfo, &processInfo);
	std::cout << "Process list with current id after killing\n";
	system(cmd_tasks.c_str());
}

void StartKillerWithName(std::string name)
{
	std::cout << "Process list with current id before killing\n";
	std::string cmd_tasks = "tasklist | find \"" + name + "\"";
	system(cmd_tasks.c_str());
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));
	std::string cmd = "--name ";
	cmd += name;
	BSTR b = _com_util::ConvertStringToBSTR(cmd.c_str());
	CreateProcess(L"..\\Debug\\Killer.exe", b, NULL, NULL,
		FALSE, NULL, NULL, NULL, &startupInfo, &processInfo);
	std::cout << "Process list with current id after killing\n";
	system(cmd_tasks.c_str());
}

int main()
{
	char variable_args[100];
	strcpy(variable_args, "notepad,telegram");
	if (!SetEnvironmentVariableA("PROC_TO_KILL", variable_args)) {
		std::cout << "Can't set variable\n";
		return 1;
	}
	std::cout << "If you want to delete process by id type 1\n";
	std::cout << "If you want to delete process by name type 2\n";
	int option;
	std::string procName;
	std::cin >> option;
	switch (option)
	{
	case 1:
		int id;
		std::cout << "Enter process id\n";
		std::cin >> id;
		StartKillerWithId(id);
		break;
	case 2:
		std::cout << "Enter process name\n";
		std::cin >> procName;
		std::for_each(procName.begin(), procName.end(), [](char& c) {
			c = ::tolower(c);
			});
		StartKillerWithName(procName);
		break;
	default:
		std::cout << "Invalid operation\n";
		break;
	}

	if (!SetEnvironmentVariableA("PROC_TO_KILL", NULL)) {
		std::cout << "Can't delete variable\n";
		return 1;
	}

	return 0;
}
