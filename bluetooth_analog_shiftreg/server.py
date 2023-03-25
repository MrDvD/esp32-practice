import bluetooth, numpy

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.bind(('74:C6:3B:38:A4:A0', 1))
sock.listen(1)

while True:
   client, addr = sock.accept()
   print(addr)
   while True:
      try:
         msg = client.recv(1024)
         print('[Message]', msg.decode())
         response = ''
         idx = int(numpy.interp(msg.decode(), [0, 1023], [0, 8]))
         for i in range(8 - idx):
            response += '0'
         for i in range(idx):
            response += '1'
         client.sendall(response.encode())
      except bluetooth.BluetoothError:
         print('CLIENT_DISCONNECTED')
         break
   # client.sendall(b"Hi too!")
   # client.close()