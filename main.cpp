#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int connectProcesses(const char* processName, int fd1, int fd2)
{
    int id = fork();
    if(id < 0) {
        std::cout << "Error ocurred in fork()\n";
        exit(1);
    }
    if(id == 0)
    {
        // Child process
        // copy first file descriptor and point it to stdin of new process
        int err1 = dup2(fd1, 0);
        // copy second file descriptor and point it to stdout of new process
        int err2 = dup2(fd2, 1);
        if(err1 == -1 || err2 == -1) {
            std::cout << "Error ocurred in dup2()\n";
            exit(2);
        }

        // replace current process with A/M/S/P process
        int err3 = execl(processName, processName, NULL);
        if(err3 == -1) {
            std::cout << "Error ocurred in execl()\n";
            exit(3);
        }
        exit(0);
    }
    return id;
}

int main()
{
    int m_a_connector[2];
    int a_p_connector[2];
    int p_s_connector[2];

    std::cout << "Stop character is \\0\n";
    std::cout << "Insert numbers\n";
    int id;
    // create pipe between m and a
    pipe(m_a_connector);
    // keep stdin as default, point m stdout to a stdin
    id = connectProcesses("./m", 0, m_a_connector[1]);
    // create pipe between a and p
    pipe(a_p_connector);
    // point a stdin to m stdout, point a stdout to p stdin
    id = connectProcesses("./a", m_a_connector[0], a_p_connector[1]);
    // create pipe between p and s
    pipe(p_s_connector);
    // point p stdin to a stdout, point p stdout to s stdin
    id = connectProcesses("./p", a_p_connector[0], p_s_connector[1]);
    // point s stdin to p stdout, point s stdout to main stdout
    id = connectProcesses("./s", p_s_connector[0], 1);

    // wait until process chain not finished
    waitpid(id, NULL, 0);
    return 0;
}