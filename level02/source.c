
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_FILE "/home/users/level03/.pass"
#define PASSWORD_LENGTH 41 // 0x29 en hexadécimal + 1 pour '\0'

int main(void) {
    char passwordFromFile[PASSWORD_LENGTH] = {0};
    char username[100] = {0};
    char password[100] = {0};
    FILE *fp;
    size_t bytesRead;

    // Ouvrir le fichier de mot de passe
    fp = fopen(PASSWORD_FILE, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: failed to open password file\n");
        exit(1);
    }

    // Lire le mot de passe du fichier
    bytesRead = fread(passwordFromFile, 1, PASSWORD_LENGTH - 1, fp);
    if (bytesRead < PASSWORD_LENGTH - 1) {
        fprintf(stderr, "ERROR: failed to read password file\n");
        fclose(fp);
        exit(1);
    }

    // Assurez-vous que la chaîne est terminée correctement
    passwordFromFile[bytesRead] = '\0';

    // Fermer le fichier de mot de passe
    fclose(fp);

    // Interface utilisateur
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    // Obtenir le nom d'utilisateur
    printf("--[ Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne

    // Obtenir le mot de passe
    printf("--[ Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne

    // Vérifier le mot de passe
    if (strcmp(password, passwordFromFile) == 0) {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");  // Lancer un shell si le mot de passe est correct
        return 0;
    }
    printf(username);
    puts(" does not have access!");
    exit(1);
}