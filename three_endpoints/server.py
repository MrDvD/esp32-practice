import socket, re

response = 'HTTP/1.0 200 OK\r\n\r\n'

get_html = re.compile(r'^GET / ')
post_buts = re.compile(r'^POST /buttons')
get_js = re.compile(r'^GET /script.js')
get_val = re.compile(r'GET /values')

get_vals = re.compile(r'button1=(\d)&buttons2=(\d)')

DATA = 'POST request was successful'

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
         js = 'HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n' + js

         led1 = '0'
         led2 = '0'

         while True:
            client, addr = resource.accept()
            print('[HTTP_BEGIN]')
            request = client.recv(1024)
            request = request.decode('utf-8')
            print(request)

            if get_html.match(request):
               client.sendall(response.encode())
               client.sendall(html.encode())
            elif get_js.match(request):
               client.sendall(js.encode())
            elif post_buts.match(request):
               result = get_vals.search(request)
               led1 = result.group(1)
               led2 = result.group(2)
               client.sendall((led1 + led2 + '\n').encode())
            elif get_val.search(request):
               res = 'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n' + led1 + led2
               client.sendall(res.encode())
            
            client.close()
            print('[HTTP_END]')