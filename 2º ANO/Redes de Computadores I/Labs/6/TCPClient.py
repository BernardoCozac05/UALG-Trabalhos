from socket import *

# Endereço IP do servidor.
# Como o teste é local, utiliza-se localhost.
serverName = '127.0.0.1'

# Porto de atendimento do servidor TCP.
serverPort = 12000

# Criação de um socket TCP sobre IPv4.
clientSocket = socket(AF_INET, SOCK_STREAM)

# Estabelecimento da ligação ao servidor.
clientSocket.connect((serverName, serverPort))

# Leitura da mensagem introduzida pelo utilizador.
sentence = input('Input lowercase sentence:')

# Envio da mensagem pela ligação TCP.
# Em Python 3, a string é convertida para bytes.
clientSocket.send(sentence.encode())

# Receção da resposta enviada pelo servidor.
modifiedSentence = clientSocket.recv(1024)

# Apresentação da resposta no ecrã.
print('From Server:', modifiedSentence.decode())

# Encerramento da ligação TCP.
clientSocket.close()