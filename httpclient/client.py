import socket

data = [b'']

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as resource:
   resource.bind(('', 2002))
   resource.connect(('192.168.180.242', int(input('Type a port: '))))
   resource.settimeout(3)
   print('[HTTP start]')
   try:
      resource.sendall(b'GET / HTTP/1.1\r\n\r\n')
      while True:
         char = resource.recv(1)
         match char:
            case b'\n':
               data.append(b'')
            case b'':
               break
            case _:
               data[-1] += char
   except TimeoutError:
      pass
   for line in data:
      print(line.decode())