import random, heapq, sys

def gerar(V, E, seed, wmax=1000):
    rnd = random.Random(seed)
    arestas = {}
    # árvore aleatória garante conexidade
    ordem = list(range(V)); rnd.shuffle(ordem)
    for i in range(1, V):
        u, v = ordem[i], ordem[rnd.randrange(i)]
        arestas[(min(u,v), max(u,v))] = rnd.randint(1, wmax)
    while len(arestas) < E:
        u, v = rnd.randrange(V), rnd.randrange(V)
        if u == v: continue
        k = (min(u,v), max(u,v))
        if k not in arestas:
            arestas[k] = rnd.randint(1, wmax)
    lst = [(u, v, w) for (u, v), w in arestas.items()]
    rnd.shuffle(lst)
    return lst

def kruskal(V, lst):
    pai = list(range(V))
    def find(x):
        while pai[x] != x:
            pai[x] = pai[pai[x]]; x = pai[x]
        return x
    total = n = 0
    for u, v, w in sorted(lst, key=lambda a: a[2]):
        a, b = find(u), find(v)
        if a != b:
            pai[a] = b; total += w; n += 1
            if n == V - 1: break
    return total, n

def prim(V, lst):
    adj = [[] for _ in range(V)]
    for u, v, w in lst:
        adj[u].append((w, v)); adj[v].append((w, u))
    vis = [False]*V; h = [(0, 0)]; total = n = 0
    while h:
        w, u = heapq.heappop(h)
        if vis[u]: continue
        vis[u] = True; total += w; n += 1
        for e in adj[u]:
            if not vis[e[1]]: heapq.heappush(h, e)
    return total, n - 1

def salvar(nome, V, lst, label=False):
    with open(nome, "w") as f:
        f.write(f"graph G {{\n  // {V} vertices, {len(lst)} arestas\n")
        for u, v, w in lst:
            extra = f', label="{w}"' if label else ""
            f.write(f"  {u} -- {v} [weight={w}{extra}];\n")
        f.write("}\n")

specs = [
    ("g01_mini.dot",          8,      14,      1, True),
    ("g02_pequeno.dot",       50,     150,     2, False),
    ("g03_medio.dot",         1000,   5000,    3, False),
    ("g04_grande.dot",        10000,  60000,   4, False),
    ("g05_muito_grande.dot",  100000, 500000,  5, False),
    ("g06_denso.dot",         2000,   1000000, 6, False),
]
gab = ["arquivo | vertices | arestas | peso total da MST | arestas na MST"]
for nome, V, E, s, lab in specs:
    lst = gerar(V, E, s, wmax=20 if lab else 1000)
    salvar(nome, V, lst, True)
    k, nk = kruskal(V, lst); p, np_ = prim(V, lst)
    assert k == p and nk == np_ == V - 1, nome
    gab.append(f"{nome} | {V} | {E} | {k} | {nk}")
    print(gab[-1], flush=True)
open("gabarito.txt", "w").write("\n".join(gab) + "\n")
