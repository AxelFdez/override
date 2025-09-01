# Level 06

## Introduction

Le programme vérifie que le serial correspond bien au serial calculé sur le nom d'utilisateur.

2 façons de trouver le serial :
  - Avec gdb et contourner la "sécurité" du programme ;
  - Refaire le calcul dans un programme ou à la main pour obtenir 

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=9f72fea8-183c-4bdc-9620-b88449bc2ae1 ;
 2. Lancer gdb et désassembler la fonction `auth` ;
 3. Ajouter 2 breakpoint :
    - Le premier pour changer la valeur de retour de ptrace (et donc passer outre la sécurité) ;
    - Le deuxième pour récupérer le serial correspondant au nom d'utilisateur.

## Etapes

(Voir [source.c](../source.c) pour le code complet)

A partir de `disas auth` :

```asm
(gdb) disas auth
Dump of assembler code for function auth:
   # ...
   0x080487b5 <+109>:   call   0x80485f0 <ptrace@plt>
   0x080487ba <+114>:   cmp    $0xffffffff,%eax
   # ...
   0x08048863 <+283>:   mov    0xc(%ebp),%eax
   0x08048866 <+286>:   cmp    -0x10(%ebp),%eax
   0x08048869 <+289>:   je     0x8048872 <auth+298>
   0x0804886b <+291>:   mov    $0x1,%eax
   0x08048870 <+296>:   jmp    0x8048877 <auth+303>
   0x08048872 <+298>:   mov    $0x0,%eax
   0x08048877 <+303>:   leave
   0x08048878 <+304>:   ret
```

On en déduit :

```sh
# On bypass la sécurité gdb (pour modifier la vérification du retour de ptrace)
(gdb) b *auth+114
Breakpoint 1 at 0x80487ba
# On met en pause pour récupérer le serial number
(gdb) b *auth+286
Breakpoint 2 at 0x8048866
```

Pour le premier, on change la valeur de comparaison à autre chose que -1, j'ai choisi 18.

```sh
Breakpoint 1, 0x080487ba in auth ()
(gdb) i r
eax            0xffffffff       -1
(gdb) set $eax=18
```

Pour le deuxième, on affiche la valeur `-0x10(%ebp)` :

```sh
Breakpoint 2, 0x08048866 in auth ()
(gdb) p *(int*)($ebp-0x10)
$1 = 6234996
```

Au complet :

```sh
level06@OverRide:~$ gdb ./level06
(gdb) b *auth+114
Breakpoint 1 at 0x80487ba
(gdb) b *auth+286
Breakpoint 2 at 0x8048866
(gdb) run
Starting program: /home/users/level06/level06
***********************************
*               level06           *
***********************************
-> Enter Login: gphilipp
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 45

Breakpoint 1, 0x080487ba in auth ()
(gdb) set $eax=18
(gdb) c
Continuing.

Breakpoint 2, 0x08048866 in auth ()
(gdb) p *(int*)($ebp-0x10)
$1 = 6234996
```

Le serial number pour `gphilipp` est `6234996`.

```sh
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: gphilipp
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234996
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

On a donc le flag :

```
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
