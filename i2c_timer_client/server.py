import bluetooth, numpy, json

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
         query = ''
         idx = int(numpy.interp(msg.decode(), [0, 1023], [0, 16]))
         for i in range(16 - idx):
            query += '0'
         for i in range(idx):
            query += '1'
         dict = {"led1": query[0:8], "led2": query[8:16]}
         response = json.dumps(dict)
         print(response)
         client.sendall(response.encode())
      except bluetooth.BluetoothError:
         print('CLIENT_DISCONNECTED')
         break
   # client.sendall(b"Hi too!")
   # client.close()