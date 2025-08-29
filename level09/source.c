#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret_backdoor(void)
{
    char command[128];

    fgets(command, 0x80, stdin);
    system(command);
    return;
}

void handle_msg(void)
{
    char buffer[140];
    int msg_len;

    // Initialisation
    memset(buffer, 0, 140);
    msg_len = 0x8c; // 140 bytes

    set_username(buffer);
    set_msg(buffer);
    puts(">: Msg sent!");
    return;
}

void set_msg(char *data)
{
    char input[1024];

    memset(input, 0, 1024);

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(input, 0x400, stdin);

    // VULNÉRABILITÉ: utilise la valeur à l'offset 0xb4 comme limite
    strncpy(data, input, *(int *)(data + 0xb4));
    return;
}

void set_username(char *data)
{
    char username[140];
    int i;

    memset(username, 0, 140);

    puts(">: Enter your username");
    printf(">>: ");
    fgets(username, 0x80, stdin);

    // Copie vers l'offset 0x8c (peut déborder a 0xb4(180) car 41 caractères peuvent être copiés)
    for (i = 0; (i < 0x29 && username[i] != '\0'); i++)
    {
        data[0x8c + i] = username[i];
    }

    printf(">: Welcome, %s", data + 0x8c);
    return;
}

int main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}