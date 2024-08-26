# caract = "n"

# pattern = "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz"

# print(pattern.find(caract))

import ctypes
import os

# On récupère la variable d'environnement
variable_name = 'SHELLCODE'
variable_value = os.environ.get(variable_name)

if variable_value:
    # On obtient un pointeur vers la variable d'environnement
    libc = ctypes.CDLL('libc.so.6')
    getenv = libc.getenv
    getenv.restype = ctypes.c_char_p
    getenv.argtypes = [ctypes.c_char_p]

    # Appel à getenv pour obtenir l'adresse de la variable d'environnement
    address = ctypes.cast(getenv(variable_name.encode('utf-8')), ctypes.c_void_p).value
    print(f'Adresse de la variable d\'environnement {variable_name}: {hex(address)}')
else:
    print(f'Variable {variable_name} non trouvée')
