import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # AF_INET = IPv4, SOCK_DGRAM = indica che uso UDP
remote_info = ("192.168.1.35", 1234)  # IP e porta del server in ascolto

i=0

while i<20:
    i=i+1
    msg = "Hello World!"
    client_socket.sendto(msg.encode(), remote_info)  # encode converte la stringa in un array di byte
