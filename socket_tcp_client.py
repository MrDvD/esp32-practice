import socket, json

data = input()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
   s.connect(('', 9007))
   print('Connected')
   s.sendall(data.encode())