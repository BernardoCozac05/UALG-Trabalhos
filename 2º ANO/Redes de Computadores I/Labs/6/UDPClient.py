from socket import *

# Endereço IP do servidor.
# Como o teste é feito na mesma máquina, utiliza-se localhost.
serverName = '127.0.0.1'

# Porto de atendimento do servidor UDP.
serverPort = 12000

# Criação de um socket UDP sobre IPv4.
clientSocket = socket(AF_INET, SOCK_DGRAM)

# Leitura da mensagem introduzida pelo utilizador.
message = input('Input lowercase sentence:')

# Envio da mensagem para o servidor.
# Em Python 3, a string é convertida para bytes com encode().
clientSocket.sendto(message.encode(), (serverName, serverPort))

# Receção da resposta enviada pelo servidor.
# O método devolve a mensagem recebida e o endereço de origem.
modifiedMessage, serverAddress = clientSocket.recvfrom(2048)

# Apresentação da resposta no ecrã.
# Os bytes recebidos são convertidos novamente para string.
print(modifiedMessage.decode())

# Encerramento do socket do cliente.
clientSocket.close()