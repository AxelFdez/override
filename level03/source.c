#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

unsigned int get_unum() {
    unsigned int num = 0;
    fflush(stdout);
    scanf("%u", &num);
    clear_stdin();
    return num;
}

void prog_timeout() {
    _exit(1);
}

int decrypt(char key) {
    char encrypted_string[] = "Q}|u`sfg~sf{}|a3";
    int len = strlen(encrypted_string);
    for (int i = 0; i < len; i++) {
        encrypted_string[i] ^= key;
    }

    if (strcmp(encrypted_string, "Congratulations!") == 0) {
        system("/bin/sh");
        return 0;
    } else {
        puts("\nInvalid Password");
        return 1;
    }
}

int test(int param_1, int param_2) {
    int         result;
    char        random;

    switch (param_1 - param_2)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            result = decrypt(param_1 - param_2);
        break;
        default:
            random = rand();
            result = decrypt(random);
        break;
    }
    return result;
}

int main() {
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    unsigned int password = get_unum();
    test(0x1337d00d, password);
    return 0;
}
