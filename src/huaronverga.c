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

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
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

#define HUARONVERGA_MAX_LLAVE (200|((200)<<8))

#if 1

typedef unsigned long long bitch_vector;

#define BITCH_MAX_NUMEROS_AGREGADOS HUARONVERGA_MAX_LLAVE
#define BITCH_TAM_MAPA ( (BITCH_MAX_NUMEROS_AGREGADOS/ (sizeof(bitch_vector) * 8)) + 1)
#define BITCH_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO

typedef struct bitch_vector_ctx {
	entero_largo_sin_signo *bitch_numeros_agregados;
	natural bitch_numeros_agregados_tam;
	bitch_vector *bitch_mapa;
} bitch_vector_ctx;

static inline bitch_vector_ctx *bitch_init() {
	bitch_vector_ctx *bvctx = NULL;
	bvctx = calloc(1, sizeof(bitch_vector_ctx));
	bvctx->bitch_mapa = calloc(BITCH_TAM_MAPA, sizeof(bitch_vector));
	assert_timeout(bvctx->bitch_mapa);
	bvctx->bitch_numeros_agregados = calloc(BITCH_MAX_NUMEROS_AGREGADOS,
			sizeof(entero_largo_sin_signo));
	assert_timeout(bvctx->bitch_numeros_agregados);
	return bvctx;
}

#define bitch_checa_int(bits, posicion, resultado) \
        __asm__ (\
                        "xor %%rdx,%%rdx\n"\
                        "movq %[bitch_posi],%%rax\n"\
                        "movq $64,%%r8\n"\
                        "divq %%r8\n"\
                        "movq $1,%[resul]\n"\
                        "mov %%rdx,%%rcx\n"\
                        "shl %%cl,%[resul]\n"\
                        "andq (%[vectors],%%rax,8),%[resul]\n"\
                        : [resul] "=b" (resultado)\
                        : [bitch_posi] "r" (posicion), [vectors] "r" (bits)\
            :"rax","rdx","rcx","r8")

static inline bool bitch_checa(bitch_vector_ctx *bvctx,
		entero_largo_sin_signo posicion) {
	entero_largo_sin_signo resultado = 0;

	bitch_checa_int(bvctx->bitch_mapa, posicion, resultado);

	return !!resultado;
}

static inline void bitch_asigna(bitch_vector_ctx *bvctx,
		entero_largo_sin_signo posicion) {
	natural idx_arreglo = 0;
	natural idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bvctx->bitch_mapa[idx_arreglo] |= (bitch_vector) ((bitch_vector) 1
			<< idx_registro);

	bvctx->bitch_numeros_agregados[bvctx->bitch_numeros_agregados_tam++] =
			posicion;
}

static inline void bitch_limpia(bitch_vector *bits,
		entero_largo_sin_signo posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] &= (bitch_vector) ~((bitch_vector) 1 << idx_registro);
}

static inline void bitch_limpia_todo(bitch_vector_ctx *bvctx) {
	for (natural i = 0; i < bvctx->bitch_numeros_agregados_tam; i++) {
		tipo_dato encendido = falso;
		entero_largo_sin_signo num_actual = bvctx->bitch_numeros_agregados[i];

		bitch_checa_int(bvctx->bitch_mapa, num_actual, encendido);
		assert_timeout(encendido);
		bitch_limpia(bvctx->bitch_mapa, num_actual);
	}

	bvctx->bitch_numeros_agregados_tam = 0;
}

