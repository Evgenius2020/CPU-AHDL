#include <stdio.h>
#include <string.h>

int getCode(char* cmd) {
    if (!strcmp(cmd, "set")) {
        return 1;
    }
    if (!strcmp(cmd, "add")) {
        return 2;
    }
    if (!strcmp(cmd, "jmp")) {
        return 3;
    }
    return 0;
}

void main(int argc, char* argv[]) {
    if (argc != 3) {
        return;
    }
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (!in || !out) {
        return;
    }

    fprintf(out, "DEPTH = 256;\n");
    fprintf(out, "WIDTH = 8;\n");
    fprintf(out, "ADDRESS_RADIX = DEC;\n");
    fprintf(out, "DATA_RADIX = DEC;\n");
    fprintf(out, "CONTENT BEGIN\n");

    char cmd[8];
    int arg;
    int currPos = 0;
    while(!feof(in)) {
        fscanf(in, "%s %d", cmd, &arg);
        fprintf(out, "%d : %d;\n", currPos, getCode(cmd) << 5 | arg);
        currPos += 8;
    }

    fprintf(out, "END;\n");
}


    