'''
Created on 13/11/2016

@author: ernesto

casos vacio:
    1 separado
    2 distancia normal
    3 a un lado no en ruta
    4 a un lado en ruta
    5 en salida
    6 mas barato mover
    7 mas barato brincar

casos salida:
    1 separada
    2 distancia normal
    3 a traves de tunel
    4 mas barato tomar tunel
    5 mas barato dar vueltesota
    6 al lado
    7 encima
    
casos cacao:

hechos:
    caca10.txt
    v1-s1
    v1-s2
    v1-s3
    v1-s6
    v1-s7
    v2-s1
    v2-s2
    v2-s3
    v2-s6
    v2-s7
    v3-s1
    v3-s2
    v3-s3
    v3-s6
    v3-s7
    v4-s1
    v4-s2
    v4-s3
    v4-s6
    v4-s7
    v5-s1
    v5-s2
    v5-s3
    v5-s6
    v5-s7
    caca11.txt
    v6-s1
    v6-s2
    v6-s3
    v6-s5
    v6-s6
    v6-s7
    caca12.txt
    v7-s1
    v7-s2
    v7-s3
    v7-s4
    v7-s6
    v7-s7

'''
import queue
import logging
import sys
import heapq

nivel_log = logging.ERROR
#nivel_log = logging.DEBUG
logger_cagada = None

def caca_comun_matrix_a_cadena(matrix):
    res = ""
    res = ('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
      for row in matrix]))
    return res
            
            
class PriorityQueue(object):
    """Priority queue based on heap, capable of inserting a new node with
    desired priority, updating the priority of an existing node and deleting
    an abitrary node while keeping invariant"""

    def __init__(self, heap=[]):
        """if 'heap' is not empty, make sure it's heapified"""

        logger_cagada.debug("pero si el orig heap %s" % heap)
        heapq.heapify(heap)
        logger_cagada.debug("a cihnga el heap %s" % heap)
        self.heap = heap
        self.entry_finder = dict({i[-1]: i for i in heap})
        logger_cagada.debug("el finder es %s" % self.entry_finder)
        self.REMOVED = sys.maxsize
        
    def es_menor_entrada_1(self, entrada_1, entrada_2):
        resul = False
        prio_1 = entrada_1[0]
        prio_2 = entrada_2[0]
        cagada_1 = entrada_1[1]
        cagada_2 = entrada_2[1]
        
        if(prio_1 == prio_2):
            if(cagada_1 is self.REMOVED):
                resul = False 
            else:
                if(cagada_2 is self.REMOVED):
                    resul = True
                else:
                    resul = cagada_1 < cagada_2
        else:
            resul = prio_1 < prio_2
        return resul
        
    def _siftdown(self, heap, startpos, pos):
        newitem = heap[pos]
        logger_cagada.debug("el nuevo meirda %s" % newitem)
        while pos > startpos:
            parentpos = (pos - 1) >> 1
            parent = heap[parentpos]
            logger_cagada.debug("q pedo con %s %s" % (newitem, parent))
#            if (newitem[1] is not self.REMOVED  and (parent[1] is self.REMOVED or newitem < parent)):
            if (self.es_menor_entrada_1(newitem, parent)):
                heap[pos] = parent
                pos = parentpos
                continue
            break
        heap[pos] = newitem

    def heappush(self, heap, item):
        heap.append(item)
        self._siftdown(heap, 0, len(heap) - 1)



    def insert(self, node, priority=0):
        """'entry_finder' bookkeeps all valid entries, which are bonded in
        'heap'. Changing an entry in either leads to changes in both."""

        if node in self.entry_finder:
            self.delete(node)
        entry = [priority, node]
        self.entry_finder[node] = entry
        logger_cagada.debug("el puto entry %s" % entry)
        logger_cagada.debug("l nodo q c agrega %s es %s" % (type(node), node))
        self.heappush(self.heap, entry)
        logger_cagada.debug("el finde aora es %s" % self.entry_finder)
        logger_cagada.debug("el heap aora es %s" % self.heap)
        self.valida_caca()

    def delete(self, node):
        """Instead of breaking invariant by direct removal of an entry, mark
        the entry as "REMOVED" in 'heap' and remove it from 'entry_finder'.
        Logic in 'pop()' properly takes care of the deleted nodes."""

        logger_cagada.debug("norrando nodo %s" % (type(node)))
        entry = self.entry_finder.pop(node)
        logger_cagada.debug("la entry q c borra %s" % entry)
        entry[-1] = self.REMOVED
        logger_cagada.debug("el heap es %s" % self.heap)
        return entry[0]

    def _siftup(self, heap, pos):
        endpos = len(heap)
        startpos = pos
        newitem = heap[pos]
        logger_cagada.debug("la nueva mierda %s" % newitem)
        # Bubble up the smaller child until hitting a leaf.
        childpos = 2 * pos + 1  # leftmost child position
        while childpos < endpos:
            # Set childpos to index of smaller child.
            rightpos = childpos + 1
            logger_cagada.debug("q la chingada %s %s" % (heap[childpos], heap[rightpos] if rightpos < endpos else "Nada"))