static inline void bitch_fini(bitch_vector_ctx *bvctx) {
	bitch_limpia_todo(bvctx);
	free(bvctx->bitch_mapa);
	free(bvctx->bitch_numeros_agregados);
	free(bvctx);
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

#if 1

//http://www.thelearningpoint.net/computer-science/data-structures-heaps-with-c-program-source-code
#define HEAP_SHIT_MAX_NODOS (200*200)
#define HEAP_SHIT_MAX_LLAVES HUARONVERGA_MAX_LLAVE
#define HEAP_SHIT_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO

typedef struct heap_shit_nodo {
	tipo_dato prioridad;
	tipo_dato llave;
	void *valor;
} heap_shit_nodo;

typedef struct heap_shit {
	bool min;
	natural heap_size;
	heap_shit_nodo heap[HEAP_SHIT_MAX_NODOS];
	natural idx_en_heap_por_llave[HEAP_SHIT_MAX_LLAVES];
} heap_shit;

/*Initialize Heap*/
static inline heap_shit *heap_shit_init(bool es_min) {
	heap_shit *heap = calloc(1, sizeof(heap_shit));
	assert_timeout(heap);
	heap->heap_size = 0;
	heap->min = es_min;
	memset(heap->heap, HEAP_SHIT_VALOR_INVALIDO, sizeof(heap->heap));
	memset(heap->idx_en_heap_por_llave, HEAP_SHIT_VALOR_INVALIDO,
			sizeof(heap->idx_en_heap_por_llave));
	return heap;
}

void heap_shit_fini(heap_shit *heap_ctx) {
	free(heap_ctx);
}

static inline bool heap_shit_nodo_valido(heap_shit_nodo *nodo) {
	assert_timeout(
			(nodo->llave!=HEAP_SHIT_VALOR_INVALIDO && nodo->prioridad!=HEAP_SHIT_VALOR_INVALIDO) || (nodo->prioridad==nodo->llave));

	return nodo->llave != HEAP_SHIT_VALOR_INVALIDO;
}

static inline natural heap_shit_idx_padre(natural idx_nodo) {
	return idx_nodo >> 1;
}

static inline natural heap_shit_idx_hijo_izq(natural idx_nodo) {
	return idx_nodo << 1;
}

/*Insert an element into the heap */
static inline void heap_shit_insert(heap_shit *heap_ctx,
		heap_shit_nodo *nodo_nuevo) {
	natural heap_size = heap_ctx->heap_size;
	heap_shit_nodo *heap = NULL;
	natural *mapeo_inv = heap_ctx->idx_en_heap_por_llave;

	heap = heap_ctx->heap;

	heap_size++;
	heap[heap_size] = *nodo_nuevo; /*Insert in the last place*/
	/*Adjust its position*/
	natural now = heap_size;
	/*
	 while (((heap_ctx->min
	 && (heap[now / 2] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
	 -1 : (int) heap[now / 2]) > (int) element)
	 || (!heap_ctx->min && (natural) heap[now / 2] < (natural) element))) {
	 */
	while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> nodo_nuevo->prioridad)
					|| (!heap_ctx->min
							&& heap[heap_shit_idx_padre(now)].prioridad
									< nodo_nuevo->prioridad))) {
//printf("caca now %u de heap %u elem %u\n",now,heap[now],element);
		natural idx_padre = heap_shit_idx_padre(now);
		tipo_dato llave_padre = heap[idx_padre]->llave;
		assert_timeout(llave_padre!= HEAP_SHIT_VALOR_INVALIDO);

		heap[now] = heap[idx_padre];
		mapeo_inv[llave_padre] = now;
		now = idx_padre;
	}
//printf("raise now %u con heap %u y elem %u res %u\n",now,heap[now / 2],element, (unsigned int)heap[now / 2]>(unsigned int)element);

	heap[now] = *nodo_nuevo;
	mapeo_inv[nodo_nuevo->llave] = now;

	heap_ctx->heap_size = heap_size;
}

