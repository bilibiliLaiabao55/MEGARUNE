text = input(">>>")
print("0x"+",0x".join("{:02x}".format(ord(c)) for c in text))

