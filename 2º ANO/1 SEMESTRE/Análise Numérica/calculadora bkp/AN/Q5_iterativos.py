# Q5 — Metodos Iterativos para Ax=b (Jacobi / Gauss-Seidel / SOR)
# MicroPython 1.9.4 (fx-CG50) — tudo por INPUT, sem editar codigo.
#
# MODO TEORICO (sem numeros):
#   - Mostra as formulas de decomposicao e a forma x^{k+1} = T x^k + c.
#
# MODO NUMERICO:
#   - Le n, A, b
#   - Escolhe metodo (J/GS/SOR)
#   - Calcula T e c (quando faz sentido) SEM inverter matrizes:
#       Jacobi: T=-D^{-1}(L+U), c=D^{-1}b
#       GS:     T=-(D+L)^{-1}U, c=(D+L)^{-1}b  (via substituicao direta)
#       SOR:    T=(D+wL)^{-1}((1-w)D - wU), c=w(D+wL)^{-1}b
#   - Pode executar iteracoes e parar por eps em ||dx||_inf ou ||r||_inf.

def _to_num(s):
    s = s.strip().replace(",", ".")
    if "/" in s:
        p = s.split("/")
        if len(p) != 2:
            raise ValueError("Frac invalida")
        return float(p[0].strip()) / float(p[1].strip())
    return float(s)

def _read_matrix(n, name):
    A = []
    i = 0
    while i < n:
        row = []
        j = 0
        while j < n:
            row.append(_to_num(input("{}[{},{}] = ".format(name, i, j))))
            j += 1
        A.append(row)
        i += 1
    return A

def _read_vector(n, name):
    v = []
    i = 0
    while i < n:
        v.append(_to_num(input("{}[{}] = ".format(name, i))))
        i += 1
    return v

def _zeros(n):
    return [0.0] * n

def _mat_vec(A, x):
    n = len(A)
    r = [0.0] * n
    i = 0
    while i < n:
        s = 0.0
        j = 0
        while j < n:
            s += A[i][j] * x[j]
            j += 1
        r[i] = s
        i += 1
    return r

def _vec_sub(a, b):
    n = len(a)
    r = [0.0] * n
    i = 0
    while i < n:
        r[i] = a[i] - b[i]
        i += 1
    return r

def _inf_norm(v):
    m = 0.0
    i = 0
    while i < len(v):
        a = v[i]
        if a < 0:
            a = -a
        if a > m:
            m = a
        i += 1
    return m

def _split_DLU(A):
    # A = D + L + U (D diagonal; L estritamente inferior; U estritamente superior)
    n = len(A)
    D = [0.0] * n
    L = [[0.0]*n for _ in range(n)]
    U = [[0.0]*n for _ in range(n)]
    i = 0
    while i < n:
        D[i] = A[i][i]
        j = 0
        while j < n:
            if i > j:
                L[i][j] = A[i][j]
            elif i < j:
                U[i][j] = A[i][j]
            j += 1
        i += 1
    return D, L, U

def _forward_solve_lower(D, L, rhs, omega):
    # resolve (D + omega*L) y = rhs  (D diagonal lista, L estritamente inferior matriz)
    n = len(D)
    y = [0.0] * n
    i = 0
    while i < n:
        s = rhs[i]
        j = 0
        while j < i:
            s -= (omega * L[i][j]) * y[j]
            j += 1
        if D[i] == 0:
            raise ValueError("Diagonal zero em i={}".format(i))
        y[i] = s / D[i]
        i += 1
    return y

def _diag_dominance_report(A):
    # devolve (ok, linha_pior, margem_min)
    n = len(A)
    ok = True
    worst_i = 0
    margin_min = 1e99
    i = 0
    while i < n:
        diag = A[i][i]
        if diag < 0:
            diag = -diag
        s = 0.0
        j = 0
        while j < n:
            if j != i:
                a = A[i][j]
                if a < 0:
                    a = -a
                s += a
            j += 1
        margin = diag - s
        if margin < margin_min:
            margin_min = margin
            worst_i = i
        if diag <= s:
            ok = False
        i += 1
    return ok, worst_i, margin_min