#            if rightpos < endpos and (heap[rightpos][1] is not self.REMOVED and (heap[childpos][1] is self.REMOVED or not heap[childpos] < heap[rightpos])):
            if rightpos < endpos and not self.es_menor_entrada_1(heap[childpos], heap[rightpos]):
                childpos = rightpos
            # Move the smaller child up.
            heap[pos] = heap[childpos]
            pos = childpos
            childpos = 2 * pos + 1
        # The leaf at pos is empty now.  Put newitem there, and bubble it up
        # to its final resting place (by sifting its parents down).
        heap[pos] = newitem
        logger_cagada.debug("puta madre %s" % heap)
        self._siftdown(heap, startpos, pos)
    
    def heappop(self, heap):
        lastelt = heap.pop()  # raises appropriate IndexError if heap is empty
        if heap:
            returnitem = heap[0]
            heap[0] = lastelt
            self._siftup(heap, 0)
            return returnitem
        self.valida_caca()
        return lastelt

    def pop(self):
        """Any popped node marked by "REMOVED" does not return, the deleted
        nodes might be popped or still in heap, either case is fine."""

        while self.heap:
            logger_cagada.debug("elem de heap %s" % self.heap)
            priority, node = self.heappop(self.heap)
            if node is not self.REMOVED:
                del self.entry_finder[node]
                return priority, node
        raise KeyError('pop from an empty priority queue')
    
    def update(self, nodo, nueva_prioridad):
        self.delete(nodo)
        self.insert(nodo, nueva_prioridad)


    def valida_caca(self, idx=0, valor_padre=0):
        tam_caca = len(self.heap)
        if(not self.heap):
            return
        entrada = self.heap[idx]
        nuevo_valor_padre = 0
        valor_propio = 0
        logger_cagada.debug("validando nodo %s(%u)" % (entrada, idx))
        if(entrada[1] is self.REMOVED):
            nuevo_valor_padre = valor_padre
            valor_propio = self.REMOVED
        else:
            nuevo_valor_padre = entrada[0]
            valor_propio = nuevo_valor_padre
            assert valor_padre <= valor_propio, "puta mierda, en nodo %s(%u) se encontro una inconsistencia, valor padre %s, valor propio %s" % (entrada, idx, valor_padre, valor_propio)
            
        idx_hijo_izq = idx * 2 + 1
        idx_hijo_der = idx_hijo_izq + 1
        
        if(idx_hijo_izq < tam_caca):
            self.valida_caca(idx_hijo_izq, nuevo_valor_padre)
            
        if(idx_hijo_der < tam_caca):
            self.valida_caca(idx_hijo_der, nuevo_valor_padre)
        
            
            
            
def huaronmierda_genera_vecinos(matrix, nodo):
    POSIBLES_MOVIMIENTOS = [(0, -1), (0, 1), (-1, 0), (1, 0)]
    tam_x = len(matrix[0])
    tam_y = len(matrix)
    pos_x = nodo[1]
    pos_y = nodo[0]
    vecinos = []
    for mov_y, mov_x in POSIBLES_MOVIMIENTOS:
        candidote_y, candidote_x = (pos_y + mov_y, pos_x + mov_x)
        if(candidote_y < tam_y and candidote_x < tam_x and candidote_y >= 0 and candidote_x >= 0 and matrix[candidote_y][candidote_x] == "1"):
            vecinos.append((candidote_y, candidote_x))
    
    return vecinos

