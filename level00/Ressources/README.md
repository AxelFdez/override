# Level 00

Pour ce 1er Exo, nous avons un programme qui affiche :
```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:
```

Pour trouver le mot de passe nous allons decompiler le programme (cf source).

L'analyse du binaire indique que le mot de passe doit etre egal a : 0x149c soit 5276.

```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$  cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```