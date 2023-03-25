import bluetooth

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.bind(('74:C6:3B:38:A4:A0', 1))
sock.listen(1)
while True:
   client, addr = sock.accept()
   print(addr)
   msg = client.recv(1024)
   print(msg)
   client.sendall(b"Hi too!")
   client.close()