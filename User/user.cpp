#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>

#include "signal.h"
#include "unistd.h"
#include "dirent.h"
#include "stdlib.h"
#include "string.h"
#include "spawn.h"
#include "sys/wait.h"

std::string to_string(int n)
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}

bool isNumber(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

const char *getProcNameFromFile(const char *fileName)
{
    std::ifstream fin(fileName);
    std::string name;
    fin >> name;
    fin >> name;
    return name.substr(1, name.length() - 2).c_str();
}

void printAllProcesses()
{
    DIR *dir = opendir("/proc");
    if (dir == NULL)
    {
        std::cout << "Can't open directory\n";
        exit(-1);
    }
    struct dirent *entity;
    entity = readdir(dir);
    while (entity != NULL)
    {
        if (isNumber(entity->d_name))
        {
            int id = atoi(entity->d_name);
            char processDir[256];
            strcpy(processDir, "/proc/");
            strcat(processDir, entity->d_name);
            strcat(processDir, "/stat");
            std::cout << id << '\t' << getProcNameFromFile(processDir) << '\n';
        }
        entity = readdir(dir);
    }
    closedir(dir);
}

void startKillerWithId(int id)
{
    std::cout << "Processes before killing\n";
    std::cout << "----------------------------\n";
    printAllProcesses();
    std::string id_str = to_string(id);
    int pid = fork();
    int status;
    std::cout << "---------------------------\n";
    if (pid == 0)
    {
        char *args[] = {"../ProcKiller/prockiller", "--id", (char *)id_str.c_str(), NULL};
        execv("../ProcKiller/prockiller", args);
    }
    else
    {
        if (waitpid(pid, &status, 0) != -1)
        {
            printf("Child exited with status %i\n", status);
        }
        else
        {
            printf("waitpid");
        }
    }
    std::cout << "---------------------------\n";
    std::cout << "Processes after killing\n";
    std::cout << "----------------------------\n";
    printAllProcesses();
}

void startKillerWithName(const char *name)
{
    std::cout << "Processes before killing\n";
    std::cout << "----------------------------\n";
    printAllProcesses();
    int pid = fork();
    int status;
    std::cout << "---------------------------\n";
    if (pid == 0)
    {
        char *args[] = {"../ProcKiller/prockiller", "--name", (char*) name, NULL};
        execv("../ProcKiller/prockiller", args);
    }
    else
    {
        if (waitpid(pid, &status, 0) != -1)
        {
            printf("Child exited with status %i\n", status);
        }
        else
        {
            printf("waitpid");
        }
    }
    std::cout << "---------------------------\n";
    std::cout << "Processes after killing\n";
    std::cout << "----------------------------\n";
    printAllProcesses();
}

int main()
{
    if(setenv("PROC_TO_KILL","vim", 1) != 0) {
        std::cout << "Can't set env variable\n";
        return -1;
    }
    printAllProcesses();
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
        startKillerWithId(id);
        break;
    case 2:
        std::cout << "Enter process name\n";
        std::cin >> procName;
        for (int i = 0; i < procName.length(); i++)
        {
            ::tolower(procName[i]);
        }
        startKillerWithName(procName.c_str());
        break;
    default:
        std::cout << "Invalid operation\n";
        break;
    }

    if(unsetenv("PROC_TO_KILL") != 0) {
       std::cout << "Can't delete variable\n";
    	return -1;
    };
    return 0;
}