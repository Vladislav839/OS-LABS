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
int main()
{
	char variable_args[100];
	strcpy(variable_args, "chrome,telegram");
	if (!SetEnvironmentVariableA("PROC_TO_KILL", variable_args)) {
		std::cout << "Can't set variable\n";
		return 1;
	}
	const int processIdToKill = 5560;
	StartKillerWithId(processIdToKill);

	if (!SetEnvironmentVariableA("PROC_TO_KILL", NULL)) {
		std::cout << "Can't delete variable\n";
		return 1;
	}

	return 0;
}