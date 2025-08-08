 #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>

unsigned int get_unum(void) {
      unsigned int number;

      number = 0;
      fflush(stdout);
      scanf("%u", &number);
      clear_stdin();
      return number;
  }

  int store_number(int *data_array) {
      unsigned int number, index;

      printf(" Number: ");
      number = get_unum();
      printf(" Index: ");
      index = get_unum();

      if ((index % 3 == 0) || (number >> 0x18 == 0xb7)) {
          puts(" *** ERROR! ***");
          puts("   This index is reserved for wil!");
          puts(" *** ERROR! ***");
          return 1;
      }
      else {
          data_array[index] = number;
          return 0;
      }
  }

  int read_number(int *data_array) {
      int index;

      printf(" Index: ");
      index = get_unum();
      printf(" Number at data[%u] is %u\n", index, data_array[index]);
      return 0;
  }

  int main(int argc, char **argv, char **envp) {
      int data_array[100];
      char command[20];
      int command_result = 0;
      int canary = __builtin_return_address(0);

      // Initialize data array to zero
      memset(data_array, 0, sizeof(data_array));

      // Clear argv and envp
      for (char **arg = argv; *arg != NULL; arg++) {
          memset(*arg, 0, strlen(*arg));
      }
      for (char **env = envp; *env != NULL; env++) {
          memset(*env, 0, strlen(*env));
      }

      puts("----------------------------------------------------\n"
           "  Welcome to wil's crappy number storage service!   \n"
           "----------------------------------------------------\n"
           " Commands:                                          \n"
           "    store - store a number into the data storage    \n"
           "    read  - read a number from the data storage     \n"
           "    quit  - exit the program                        \n"
           "----------------------------------------------------\n"
           "   wil has reserved some storage :>                 \n"
           "----------------------------------------------------\n");

      while (true) {
          printf("Input command: ");
          fgets(command, sizeof(command), stdin);

          // Remove newline character
          command[strcspn(command, "\n")] = 0;

          if (strcmp(command, "store") == 0) {
              command_result = store_number(data_array);
          }
          else if (strcmp(command, "read") == 0) {
              command_result = read_number(data_array);
          }
          else if (strcmp(command, "quit") == 0) {
              return 0;
          }

          // Print result
          if (command_result == 0) {
              printf(" Completed %s command successfully\n", command);
          } else {
              printf(" Failed to do %s command\n", command);
          }

          // Clear buffer
          memset(command, 0, sizeof(command));
      }

      return 0;
  }