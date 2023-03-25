import socket, re

response = 'HTTP/1.0 200 OK\r\n\r\n'

get_html = re.compile(r'^GET / ')
post_buttons = re.compile(r'^POST /buttons_update')
get_js = re.compile(r'^GET /script.js')
get_vals = re.compile(r'^GET /stored_values')
parse_state = re.compile(r'State: (\d+)')

with open('webpage.html', 'r', encoding='utf-8') as webpage:
   with open('script.js', 'r', encoding='utf-8') as script:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as resource:
         port = int(input('Type a port: '))
         resource.bind(('', port))
         resource.listen(port)

         html = webpage.readlines()
         html = ''.join(html)

         js = script.readlines()
         js = ''.join(js)

         states = '0'

         while True:
            client, addr = resource.accept()
            print('[HTTP_BEGIN]')
            request = client.recv(1024)
            request = request.decode()
            print(request)

            if get_html.match(request):
               client.sendall(response.encode())
               client.sendall(html.encode())
            elif get_js.match(request):
               header = 'HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n' + js
               client.sendall(header.encode())
            elif post_buttons.match(request):
               states = parse_state.search(request).group(1)
               header = 'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n' + states
               client.sendall(header.encode())
            elif get_vals.search(request):
               header = 'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n' + states
               print(header)
               client.sendall(header.encode())
            else:
               header = 'HTTP/1.1 404 Not Found\r\n\r\n'
               client.sendall(header.encode())
            
            client.close()
            print('[HTTP_END]')