static inline void *heap_shit_delete(heap_shit *heap_ctx, natural idx_a_borrar) {
	natural heap_size = heap_ctx->heap_size;
	natural child, now;
	heap_shit_nodo *minElement, *lastElement;
	heap_shit_nodo *heap;
	natural *mapeo_inv = heap_ctx->idx_en_heap_por_llave;
	heap_shit_nodo borrado = { 0 };
	void *resultado;

	heap = heap_ctx->heap;

	assert_timeout(heap_size >= idx_a_borrar);
	assert_timeout(idx_a_borrar);

	/* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
	 Now heap[1] has to be filled. We put the last element in its place and see if it fits.
	 If it does not fit, take minimum element among both its children and replaces parent with it.
	 Again See if the last element fits in that place.*/
	minElement = heap + idx_a_borrar;
	resultado = minElement->valor;
	lastElement = heap + heap_size--;
	borrado = *minElement;

	now = idx_a_borrar;
	/*
	 if (((heap_ctx->min
	 && (heap[now >> 1] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
	 -1 : (int) heap[now >> 1]) > (int) lastElement)
	 || (!heap_ctx->min
	 && (natural) heap[now >> 1] < (natural) lastElement))) {
	 */
	if (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> lastElement->prioridad)
					|| (!heap_ctx->min
							&& (heap[heap_shit_idx_padre(now)].prioridad
									< lastElement->prioridad)))) {
		/*
		 while (((heap_ctx->min
		 && (heap[now / 2] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
		 -1 : (int) heap[now / 2]) > (int) lastElement)
		 || (!heap_ctx->min
		 && (natural) heap[now / 2] < (natural) lastElement))) {
		 */
		//printf("caca now %u de heap %u elem %u\n",now,heap[now],element);
		while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
				&& ((heap_ctx->min
						&& heap[heap_shit_idx_padre(now)].prioridad
								> lastElement->prioridad)
						|| (!heap_ctx->min
								&& (heap[heap_shit_idx_padre(now)].prioridad
										< lastElement->prioridad)))) {
			natural idx_padre = heap_shit_idx_padre(now);
			tipo_dato llave_padre = heap[idx_padre];

			assert_timeout(llave_padre != HEAP_SHIT_VALOR_INVALIDO);

			heap[now] = heap[idx_padre];
			mapeo_inv[llave_padre] = now;
			now = idx_padre;
		}
	} else {

		/* now refers to the index at which we are now */
		for (now = idx_a_borrar; heap_shit_idx_hijo_izq(now) <= heap_size; now =
				child) {
			/* child is the index of the element which is minimum among both the children */
			/* Indexes of children are i*2 and i*2 + 1*/
			child = heap_shit_idx_hijo_izq(now);
			/*child!=heap_size beacuse heap[heap_size+1] does not exist, which means it has only one
			 child */
			if (child != heap_size
					&& ((heap_ctx->min
							&& heap[child + 1].prioridad < heap[child].prioridad)
							|| (!heap_ctx->min
									&& heap[child + 1].prioridad
											> heap[child].prioridad))) {
				child++;
			}
			/* To check if the last element fits ot not it suffices to check if the last element
			 is less than the minimum element among both the children*/
			//printf("last %u heap %u\n",lastElement,heap[child]);
			if ((heap_ctx->min && lastElement->prioridad > heap[child].prioridad)
					|| (!heap_ctx->min
							&& lastElement->prioridad < heap[child].prioridad)) {
				heap[now] = heap[child];
				mapeo_inv[heap[child]->llave] = now;
			} else /* It fits there */
			{
				break;
			}
		}
	}

	memset(minElement, HEAP_SHIT_VALOR_INVALIDO, sizeof(heap_shit_nodo));
	mapeo_inv[minElement->llave] = HEAP_SHIT_VALOR_INVALIDO;
	if (heap_size) {
		heap[now] = *lastElement;
		mapeo_inv[lastElement->llave] = now;
	}
	heap_ctx->heap_size = heap_size;
	return *resultado;
}

static inline void *heap_shit_borrar_directo(heap_shit *heap_ctx,
		tipo_dato llave, heap_shit_nodo *resultado) {
	natural heap_size = heap_ctx->heap_size;
	natural (*indices_valores)[HEAP_SHIT_MAX_LLAVES] =
			heap_ctx->idx_en_heap_por_llave;
	assert_timeout(heap_size);

	natural idx_a_borrar = indices_valores[llave];
	assert_timeout(idx_a_borrar != HEAP_SHIT_VALOR_INVALIDO);

	return heap_shit_delete(heap_ctx, idx_a_borrar);
}

