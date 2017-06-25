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

def huaronmierda_par_a_coordenada(matrix, nodo):
    tam_caca = len(matrix[0]) - 4
    return (nodo[0] - 2) * tam_caca + (nodo[1] - 2)

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
        logger_cagada.debug("el destino %s,%s es una mierda, no c puede proceder" % (destino))
        return
    
    ruta_mierda = huaronmierda_bfs_sin_nodo(matrix, origen, destino, mastorbo)
    
    logger_cagada.debug("la rutal resultante %s" % ruta_mierda)
#    logger_cagada.debug("la matrix chostos\n%s" % (caca_comun_matrix_a_cadena(matrix_chostos)))
    
    if(ruta_mierda):
        tam_ruta = len(ruta_mierda)
        logger_cagada.debug("chosto de %s(%s) a %s(%s) calculado " % (huaronmierda_par_a_coordenada(matrix, origen), origen, huaronmierda_par_a_coordenada(matrix, destino), destino))
        matrix_chostos[huaronmierda_par_a_coordenada(matrix, origen)][huaronmierda_par_a_coordenada(matrix, destino)] = tam_ruta - 1
        matrix_chostos[huaronmierda_par_a_coordenada(matrix, destino)][huaronmierda_par_a_coordenada(matrix, origen)] = tam_ruta - 1
    else:
        matrix_chostos[huaronmierda_par_a_coordenada(matrix, origen)][huaronmierda_par_a_coordenada(matrix, destino)] = chosto_brinco
        matrix_chostos[huaronmierda_par_a_coordenada(matrix, destino)][huaronmierda_par_a_coordenada(matrix, origen)] = chosto_brinco
        
        
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

def huaronmierda_core(matrix, caca, salida, vacio, costo_trampa):
    chosto_total = 0
    chosto_cuellos = sys.maxsize
    chosto_sin_cuellos = sys.maxsize
    eliminados = []
    ignorados = []
    matrixes_chostos = []
    if(matrix[caca[0]][caca[1]] != "1"):
        return sys.maxsize
    if(matrix[salida[0]][salida[1]] != "1"):
        return sys.maxsize
                
    if(caca == salida):
        return 0
    
    ignorados = [caca, salida, vacio]
    matrix_rodeada = huaronmierda_crea_matrix_rodeada(matrix)
    logger_cagada.debug("la matrix rodead\n%s" % caca_comun_matrix_a_cadena(matrix_rodeada))
    
    for direccion in range(HUARONMIERDA_DIRECCION_FINAL + 1):
        nueva_mierda = []
        for _ in range(len(matrix) * len(matrix[0])):
            nueva_mierda.append([0] * len(matrix[0]) * len(matrix))
        matrixes_chostos.append(nueva_mierda)
        for idx_fila, fila in enumerate(matrix, 2):
            for idx_col, _ in enumerate(fila, 2):
                huaronmierda_precaca((idx_fila , idx_col), matrix_rodeada, costo_trampa, direccion , matrixes_chostos)
    
        logger_cagada.debug("la matrix de direccion %s kedo:\n%s" % (direccion, caca_comun_matrix_a_cadena(nueva_mierda)))
        
        
    return chosto_total
        
def huaronmierda_main():
    lineas = list(sys.stdin)
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
