#include "../intcode.h"
#include <sys/ioctl.h>
#include <signal.h>

struct Packets {
    long destination;
    long x;
    long y;
    long *packet;
};

int networkPacket(char *input, bool runNAT) {
    struct Program *program = loadProgram(input);
    int packet = 0;

    if (program) {
        struct Program *programs[50];
        struct Packets packets[50];
        long data;
        int status = 0;
        int on = 1;
        struct Program *nat;
        struct Packets natPackets = { 0, 0, 0, NULL };
        struct Packets resumePackets = { 0, 0, 0, NULL };

        for (int i = 0; i < 50; i++) {
            programs[i] = copyProgram(program, i);

            pipe(programs[i]->input);
            pipe(programs[i]->output);

            packets[i].packet = &packets[i].destination;
        }

        for (int i = 0; i < 50; i++) {
            programs[i]->identifier = fork();

            if (programs[i]->identifier == 0) {
                for (int j = 0; j < 50; j++) {
                    if (j != i) {
                        close(programs[j]->input[WRITE]);
                        close(programs[j]->output[READ]);
                        close(programs[j]->input[READ]);
                        close(programs[j]->output[WRITE]);
                    } else {
                        close(programs[j]->input[WRITE]);
                        close(programs[j]->output[READ]);
                    }
                }

                runProgram(programs[i]);

                exit(0);
            }
        }

        if (runNAT) {
            nat = program;

            pipe(nat->input);
            pipe(nat->output);

            nat->identifier = fork();

            if (nat->identifier == 0) {
                close(nat->input[WRITE]);
                close(nat->output[READ]);

                for (int i = 0; i < 50; i++) {
                    close(programs[i]->input[WRITE]);
                    close(programs[i]->output[READ]);
                    close(programs[i]->input[READ]);
                    close(programs[i]->output[WRITE]);
                }

                runProgram(nat);

                exit(0);
            }

            ioctl(nat->input[WRITE], FIONBIO, &on);
            ioctl(nat->output[READ], FIONBIO, &on);
        } else {
            freeProgram(program);
        }

        for (int i = 0; i < 50; i++) {
            close(programs[i]->input[READ]);
            close(programs[i]->output[WRITE]);

            ioctl(programs[i]->input[WRITE], FIONBIO, &on);
            ioctl(programs[i]->output[READ], FIONBIO, &on);

            data = i;
            write(programs[i]->input[WRITE], &data, sizeof(data));
        }

        struct Program *destinationProgram;
        bool idle;

        while (!packet) {
            idle = true;

            for (int i = 0; !packet && i < 50; i++) {
                if (read(programs[i]->output[READ], &data, sizeof(data)) >= 0) {
                    idle = false;

                    *packets[i].packet = data;

                    if (packets[i].packet == &packets[i].y) {
                        int destination = packets[i].destination;

                        if (destination == 255) {
                            if (runNAT) {
                                natPackets.destination = destination;
                                natPackets.x = packets[i].x;
                                natPackets.y = packets[i].y;

                                destinationProgram = nat;
                            } else {
                                packet = data;
                                break;
                            }
                        } else {
                            destinationProgram = programs[destination];
                        }

                        data = packets[i].x;
                        write(destinationProgram->input[WRITE], &data, sizeof(data));

                        data = packets[i].y;
                        write(destinationProgram->input[WRITE], &data, sizeof(data));

                        packets[i].packet = &packets[i].destination;
                    } else {
                        ++packets[i].packet;
                    }
                } else {
                    data = -1;
                    write(programs[i]->input[WRITE], &data, sizeof(data));
                }
            }

            if (runNAT && natPackets.destination && idle) {
                destinationProgram = programs[0];

                data = natPackets.x;
                write(destinationProgram->input[WRITE], &data, sizeof(data));

                data = natPackets.y;
                write(destinationProgram->input[WRITE], &data, sizeof(data));

                if (resumePackets.destination && natPackets.y == resumePackets.y) {
                    packet = natPackets.y;
                    break;
                }

                resumePackets = natPackets;
                natPackets.destination = 0;
            }
        }

        if (runNAT) {
            nat->running = false;

            kill(nat->identifier, SIGKILL);

            freeProgram(nat);
        }

        for (int i = 0; i < 50; i++) {
            programs[i]->running = false;

            kill(programs[i]->identifier, SIGKILL);

            freeProgram(programs[i]);
        }
    }

    return packet;
}