static inline void *heap_shit_borra_torpe(heap_shit *heap_ctx,
		heap_shit_nodo *resultado) {
	if (heap_ctx->heap_size) {
		return heap_shit_borrar_directo(heap_ctx, heap_ctx->heap[1].llave);
	} else {
		assert_timeout(heap_ctx->heap[1].llave==HEAP_SHIT_VALOR_INVALIDO);
		return NULL;
	}
}

#if  0
void heap_shit_dumpear(heap_shit *heap_ctx) {
	natural heap_size = heap_ctx->heap_size;
	natural *mapeo_inv = heap_ctx->mapeo_idx_heap_a_pos_ocurrencia;
	for (int i = 0; i <= heap_size; i++) {
		caca_log_debug("mierda %u es %u", i, heap_ctx->heap[i]);
	}
	caca_log_debug("los mapeos inversos:");
	for (int i = 0; i < MAX_NUMEROS; i++) {
		caca_log_debug("la llave %u mapea a %u", i, mapeo_inv[i]);
	}
	caca_log_debug("las ocurrencias de cada pendejo:");
	for (int i = 0; i < MAX_INDEX; i++) {
		if (heap_ctx->num_indices_valores[i]) {
			caca_log_debug("el num %u tiene las ocurrencias %s", i,
					caca_arreglo_a_cadena_natural(heap_ctx->indices_valores[i],heap_ctx->num_indices_valores[i],FRAUDUCACA_BUF_STATICO));
		}
	}
}

void heap_shit_valida_referencias_inversas(heap_shit *heap_ctx) {
	natural num_elems = 0;
	natural num_elems_mapeo = 0;
	natural heap_size = heap_ctx->heap_size;
	tipo_dato *heap = heap_ctx->heap;
	natural (*indices_valores)[MAX_NUMEROS] = heap_ctx->indices_valores;
	natural *num_indices_valores = heap_ctx->num_indices_valores;
	natural *mapeo_inv = heap_ctx->mapeo_idx_heap_a_pos_ocurrencia;
	bitch_vector valores_ya_validados[(MAX_NUMEROS / 64) + 1] = {0};
	for (int i = 1; i < heap_size + 1; i++) {
		tipo_dato num_act = heap[i];
		natural *indices_valores_act = indices_valores[num_act];
		natural num_indices_valores_act = num_indices_valores[num_act];
		if (num_indices_valores_act
				&& !caca_comun_checa_bit(valores_ya_validados, num_act)) {
			for (int j = 0; j < num_indices_valores_act; j++) {
				tipo_dato num_act_ocu = 0;
				natural indice_valor_act = 0;
				natural indice_pos_ocurrencia_en_mapa = 0;

				indice_valor_act = indices_valores_act[j];
				num_act_ocu = heap[indice_valor_act];
				assert_timeout(num_act_ocu == num_act);

				indice_pos_ocurrencia_en_mapa = mapeo_inv[indice_valor_act];
				assert_timeout(
						indice_pos_ocurrencia_en_mapa
						!= FRAUDUCACA_VALOR_INVALIDO);

				assert_timeout(j == indice_pos_ocurrencia_en_mapa);

				num_elems++;
			}
			caca_comun_asigna_bit(valores_ya_validados, num_act);
		}
	}
	assert_timeout(heap_size == num_elems);
	for (int i = 0; i < MAX_NUMEROS; i++) {
		natural idx_en_heap = 0;
		natural idx_en_pos_ocurrencias = 0;
		natural num_indices_valores_act = 0;
		tipo_dato num_act = 0;

		idx_en_heap = i;
		idx_en_pos_ocurrencias = mapeo_inv[i];
		if (idx_en_pos_ocurrencias != FRAUDUCACA_VALOR_INVALIDO) {
			num_act = heap[idx_en_heap];

			num_indices_valores_act = num_indices_valores[num_act];
			assert_timeout(idx_en_pos_ocurrencias < num_indices_valores_act);

			num_elems_mapeo++;
		}
	}

	assert_timeout(heap_size == num_elems_mapeo);
}

