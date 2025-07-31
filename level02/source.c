
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_FILE "/home/users/level03/.pass"
#define PASSWORD_LENGTH 42

int main(void) {
    char passwordFromFile[PASSWORD_LENGTH] = {0};
    char username[100] = {0};
    char password[100] = {0};
    FILE *fp;
    size_t bytesRead;

    // Ouvrir le fichier de mot de passe sors si le fichier n'est pas lisible
    fp = fopen(PASSWORD_FILE, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: failed to open password file\n");
        exit(1);
    }

    // si le mot de passe ne fait pas 41 caractères, on sort
    bytesRead = fread(passwordFromFile, 1, PASSWORD_LENGTH - 1, fp);
    if (bytesRead < PASSWORD_LENGTH - 1) {
        fprintf(stderr, "ERROR: failed to read password file\n");
        fclose(fp);
        exit(1);
    }

    // ajout du caractère de fin de chaîne
    passwordFromFile[bytesRead] = '\0';

    // Fermer le fichier de mot de passe
    fclose(fp);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    // Obtenir le nom d'utilisateur
    printf("--[ Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Supprime le caractère de nouvelle ligne

    // Obtenir le mot de passe
    printf("--[ Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    // Compare le mot de passe, et les 41 premiers caractères du fichier
    if (strncmp(password, passwordFromFile, 41) == 0) {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");  // Lancer un shell si le mot de passe est correct
        return 0;
    }
    printf(username);
    puts(" does not have access!");
    exit(1);
}