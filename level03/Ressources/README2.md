# Level 03

## Introduction

Il s'agit de casser la méthode de cryptage soit par calcul (résoudre l'équation), soit par bruteforce (en bouclant sur les 21 possibilités).

J'ai fais les deux méthodes.

---

On constate que la difficulté augmente (au niveau de la protection du code) :

```text
RELRO       	STACK CANARY  	NX        	PIE         	RPATH  	RUNPATH  	FILE
Partial RELRO   Canary found  	NX enabled	No PIE      	No RPATH   No RUNPATH   /home/users/level03/level03
```

On constate :
 - Partial RELRO : https://www.google.com/search?client=firefox-b-d&q=Partial+RELRO 
 - Canary found : https://www.google.com/search?client=firefox-b-d&q=Canary+found 
 - NX enabled : https://www.google.com/search?client=firefox-b-d&q=+NX+enabled 

## Resumé

 1. Décompiler le programme (https://dogbolt.org/?id=5794bb2b-b8b6-4ff4-b1f8-61bab5dffc14) ;
 2. Résoudre l'équation (problème mathématique) ;
   - La valeur ``Q}|u`sfg~sf{}|a3"`` doit devenir `Congratulations!` ;
   - Si `pass[i] ^= a1;` soit `a ^= b` soit `a ^ b = c`  
     donc on a : `a ^ c = b`  
     `'Q' ^ a1 = 'C'` donc `'Q' ^ 'C' = 18`  

## Etapes

(Voir [source.c](../source.c) pour le code complet)

Avec le code source, on voit que "Invalid Password" est géré par la fonction decrypt. Le password doit être un nombre (`%d`) utilisé par scanf, la différence entre le chiffre - qu’on saisie et la constante `322424845` - doit être inférieur à 21, sinon on doit être très chanceux.

Ensuite, soit en utilisant une boucle pour tester les 21 possibilités ou par calcul (voir explication code ci-dessous => `'Q' ^ 'C' = 18`), on apprend que la différence doit être de 18.

Donc `322424845 - 18 = 322424827`.

[./decrypt2.c](./decrypt2.c) :

```c
#include <stdio.h>
#include <math.h>
#include <string.h>

int decrypt(char a1)
{
  char pass[29];

  pass[17] = '\0';
  strcpy(pass, "Q}|u`sfg~sf{}|a3");
  for (int i = 0, len = strlen(pass); i < len; ++i)
	pass[i] ^= a1;
  if (!strcmp(pass, "Congratulations!")) {
	printf("==> %s <== \n", pass);
	return 1;
  } else {
	printf("invalid: %s\n", pass);
	return 0;
  }
}

int main() {
	/*
	Si v4[i] ^= a1; soit a ^= b soit a ^ b = c
	donc on a : a ^ c = b
	'Q' ^ a1 = 'C' donc 'Q' ^ 'C' = 18
	*/
	printf("%d\n", 'Q' ^ 'C'); // 18
	for (char i = 0; i <= 21; i++) {
    	if (decrypt(i)) {
        	printf("\nok ==> %d <==\n\n", i);
    	}
	}
}
```

Plus qu'a essayer :
```text
level03@OverRide:~$ (echo 322424845 - 18 | bc; cat) | ./level03
***********************************
*           	level03     	**
***********************************
whoami
level04
```

Sans utiliser pipe :
```
level03@OverRide:~$ echo 322424845 - 18 | bc
322424827
level03@OverRide:~$ ./level03
***********************************
*           	level03     	**
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```

On a donc le flag :
```
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```