def _jacobi_T_c(D, L, U, b):
    n = len(D)
    T = [[0.0]*n for _ in range(n)]
    c = [0.0]*n
    i = 0
    while i < n:
        if D[i] == 0:
            raise ValueError("Diagonal zero em i={}".format(i))
        c[i] = b[i] / D[i]
        j = 0
        while j < n:
            T[i][j] = -(L[i][j] + U[i][j]) / D[i]
            j += 1
        i += 1
    return T, c

def _gs_T_c(D, L, U, b):
    # T = -(D+L)^{-1} U  ;  c = (D+L)^{-1} b
    n = len(D)
    T = [[0.0]*n for _ in range(n)]
    # colunas de U
    j = 0
    while j < n:
        col = [0.0]*n
        i = 0
        while i < n:
            col[i] = U[i][j]
            i += 1
        y = _forward_solve_lower(D, L, col, 1.0)
        i = 0
        while i < n:
            T[i][j] = -y[i]
            i += 1
        j += 1
    c = _forward_solve_lower(D, L, b, 1.0)
    return T, c

def _sor_T_c(D, L, U, b, w):
    # T = (D+wL)^{-1}((1-w)D - wU) ; c = w(D+wL)^{-1}b
    n = len(D)
    # construir N = (1-w)D - wU  (N é upper + diagonal)
    N = [[0.0]*n for _ in range(n)]
    i = 0
    while i < n:
        N[i][i] = (1.0 - w) * D[i]
        j = i+1
        while j < n:
            N[i][j] = -w * U[i][j]
            j += 1
        i += 1

    T = [[0.0]*n for _ in range(n)]
    # colunas de N
    j = 0
    while j < n:
        col = [0.0]*n
        i = 0
        while i < n:
            col[i] = N[i][j]
            i += 1
        y = _forward_solve_lower(D, L, col, w)
        i = 0
        while i < n:
            T[i][j] = y[i]
            i += 1
        j += 1

    c = _forward_solve_lower(D, L, b, w)
    i = 0
    while i < n:
        c[i] = w * c[i]
        i += 1
    return T, c

def _print_matrix(M, name):
    print(name + " =")
    i = 0
    while i < len(M):
        print(M[i])
        i += 1

def _theory():
    print("")
    print("=== MODO TEORICO (Q5) ===")
    print("Decomposicao: A = D + L + U")
    print("  D: diagonal de A")
    print("  L: parte estritamente inferior de A")
    print("  U: parte estritamente superior de A")
    print("")
    print("Jacobi:")
    print("  x^{k+1} = -D^{-1}(L+U) x^k + D^{-1} b")
    print("  T = -D^{-1}(L+U) ,  c = D^{-1} b")
    print("")
    print("Gauss-Seidel:")
    print("  (D+L) x^{k+1} = b - U x^k")
    print("  x^{k+1} = -(D+L)^{-1}U x^k + (D+L)^{-1} b")
    print("  T = -(D+L)^{-1}U ,  c = (D+L)^{-1} b")
    print("")
    print("SOR (w):")
    print("  (D + wL) x^{k+1} = w b + [(1-w)D - wU] x^k")
    print("  x^{k+1} = (D+wL)^{-1}((1-w)D - wU) x^k + w(D+wL)^{-1} b")
    print("  T = (D+wL)^{-1}((1-w)D - wU) ,  c = w(D+wL)^{-1} b")
    print("")
    print("Observacao: (D+L) e (D+wL) sao triangulares inferiores -> resolve-se por substituicao direta.")
    print("Para convergencia: condicao suficiente comum: A estritamente diagonalmente dominante (por linhas).")
    print("")

