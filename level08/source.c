 #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <unistd.h>

  void log_wrapper(FILE *log_file, char *prefix, char *filename) {
      char buffer[264];

      // Copier le préfixe dans le buffer
      strcpy(buffer, prefix);

      // Ajouter le nom de fichier à la fin du préfixe
      size_t prefix_len = strlen(buffer);
      snprintf(buffer + prefix_len, 254 - prefix_len, "%s", filename);

      // Enlever le caractère de nouvelle ligne s'il existe
      size_t newline_pos = strcspn(buffer, "\n");
      buffer[newline_pos] = '\0';

      // Écrire dans le fichier de log
      fprintf(log_file, "LOG: %s\n", buffer);
  }

  int main(int argc, char **argv) {
      FILE *log_file;
      FILE *input_file;
      int output_fd;
      char output_path[104];
      int c;

      // Vérifier les arguments
      if (argc != 2) {
          printf("Usage: %s filename\n", argv[0]);
          return 1;
      }

      // Ouvrir le fichier de log
      log_file = fopen("./backups/.log", "w");
      if (log_file == NULL) {
          printf("1ERROR: Failed to open %s\n", "./backups/.log");
          exit(1);
      }

      // Logger le début de la sauvegarde
      log_wrapper(log_file, "Starting back up: ", argv[1]);

      // Ouvrir le fichier source
      input_file = fopen(argv[1], "r");
      if (input_file == NULL) {
          printf("2ERROR: Failed to open %s\n", argv[1]);
          exit(1);
      }

      // Construire le chemin de destination
      strncpy(output_path, "./backups/", 11);
      strncat(output_path, argv[1], 99 - strlen(output_path));

      // Créer le fichier de destination
      output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0660);
      if (output_fd < 0) {
          printf("3ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
          exit(1);
      }

      // Copier le contenu du fichier
      while ((c = fgetc(input_file)) != EOF) {
          write(output_fd, &c, 1);
      }

      // Logger la fin de la sauvegarde
      log_wrapper(log_file, "Finished back up ", argv[1]);

      // Fermer les fichiers
      fclose(input_file);
      close(output_fd);
      fclose(log_file);

      return 0;
  }