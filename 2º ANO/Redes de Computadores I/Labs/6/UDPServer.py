from socket import *

# Porto onde o servidor ficará à escuta.
serverPort = 12000

# Criação de um socket UDP sobre IPv4.
serverSocket = socket(AF_INET, SOCK_DGRAM)

# Associação do socket ao porto definido.
# A string vazia indica que o servidor aceita datagramas
# dirigidos a qualquer interface da máquina.
serverSocket.bind(('', serverPort))

# Mensagem informativa de arranque do servidor.
print("The server is ready to receive")

# Ciclo infinito para atendimento contínuo de pedidos.
while 1:
    # Receção de uma mensagem enviada por um cliente.
    # São obtidos os dados e o endereço do emissor.
    message, clientAddress = serverSocket.recvfrom(2048)

    # Conversão da mensagem para letras maiúsculas.
    modifiedMessage = message.upper()

    # Envio da resposta para o mesmo cliente.
    serverSocket.sendto(modifiedMessage, clientAddress)