void heap_shit_valida_invariante(heap_shit *heap_ctx, natural idx_heap) {
	natural heap_size = heap_ctx->heap_size;
	tipo_dato *heap = heap_ctx->heap;
	if (idx_heap < heap_size) {
		tipo_dato num_act = heap[idx_heap];
		natural idx_heap_hijo_izq = (idx_heap << 1);
		if (idx_heap_hijo_izq < heap_size) {
			tipo_dato num_act_hijo_izq = heap[idx_heap_hijo_izq];
			if (heap_ctx->min) {
				assert_timeout(num_act <= num_act_hijo_izq);
			} else {
				assert_timeout(num_act >= num_act_hijo_izq);
			}
		}
		if (idx_heap_hijo_izq + 1 < heap_size) {
			tipo_dato num_act_hijo_der = heap[idx_heap_hijo_izq + 1];
			if (heap_ctx->min) {
				assert_timeout(num_act <= num_act_hijo_der);
			} else {
				assert_timeout(num_act >= num_act_hijo_der);
			}
		}
		heap_shit_valida_invariante(heap_ctx, idx_heap_hijo_izq);
		heap_shit_valida_invariante(heap_ctx, idx_heap_hijo_izq + 1);
	}
}

void heap_shit_valida_mierda(heap_shit *heap_ctx) {
	heap_shit_valida_referencias_inversas(heap_ctx);
	heap_shit_valida_invariante(heap_ctx, 1);
}
#endif

#endif

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
	puto_cardinal matrix_predecesores[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS];
	tipo_dato chosto_brinco;
} huaronverga_ctx;

typedef struct huaronverga_args {
	puto_cardinal *abuelo;
	puto_cardinal *padre;
	puto_cardinal *hijo;
	puto_cardinal *mas_arriba;
	puto_cardinal *mas_abajo;
	tipo_dato chosto_act_hijo;
	huaronverga_caso_familiar cacaso;
} huaronverga_args;

puto_cardinal movimientos_decendiente_directo[] = { { 1, 0 }, { 0, 1 },
		{ -1, 0 }, { 0, -1 } };

puto_cardinal movimientos_posible_abuelo[][2] = {
		[vertical_huaronverga_caso_familiar]= { { 0, 1 }, { 0, -1 } },
		[horizontal_huaronverga_caso_familiar ]= { { 1, 0 }, { -1, 0 } },
		[esq_1_huaronverga_caso_familiar ... final_huaronverga_caso_familiar
				]= { {
				HUARONVERGA_VALOR_INVALIDO,
				HUARONVERGA_VALOR_INVALIDO }, {
				HUARONVERGA_VALOR_INVALIDO,
				HUARONVERGA_VALOR_INVALIDO } } };
natural movimientos_posible_abuelo_tam[] = { [vertical_huaronverga_caso_familiar
		]= 2, [horizontal_huaronverga_caso_familiar ]=2 };

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

static inline tipo_dato huaronverga_compacta_coordenada(puto_cardinal *nodo) {
	tipo_dato coord_xy_compacta = 0;

	coord_xy_compacta = (nodo->coord_x << 8) | nodo->coord_y;

	assert_timeout(coord_xy_compacta<=HEAP_SHIT_MAX_LLAVES);

	return coord_xy_compacta;
}

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
				{ .coord_x = padre->coord_x + movimiento_act->coord_x,
						.coord_y =
						padre->coord_y + movimiento_act->coord_y };
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
				{ .coord_x = padre->coord_x + movimiento_act->coord_x,
						.coord_y =
						padre->coord_y + movimiento_act->coord_y };
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

static inline void huaronverga_genera_vecino_en_idx(puto_cardinal *origen,
		puto_cardinal *vecino,
		huaronverga_movimientos_decendiente_directo_idx idx) {
	puto_cardinal *mov_act = movimientos_decendiente_directo + idx;
	vecino->coord_x = origen->coord_x + mov_act->coord_x;
	vecino->coord_y = origen->coord_y + mov_act->coord_y;
}

