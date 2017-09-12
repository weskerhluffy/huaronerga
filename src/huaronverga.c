#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO (tipo_dato)INT_MAX
#define CACA_COMUN_IDX_INVALIDO ((natural)UNIVERGA_VALOR_INVALIDO)

typedef unsigned int natural;
typedef natural tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){sleep(10);abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%12d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

#endif

#if 1

#define QUEUE_VALOR_INVALIDO LONG_LONG_MAX

/* a link in the queue, holds the info and point to the next Node*/
typedef struct Node_t {
	void *datos;
	struct Node_t *prev;
} queue_nodo;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct queue_t {
	queue_nodo *head;
	queue_nodo *tail;
	int size;
	int limit;
} queue_t;

queue_t *queue_construye(int limit) {
	queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
	if (queue == NULL) {
		return NULL;
	}
	if (limit <= 0) {
		limit = 65535;
	}
	queue->limit = limit;
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

bool queue_vacia(queue_t* pQueue) {
	if (pQueue == NULL) {
		return verdadero;
	}
	if (pQueue->size == 0) {
		return verdadero;
	} else {
		return falso;
	}
}

void queue_destruye(queue_t *queue) {
	while (!queue_vacia(queue)) {
		queue_decula(queue);
	}
	free(queue);
}

bool queue_encula(queue_t *pQueue, void *mierda) {
	/* Bad parameter */
	queue_nodo *item = (queue_nodo *) calloc(1, sizeof(queue_nodo));
	if ((pQueue == NULL) || (item == NULL)) {
		return falso;
	}
	// if(pQueue->limit != 0)
	if (pQueue->size >= pQueue->limit) {
		free(item);
		return falso;
	}
	item->datos = mierda;
	/*the queue is empty*/
	item->prev = NULL;
	if (pQueue->size == 0) {
		pQueue->head = item;
		pQueue->tail = item;

	} else {
		/*adding item to the end of the queue*/
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	pQueue->size++;
	return verdadero;
}

void *queue_decula(queue_t *pQueue) {
	/*the queue is empty or bad param*/
	queue_nodo *item;
	void *mierda;
	if (queue_vacia(pQueue))
		return QUEUE_VALOR_INVALIDO;
	item = pQueue->head;
	mierda = item->datos;
	pQueue->head = (pQueue->head)->prev;
	pQueue->size--;
	free(item);
	return mierda;
}

/*
 int main() {
 int i;
 Queue *pQ = ConstructQueue(7);
 NODE *pN;

 for (i = 0; i < 9; i++) {
 pN = (NODE*) malloc(sizeof(NODE));
 pN->data.info = 100 + i;
 Enqueue(pQ, pN);
 }

 while (!isEmpty(pQ)) {
 pN = Dequeue(pQ);
 printf("\nDequeued: %d", pN->data);
 free(pN);
 }
 DestructQueue(pQ);
 return (EXIT_SUCCESS);
 }
 */

#endif

typedef struct puto_cardinal {
	union {
		struct {
			int coordenada_y_puto_cardinal;
			int coordenada_x_puto_cardinal;
		} separados_puto_cardinal;
		entero_largo coordenadas_juntas_puto_cardinal;
	} datos_puto_cardinal;
	void *extra;
} puto_cardinal;

#define coord_x datos_puto_cardinal.separados_puto_cardinal.coordenada_x_puto_cardinal
#define coord_y datos_puto_cardinal.separados_puto_cardinal.coordenada_y_puto_cardinal
#define coord_xy datos_puto_cardinal.coordenadas_juntas_puto_cardinal

#define HUARONVERGA_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO

typedef enum huaronverga_caso_familiar {
	vertical_huaronverga_caso_familiar = 0,
	horizontal_huaronverga_caso_familiar,
	esq_1_huaronverga_caso_familiar,
	esq_2_huaronverga_caso_familiar,
	esq_3_huaronverga_caso_familiar,
	esq_4_huaronverga_caso_familiar,
	final_huaronverga_caso_familiar,
	invalido_huaronverga_caso_familiar = HUARONVERGA_VALOR_INVALIDO
} huaronverga_caso_familiar;

typedef enum huaronverga_movimientos_decendiente_directo_idx {
	arriba_huaronverga_movimientos_decendiente_directo_idx = 0,
	derecha_huaronverga_movimientos_decendiente_directo_idx,
	abajo_huaronverga_movimientos_decendiente_directo_idx,
	izquierda_huaronverga_movimientos_decendiente_directo_idx,
	ultimo_huaronverga_movimientos_decendiente_directo_idx,
	invalido_huaronverga_movimientos_decendiente_directo_idx = HUARONVERGA_VALOR_INVALIDO
} huaronverga_movimientos_decendiente_directo_idx;
#define primero_huaronverga_movimientos_decendiente_directo_idx arriba_huaronverga_movimientos_decendiente_directo_idx

#define HUARONVERGA_MAX_FILAS 200
#define HUARONVERGA_MAX_COLUMNAS 200
#define HUARONVERGA_MAX_ELEMENTOS (HUARONVERGA_MAX_FILAS*HUARONVERGA_MAX_COLUMNAS)
#define HUARONVERGA_CARACTER_BLOQUE_LIBRE '1'

typedef struct huaronverga_ctx {
	byteme matrix[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS];
	byteme matrix_rodeada[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS
			+ 4];
	tipo_dato matrix_chosto_brinco[final_huaronverga_caso_familiar][HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS];
	tipo_dato matrix_chosto_minimo[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS];
	natural matrix_predecesores[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS];
	tipo_dato chosto_brinco;
} huaronverga_ctx;

typedef struct huaronverga_args {
	puto_cardinal *abuelo;
	puto_cardinal *padre;
	puto_cardinal *hijo;
	puto_cardinal *mas_arriba;
	puto_cardinal *mas_abajo;
	huaronverga_caso_familiar cacaso;
	tipo_dato chosto_act_hijo;
} huaronverga_args;

puto_cardinal movimientos_decendiente_directo[] = { { 1, 0 }, { 0, 1 },
		{ -1, 0 }, { 0, -1 } };

puto_cardinal putos_tmp[HUARONVERGA_MAX_ELEMENTOS] = {
		[0 ... HUARONVERGA_MAX_ELEMENTOS]=HUARONVERGA_VALOR_INVALIDO };

#if 0
natural cacasos_decendiente[]= {
	// hijo:	ARRIBA								DERECHA								ABAJO								IZQUIERDA
	{	HUARONVERGA_VALOR_INVALIDO, HUARONMIERDA_DIRECCION_ESQ_4, HUARONMIERDA_DIRECCION_VERTICAL, HUARONMIERDA_DIRECCION_ESQ_3}, // abuelo: ARRIBA
	{	HUARONMIERDA_DIRECCION_ESQ_4, HUARONVERGA_VALOR_INVALIDO, HUARONMIERDA_DIRECCION_ESQ_1, HUARONMIERDA_DIRECCION_HORIZONTAL}, // abuelo: DERECHA
	{	HUARONMIERDA_DIRECCION_VERTICAL, HUARONMIERDA_DIRECCION_ESQ_1, HUARONVERGA_VALOR_INVALIDO, HUARONMIERDA_DIRECCION_ESQ_2}, // abuelo: ABAJO
	{	HUARONMIERDA_DIRECCION_ESQ_3, HUARONMIERDA_DIRECCION_HORIZONTAL, HUARONMIERDA_DIRECCION_ESQ_2, HUARONVERGA_VALOR_INVALIDO} // abuelo: IZQUIERDA

};
#endif

huaronverga_caso_familiar cacasos_abuelo_hijo[][ultimo_huaronverga_movimientos_decendiente_directo_idx] =
		{
// hijo:	ARRIBA								DERECHA								ABAJO								IZQUIERDA
				{ invalido_huaronverga_caso_familiar,
						esq_4_huaronverga_caso_familiar,
						vertical_huaronverga_caso_familiar,
						esq_3_huaronverga_caso_familiar }, // abuelo: ARRIBA
				{ esq_4_huaronverga_caso_familiar,
						invalido_huaronverga_caso_familiar,
						esq_1_huaronverga_caso_familiar,
						horizontal_huaronverga_caso_familiar }, // abuelo: DERECHA
				{ vertical_huaronverga_caso_familiar,
						esq_1_huaronverga_caso_familiar,
						invalido_huaronverga_caso_familiar,
						esq_2_huaronverga_caso_familiar }, // abuelo: ABAJO
				{ esq_3_huaronverga_caso_familiar,
						horizontal_huaronverga_caso_familiar,
						esq_2_huaronverga_caso_familiar,
						invalido_huaronverga_caso_familiar } // abuelo: IZQUIERDA
		};

static inline huaronverga_caso_familiar huaronverga_obten_caso_familiar(
		huaronverga_args *hvargs) {
	puto_cardinal *abuelo = hvargs->abuelo;
	puto_cardinal *padre = hvargs->padre;
	puto_cardinal *hijo = hvargs->hijo;
	huaronverga_caso_familiar cacaso = invalido_huaronverga_caso_familiar;
	huaronverga_movimientos_decendiente_directo_idx caso_abuelo_idx =
			invalido_huaronverga_movimientos_decendiente_directo_idx;
	huaronverga_movimientos_decendiente_directo_idx caso_hijo_idx =
			invalido_huaronverga_movimientos_decendiente_directo_idx;

	for (caso_abuelo_idx =
	primero_huaronverga_movimientos_decendiente_directo_idx;
			caso_abuelo_idx
					< ultimo_huaronverga_movimientos_decendiente_directo_idx;
			caso_abuelo_idx++) {

		puto_cardinal *movimiento_act = movimientos_decendiente_directo
				+ caso_abuelo_idx;

		puto_cardinal *potencial_abuelo = &(puto_cardinal )
				{
				.coord_x = padre->coord_x + movimiento_act->coord_x,
						.coord_y = padre->coord_y + movimiento_act->coord_y };
		if (potencial_abuelo->coord_xy == abuelo->coord_xy) {
			break;
		}

	}
	assert_timeout(
			caso_abuelo_idx
					!= invalido_huaronverga_movimientos_decendiente_directo_idx);

	for (caso_hijo_idx =
	primero_huaronverga_movimientos_decendiente_directo_idx;
			caso_hijo_idx
					< ultimo_huaronverga_movimientos_decendiente_directo_idx;
			caso_hijo_idx++) {

		puto_cardinal *movimiento_act = movimientos_decendiente_directo
				+ caso_hijo_idx;

		puto_cardinal *potencial_hijo = &(puto_cardinal )
				{
				.coord_x = padre->coord_x + movimiento_act->coord_x,
						.coord_y = padre->coord_y + movimiento_act->coord_y };
		if (potencial_hijo->coord_xy == hijo->coord_xy) {
			break;
		}

	}

	assert_timeout(
			caso_hijo_idx
					!= invalido_huaronverga_movimientos_decendiente_directo_idx);

	cacaso = cacasos_abuelo_hijo[caso_abuelo_idx][caso_hijo_idx];

	assert_timeout(caso_hijo_idx != invalido_huaronverga_caso_familiar);

	hvargs->cacaso = cacaso;
	return cacaso;
}

static inline void huaronverga_genera_vecinos(puto_cardinal *caca,
		puto_cardinal *vecinos) {

	for (huaronverga_movimientos_decendiente_directo_idx i =
	primero_huaronverga_movimientos_decendiente_directo_idx;
			i < invalido_huaronverga_movimientos_decendiente_directo_idx; i++) {
		puto_cardinal *mov_act = movimientos_decendiente_directo + i;
		puto_cardinal *vecino_act = vecinos + i;

		vecino_act->coord_x = caca->coord_x + mov_act->coord_x;
		vecino_act->coord_y = caca->coord_y + mov_act->coord_y;
	}
}

static inline byteme huaronverga_obten_valor_en_coord_stack(
		byteme matrix[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS + 4],
		puto_cardinal *puto) {
	byteme valor = 0;

	valor = matrix[puto->coord_x][puto->coord_y];
	return valor;
}

static inline void huaronverga_pon_valor_en_coord_stack(
		puto_cardinal matrix[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS
				+ 4], puto_cardinal *puto, byteme valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline void huaronverga_pon_valor_en_coord_heap(puto_cardinal ***matrix,
		puto_cardinal *puto, puto_cardinal *valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline puto_cardinal *huaronverga_obten_valor_en_coord_heap(
		puto_cardinal ***matrix, puto_cardinal *puto) {
	puto_cardinal *valor = NULL;

	valor = matrix[puto->coord_x][puto->coord_y];
	return valor;
}

static inline void huaronverga_genera_vecino_en_idx(puto_cardinal *origen,
		puto_cardinal *vecino,
		huaronverga_movimientos_decendiente_directo_idx idx) {
	puto_cardinal *mov_act = movimientos_decendiente_directo + idx;
	vecino->coord_x = origen->coord_x + mov_act->coord_x;
	vecino->coord_y = origen->coord_y + mov_act->coord_y;
}

static inline tipo_dato huaronverga_chosto_por_busqueda_en_amplitud(
		byteme matrix[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS + 4],
		puto_cardinal *inicio, puto_cardinal *fin) {
	tipo_dato chosto = 0;
	queue_t *kha = NULL;
	puto_cardinal ***predecesores = NULL;
	puto_cardinal *predecesor_actual = NULL;
	bool se_encontro_fin = falso;
	natural putos_tmp_cnt = 0;

	memset(putos_tmp, HUARONVERGA_VALOR_INVALIDO, sizeof(putos_tmp));

	predecesores = calloc(HUARONVERGA_MAX_FILAS, sizeof(puto_cardinal **));
	assert_timeout(predecesores);
	for (int i = 0; i < HUARONVERGA_MAX_FILAS; i++) {
		predecesores[i] = calloc(HUARONVERGA_MAX_COLUMNAS,
				sizeof(puto_cardinal *));
		assert_timeout(predecesores[i]);
	}

	if (huaronverga_obten_valor_en_coord_stack(matrix,
			inicio)!=HUARONVERGA_CARACTER_BLOQUE_LIBRE || huaronverga_obten_valor_en_coord(matrix, fin)!=HUARONVERGA_CARACTER_BLOQUE_LIBRE) {
		return HUARONVERGA_VALOR_INVALIDO ;
	}

	kha = queue_construye(HUARONVERGA_MAX_FILAS * HUARONVERGA_MAX_COLUMNAS);

	queue_encula(kha, inicio);
	while (!queue_vacia(kha)) {
		puto_cardinal *act = queue_decula(kha);

		for (huaronverga_movimientos_decendiente_directo_idx vecino_idx = 0;
				vecino_idx
						< ultimo_huaronverga_movimientos_decendiente_directo_idx;
				vecino_idx++) {
			puto_cardinal *vecino = putos_tmp + putos_tmp_cnt++;
			huaronverga_genera_vecino_en_idx(act, vecino, vecino_idx);
			if (huaronverga_obten_valor_en_coord(matrix,
					vecino)==HUARONVERGA_CARACTER_BLOQUE_LIBRE) {
				queue_encula(kha, vecino);
				huaronverga_pon_valor_en_coord_heap(predecesores, vecino, act);
				if (vecino->coord_xy == fin->coord_xy) {
					break;
					se_encontro_fin = verdadero;
				}
			}
		}
	}

	if (se_encontro_fin) {
		predecesor_actual = fin;
		while (predecesor_actual->coord_xy != inicio->coord_xy) {
			predecesor_actual = huaronverga_obten_valor_en_coord_heap(
					predecesores, predecesor_actual);
			chosto++;
		}
		assert_timeout(predecesor_actual->coord_xy==inicio->coord_xy);
	}

	queue_destruye(kha);

	for (int i = 0; i < HUARONVERGA_MAX_FILAS; i++) {
		free(predecesores[i]);
	}

	free(predecesores);

	return chosto;
}

static inline tipo_dato huaronverga_chosto_por_busqueda_en_amplitud_brincando(
		byteme matrix[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS + 4],
		puto_cardinal *inicio, puto_cardinal *invalido, puto_cardinal *fin) {
	tipo_dato chosto = 0;
	byteme valor_ant = 0;

	valor_ant = huaronverga_obten_valor_en_coord_stack(matrix, invalido);

	chosto = huaronverga_chosto_por_busqueda_en_amplitud(matrix, inicio, fin);

	huaronverga_pon_valor_en_coord_stack(matrix, invalido, valor_ant);

	return chosto;
}

static inline tipo_dato huaronverga_calcula_chosto_brinco(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	tipo_dato res = 0;
	puto_cardinal *abuelo = hvargs->abuelo;
	puto_cardinal *padre = hvargs->padre;
	puto_cardinal *hijo = hvargs->hijo;
	puto_cardinal *mas_abajo = &(puto_cardinal ) { 0 };
	puto_cardinal *mas_arriba = &(puto_cardinal ) { 0 };

	assert_timeout(abuelo->coord_xy!=padre->coord_xy);
	assert_timeout(padre->coord_xy!=hijo->coord_xy);
	assert_timeout(hijo->coord_xy!=abuelo->coord_xy);

	mas_abajo = *hvargs->mas_abajo;
	mas_arriba = *hvargs->mas_arriba;
	mas_abajo->coord_x += 2;
	mas_abajo->coord_y += 2;
	mas_arriba->coord_x += 2;
	mas_arriba->coord_y += 2;

	res = huaronverga_chosto_por_busqueda_en_amplitud_brincando(
			ctx->matrix_rodeada, mas_abajo, padre, mas_arriba);

	return res;
}

static inline tipo_dato huaronverga_obten_chosto_brinco(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	huaronverga_caso_familiar cacaso = hvargs->cacaso;
	puto_cardinal *abuelo = hvargs->abuelo;
	puto_cardinal *padre = hvargs->padre;
	puto_cardinal *hijo = hvargs->hijo;
	puto_cardinal *mas_abajo = NULL;
	puto_cardinal *mas_arriba = NULL;
	tipo_dato res = 0;

	assert_timeout(abuelo->coord_xy!=padre->coord_xy);
	assert_timeout(padre->coord_xy!=hijo->coord_xy);
	assert_timeout(hijo->coord_xy!=abuelo->coord_xy);

	mas_abajo = abuelo->coord_xy < hijo->coord_xy ? abuelo : hijo;
	mas_arriba = abuelo->coord_xy > hijo->coord_xy ? abuelo : hijo;
	hvargs->mas_abajo = mas_abajo;
	hvargs->mas_arriba = mas_arriba;

	res =
			ctx->matrix_chosto_brinco[cacaso][mas_abajo->coord_x][mas_abajo->coord_y];

	if (res == HUARONVERGA_VALOR_INVALIDO) {
		res = huaronverga_calcula_chosto_brinco(ctx, hvargs);
		assert_timeout(res!=HUARONVERGA_VALOR_INVALIDO);
		ctx->matrix_chosto_brinco[cacaso][mas_abajo->coord_x][mas_abajo->coord_y] =
				res;
	}

	return res;
}


int main() {
	return EXIT_SUCCESS;
}
