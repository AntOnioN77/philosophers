#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

enum actions
{
    TAKE_F,
    SLEEP,
    DEAD,
    EAT,
    TINK,
} typedef e_actions;

void verifi_one(char *line_execution,char *logfile)
{
    int fd;
    int timestamp;
    int name;
    e_actions action;

    fd = open(logfile, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return;
    }
    parse();
    

    close(fd);
}

int main(int argc, char **argv, char **env)
{
    int i;
    char *logfile;
    FILE *testfile;
    char line_execution[256];

    execve("./execution.sh", NULL, env);
    testfile = fopen("TEST/test.txt", "r");

    i = 1;
    while (1)
    {
        if (asprintf(&logfile, "TEST/%d.log", i) == -1)
        {
            perror("Error allocating memory");
            break;
        }

        if (access(logfile, R_OK) != 0)
        {
            free(logfile);
            break;
        }
        fgets(line_execution, sizeof(line_execution), testfile);
        verifi_one(line_execution, logfile);
        free(logfile);
        i++;
    }

    fclose(testfile);
    return 0;
}