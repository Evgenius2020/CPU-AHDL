#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Macros {
	char key[16];
	int value;
	char isNumCmd;
	struct Macros* next;
} Macros;

Macros* createMacros() {
	Macros* macros = malloc(sizeof(Macros));
	macros->next = NULL;
	macros->isNumCmd = 0;
	return macros;
}

Macros* getMacrosbyKey(char* key, Macros* macrosListHead) {
	while (1) {
		if (!macrosListHead) {
			return NULL;
		}
		if (!strcmp(macrosListHead->key, key)) {
			return macrosListHead;
		}
		macrosListHead = macrosListHead->next;
	}
}

void destroyMacrosList(Macros* listHead) {
	if (!listHead) {
		return;
	}

	Macros* next = listHead->next;
	free(listHead);
	destroyMacrosList(next);
}

typedef struct Instruction {
	int cmdCode;
	char arg[16];
	int value; // for 'num' functions
	struct Instruction* next;
} Insctruction;

Insctruction* createInstruction() {
	Insctruction* instruction = malloc(sizeof(Insctruction));
	instruction->next = NULL;
	instruction->value = -1;
	return instruction;
}

void toNextLine(FILE* in) {
	char curr;
	while (1) {
		if (feof(in)) {
			return;
		}
		fscanf(in, "%c", &curr);
		if (curr == '\n') {
			return;
		}
	}
}

void compile(FILE* in, FILE* out) {
	char curr[16];
	char buf[16];
	Macros* macros;
	Macros* macrosListHead = NULL;
	Insctruction* instruction;
	Insctruction* instructionQueueBegin = NULL;
	Insctruction* instructionQueueEnd = NULL;
	int currPos = 0;
	int codePos = 0;
	int arg;

	while (!feof(in)) {
		fscanf(in, "%s", curr);
		if (curr[0] == '/') {
			toNextLine(in);
		}
		else if (curr[0] == '.') {
			macros = createMacros();

			if (!strcmp(curr, ".cmd")) {
				fscanf(in, "%s %d %s", macros->key, &macros->value, buf);
				macros->isNumCmd = !strcmp(buf, "num");
			}
			else if (!strcmp(curr, ".variable")) {
				fscanf(in, "%s", macros->key);
				// Initialization
				fscanf(in, "%d", &arg);
				fprintf(out, "%d : %d;\n", currPos, arg);
				macros->value = currPos;
				currPos += 1;
				codePos += 1;
			}
			else { // Lable
				strcpy(macros->key, curr);
				macros->value = currPos;
			}

			macros->next = macrosListHead;
			macrosListHead = macros;
			toNextLine(in);
			continue;
		}
		else {
			macros = getMacrosbyKey(curr, macrosListHead);
			if (macros == NULL) {
				printf("Unknown command\n");
				return;
			}
			instruction = createInstruction();
			instruction->cmdCode = macros->value;
			if (macros->isNumCmd) {
				fscanf(in, "%d ", &instruction->value);
			}
			else {
				fscanf(in, "%s ", instruction->arg);
			}
			currPos += 1;

			// Inserting into queue.
			if (!instructionQueueBegin) {
				instructionQueueBegin = instruction;
			}
			if (!instructionQueueEnd) {
				instructionQueueEnd = instruction;
			}
			else {
				instructionQueueEnd->next = instruction;
				instructionQueueEnd = instruction;
			}
		}
	}

	while (instructionQueueBegin) {
		instruction = instructionQueueBegin;
		if (instruction->value != -1) {
			fprintf(out, "%d : %d;\n", codePos, instruction->cmdCode << 5 | instruction->value);
		}
		else {
			macros = getMacrosbyKey(instruction->arg, macrosListHead);
			fprintf(out, "%d : %d;\n", codePos, instruction->cmdCode << 5 | macros->value);
		}
		codePos += 1;
		instructionQueueBegin = instructionQueueBegin->next;
		free(instruction);
	}

	destroyMacrosList(macrosListHead);
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
	compile(in, out);
	fprintf(out, "END;\n");

	fclose(in);
	fclose(out);
}