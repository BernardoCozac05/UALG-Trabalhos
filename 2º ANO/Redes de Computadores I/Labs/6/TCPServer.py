from socket import *

# Porto onde o servidor ficará à escuta.
serverPort = 12000

# Criação de um socket TCP sobre IPv4.
serverSocket = socket(AF_INET, SOCK_STREAM)

# Associação do socket ao porto definido.
serverSocket.bind(('', serverPort))

# Colocação do socket em modo de escuta.
# O argumento 1 indica a dimensão máxima da fila de ligações pendentes.
serverSocket.listen(1)

# Mensagem informativa de arranque do servidor.
print('The server is ready to receive')

# Ciclo infinito para atendimento contínuo de clientes.
while 1:
    # Aceitação de uma nova ligação.
    # É criado um socket específico para comunicar com esse cliente.
    connectionSocket, addr = serverSocket.accept()

    # Receção da mensagem enviada pelo cliente.
    sentence = connectionSocket.recv(1024)

    # Conversão da mensagem para letras maiúsculas.
    capitalizedSentence = sentence.upper()

    # Envio da resposta ao cliente.
    connectionSocket.send(capitalizedSentence)

    # Encerramento da ligação com o cliente atual.
    connectionSocket.close()