import bluetooth

BDADDR = '74:C6:3B:38:A4:A0'

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((BDADDR, 1))
print('[CONNECTED]')
sock.send('Hi there')
sock.close()