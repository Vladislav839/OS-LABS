#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "dirent.h"
#include "string.h"
#include "unistd.h"
#include "signal.h"

int processId;
char processName[100];

bool isNumber(char* str)
{
	for(int i = 0; i < strlen(str); i++)
	{
		if(!isdigit(str[i])) return false;
	}
	return true;
}

std::string to_string(int n)
{
        std::ostringstream stm;
        stm << n ;
        return stm.str() ;
}

const char* getProcNameFromFile(const char* fileName)
{
	std::ifstream fin(fileName);
	std::string name;
	fin >> name;
	fin >> name;
	return name.substr(1, name.length() - 2).c_str();
}

void getProcessName(int argv, char* args[])
{
	for (int i = 0; i < argv; i++)
	{
		if ((strcmp(args[i], "--name") == 0) && i + 1 < argv)
		{
			char* process_name = args[i + 1];

			strcpy(processName, process_name);
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

void killProcessById()
{
	DIR* dir = opendir("/proc");
	if(dir == NULL) {
		std::cout << "Can't open directory\n";
		exit(-1);
	}
	struct dirent* entity;
	entity = readdir(dir);
	while (entity != NULL)
	{
		if(isNumber(entity->d_name))
		{
			int id = atoi(entity->d_name);
			if(processId == id) 
			{
				int ret;
				ret = kill(id,SIGHUP);
				if(ret == 0) {
					std::cout << "Complete\n";
				} else {
					std::cout << "Error occured while killing the process\n";
				}
			}
		}
		entity = readdir(dir);
	}
	closedir(dir);
}

void killProcessByName(const char* proc_name)
{
    DIR* dir = opendir("/proc");
	if(dir == NULL) {
		std::cout << "Can't open directory\n";
		exit(-1);
	}
	struct dirent* entity;
	entity = readdir(dir);
	while (entity != NULL)
	{
		if(isNumber(entity->d_name))
		{
			int id = atoi(entity->d_name);
			char processDir[256];
  			strcpy(processDir, "/proc/");      
			strcat(processDir, entity->d_name);
			strcat(processDir, "/stat");
			if(strcmp(getProcNameFromFile(processDir), proc_name) == 0)
			{
				int ret;
				ret = kill(id,SIGHUP);
				if(ret == 0) {
					std::cout << "Complete\n";
				} else {
					std::cout << "Error occured while killing the process\n";
				}
			}
		}
		entity = readdir(dir);
	}
	closedir(dir);
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
		token += ".exe";
		for(int i = 0; i < token.length(); i++) {
			::tolower(token[i]);
		}
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void killEnvProcesses()
{
	char* env_text = std::getenv("PROC_TO_KILL");
	if(env_text != NULL) {
		std::string text(env_text);
		std::vector<std::string> tokens = split(text, ",");
		for(int i = 0; i < tokens.size(); i++)
		{
			if(tokens[i].find(".exe") != -1)
			{
				tokens[i].erase(tokens[i].end() - 4, tokens[i].end());
			}
			killProcessByName(tokens[i].c_str());
		}
	}
}

int main(int argc, char* argv[])
{
	getProcessId(argc, argv);
	getProcessName(argc, argv);
	killProcessByName(processName);
	killProcessById();
	killEnvProcesses();
    return 0;
}