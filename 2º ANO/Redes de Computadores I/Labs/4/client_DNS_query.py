import random
import socket
import struct
import sys

# Dados fornecidos pelo professor
DNS_SERVER = "10.10.22.228"
DNS_PORT = 53
TIMEOUT = 5

# Tipos DNS suportados
QTYPE_MAP = {
    "A": 1,
    "NS": 2,
    "CNAME": 5,
    "MX": 15,
    "AAAA": 28,
}

REVERSE_QTYPE_MAP = {v: k for k, v in QTYPE_MAP.items()}


def encode_domain_name(domain):
    """
    Converte um domínio para o formato DNS.
    Exemplo: ualg.pt -> 04 75 61 6c 67 02 70 74 00
    """
    domain = domain.strip().strip(".")
    if not domain:
        raise ValueError("Domínio inválido.")

    labels = domain.split(".")
    encoded = b""

    for label in labels:
        if len(label) > 63:
            raise ValueError(f"Label demasiado grande: {label}")
        encoded += struct.pack("!B", len(label))
        encoded += label.encode("ascii")

    encoded += b"\x00"
    return encoded


def build_dns_query(domain, qtype=1, recursion_desired=True):
    """
    Constrói uma query DNS manualmente.
    Header DNS:
      ID, FLAGS, QDCOUNT, ANCOUNT, NSCOUNT, ARCOUNT
    """
    transaction_id = random.randint(0, 65535)

    # Standard query
    # RD=1 se recursion_desired=True
    flags = 0x0100 if recursion_desired else 0x0000

    qdcount = 1
    ancount = 0
    nscount = 0
    arcount = 0

    header = struct.pack(
        "!HHHHHH",
        transaction_id,
        flags,
        qdcount,
        ancount,
        nscount,
        arcount,
    )

    qname = encode_domain_name(domain)
    qclass = 1  # IN
    question = qname + struct.pack("!HH", qtype, qclass)

    return transaction_id, header + question


def decode_name(message, offset):
    """
    Lê um nome DNS, incluindo compression pointers.
    Devolve:
      (nome, novo_offset)
    """
    labels = []
    jumped = False
    original_offset = offset

    while True:
        if offset >= len(message):
            raise ValueError("Offset fora da mensagem ao descodificar nome DNS.")

        length = message[offset]

        # compression pointer
        if (length & 0xC0) == 0xC0:
            if offset + 1 >= len(message):
                raise ValueError("Pointer DNS incompleto.")

            pointer = struct.unpack("!H", message[offset:offset + 2])[0] & 0x3FFF

            if not jumped:
                original_offset = offset + 2

            offset = pointer
            jumped = True
            continue

        # fim do nome
        if length == 0:
            offset += 1
            break

        offset += 1

        if offset + length > len(message):
            raise ValueError("Label DNS fora dos limites da mensagem.")

        label = message[offset:offset + length].decode("ascii", errors="replace")
        labels.append(label)
        offset += length

    name = ".".join(labels)

    if jumped:
        return name, original_offset
    return name, offset


def parse_header(data):
    if len(data) < 12:
        raise ValueError("Resposta DNS demasiado curta para conter header.")

    transaction_id, flags, qdcount, ancount, nscount, arcount = struct.unpack(
        "!HHHHHH", data[:12]
    )

    return {
        "id": transaction_id,
        "flags": flags,
        "qdcount": qdcount,
        "ancount": ancount,
        "nscount": nscount,
        "arcount": arcount,
    }


def parse_flags(flags):
    return {
        "QR": (flags >> 15) & 0x1,
        "OPCODE": (flags >> 11) & 0xF,
        "AA": (flags >> 10) & 0x1,
        "TC": (flags >> 9) & 0x1,
        "RD": (flags >> 8) & 0x1,
        "RA": (flags >> 7) & 0x1,
        "Z": (flags >> 4) & 0x7,
        "RCODE": flags & 0xF,
    }


def parse_question_section(message, offset, qdcount):
    questions = []

    for _ in range(qdcount):
        qname, offset = decode_name(message, offset)

        if offset + 4 > len(message):
            raise ValueError("Question section incompleta.")

        qtype, qclass = struct.unpack("!HH", message[offset:offset + 4])
        offset += 4

        questions.append({
            "qname": qname,
            "qtype": qtype,
            "qclass": qclass,
        })

    return questions, offset


def parse_rdata(message, rtype, rdata_offset, rdlength):
    rdata = message[rdata_offset:rdata_offset + rdlength]

    if rtype == 1 and rdlength == 4:  # A
        return socket.inet_ntoa(rdata)

    if rtype == 28 and rdlength == 16:  # AAAA
        return socket.inet_ntop(socket.AF_INET6, rdata)

    if rtype in (2, 5):  # NS, CNAME
        name, _ = decode_name(message, rdata_offset)
        return name

    if rtype == 15 and rdlength >= 3:  # MX
        preference = struct.unpack("!H", rdata[:2])[0]
        exchange, _ = decode_name(message, rdata_offset + 2)
        return {
            "preference": preference,
            "exchange": exchange,
        }

    return rdata.hex()


def parse_resource_records(message, offset, count):
    records = []

    for _ in range(count):
        name, offset = decode_name(message, offset)

        if offset + 10 > len(message):
            raise ValueError("Resource Record incompleto.")

        rtype, rclass, ttl, rdlength = struct.unpack(
            "!HHIH", message[offset:offset + 10]
        )
        offset += 10

        if offset + rdlength > len(message):
            raise ValueError("RDATA fora dos limites da mensagem.")

        rdata_offset = offset
        parsed_rdata = parse_rdata(message, rtype, rdata_offset, rdlength)
        offset += rdlength

        records.append({
            "name": name,
            "type": rtype,
            "type_name": REVERSE_QTYPE_MAP.get(rtype, f"TYPE_{rtype}"),
            "class": rclass,
            "ttl": ttl,
            "rdlength": rdlength,
            "rdata": parsed_rdata,
        })

    return records, offset


