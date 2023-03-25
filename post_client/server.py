import socket, re, datetime

timeout = '???'
temperature = '??.??'

get_html = re.compile(r'^GET / ')
get_js = re.compile(r'^GET /script.js')
get_time = re.compile(r'^GET /time')

post_update = re.compile(r'^POST /update')
post_timeout = re.compile(r'^POST /timeout')

parse_temp = re.compile(r'temp=(.+)')
parse_timeout = re.compile(r'timeout=(\d+)')

with open('webpage.html', 'r', encoding='utf-8') as webpage:
   with open('script.js', 'r', encoding='utf-8') as script:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as resource:
         with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as info:
            port = int(input('Type a port: '))
            resource.bind(('', port))
            resource.listen(port)
            info.bind(('', port + 1))
            info.listen(port + 1)

            html = webpage.readlines()
            html = ''.join(html)

            js = script.readlines()
            js = ''.join(js)

            while True:
               client, addr = resource.accept()
               print('[HTTP_BEGIN]')
               request = client.recv(1024)
               request = request.decode()
               print(request)

               if get_html.match(request):
                  header = f'HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n{html}'
                  client.sendall(header.encode())
               elif get_js.match(request):
                  header = f'HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n{js}'
                  client.sendall(header.encode())
               elif post_update.match(request):
                  client_t, addr_t = info.accept()
                  request_t = client_t.recv(1024)
                  request_t = request_t.decode()
                  client_t.close()
                  print(request_t, '=====')
                  temperature = parse_temp.search(request_t).group(1)
                  temperature = str(round(float(temperature), 2))
                  payload = f'{temperature}&{timeout}'
                  print(payload)
                  header = f'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n{payload}'
                  client.sendall(header.encode())
               elif post_timeout.search(request):
                  timeout = parse_timeout.search(request)
                  timeout = timeout.group(1)
                  header = f'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n{temperature}'
                  client.sendall(header.encode())
               elif get_time.match(request):
                  time = datetime.datetime.now()
                  time = time.strftime('%H:%M')
                  header = f'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n{time}'
                  client.sendall(header.encode())
               else:
                  header = 'HTTP/1.1 404 Not Found\r\n\r\n'
                  client.sendall(header.encode())
               
               client.close()
               print('[HTTP_END]')