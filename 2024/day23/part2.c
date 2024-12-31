/* Day 23, part 2 = gt,ha,ir,jn,jq,kb,lr,lt,nl,oj,pp,qh,vy */

#include "../../aoc.h"
#include "computers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Computers *computers = getComputers(aoc.input);
    char *answer = NULL;

    if (computers) {
        struct Computer *computer;
        struct Computers *computerConnections;
        struct Computer *connection;
        struct Computers *connectionConnections;
        struct Computers *lan = (struct Computers *)calloc(1, sizeof(struct Computers));

        char *password = NULL;
        struct Strings *passwords = (struct Strings *)calloc(1, sizeof(struct Strings));
        struct String *passwordCount;
        int maxComputers = 0;

        for (int i = 0; i < computers->count; i++) {
            computer = computers->data[i];
            computerConnections = computer->connections;

            for (int j = 0; j < computerConnections->count; j++) {
                connection = computerConnections->data[j];
                connectionConnections = connection->connections;

                lan->count = 0;
                addComputer(lan, computer, true, true);
                addComputer(lan, connection, true, true);

                for (int k = 0; k < computerConnections->count; k++) {
                    if (findComputer(connectionConnections, computerConnections->data[k]->name, 
                        0, connectionConnections->count)) {
                        addComputer(lan, computerConnections->data[k], true, true);
                    }
                }

                for (int k = 0; k < connectionConnections->count; k++) {
                    if (findComputer(computerConnections, connectionConnections->data[k]->name, 
                        0, computerConnections->count)) {
                        addComputer(lan, connectionConnections->data[k], true, true);
                    }
                }

                qsort(lan->data, lan->count, sizeof(struct Computer *), compareComputers);

                password = (char *)malloc(lan->count * 3);
                strcpy(password, "");

                for (int k = 0; k < lan->count; k++) {
                    sprintf(password + strlen(password), "%s%s", lan->data[k]->name, 
                        k < lan->count - 1 ? "," : "");
                }

                passwordCount = getString(passwords, password);

                if (++passwordCount->count > maxComputers) {
                    maxComputers = passwordCount->count;
                    answer = (char *)realloc(answer, strlen(password) + 1);
                    strcpy(answer, password);
                }

                free(password);
            }
        }

        freeComputers(lan, false);
        freeComputers(computers, true);
        freeStrings(passwords, true);
    }

    print(aoc, STRING, answer);

    free(answer);

    return 0;
}