def send_dns_query(server, port, query):
    """
    Envia a query DNS por UDP.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.settimeout(TIMEOUT)

        print("A enviar para:", (server, port))
        print("Tamanho da query:", len(query))
        print("Bytes da query (hex):", query.hex())

        sent = sock.sendto(query, (server, port))
        print("Bytes enviados:", sent)

        response, addr = sock.recvfrom(4096)
        print("Resposta recebida de:", addr)
        print("Tamanho da resposta:", len(response))
        return response


def perform_query(domain, qtype, recursion_desired):
    query_id, query = build_dns_query(domain, qtype, recursion_desired)
    response = send_dns_query(DNS_SERVER, DNS_PORT, query)
    return query_id, response


def print_header(header):
    print("=== HEADER ===")
    print(f"ID:       0x{header['id']:04x}")
    print(f"FLAGS:    0x{header['flags']:04x}")
    print(f"QDCOUNT:  {header['qdcount']}")
    print(f"ANCOUNT:  {header['ancount']}")
    print(f"NSCOUNT:  {header['nscount']}")
    print(f"ARCOUNT:  {header['arcount']}")

    f = parse_flags(header["flags"])
    print("\nInterpretação das flags:")
    print(f"  QR     = {f['QR']} ({'reply' if f['QR'] else 'query'})")
    print(f"  OPCODE = {f['OPCODE']}")
    print(f"  AA     = {f['AA']}")
    print(f"  TC     = {f['TC']}")
    print(f"  RD     = {f['RD']}")
    print(f"  RA     = {f['RA']}")
    print(f"  Z      = {f['Z']}")
    print(f"  RCODE  = {f['RCODE']}")
    print()


def print_questions(questions):
    print("=== QUESTION SECTION ===")
    for i, q in enumerate(questions, 1):
        print(f"Pergunta {i}:")
        print(f"  QNAME  = {q['qname']}")
        print(f"  QTYPE  = {q['qtype']} ({REVERSE_QTYPE_MAP.get(q['qtype'], 'UNKNOWN')})")
        print(f"  QCLASS = {q['qclass']}")
    print()


def print_records(title, records):
    print(f"=== {title} ===")
    if not records:
        print("(sem registos)\n")
        return

    for i, rr in enumerate(records, 1):
        print(f"Registo {i}:")
        print(f"  NAME     = {rr['name']}")
        print(f"  TYPE     = {rr['type']} ({rr['type_name']})")
        print(f"  CLASS    = {rr['class']}")
        print(f"  TTL      = {rr['ttl']}")
        print(f"  RDLENGTH = {rr['rdlength']}")
        print(f"  RDATA    = {rr['rdata']}")
    print()


def main():
    if len(sys.argv) < 2:
        print("Uso: python3 DNS_client_query.py <dominio> [TIPO]")
        print("Exemplo: python3 DNS_client_query.py ualg.pt A")
        sys.exit(1)

    domain = sys.argv[1]
    qtype_name = sys.argv[2].upper() if len(sys.argv) >= 3 else "A"

    if qtype_name not in QTYPE_MAP:
        print(f"Tipo DNS não suportado: {qtype_name}")
        print(f"Tipos suportados: {', '.join(QTYPE_MAP.keys())}")
        sys.exit(1)

    qtype = QTYPE_MAP[qtype_name]

    print(f"Servidor DNS: {DNS_SERVER}:{DNS_PORT}")
    print(f"Domínio:      {domain}")
    print(f"Tipo:         {qtype_name}")
    print()

    response = None
    query_id = None

    # 1ª tentativa: RD=1
    try:
        print("Tentativa 1: query com recursion desired (RD=1)")
        query_id, response = perform_query(domain, qtype, recursion_desired=True)
    except socket.timeout:
        print("Sem resposta com RD=1.\n")
    except Exception as e:
        print(f"Erro na tentativa com RD=1: {e}\n")

    # 2ª tentativa: RD=0
    if response is None:
        try:
            print("Tentativa 2: query sem recursion desired (RD=0)")
            query_id, response = perform_query(domain, qtype, recursion_desired=False)
        except socket.timeout:
            print("Sem resposta com RD=0.\n")
        except Exception as e:
            print(f"Erro na tentativa com RD=0: {e}\n")

    if response is None:
        print("Timeout: não foi recebida resposta do servidor DNS em nenhuma tentativa.")
        sys.exit(1)

    try:
        header = parse_header(response)

        if header["id"] != query_id:
            print("Aviso: o transaction ID da resposta não coincide com o da query.")
            print(f"Query ID:    0x{query_id:04x}")
            print(f"Resposta ID: 0x{header['id']:04x}")
            print()

        offset = 12

        questions, offset = parse_question_section(response, offset, header["qdcount"])
        answers, offset = parse_resource_records(response, offset, header["ancount"])
        authority, offset = parse_resource_records(response, offset, header["nscount"])
        additional, offset = parse_resource_records(response, offset, header["arcount"])

        print()
        print_header(header)
        print_questions(questions)
        print_records("ANSWER SECTION", answers)
        print_records("AUTHORITY SECTION", authority)
        print_records("ADDITIONAL SECTION", additional)

    except Exception as e:
        print("A resposta foi recebida, mas ocorreu erro ao interpretá-la.")
        print("Erro:", e)
        print("Resposta em hex:")
        print(response.hex())
        sys.exit(1)


if __name__ == "__main__":
    main()