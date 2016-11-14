'''
Created on 13/11/2016

@author: ernesto
'''
import queue
import logging
import sys

nivel_log = logging.ERROR
nivel_log = logging.DEBUG
logger_cagada = None


def huaronmierda_genera_vecinos(matrix, nodo):
    POSIBLES_MOVIMIENTOS = [(0, -1), (0, 1), (-1, 0), (1, 0)]
    tam_x = len(matrix[0])
    tam_y = len(matrix)
    pos_x = nodo[1]
    pos_y = nodo[0]
    vecinos = []
    for mov_y, mov_x in POSIBLES_MOVIMIENTOS:
        candidote_y, candidote_x = (pos_y + mov_y, pos_x + mov_x)
        if(candidote_y < tam_y and candidote_x < tam_x and matrix[candidote_y][candidote_x] == "1"):
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
        logger_cagada.debug("para el nodo %s los vecinos %s" % (nodo_act, vecinos))
        for vecino in vecinos:
            if(vecino not in ya_visitados):
                antecesor[vecino] = nodo_act
                q.put(vecino)
    
    if(destino in antecesor):
        nodo_act = antecesor[destino]
        while(nodo_act):
            ruta.insert(0, nodo_act)
            if(nodo_act in antecesor):
                nodo_act = antecesor[nodo_act]
            else:
                nodo_act = None

        ruta.insert(0, origen)
    
    logger_cagada.debug("la ruta es %s" % ruta)
    
    return ruta

def huaronmierda_bfs_sin_nodo(matrix, origen, destino, nodo_caca):
    nodo_caca_x = nodo_caca[1]
    nodo_caca_y = nodo_caca[0]
    
    matrix[nodo_caca_y][nodo_caca_x] = "0"
    
    res = huaronmierda_bfs(matrix, origen, destino)
    matrix[nodo_caca_y][nodo_caca_x] = "1"
    
    return res

def huaronmierda_core(matrix, caca, salida, vacio, costo_trampa):
    costo_vacio_caca = 0
    chosto_brincos = 0
    chosto_total = 0
    if(caca == salida):
        return 0
    ruta_caca_salida = huaronmierda_bfs(matrix, caca, salida)
    tam_ruta_caca_salida = len(ruta_caca_salida)
    logger_cagada.debug("la ruta de caca a sal %s" % ruta_caca_salida)
    if(not tam_ruta_caca_salida):
        return costo_trampa * 2
    if(tam_ruta_caca_salida - 1 > costo_trampa * 2):
        return costo_trampa * 2
    if(tam_ruta_caca_salida - 1 > costo_trampa):
        return costo_trampa * 2
    
    if(ruta_caca_salida[1] != vacio):
        ruta_vacio_caca = huaronmierda_bfs(matrix, vacio, ruta_caca_salida[1])
        tam_ruta_vacio_caca = len(ruta_vacio_caca)
        logger_cagada.debug("la ruta del vacio a la caca %s" % ruta_vacio_caca)
        if(not tam_ruta_vacio_caca):
            costo_vacio_caca = costo_trampa
        else:
            costo_vacio_caca = tam_ruta_vacio_caca - 1
            
        if(costo_vacio_caca > costo_trampa * 2):
            return costo_trampa * 2
        if(costo_vacio_caca > costo_trampa):
            costo_vacio_caca = costo_trampa
    
    if(ruta_caca_salida[1] == salida):
        assert tam_ruta_caca_salida == 2
        if(salida == vacio):
            return 1
        else:
            return costo_vacio_caca + 1
        
    
    for idx_nodo_act, nodo_act in enumerate(ruta_caca_salida[:-2]):
        nodo_dest = ruta_caca_salida[idx_nodo_act + 2]
        nodo_brincado = ruta_caca_salida[idx_nodo_act + 1]
        ruta_brinca = huaronmierda_bfs_sin_nodo(matrix, nodo_act, nodo_dest, nodo_brincado)
        logger_cagada.debug("la ruta de %s a %s brincando %s es %s" % (nodo_act, nodo_dest, nodo_brincado, ruta_brinca))
        if(not ruta_brinca):
            return costo_trampa * 2
        assert len(ruta_brinca) > 1
        chosto_brincos += len(ruta_brinca) - 1
    
    chosto_total = costo_vacio_caca + tam_ruta_caca_salida - 1 + chosto_brincos
    
    return chosto_total


        
def huaronmierda_main():
    lineas = list(sys.stdin)
    filas, columnas, costo_trampa, num_cacasos = [int(x) for x in lineas[0].strip().split(" ")]
    matrix = [["" for _ in range(columnas)] for _ in range(filas)]
    for idx_fila, linea_matrix in enumerate(lineas[1:filas + 1]):
        for idx_charac, charac in enumerate(linea_matrix):
            matrix[idx_fila][idx_charac] = charac
    
    logger_cagada.debug("la matrix es %s" % matrix)
    for cacaso in lineas[filas + 1:]:
        vacio_x, vacio_y, caca_x, caca_y, sal_x, sal_y = [int(x) for x in cacaso.strip().split(" ")]
        vacio = (vacio_y, vacio_x)
        caca = (caca_y, caca_x)
        sal = (sal_y, sal_x)
        
        logger_cagada.debug("con el caso vacio %s,caca %s y salida %s" % (vacio, caca, sal))
        
        chosto = huaronmierda_core(matrix, caca, sal, vacio, costo_trampa)
        
        logger_cagada.debug("el agosto al chosto %u" % (chosto))
        
    
    

if __name__ == '__main__':
        FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
        logging.basicConfig(level=nivel_log, format=FORMAT)
        logger_cagada = logging.getLogger("asa")
        logger_cagada.setLevel(nivel_log)
