s = input("x: ").strip().replace(",", ".")
t = int(input("tipo (1=def, 2=excesso): "))

p = s.find(".")
d = 0 if p == -1 else (len(s) - p - 1) 
xbar = float(s)
u = 10 ** (-d)

if t == 1:
    a = xbar
    b = xbar + u
    dx = u
    print("x E [{}, {}]".format(a, b))
    print("dx = {}".format(dx))
elif t == 2:
    a = xbar - u
    b = xbar
    dx = u
    print("x E [{}, {}]".format(a, b))
    print("dx = {}".format(dx))
else:
    print("Erro")
