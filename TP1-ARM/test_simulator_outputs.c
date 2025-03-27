#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX_OUTPUT 8192

// void run_command_and_capture(const char* cmd, char* output) {
//     FILE* fp;
//     char buffer[256];
//     output[0] = '\0';

//     fp = popen(cmd, "r");
//     if (fp == NULL) {
//         perror("Error ejecutando el comando");
//         exit(1);
//     }

//     while (fgets(buffer, sizeof(buffer), fp) != NULL) {
//         strcat(output, buffer);
//     }

//     pclose(fp);
// }

// void run_simulator_and_get_rdump(const char* simulator, const char* file, char* output) {
//     char command[256];
//     snprintf(command, sizeof(command), "(echo go && echo rdump && echo q) | %s %s", simulator, file);
//     run_command_and_capture(command, output);
// }

// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Uso: %s <archivo_a_testear>\n", argv[0]);
//         return 1;
//     }

//     char* testfile = argv[1];
//     char ref_output[MAX_OUTPUT];
//     char sim_output[MAX_OUTPUT];

//     char full_path[256];
//     snprintf(full_path, sizeof(full_path), "inputs/bytecode/%s", testfile);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_OUTPUT 8192

void run_command_and_capture(const char* cmd, char* output) {
    FILE* fp;
    char buffer[256];
    output[0] = '\0';

    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("Error ejecutando el comando");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(output, buffer);
    }

    pclose(fp);
}

void run_simulator_and_get_rdump(const char* simulator, const char* file, char* output) {
    char command[256];
    snprintf(command, sizeof(command), "(echo go && echo rdump && echo q) | %s %s", simulator, file);
    run_command_and_capture(command, output);
}

void test_run_command_and_capture() {
    char output[MAX_OUTPUT];
    run_command_and_capture("echo Hello, World!", output);
    assert(strcmp(output, "Hello, World!\n") == 0);
    printf("test_run_command_and_capture passed\n");
}

void test_run_simulator_and_get_rdump() {
    char output[MAX_OUTPUT];
    run_simulator_and_get_rdump("echo", "Hello, World!", output);
    assert(strcmp(output, "go\nrdump\nq\nHello, World!\n") == 0);
    printf("test_run_simulator_and_get_rdump passed\n");
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* testfile = argv[1];
        char ref_output[MAX_OUTPUT];
        char sim_output[MAX_OUTPUT];

        char full_path[256];
        snprintf(full_path, sizeof(full_path), "inputs/bytecode/%s", testfile);

        run_simulator_and_get_rdump("./ref_sim_x86", full_path, ref_output);

        system("cd src && make && cd ..");

        run_simulator_and_get_rdump("./src/sim", full_path, sim_output);

        printf("[%s]: %s\n", testfile, strcmp(ref_output, sim_output) == 0 ? "True" : "False");

        return 0;
    } else {
        test_run_command_and_capture();
        test_run_simulator_and_get_rdump();
        return 0;
    }
}