def huaronmierda_bfs(matrix, origen, destino):
    q = queue.Queue()
    ya_visitados = set()
    antecesor = {}
    ruta = []
    
    q.put(origen)
    while not q.empty():
        nodo_act = q.get()
        if(nodo_act == destino):
            logger_cagada.debug("se enconrto el destino")
            break
        ya_visitados.add(nodo_act)
        vecinos = huaronmierda_genera_vecinos(matrix, nodo_act)
        for vecino in vecinos:
            if(vecino not in ya_visitados):
                antecesor[vecino] = nodo_act
                q.put(vecino)
    
    if(destino in antecesor):
        ruta.insert(0, destino)
        nodo_act = antecesor[destino]
        while(nodo_act):
            ruta.insert(0, nodo_act)
            if(nodo_act in antecesor):
                nodo_act = antecesor[nodo_act]
            else:
                nodo_act = None

    logger_cagada.debug("la ruta de %s a %s es %s" % (origen, destino, ruta))
    
    return ruta

def huaronmierda_bfs_sin_nodo(matrix, origen, destino, nodo_caca):
    nodo_caca_x = nodo_caca[1]
    nodo_caca_y = nodo_caca[0]
    
    valor_original = matrix[nodo_caca_y][nodo_caca_x] 
    
    matrix[nodo_caca_y][nodo_caca_x] = "0"
    
    res = huaronmierda_bfs(matrix, origen, destino)
    matrix[nodo_caca_y][nodo_caca_x] = valor_original
    
    return res

# Casos de esquinados
# ----------+----------
# 2222222222|1111111111
#          2|1
#          2|1
#          2|1
# |4       2|1       3|
# |4       2|1       3|
# |4       2|1       3|
# |4       2|1       3|
# |44444444 | 33333333|
# ----------+----------
# |11111111 | 22222222|
# |1       3|4       2|
# |1       3|4       2| 
# |1       3|4       2| 
# |1       3|4       2| 
#          3|4
#          3|4
#          3|4
# 3333333333|4444444444
# ----------+----------
#


HUARONMIERDA_DIRECCION_VERTICAL = 0
HUARONMIERDA_DIRECCION_HORIZONTAL = 1
HUARONMIERDA_DIRECCION_ESQ_1 = 2
HUARONMIERDA_DIRECCION_ESQ_2 = 3
HUARONMIERDA_DIRECCION_ESQ_3 = 4
HUARONMIERDA_DIRECCION_ESQ_4 = 5
HUARONMIERDA_DIRECCION_FINAL = HUARONMIERDA_DIRECCION_ESQ_4
def huaronmierda_precaca(origen, matrix, chosto_brinco, direccion, matrixes_chostos):
    factor_destino_x = 0
    factor_destino_y = 0
    factor_mastorbo_x = 0
    factor_mastorbo_y = 0
    if(direccion == HUARONMIERDA_DIRECCION_VERTICAL):
        factor_destino_x = 2
        factor_mastorbo_x = 1
    else:
        if(direccion == HUARONMIERDA_DIRECCION_HORIZONTAL):
            factor_destino_y = 2
            factor_mastorbo_y = 1
        else:
            if(direccion == HUARONMIERDA_DIRECCION_ESQ_1):
                factor_destino_x = 1
                factor_destino_y = 1
                factor_mastorbo_x = 1
            else:
                if(direccion == HUARONMIERDA_DIRECCION_ESQ_2):
                    factor_destino_x = 1
                    factor_destino_y = -1
                    factor_mastorbo_x = 1
                else:
                    if(direccion == HUARONMIERDA_DIRECCION_ESQ_3):
                        factor_destino_x = 1
                        factor_destino_y = 1
                        factor_mastorbo_y = 1
                    else:
                        assert direccion == HUARONMIERDA_DIRECCION_ESQ_4, "q mierda, la direccion dad es %s" % direccion
                        factor_destino_x = 1
                        factor_destino_y = -1
                        factor_mastorbo_y = -1
            
    matrix_chostos = matrixes_chostos[direccion]
    
    destino = (_, _) = (origen[0] + factor_destino_x, origen[1] + factor_destino_y)
    mastorbo = (_, _) = (origen[0] + factor_mastorbo_x, origen[1] + factor_mastorbo_y)
    logger_cagada.debug("calculando cohosto de %s a %s con estorbo %s" % (origen, destino, mastorbo))
    
    if(matrix[destino[0]][destino[1]] != '1'):
        logger_cagada.debug("el destino (%s, %s) es una mierda, no c puede proceder" % (destino))
        return
    
    ruta_mierda = huaronmierda_bfs_sin_nodo(matrix, origen, destino, mastorbo)
    
    logger_cagada.debug("la rutal resultante %s" % ruta_mierda)
