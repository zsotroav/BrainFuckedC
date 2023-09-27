#include <stdio.h>

// Preprocessor statement, not a global variable
// Only using defines to have them easily updatable
#define TAPE_LEN 32768

/**
 * @brief Sanity is a basic sanity (syntax) checker for code loops
 * @param code The code to check
 * @return 0 if no issues encountered, index of problematic command otherwise
 */
int sanity(const char* code) {    // const due to clang-tidy warnings
    int depth = 0;      // Current depth of loops
    int index = 0;      // Index of last loop open

    for (int i = 0; code[i] != 0; ++i) {
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

    // There may be multiple unclosed loops, but sanity will always complain about the last one
    printf("[SANITY] ERROR: Unclosed loop at index %d", index);
    return index;
}

int main() {
    /**
     * The debugger is weird: when using breakpoints the OS may buffer the text for too long
     * and not have anything on stdout while debugging through breakpoints.
     */
    setvbuf(stdout, NULL, _IONBF, 0);

    char programkod[] = "+[>,+]+[<-.]"; // Reverser
    //char programkod[]="[ThisprogramprintsSierpinskitriangleon80-columndisplay.]>++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++*****Made*By:*NYYRIKKI*2002*****";

    signed char tape[TAPE_LEN] = {0};
    signed char *p = tape;


    if (sanity(programkod)) return 1;  // Sanity checker encountered an error

    for (int i = 0; programkod[i] != 0; ++i) {

        int depth = 1;
        switch (programkod[i]) {
            case '.': printf("%c", *p); break;
            case '+': ++*p; break;
            case '-': --*p; break;
            case '>':
                if (++p < tape + TAPE_LEN) break;
                printf("\n[MOVER>] Error: Code ran off the tape!");
                return 1;

            case '<':
                if (--p >= tape) break;
                printf("\n[MOVER<] Error: Code fell off the tape!");
                return 1;

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
                // Note: CLion and EOF are a bit weird sometimes, and it may not always register through scanf properly
                if (scanf("%c", p) != 1) *p = -1;
                // printf("%c", *p); // Debug echo
                break;
        }
    }

    return 0;
}
