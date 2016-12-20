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

def caca_comun_matrix_a_cadena(matrix):
    res = ""
    res = ('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
      for row in matrix]))
    return res

def huaronmierda_elimina_cuellos_botella(matrix, eliminados, ignorados):
    num_filas = len(matrix)
    num_cols = len(matrix[0])
#    movimientos_alrededor = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]
    arriba = [(-1, 0), (-1, 1), (-1, -1)]
    abajo = [(1, 1), (1, 0), (1, -1)]
    izq = [ (1, -1), (0, -1), (-1, -1)]
    der = [(-1, 1), (0, 1), (1, 1)]
    for idx_y, fila in enumerate(matrix):
        for idx_x, columna in enumerate(fila):
            eliminar_cuadrito = False
            continuar_validacion = True
            idx_arriba = 0
            idx_abajo = 1
            idx_izq = 2
            idx_der = 3
            blokeado_lado = [False, False, False, False]
            nombres_lados = ["arriba", "abajo", "izq", "der"]
            
            if((idx_y, idx_x) in ignorados):
                logger_cagada.debug("no se puede eliminar %u,%u x q esta en lista de ignorado" % (idx_y, idx_x))
                continue
            
            for idx_lado, lado in enumerate([arriba, abajo, izq, der]):
                for pos_lado in lado:
                    pos_y = idx_y + pos_lado[0]
                    pos_x = idx_x + pos_lado[1]
                    if(pos_y < 0 or pos_y >= num_filas):
                        logger_cagada.debug("en lado %s esta blokeado por estar en los limites y" % nombres_lados[idx_lado])
#                        blokeado_lado[idx_lado] = True
                        break
                    if(pos_x < 0 or pos_x >= num_cols):
                        logger_cagada.debug("en lado %s esta blokeado por estar en los limites x" % nombres_lados[idx_lado])
#                        blokeado_lado[idx_lado] = True
                        break
                    if(matrix[pos_y][pos_x] != "1"):
                        logger_cagada.debug("en %u,%u  de valor %s el lado %s esta blokeado por ser fijo el cuadro %u,%u de valor %s" % (idx_y, idx_x, matrix[idx_y][idx_x], nombres_lados[idx_lado], pos_y, pos_x, matrix[pos_y][pos_x]))
                        blokeado_lado[idx_lado] = True
                        break
                    
                logger_cagada.debug("en %u,%u el lado %s esta blokeado %s" % (idx_y, idx_x, nombres_lados[idx_lado], blokeado_lado[idx_lado]))
            
            if(num_cols > 1 and num_filas > 1):
                if(idx_x == idx_y and not idx_x):
                    if(matrix[idx_y + 1][idx_x + 1] != "1"):
                        logger_cagada.debug("esquina izq arriba tiene blokeo abajo der")
                        eliminar_cuadrito = True
                    continuar_validacion = False
                if(idx_x == num_cols - 1 and idx_y == num_filas - 1):
                    if(matrix[idx_y - 1][idx_x - 1] != "1"):
                        logger_cagada.debug("esquina der abajo tiene blokeo arriba izq")
                        eliminar_cuadrito = True
                    continuar_validacion = False
                if(idx_x == num_cols - 1 and not idx_y): 
                    if(matrix[idx_y + 1][idx_x - 1] != "1"):
                        logger_cagada.debug("esq der arriba tiene blokeo abajo izq")
                        eliminar_cuadrito = True
                    continuar_validacion = False
                if(idx_y == num_filas - 1 and not idx_x):
                    if(matrix[idx_y - 1][idx_x + 1] != "1"):
                        logger_cagada.debug("esq izq abajo tiene blokeo arriba der")
                        eliminar_cuadrito = True
                    continuar_validacion = False
                
            if(continuar_validacion):
                if(not idx_y):
                    if(num_filas > 1 and matrix[1][idx_x] != "1"):
                        eliminar_cuadrito = True
                    continuar_validacion = False
                    
                if(not idx_x):
                    if(num_cols > 1 and matrix[idx_y][1] != "1"):
                        eliminar_cuadrito = True
                    continuar_validacion = False
                
                if(idx_y == num_filas - 1):
                    if(num_filas > 1 and matrix[idx_y - 1][idx_x] != "1"):
                        eliminar_cuadrito = True
                    continuar_validacion = False
                    
                if(idx_x == num_cols - 1):
                    if(num_cols > 1 and matrix[idx_y][idx_x - 1] != "1"):
                        eliminar_cuadrito = True
                    continuar_validacion = False
            
            if(continuar_validacion):
                if(blokeado_lado[idx_arriba] and blokeado_lado[idx_abajo]):
                    logger_cagada.debug("cuadro %u,%u sera eliminado, arriba i abajo frankeado" % (idx_y, idx_x))
                    eliminar_cuadrito = True
                    
                if(blokeado_lado[idx_izq] and blokeado_lado[idx_der]):
                    logger_cagada.debug("cuadro %u,%u sera eliminado, izq y der frankeado" % (idx_y, idx_x))
                    eliminar_cuadrito = True
            
            if(eliminar_cuadrito):
                eliminados.append((idx_y, idx_x))
                
    for pos in eliminados:
        matrix[pos[0]][pos[1]] = "X"
                