#    logger_cagada.debug("la matrix chostos\n%s" % (caca_comun_matrix_a_cadena(matrix_chostos)))
    
    chosto_mierda = 0
    if(ruta_mierda):
        tam_ruta = len(ruta_mierda)
        chosto_mierda = tam_ruta - 1
    else:
        chosto_mierda = chosto_brinco
        
    matrix_chostos[origen[0] - 2][origen[1] - 2] = chosto_mierda
    logger_cagada.debug("chosto de %s a %s calculado es %s" % ((origen[0] - 2, origen[1] - 2), (destino[0] - 2, destino[1] - 2), chosto_brinco))
        
        
def huaronmierda_crea_matrix_rodeada(matrix):
    tam_fila_matrix = len(matrix[0])
    nueva_mierda = [['0'] * (tam_fila_matrix + 4)]
    nueva_mierda.append(['0'] * (tam_fila_matrix + 4))
    
    for fila in matrix:
        nueva_mierda.append(['0', '0'] + fila + ['0', '0'])
        logger_cagada.debug("la fila q c anadio %s" % (nueva_mierda[-1]))
    
    nueva_mierda.append(['0'] * (tam_fila_matrix + 4))
    nueva_mierda.append(['0'] * (tam_fila_matrix + 4))
    return nueva_mierda


HUARONMIERDA_CACASO_ARRIBA = 0
HUARONMIERDA_CACASO_DERECHA = 1
HUARONMIERDA_CACASO_ABAJO = 2
HUARONMIERDA_CACASO_IZQUIERDA = 3
HUARONMIERDA_CACASO_ULTIMO = 3
HUARONMIERDA_CACASOS_DESCENDIENTE_DIRECTOS_MOVIMIENTOS = [(1, 0), (0, 1), (-1, 0), (0, -1)]
# HUARONMIERDA_CACASOS_DESCENDIENTE_DIRECTOS_CODIGOS = [HUARONMIERDA_CACASO_ARRIBA, HUARONMIERDA_CACASO_DERECHA, HUARONMIERDA_CACASO_ABAJO, HUARONMIERDA_CACASO_IZQUIERDA]
HUARONMIERDA_CACASOS_DESCENDIENTE = [
    # hijo:	ARRIBA								DERECHA								ABAJO								IZQUIERDA 
    [		-1, 								HUARONMIERDA_DIRECCION_ESQ_4, 		HUARONMIERDA_DIRECCION_VERTICAL, 	HUARONMIERDA_DIRECCION_ESQ_3],  # abuelo: ARRIBA
    [		HUARONMIERDA_DIRECCION_ESQ_4, 		 -1, 								HUARONMIERDA_DIRECCION_ESQ_1, 		HUARONMIERDA_DIRECCION_HORIZONTAL],  # abuelo: DERECHA
    [		HUARONMIERDA_DIRECCION_VERTICAL, 	HUARONMIERDA_DIRECCION_ESQ_1, 		 -1, 								HUARONMIERDA_DIRECCION_ESQ_2],  # abuelo: ABAJO
    [		HUARONMIERDA_DIRECCION_ESQ_3, 		HUARONMIERDA_DIRECCION_HORIZONTAL, 	HUARONMIERDA_DIRECCION_ESQ_2, 		 -1]  # abuelo: IZQUIERDA
]
def huaronmierda_determina_caso_predecesor(abuelo, padre, hijo):
    global HUARONMIERDA_CACASOS_DESCENDIENTE_DIRECTOS_MOVIMIENTOS
    global HUARONMIERDA_CACASOS_DESCENDIENTE
    idx_caso_abuelo = -1
    idx_caso_hijo = -1
    for idx_caso, movs in enumerate(HUARONMIERDA_CACASOS_DESCENDIENTE_DIRECTOS_MOVIMIENTOS):
        potencial_abuelo = (padre[0] + movs[0], padre[1] + movs[1])
        if(abuelo == potencial_abuelo):
            idx_caso_abuelo = idx_caso
            break
    
    assert idx_caso_abuelo != -1, "con abuelo %s, padre %s e ijo %s el cacaso de abuelo faio" % (abuelo, padre, hijo)
        
    for idx_caso, movs in enumerate(HUARONMIERDA_CACASOS_DESCENDIENTE_DIRECTOS_MOVIMIENTOS):
        potencial_hijo = (padre[0] + movs[0], padre[1] + movs[1])
        if(hijo == potencial_hijo):
            idx_caso_hijo = idx_caso
            break
    
    assert idx_caso_abuelo != -1, "con abuelo %s, padre %s e ijo %s el cacaso de hijo faio" % (abuelo, padre, hijo)
    
    logger_cagada.debug("el abuelo %s, padre %s e hijo %s, abuelo caso %s hijo caso %s" % (abuelo, padre, hijo, idx_caso_abuelo, idx_caso_hijo))
    
    idx_caso_predecesor = HUARONMIERDA_CACASOS_DESCENDIENTE[idx_caso_abuelo][idx_caso_hijo]
    
    assert idx_caso_predecesor != -1, "con abuelo %s, padre %s e ijo %s el cacaso de predecesor faio" % (abuelo, padre, hijo)
    
    logger_cagada.debug("el caso decendiente %s" % idx_caso_predecesor)

    return idx_caso_predecesor
        