#define huaronverga_obten_valor_en_coord(matrix,puto) matrix[puto->coord_x][puto->coord_y]

static inline natural huaronverga_genera_vecinos_validos(huaronverga_ctx *ctx,
		puto_cardinal *caca, puto_cardinal *vecinos) {
	natural vecinos_cnt = 0;

	for (huaronverga_movimientos_decendiente_directo_idx i =
	primero_huaronverga_movimientos_decendiente_directo_idx;
			i < invalido_huaronverga_movimientos_decendiente_directo_idx; i++) {
		puto_cardinal *vecino_act = vecinos + vecinos_cnt;

		huaronverga_genera_vecino_en_idx(caca, vecino_act, i);
		if (huaronverga_obten_valor_en_coord(ctx->matrix_rodeada,
				vecino_act)== HUARONVERGA_CARACTER_BLOQUE_LIBRE) {
			vecinos_cnt;
		}
	}
	return vecinos_cnt;
}

static inline void huaronverga_pon_valor_en_coord_stack_byteme(
		byteme matrix[HUARONVERGA_MAX_FILAS + 4][HUARONVERGA_MAX_COLUMNAS + 4],
		puto_cardinal *puto, byteme valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline void huaronverga_pon_valor_en_coord_stack_puto_cardinal(
		byteme matrix[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS],
		puto_cardinal *puto, puto_cardinal *valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline void huaronverga_pon_valor_en_coord_stack_natural(
		natural matrix[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS],
		puto_cardinal *puto, natural valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline void huaronverga_pon_valor_en_coord_heap(puto_cardinal ***matrix,
		puto_cardinal *puto, puto_cardinal *valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
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
	static puto_cardinal putos_tmp[] = { [0 ... HUARONVERGA_MAX_ELEMENTOS
			>> 1]=HUARONVERGA_VALOR_INVALIDO, [((HUARONVERGA_MAX_ELEMENTOS >> 1)
			+ 1) ... HUARONVERGA_MAX_ELEMENTOS ]=HUARONVERGA_VALOR_INVALIDO };
	bitch_vector_ctx *bvctx = NULL;

	memset(putos_tmp, HUARONVERGA_VALOR_INVALIDO, sizeof(putos_tmp));

	bvctx = bitch_init();

	predecesores = calloc(HUARONVERGA_MAX_FILAS, sizeof(puto_cardinal **));
	assert_timeout(predecesores);
	for (int i = 0; i < HUARONVERGA_MAX_FILAS; i++) {
		predecesores[i] = calloc(HUARONVERGA_MAX_COLUMNAS,
				sizeof(puto_cardinal *));
		assert_timeout(predecesores[i]);
	}

	if (huaronverga_obten_valor_en_coord(matrix,
			inicio)!=HUARONVERGA_CARACTER_BLOQUE_LIBRE || huaronverga_obten_valor_en_coord(matrix, fin)!=HUARONVERGA_CARACTER_BLOQUE_LIBRE) {
		return HUARONVERGA_VALOR_INVALIDO;
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

			if (huaronverga_obten_valor_en_coord(matrix, vecino)== HUARONVERGA_CARACTER_BLOQUE_LIBRE
			&& !bitch_checa(bvctx,
					huaronverga_compacta_coordenada(vecino))) {
				queue_encula(kha, vecino);
				huaronverga_pon_valor_en_coord_heap(predecesores, vecino, act);
				bitch_asigna(bvctx, huaronverga_compacta_coordenada(vecino));
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
			predecesor_actual = huaronverga_obten_valor_en_coord(predecesores,
					predecesor_actual);
			chosto++;
		}
		assert_timeout(predecesor_actual->coord_xy==inicio->coord_xy);
	}

	queue_destruye(kha);
	bitch_fini(bvctx);

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

	valor_ant = huaronverga_obten_valor_en_coord(matrix, invalido);

	chosto = huaronverga_chosto_por_busqueda_en_amplitud(matrix, inicio, fin);

	huaronverga_pon_valor_en_coord_stack_byteme(matrix, invalido, valor_ant);

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
	huaronverga_caso_familiar cacaso = invalido_huaronverga_caso_familiar;
	puto_cardinal *abuelo = hvargs->abuelo;
	puto_cardinal *padre = hvargs->padre;
	puto_cardinal *hijo = hvargs->hijo;
	puto_cardinal *mas_abajo = NULL;
	puto_cardinal *mas_arriba = NULL;
	tipo_dato res = 0;

	assert_timeout(abuelo->coord_xy!=padre->coord_xy);
	assert_timeout(padre->coord_xy!=hijo->coord_xy);
	assert_timeout(hijo->coord_xy!=abuelo->coord_xy);

	cacaso = huaronverga_obten_caso_familiar(hvargs);
	hvargs->cacaso = cacaso;

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

static inline tipo_dato huaronverga_mejora_chosto_hijo(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	tipo_dato chosto = hvargs->chosto_act_hijo;
	huaronverga_caso_familiar cacaso = hvargs->cacaso;
	puto_cardinal *abuelo = hvargs->abuelo;
	puto_cardinal *padre = hvargs->padre;
	puto_cardinal *hijo = hvargs->hijo;

	for (int i = 0; i < movimientos_posible_abuelo_tam[cacaso]; i++) {
		tipo_dato chosto_posible_bisabuelo_padre = 0;
		tipo_dato chosto_posible_abuelo_hijo = 0;
		tipo_dato chosto_posible_abuelo = 0;
		tipo_dato chosto_hijo = 0;
		puto_cardinal *mov_act = &movimientos_posible_abuelo[cacaso][i];
		puto_cardinal *posible_abuelo = &(puto_cardinal ) { .coord_x =
				padre->coord_x + mov_act->coord_x, .coord_y =
				padre->coord_y + mov_act->coord_y };
		puto_cardinal *posible_bisabuelo = &huaronverga_obten_valor_en_coord(
				ctx->matrix_predecesores, posible_abuelo);
//				&ctx->matrix_predecesores[posible_abuelo->coord_x][posible_abuelo->coord_y];

		assert_timeout(posible_abuelo->coord_x!=HUARONVERGA_VALOR_INVALIDO);
		assert_timeout(posible_abuelo->coord_y!=HUARONVERGA_VALOR_INVALIDO);

		chosto_posible_abuelo = huaronverga_obten_valor_en_coord(
				ctx->matrix_chosto_minimo, posible_abuelo);
		//ctx->matrix_chosto_minimo[posible_abuelo->coord_x][posible_abuelo->coord_y];
		assert_timeout(
				chosto_posible_abuelo->coord_x!=HUARONVERGA_VALOR_INVALIDO);
		assert_timeout(
				chosto_posible_abuelo->coord_y!=HUARONVERGA_VALOR_INVALIDO);

		chosto_posible_bisabuelo_padre = huaronverga_obten_chosto_brinco(ctx,
				&(huaronverga_args ) { .cacaso = cacaso, .abuelo =
								posible_bisabuelo, .padre = posible_abuelo,
								.hijo = padre });
		assert_timeout(
				chosto_posible_bisabuelo_padre!=HUARONVERGA_VALOR_INVALIDO);

		chosto_posible_abuelo_hijo = huaronverga_obten_chosto_brinco(ctx,
				&(huaronverga_args ) { .cacaso = cacaso, .abuelo =
								posible_abuelo, .padre = padre, .hijo = hijo });

		assert_timeout(chosto_posible_abuelo_hijo!=HUARONVERGA_VALOR_INVALIDO);

		chosto_hijo = chosto_posible_abuelo + chosto_posible_bisabuelo_padre
				+ chosto_posible_abuelo_hijo;

		if (chosto_hijo < chosto) {
			chosto = chosto_hijo;
		}
	}

	return chosto;
}

static inline void huaronverga_dickstra(huaronverga_ctx *ctx,
		puto_cardinal *cacao, puto_cardinal *vacio, puto_cardinal *salida) {
	natural (*matrix_predecesores)[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS] =
			ctx->matrix_predecesores;
	tipo_dato (*matrix_chosto_minimo)[HUARONVERGA_MAX_FILAS][HUARONVERGA_MAX_COLUMNAS] =
			ctx->matrix_chosto_minimo;
	puto_cardinal vecinos_tmp[ultimo_huaronverga_movimientos_decendiente_directo_idx] =
			{ 0 };
	natural vecinos_tmp_cnt = 0;

	puto_cardinal *putos_tmp = NULL;
	natural putos_tmp_cnt = 0;

	heap_shit *cola_prioridad = NULL;

	putos_tmp = calloc(HUARONVERGA_MAX_ELEMENTOS << 1, sizeof(putos_tmp));
	assert_timeout(putos_tmp);

	cola_prioridad = heap_shit_init(verdadero);

	vecinos_tmp_cnt = huaronverga_genera_vecinos_validos(ctx, cacao,
			vecinos_tmp);

	for (int i = 0; i < vecinos_tmp_cnt; i++) {
		natural chosto_vacio_primer_mov = 0;
		puto_cardinal *vecino_act = putos_tmp + putos_tmp_cnt++;
		*vecino_act = vecinos_tmp[i];

		chosto_vacio_primer_mov =
				huaronverga_chosto_por_busqueda_en_amplitud_brincando(
						ctx->matrix_rodeada, vacio, cacao, vecino_act);

		putos_tmp[putos_tmp_cnt] = *vecino_act;

		heap_shit_insert(cola_prioridad, &(heap_shit_nodo ) { .prioridad =
						chosto_vacio_primer_mov, .llave =
						huaronverga_compacta_coordenada(vecino_act), .valor =
						vecino_act });
		huaronverga_pon_valor_en_coord_stack_puto_cardinal(matrix_predecesores,
				vecino_act, cacao);
		huaronverga_pon_valor_en_coord_stack_natural(matrix_chosto_minimo,
				vecino_act, chosto_vacio_primer_mov);
	}

	while (cola_prioridad->heap_size) {
		puto_cardinal *padre = heap_shit_borra_torpe(cola_prioridad);
		assert_timeout(padre);
		tipo_dato chosto_padre = huaronverga_obten_valor_en_coord(
				matrix_chosto_minimo, padre);
		puto_cardinal *abuelo = huaronverga_obten_valor_en_coord(
				matrix_predecesores, padre);
		assert_timeout(abuelo);
		vecinos_tmp_cnt = huaronverga_genera_vecinos_validos(ctx, padre,
				vecinos_tmp);
		for (int i = 0; i < vecinos_tmp_cnt; i++) {
			puto_cardinal *hijo = putos_tmp + putos_tmp_cnt++;
			*hijo = vecinos_tmp[i];
			huaronverga_args *hvargs = &(huaronverga_args ) { .abuelo = abuelo,
							.padre = padre, .hijo = hijo };
			tipo_dato chosto_padre_hijo = huaronverga_obten_chosto_brinco(ctx,
					hvargs);
			tipo_dato chosto_hijo = chosto_padre + chosto_padre_hijo;

			hvargs->chosto_act_hijo = chosto_hijo;
			chosto_hijo = huaronverga_mejora_chosto_hijo(ctx, hvargs);

			huaronverga_pon_valor_en_coord_stack_puto_cardinal(
					matrix_predecesores, hijo, padre);
			huaronverga_pon_valor_en_coord_stack_natural(matrix_chosto_minimo,
					hijo, chosto_hijo);
			heap_shit_insert(cola_prioridad,
					&(heap_shit_nodo ) { .prioridad = chosto_hijo,
									.llave = huaronverga_compacta_coordenada(
											hijo), .valor = hijo });
		}

	}

	heap_shit_fini(cola_prioridad);
	free(cola_prioridad);
	free(putos_tmp);
}

int main() {
	return EXIT_SUCCESS;
}
