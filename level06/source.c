int authenticate(char *username, unsigned int serial_number) {
    size_t newline_position, username_length;
    int status;
    long debug_check;
    int index;
    unsigned int expected_serial;
    // Supprimer le caractère de nouvelle ligne du nom d'utilisateur
    newline_position = strcspn(username, "\n");
    username[newline_position] = '\0';
    // Obtenir la longueur du nom d'utilisateur (max 32 caractères)
    username_length = strnlen(username, 32);
    // Le nom d'utilisateur doit avoir au moins 6 caractères
    if ((int)username_length < 6) {
        return 1; // Échec de l'authentification
    }
    // Vérification anti-débogage avec ptrace
    debug_check = ptrace(PTRACE_TRACEME, 0, 0, 0);
    if (debug_check == -1) {
        // Le programme est en cours de débogage
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m\'---------------------------\'");
        return 1; // Échec de l'authentification
    }
    // Calculer le numéro de série attendu basé sur le nom d'utilisateur
    // Commencer avec le 4ème caractère XOR avec 4919 (0x1337), puis ajouter 6221293 (0x5eeded)
    expected_serial = ((int)username[3] ^ 4919) + 6221293;
    // Traiter chaque caractère du nom d'utilisateur
    for (index = 0; index < (int)username_length; index++) {
        // Vérifier les caractères non-imprimables
        if (username[index] < ' ') {
            return 1; // Échec de l'authentification
        }
        // Mettre à jour le numéro de série calculé avec l'algorithme basé sur les caractères
        expected_serial = expected_serial + ((int)username[index] ^ expected_serial) % 1337;
    }
    // Comparer le numéro de série fourni avec le numéro calculé
    if (serial_number == expected_serial) {
        return 0; // Authentification réussie
    } else {
        return 1; // Échec de l'authentification
    }
  }

int main(void) {
    int authentication_result;
    char username[32];
    unsigned int user_serial;
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(username, 32, stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &user_serial);  // Lire un entier non signé
    authentication_result = authenticate(username, user_serial);
    if (authentication_result == 0) {
        puts("Authenticated!");
        system("/bin/sh");  
    }
    return (authentication_result != 0);
  }