def _iterate(A, b, method, x0, iters, eps_dx, eps_r, w):
    D, L, U = _split_DLU(A)
    x = x0[:]
    k = 0
    while k < iters:
        if method == "J":
            # x_new = D^{-1}(b - (L+U)x)
            n = len(D)
            x_new = [0.0]*n
            i = 0
            while i < n:
                s = b[i]
                j = 0
                while j < n:
                    if j != i:
                        s -= (A[i][j]) * x[j]
                    j += 1
                if D[i] == 0:
                    raise ValueError("Diagonal zero em i={}".format(i))
                x_new[i] = s / D[i]
                i += 1

        elif method == "GS":
            # (D+L)x_new = b - Ux
            rhs = _vec_sub(b, _mat_vec(U, x))
            x_new = _forward_solve_lower(D, L, rhs, 1.0)

        else:
            # SOR
            # (D+wL)x_new = w b - [wU + (w-1)D] x
            n = len(D)
            Ux = _mat_vec(U, x)
            rhs = [0.0]*n
            i = 0
            while i < n:
                rhs[i] = w*b[i] - (w*Ux[i] + (w-1.0)*D[i]*x[i])
                i += 1
            x_new = _forward_solve_lower(D, L, rhs, w)

        dx = _vec_sub(x_new, x)
        ndx = _inf_norm(dx)
        r = _vec_sub(_mat_vec(A, x_new), b)
        nr = _inf_norm(r)

        print("k={}  x={}".format(k+1, x_new))
        print("   ||dx||_inf = {}".format(ndx))
        print("   ||r||_inf  = {}".format(nr))

        x = x_new

        if eps_dx > 0 and ndx < eps_dx:
            print("STOP: ||dx||_inf < eps_dx")
            break
        if eps_r > 0 and nr < eps_r:
            print("STOP: ||r||_inf < eps_r")
            break

        k += 1
    return x

def main():
    print("Q5 — Metodos Iterativos (Jacobi / Gauss-Seidel / SOR)")
    print("1 - Modo teorico (sem numeros)")
    print("2 - Modo numerico (com A e b)")
    mode = input("Opcao = ").strip()

    if mode == "1":
        _theory()
        return

    if mode != "2":
        print("Opcao invalida.")
        return

    n = int(input("Dimensao n = ").strip())
    print("Introduza A:")
    A = _read_matrix(n, "A")
    print("Introduza b:")
    b = _read_vector(n, "b")

    ok, wi, margin = _diag_dominance_report(A)
    print("")
    if ok:
        print("Aviso: A e estritamente diagonalmente dominante (por linhas) -> convergencia tipicamente garantida.")
    else:
        print("Aviso: A NAO e estritamente diagonalmente dominante (linha pior i={} com margem diag-soma = {}).".format(wi, margin))
        print("      Pode divergir (depende do metodo e do problema).")

    print("")
    print("Escolha metodo:")
    print("1 - Jacobi")
    print("2 - Gauss-Seidel")
    print("3 - SOR (variacao)")
    op = input("Opcao = ").strip()

    method = None
    w = 1.0
    if op == "1":
        method = "J"
    elif op == "2":
        method = "GS"
    elif op == "3":
        method = "SOR"
        w = _to_num(input("w (ex: 1.2) = ").strip())
    else:
        print("Opcao invalida.")
        return

    D, L, U = _split_DLU(A)

    show_Tc = input("Calcular e mostrar T e c? (s/n) ").strip().lower()
    if show_Tc == "s":
        try:
            if method == "J":
                T, c = _jacobi_T_c(D, L, U, b)
            elif method == "GS":
                T, c = _gs_T_c(D, L, U, b)
            else:
                T, c = _sor_T_c(D, L, U, b, w)
            _print_matrix(T, "T")
            print("c =", c)
        except Exception as e:
            print("Nao foi possivel calcular T,c:", e)

    run = input("Executar iteracoes? (s/n) ").strip().lower()
    if run != "s":
        return

    use_x0 = input("Dar x0 manualmente? (s/n) ").strip().lower()
    if use_x0 == "s":
        x0 = _read_vector(n, "x0")
    else:
        x0 = _zeros(n)

    iters = int(input("Numero maximo de iteracoes = ").strip())
    eps_dx = _to_num(input("eps_dx (<=0 desliga) = ").strip())
    eps_r  = _to_num(input("eps_r  (<=0 desliga) = ").strip())

    x = _iterate(A, b, method, x0, iters, eps_dx, eps_r, w)
    print("")
    print("Resultado final x =", x)

main()
