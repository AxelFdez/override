#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char buffer[100];  // Déclaration d'un tableau de caractères pour stocker l'entrée.

    fgets(buffer, 100, stdin);  // Lecture de la chaîne de l'utilisateur avec protection contre le dépassement de tampon.

    // Boucle pour parcourir chaque caractère de la chaîne.
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
            buffer[i] ^= 0x20;  // Conversion des majuscules en minuscules par XOR avec 0x20.
        }
    }

    printf(buffer);  // Affichage de la chaîne modifiée.
    exit(0);  // Sortie propre du programme.
}
