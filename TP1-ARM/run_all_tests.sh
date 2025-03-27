# #!/bin/bash

# # Compilar simulador una vez al comienzo
# if [ -d "src" ]; then
#     cd src && make && cd ..
# else
#     echo "Error: La carpeta src no existe."
#     exit 1
# fi

# # Verificar que ref_sim_x86 exista
# if [ ! -f "./ref_sim_x86" ]; then
#     echo "Error: El simulador de referencia ref_sim_x86 no se encontró."
#     exit 1
# fi

# # Iterar sobre todos los archivos .x en inputs/bytecode
# for file in inputs/bytecodes/*.x; do
#     if [ ! -f "$file" ]; then
#         echo "No se encontraron archivos .x en inputs/bytecode"
#         exit 1
#     fi

#     name=$(basename "$file")
#     ./test_simulator_outputs "$name"
# done

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT 8192
#define MAX_LINES 512

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT 8192
#define MAX_LINES 512

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

bool is_relevant_line(const char* line) {
    return (strstr(line, "X") == line ||
            strstr(line, "PC") == line ||
            strstr(line, "FLAG_N") == line ||
            strstr(line, "FLAG_Z") == line ||
            strstr(line, "Instruction Count") == line);
}

void extract_relevant_lines(const char* full_output, char lines[MAX_LINES][128], int* count) {
    char copy[MAX_OUTPUT];
    strncpy(copy, full_output, MAX_OUTPUT);
    *count = 0;
    char* line = strtok(copy, "\n");
    while (line != NULL && *count < MAX_LINES) {
        while (*line == ' ') line++; // Skip leading spaces
        if (is_relevant_line(line)) {
            strncpy(lines[*count], line, 127);
            lines[*count][127] = '\0';
            (*count)++;
        }
        line = strtok(NULL, "\n");
    }
}

bool compare_outputs_verbose(const char* ref_out, const char* sim_out, const char* testfile) {
    char ref_lines[MAX_LINES][128];
    char sim_lines[MAX_LINES][128];
    int ref_count = 0, sim_count = 0;

    extract_relevant_lines(ref_out, ref_lines, &ref_count);
    extract_relevant_lines(sim_out, sim_lines, &sim_count);

    bool equal = true;
    if (ref_count != sim_count) {
        equal = false;
    }

    for (int i = 0; i < ref_count && i < sim_count; i++) {
        if (strcmp(ref_lines[i], sim_lines[i]) != 0) {
            equal = false;
        }
    }

    printf("[%s]: %s\n", testfile, equal ? "True" : "False");

    if (!equal) {
        printf("-- Diferencias detectadas --\n");
        printf("%-40s | %-40s\n", "Simulador de referencia", "Tu simulador");
        printf("%-40s | %-40s\n", "----------------------------------------", "----------------------------------------");
        int max = ref_count > sim_count ? ref_count : sim_count;
        for (int i = 0; i < max; i++) {
            const char* ref = (i < ref_count) ? ref_lines[i] : "(línea faltante)";
            const char* sim = (i < sim_count) ? sim_lines[i] : "(línea faltante)";
            if (strcmp(ref, sim) != 0) {
                printf("%-40s | %-40s\n", ref, sim);
            }
        }
        printf("----------------------------\n");
    }

    return equal;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_a_testear>\n", argv[0]);
        return 1;
    }

    char* testfile = argv[1];
    char ref_output[MAX_OUTPUT];
    char sim_output[MAX_OUTPUT];

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "inputs/bytecode/%s", testfile);

    run_simulator_and_get_rdump("./ref_sim_x86", full_path, ref_output);
    system("cd src && make >/dev/null 2>&1 && cd ..");
    run_simulator_and_get_rdump("./src/sim", full_path, sim_output);

    compare_outputs_verbose(ref_output, sim_output, testfile);

    return 0;
}
