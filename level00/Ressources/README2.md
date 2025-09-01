# Level 00

## Introduction

Ce niveau 42 est assez évident.  
Il s'agit de saisir le mot de passe (5276) qui apparait en clair dans le code décompilé.

## Resumé

 1. Décompiler le programme ;
 2. Lire le code et trouvé le mot de passe (5276).
 3. Lancer le binaire est saisir le code dans le prompt.

## Etapes

Commande pour se connecter à la VM et récupérer le fichier.  
Le mot de passe est : `level00`

```sh
ssh level00@127.0.0.1 -p 4242
scp -P 4242 level00@127.0.0.1:/home/users/level00/level00 level00
```

Code décompilé :

```c
int main(int argc, char** argv, char** envp)
{
	puts("********************************…");
	puts("* \t 	-Level00 -\t\t  *");
	puts("********************************…");
	printf("Password:");
	int input;
	scanf("%d", &input);
    
	if (input != 0x149c) // = 5276
	{
    	puts("\nInvalid Password!");
    	return 1;
	}
    
	puts("\nAuthenticated!");
	system("/bin/sh");
	return 0;
}
```

C’est évident, le mot de passe est `0x149c` soit `5276`.

On utilise ensuite le mot de passe :

```sh
level00@OverRide:~$ ./level00
***********************************
*        	-Level00 -       	*
***********************************
Password:5276

Authenticated!
$
```

Donc le flag :

```sh
cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