def huaronmierda_restaura_cuellos_botella(matrix, eliminados):
    for pos in eliminados:
        matrix[pos[0]][pos[1]] = "1"
        logger_cagada.debug("restaurada pos %u,%u" % (pos[0], pos[1]))
            
            
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
    
    matrix[nodo_caca_y][nodo_caca_x] = "0"
    
    res = huaronmierda_bfs(matrix, origen, destino)
    matrix[nodo_caca_y][nodo_caca_x] = "1"
    
    return res

def huaronmierda_core(matrix, caca, salida, vacio, costo_trampa):
    costo_vacio_caca = 0
    chosto_brincos = 0
    chosto_total = 0
    chosto_caca_salida = 0
    if(matrix[caca[0]][caca[1]] != "1"):
        return -1
    if(matrix[salida[0]][salida[1]] != "1"):
        return -1
                
    if(caca == salida):
        return 0
    
    eliminados = []
    ignorados = [caca, salida, vacio]
    huaronmierda_elimina_cuellos_botella(matrix, eliminados, ignorados)
    logger_cagada.debug("despues de eliminar cuellos de botella\n%s", caca_comun_matrix_a_cadena(matrix))
    logger_cagada.debug("calculando ruta de caca a salida")
    ruta_caca_salida = huaronmierda_bfs(matrix, caca, salida)
    tam_ruta_caca_salida = len(ruta_caca_salida)
    logger_cagada.debug("la ruta de caca a sal %s" % ruta_caca_salida)
    
    if(not tam_ruta_caca_salida):
        return -1
    
    if(ruta_caca_salida[1] != vacio):
        ruta_vacio_caca = huaronmierda_bfs_sin_nodo(matrix, vacio, ruta_caca_salida[1], caca)
        tam_ruta_vacio_caca = len(ruta_vacio_caca)
        logger_cagada.debug("la ruta del vacio a la caca %s" % ruta_vacio_caca)
        if(not tam_ruta_vacio_caca):
            costo_vacio_caca = costo_trampa
        else:
            costo_vacio_caca = tam_ruta_vacio_caca - 1
            
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
            chosto_brincos += costo_trampa
        else:
            assert len(ruta_brinca) > 1
            chosto_brincos += len(ruta_brinca) - 1
    
    logger_cagada.debug("l costo brincos rosa %u" % chosto_brincos)

    chosto_caca_salida = tam_ruta_caca_salida - 1 + chosto_brincos
    logger_cagada.debug("de la caca a sal %u" % chosto_caca_salida)
    chosto_total = chosto_caca_salida + costo_vacio_caca 
        
    return chosto_total
        
def huaronmierda_main():
    lineas = list(sys.stdin)
    filas, columnas, costo_trampa, num_cacasos = [int(x) for x in lineas[0].strip().split(" ")]
    matrix = [["" for _ in range(columnas)] for _ in range(filas)]
    logger_cagada.debug("mat vacia %s" % matrix)
    for idx_fila, linea_matrix in enumerate(lineas[1:filas + 1]):
        caracs = linea_matrix.strip().split(" ")
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
        
        logger_cagada.debug("el agosto al chosto %u" % (chosto))
        print(chosto)
        
if __name__ == '__main__':
        FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
        logging.basicConfig(level=nivel_log, format=FORMAT)
        logger_cagada = logging.getLogger("asa")
        logger_cagada.setLevel(nivel_log)
        huaronmierda_main()