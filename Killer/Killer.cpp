#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>
#include <comdef.h>
#include <string>
#include <algorithm>

int processId;
char processName[100];

void getProcessName(int argv, char* args[])
{
	for (int i = 0; i < argv; i++)
	{
		if ((strcmp(args[i], "--name") == 0) && i + 1 < argv)
		{
			char* process_name = args[i + 1];

			strcpy(processName, process_name);
			strcat(processName, ".exe");
			return;
		}
	}
	processName[0] = '\0';
}

void getProcessId(int argv, char* args[]) 
{
	for (int i = 0; i < argv; i++)
	{
		if ((strcmp(args[i], "--id") == 0) && i + 1 < argv)
		{
			processId = atoi(args[i + 1]);
			return;
		}
	}
	processId = -1;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

bool consists(std::vector<std::string>& arguments, std::string& argument)
{
	return std::find(arguments.begin(), arguments.end(), argument) != arguments.end();
}

int main(int argv, char* args[])
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	getProcessId(argv, args);
	getProcessName(argv, args);

	char value[100];
	GetEnvironmentVariableA("PROC_TO_KILL", value, sizeof(value));
	std::string arguments_string = "chrome,telegram";

	std::vector<std::string> arguments = split(arguments_string, ",");

	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processSnap == INVALID_HANDLE_VALUE)
		return -1;

	if (Process32First(processSnap, &processInfo) == TRUE)
	{
		while (Process32Next(processSnap, &processInfo) == TRUE)
		{
			_bstr_t b(processInfo.szExeFile);
			//std::cout << b << '\t' << processInfo.th32ProcessID << '\n';
			std::string current_name(b);
			if (((int)processInfo.th32ProcessID == processId) || 
				(strcmp(b, processName) == 0) ||
				consists(arguments, current_name))
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);

				bool terminated = TerminateProcess(hProcess, 0);

				if (!terminated) {
					std::cout << "Can't terminate process\n";
					return -1;
				}

				CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(processSnap);
	return 0;
}