def huaronmierda_dijkstra(source, destino, matrix, matrixes_chostos, vacio, chosto_trampa):

    processed = set()
    # WTF: x q sto s necesario? el heap default es vacio, q mamada
    pq = PriorityQueue(heap=[])
    shortest_path = {}
    predecesores = {}
    ruta_salida = []
    
    logger_cagada.debug("oie zhi")
    for idx_fila, fila in enumerate(matrix):
        for idx_col, vale in enumerate(fila):
            if(vale == '1'):
                logger_cagada.debug("insertando (%s, %s) en pq" % ((idx_fila, idx_col)))
                pq.insert((idx_fila, idx_col), sys.maxsize)
                
    size = len(pq.heap)
    
    logger_cagada.debug("bueno pero q mierda %s" % (type(source)))
    pq.update(source, 0)
            
    while size > len(processed):
        logger_cagada.debug("size es %u len de proc %u" % (size, len(processed)))
        min_dist, new_node = pq.pop()
        logger_cagada.debug("mierda min %u node %s" % (min_dist, new_node))
        if(min_dist == pq.REMOVED):
            logger_cagada.debug("o mierda, no ai mas camino")
            break
        processed.add(new_node)
#        uncharted.remove(new_node)
        shortest_path[new_node] = min_dist
        
        for vecino in huaronmierda_genera_vecinos(matrix, new_node):
            if(vecino not in processed):
                if(new_node == source):
#                    edge_dist = 1
                    if(vecino != vacio):
                        ruta_vacio_hijo = huaronmierda_bfs_sin_nodo(matrix, vacio, vecino, new_node)
                        if(ruta_vacio_hijo):
                            edge_dist = len(ruta_vacio_hijo) - 1
                        else:
                            edge_dist = chosto_trampa
                    else:
                        edge_dist = 0
                    logger_cagada.debug("el chosto de mover la 1rea vez, o sea de poner el vacio %s" % edge_dist)
                    edge_dist += 1
                else:
                    predecesor_padre = new_node
                    predecesor_abuelo = predecesores[predecesor_padre]
                    caso_familiar = huaronmierda_determina_caso_predecesor(predecesor_abuelo, predecesor_padre, vecino)
                    mas_abajo = sorted([predecesor_abuelo, vecino])[0]
                    logger_cagada.debug("el de mas abajo entre abuelo %s y hijo %s  es %s" % (predecesor_abuelo, vecino, mas_abajo))
                    chosto = matrixes_chostos[caso_familiar][mas_abajo[0]][mas_abajo[1]]
                    chosto += 1
                    logger_cagada.debug("el chosto de mover %s a %s es %s" % (predecesor_abuelo, vecino, chosto))
                    edge_dist = chosto
                new_dist = 0
                new_predecesor = None
                old_dist = pq.delete(vecino)
                logger_cagada.debug("comparando old %u con %u ", old_dist, min_dist)
                if(min_dist + edge_dist < old_dist):
                    new_dist = min_dist + edge_dist
                    new_predecesor = new_node
                else:
                    new_dist = old_dist
                    if(new_node == source):
                        new_predecesor = source
                    else:
                        new_predecesor = predecesores.pop(vecino, None)
                
                predecesores[vecino] = new_predecesor
                pq.insert(vecino, new_dist)
                
        logger_cagada.debug("asta aora shotest %s y predecesores %s " % (shortest_path, predecesores))
        
    ultimo_puto = destino
    while(ultimo_puto):
