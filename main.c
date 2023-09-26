#include <stdio.h>
#include <stdint.h>

// Preprocessor statement, not a global variable
#define TAPE_LEN 32768
#define PROGRAM_LEN 13

/**
 * @brief Sanity is a basic sanity checker for code loops
 * @param code The code to check
 * @return 0 if no issues encountered, index of problematic command otherwise
 */
int sanity(const char* code){
    int depth = 0;      // Current depth of loops
    int index = 0;      // Index of last loop open

    for (int i = 0; i < PROGRAM_LEN; ++i) {
        if (code[i] == '[') {
            depth++;
            index = i;
        }
        else if (code[i] == ']') {
            depth--;
            if (depth >= 0) continue;

            printf("[SANITY] ERROR: Closing unopened loop at index %d", i);
            return i;
        }
    }
    if (depth == 0) return 0;

    // There may be multiple unclosed loops, but sanity will always return the last one
    printf("[SANITY] ERROR: Unclosed loop at index %d", index);
    return index;
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    // printf("Hello, Brainfuck!\n\n");

    /**
     *  BF description from https://en.wikipedia.org/wiki/Brainfuck
     *
     *  >	Increment the data pointer by one (to point to the next cell to the right).
     *  <	Decrement the data pointer by one (to point to the next cell to the left).
     *  +	Increment the byte at the data pointer by one.
     *  -	Decrement the byte at the data pointer by one.
     *  .	Output the byte at the data pointer.
     *  ,	Accept one byte of input, storing its value in the byte at the data pointer.
     *  [	If the byte at the data pointer is zero, then instead of moving the instruction pointer
     *      forward to the next command, jump it forward to the command after the matching ] command.
     *  ]	If the byte at the data pointer is nonzero, then instead of moving the instruction pointer
     *      forward to the next command, jump it back to the command after the matching [ command.
     */

    int8_t tape[TAPE_LEN] = {0};
    int8_t *p = tape;

    char programkod[] = "+[>,+]+[<-.]";
    //char programkod[]="[ThisprogramprintsSierpinskitriangleon80-columndisplay.]>++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++*****Made*By:*NYYRIKKI*2002*****";

    if (sanity(programkod)) return 1;  // Sanity checker encountered an error


    for (int i = 0; i < PROGRAM_LEN; ++i) {

        int depth = 1;
        switch (programkod[i]) {
            case '.': printf("%c", *p); break;
            case '+': ++*p; break;
            case '-': --*p; break;
            case '>':
                if (++p > tape + TAPE_LEN){
                    printf("\n[MOVER>] Error: Code ran off the tape!");
                    return 1;
                }
                break;

            case '<':
                if (--p < tape){
                    printf("\n[MOVER<] Error: Code fell off the tape!");
                    return 1;
                }
                break;

            case '[':
                if (*p != 0) break;
                while (depth){
                    i++;
                    if (programkod[i] == '[') depth++;
                    else if (programkod[i] == ']') depth--;
                }
                break;

            case ']':
                if (*p == 0) break;
                while (depth){
                    i--;
                    if (programkod[i] == ']') depth++;
                    else if (programkod[i] == '[') depth--;
                }
                break;

            case ',':
                if (scanf("%c", p) != 1) *p = -1;
                // printf("%c", *p);
                break;
        }

    }

    return 0;
}
