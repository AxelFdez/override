#include <stdio.h>
#include <math.h>
#include <string.h>

int decrypt(char a1)
{
  char flag[29];

  flag[17] = '\0';
  strcpy(flag, "Q}|u`sfg~sf{}|a3");
  for (int i = 0, len = strlen(flag); i < len; ++i)
	flag[i] ^= a1;
  if (!strcmp(flag, "Congratulations!")) {
	printf("==> %s <== \n", flag);
	return 1;
  } else {
	printf("invalid: %s\n", flag);
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

/*
18
invalid: Q}|u`sfg~sf{}|a3
invalid: P|}targfrgz|}`2
invalid: S~wbqde|qdy~c1
invalid: R~vcped}pex~b0
invalid: Uyxqdwbczwbyxe7
invalid: Txypevcb{vc~xyd6
invalid: W{zsfu`axu`}{zg5
invalid: Vz{rgta`yta|z{f4
invalid: Yut}h{nov{nsuti;
invalid: Xtu|izonwzortuh:
invalid: [wvjylmtylqwvk9
invalid: Zvw~kxmluxmpvwj8
invalid: ]qpyljkrjwqpm?
invalid: \pqxm~kjs~kvpql>
invalid: _sr{n}hip}husro=
invalid: ^rszo|ihq|itrsn<
invalid: Amlepcvwncvkmlq#
invalid: @lmdqbwvobwjlmp"
==> Congratulations! <== 

ok ==> 18 <==

invalid: Bnofs`utm`uhnor 
invalid: Eihatgrsjgroihu'
invalid: Dhi`ufsrkfsnhit&
*/