#        logger_cagada.debug("insertando a la ruta %s aora es %s" % (ultimo_puto, ruta_salida))
        ruta_salida.insert(0, ultimo_puto)
        ultimo_puto = predecesores.pop(ultimo_puto, None)
    
    logger_cagada.debug("la puta ruta %s" % ruta_salida)
    
    if(len(ruta_salida) == 1):
        ruta_salida = []
    
    return shortest_path, ruta_salida

def huaronmierda_core(matrix, caca, salida, vacio, costo_trampa):
    chosto_total = 0
    matrixes_chostos = []
    if(caca == salida):
        return 0
    if(matrix[caca[0]][caca[1]] != "1"):
        return sys.maxsize
    if(matrix[salida[0]][salida[1]] != "1"):
        return sys.maxsize
    if(matrix[vacio[0]][vacio[1]] != '1'):
        return sys.maxsize
                
    
    matrix_rodeada = huaronmierda_crea_matrix_rodeada(matrix)
    logger_cagada.debug("la matrix rodead\n%s" % caca_comun_matrix_a_cadena(matrix_rodeada))
    
    for direccion in range(HUARONMIERDA_DIRECCION_FINAL + 1):
        nueva_mierda = []
        for _ in range(len(matrix)):
            nueva_mierda.append([0] * len(matrix[0]))
        matrixes_chostos.append(nueva_mierda)
        logger_cagada.debug("anadida matriz chostos mide %u x %u" % (len(nueva_mierda), len(nueva_mierda[0])))
        for idx_fila, fila in enumerate(matrix, 2):
            for idx_col, _ in enumerate(fila, 2):
                huaronmierda_precaca((idx_fila , idx_col), matrix_rodeada, costo_trampa, direccion , matrixes_chostos)
    
        logger_cagada.debug("la matrix de direccion %s kedo:\n%s" % (direccion, caca_comun_matrix_a_cadena(nueva_mierda)))

    
    logger_cagada.debug("el bloke caca-o %s" % (type(caca)))
    distancias_cortas, ruta_caca = huaronmierda_dijkstra(caca, salida, matrix, matrixes_chostos, vacio, costo_trampa)
    
    logger_cagada.debug("las distancias + cortas %s" % distancias_cortas)
    for mierda in ruta_caca:
        chosto_nodo = distancias_cortas[mierda]
        logger_cagada.debug("la distancia de %s a %s es %s" % (caca, mierda, chosto_nodo))
    
    if(ruta_caca):
        chosto_cacao = distancias_cortas[salida]
        chosto_total = chosto_cacao 
    else:
        chosto_total = sys.maxsize
    
        
    return chosto_total
        
def huaronmierda_main():
    lineas_sin_dep = list(sys.stdin)
    lineas = list(filter(lambda linea:linea[0] != "#", lineas_sin_dep))
    filas, columnas, costo_trampa, num_cacasos = [int(x) for x in lineas[0].strip().split(" ")]
    matrix = [["" for _ in range(columnas)] for _ in range(filas)]
    logger_cagada.debug("mat vacia %s" % matrix)
    for idx_fila, linea_matrix in enumerate(lineas[1:filas + 1]):
        caracs = linea_matrix.strip().split()
        for idx_charac, charac in enumerate(caracs):
            logger_cagada.debug("poniendo caracter %s en %u,%u" % (charac, idx_fila, idx_charac))
            matrix[idx_fila][idx_charac] = charac
    
    logger_cagada.debug("la matrix es\n%s" % caca_comun_matrix_a_cadena(matrix))
    assert num_cacasos == len(lineas[filas + 1:])
    for cacaso in lineas[filas + 1:]:
        vacio_x, vacio_y, caca_x, caca_y, sal_x, sal_y = [int(x) for x in cacaso.strip().split(" ")]
        vacio = (vacio_x - 1, vacio_y - 1)
        caca = (caca_x - 1, caca_y - 1)
        sal = (sal_x - 1, sal_y - 1)
        
        logger_cagada.debug("con el caso vacio %s,caca %s y salida %s" % (vacio, caca, sal))
        
        chosto = huaronmierda_core(matrix, caca, sal, vacio, costo_trampa)
        if(chosto == sys.maxsize):
            chosto = -1
        
        logger_cagada.debug("el agosto al chosto %u" % (chosto))
        print(chosto)
        
if __name__ == '__main__':
        FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
        logging.basicConfig(level=nivel_log, format=FORMAT)
        logger_cagada = logging.getLogger("asa")
        logger_cagada.setLevel(nivel_log)
        huaronmierda_main()
