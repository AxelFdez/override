#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
dumb memoire de la structure (avec "my_username" et "my_message")
(gdb) x/180cb $buf
0x7fffffffe510:[m  y  _  m  e  s  s  a
0x7fffffffe518: g  e  \n 0  0  0  0  0       
0x7fffffffe520: 0  0  0  0  0  0  0  0       
0x7fffffffe528: 0  0  0  0  0  0  0  0       
0x7fffffffe530: 0  0  0  0  0  0  0  0       
0x7fffffffe538: 0  0  0  0  0  0  0  0       
0x7fffffffe540: 0  0  0  0  0  0  0  0       
0x7fffffffe548: 0  0  0  0  0  0  0  0       
0x7fffffffe550: 0  0  0  0  0  0  0  0       
0x7fffffffe558: 0  0  0  0  0  0  0  0       
0x7fffffffe560: 0  0  0  0  0  0  0  0       
0x7fffffffe568: 0  0  0  0  0  0  0  0       
0x7fffffffe570: 0  0  0  0  0  0  0  0       
0x7fffffffe578: 0  0  0  0  0  0  0  0       
0x7fffffffe580: 0  0  0  0  0  0  0  0       
0x7fffffffe588: 0  0  0  0  0  0  0  0       
0x7fffffffe590: 0  0  0  0  0  0  0  0       
0x7fffffffe598: 0  0  0  0] size = 140
0x7fffffffe598:            [m  y  _  u      
0x7fffffffe5a0: s  e  r  n  a  m  e \n      
0x7fffffffe5a8: 0  0  0  0  0  0  0  0       
0x7fffffffe5b0: 0  0  0  0  0  0  0  0       
0x7fffffffe5b8: 0  0  0  0  0  0  0  0       
0x7fffffffe5c0: 0  0  0  0] size = 40
(gdb) x/dh $buf+140+40
0x7fffffffe5c4: 140
*/
typedef struct message {
    char        msg[140];       // 0x7fffffffe510
    char        username[40];   // 0x7fffffffe59c
    int         len;            // 0x7fffffffe5c4
}   t_message;

void secret_backdoor()
{
    char command[128];

    fgets(command, 128, stdin);
    system(command);
}

void set_msg(t_message *msg)
{
    char        input[1024] = {0};

    printf(">: Msg @Unix-Dude\n");
    printf(">>: ");
    fgets(input, 1024, stdin);

    /*
    * VULNÉRABILITÉ 2:
    * Si msg->len a été agrandi grâce à la première
    * vulnérabilité, strncpy n'est plus sécurisé.
    */
    strncpy(msg->msg, input, msg->len);
}

void set_username(t_message *msg)
{
    char        input[128] = {0};

    printf(">: Enter your username\n");
    printf(">>: ");
    fgets(input, 128, stdin);

    /*
    * VULNÉRABILITÉ 1:
    * La saisie accepte 128 caractères ;
    * On peut écrire 41 caractères dans msg->username[i]
    * alors que la taille de `msg->username` est 140.
    * On peut déborder à l'offset 180 de la structure
    * car 41 caractères peuvent être copiés
    */
    for (int i = 0; i <= 40 && input[i]; i++)
    {
        msg->username[i] = input[i];
    }

    printf(">: Welcome, %s", msg->username);
}

void handle_msg()
{
    t_message       msg = {0};
    // memset(m.msg, 0, 140);
    // memset(m.username, 0, 40);
    msg.len = 140;

    set_username(&msg);
    set_msg(&msg);
    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}
