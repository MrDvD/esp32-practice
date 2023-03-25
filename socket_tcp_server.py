import socket, json

port = int(input("Type a port: "))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
   s.bind(('192.168.180.242', port))
   s.listen(port)
   while True:
      #print('Waiting for a client')
      cli, addr = s.accept()
      #print('Got a connection!')
      with cli as c:
         block = c.recv(4).decode()
         if not block: continue
         print(repr(block))
         