#include <stdio.h>
#include <string.h>

#define USERNAME "dat_wil"
#define PASSWORD "admin"

int verify_user_name(const char* username) {
    // Vérifie si le nom d'utilisateur correspond à la valeur définie
    return strcmp(username, USERNAME) == 0 ? 0 : 1;
}

int verify_user_pass(const char* password) {
    // Vérifie si le mot de passe correspond à la valeur définie
    return strcmp(password, PASSWORD) == 0 ? 0 : 1;
}

int main(void) {
    char username[256];
    char password[100];
    int login_result;

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Retire le caractère de nouvelle ligne

    login_result = verify_user_name(username);
    if (login_result == 0) {
        puts("Enter Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0'; // Retire le caractère de nouvelle ligne

        login_result = verify_user_pass(password);
        if (login_result == 0) {
            puts("Access Granted.");
            return 0;
        } else {
            puts("nope, incorrect password...\n");
        }
    } else {
        puts("nope, incorrect username...\n");
    }

    return 1; // Retourne 1 si l'authentification échoue
}