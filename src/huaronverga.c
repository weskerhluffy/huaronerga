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
#include <ctype.h>

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
typedef unsigned long long bitch_vector;
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

#define caca_log_debug(formato, args...) 0;

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

#endif

#define HV_MAX_LLAVE (200|((200)<<8))
#define HV_MAX_LLAVE_RODEADA (204|((204)<<8))

#if 1

#define BITCH_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO

typedef struct bitch_vector_ctx {
	natural bitch_numeros_agregados_tam;
	bitch_vector *bitch_mapa;
} bitch_vector_ctx;

static inline bitch_vector_ctx *bitch_init(natural max_nums) {
	bitch_vector_ctx *bvctx = NULL;
	bvctx = calloc(1, sizeof(bitch_vector_ctx));
	bvctx->bitch_mapa = calloc(((max_nums / (sizeof(bitch_vector) * 8)) + 1),
			sizeof(bitch_vector));
	assert_timeout(bvctx->bitch_mapa);
	return bvctx;
}

static inline bool bitch_checa(bitch_vector_ctx *bvctx,
		entero_largo_sin_signo posicion) {
	entero_largo_sin_signo resultado = 0;
	natural idx_arreglo = 0;
	natural idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	resultado = bvctx->bitch_mapa[idx_arreglo]
			& (bitch_vector) ((bitch_vector) 1 << idx_registro);

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
	bvctx->bitch_numeros_agregados_tam++;

}

static inline void bitch_limpia(bitch_vector_ctx *bvctx,
		entero_largo_sin_signo posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;
	bitch_vector *bits = bvctx->bitch_mapa;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] &= (bitch_vector) ~((bitch_vector) 1 << idx_registro);

	bvctx->bitch_numeros_agregados_tam--;
}

static inline void bitch_fini(bitch_vector_ctx *bvctx) {
	free(bvctx->bitch_mapa);
	free(bvctx);
}

#endif

#if 1

#define QUEUE_VALOR_INVALIDO ((void *)LONG_LONG_MAX)

typedef struct Node_t {
	void *datos;
	struct Node_t *prev;
} queue_nodo;

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

void *queue_decula(queue_t *pQueue) {
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
void queue_destruye(queue_t *queue) {
	while (!queue_vacia(queue)) {
		queue_decula(queue);
	}
	free(queue);
}

bool queue_encula(queue_t *pQueue, void *mierda) {
	queue_nodo *item = (queue_nodo *) calloc(1, sizeof(queue_nodo));
	if ((pQueue == NULL) || (item == NULL)) {
		return falso;
	}
	if (pQueue->size >= pQueue->limit) {
		free(item);
		return falso;
	}
	item->datos = mierda;
	item->prev = NULL;
	if (pQueue->size == 0) {
		pQueue->head = item;
		pQueue->tail = item;

	} else {
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	pQueue->size++;
	return verdadero;
}

#endif

typedef struct pcrd {
	union {
		struct {
			int coordenada_y_pcrd;
			int coordenada_x_pcrd;
		} separados_pcrd;
		entero_largo coordenadas_juntas_pcrd;
	} datos_pcrd;
	void *extra;
} pcrd;

#define coord_x datos_pcrd.separados_pcrd.coordenada_x_pcrd
#define coord_y datos_pcrd.separados_pcrd.coordenada_y_pcrd
#define coord_xy datos_pcrd.coordenadas_juntas_pcrd

static inline short pcrd_compacta_a_corto(pcrd *nodo) {
	int coord_xy_compacta = 0;

	coord_xy_compacta = (nodo->coord_x << 8) | nodo->coord_y;

	return coord_xy_compacta;
}

#define pcrd_a_cadena_buffer_local(puto) pcrd_a_cadena((puto),CACA_COMUN_BUF_STATICO)

#if 1
typedef natural hm_iter;
#define HASH_MAP_VALOR_INVALIDO ((hm_iter)CACA_COMUN_VALOR_INVALIDO)
typedef struct hash_map_entry {
	entero_largo llave;
	entero_largo valor;
} hm_entry;
typedef struct hash_map_cubeta {
	uint64_t hash;
	hm_entry *entry;
} hm_cubeta;
typedef struct hmrh {
	hm_cubeta *buckets_;
	uint64_t num_buckets_;
	uint64_t num_buckets_used_;
	uint64_t probing_min_;
	uint64_t probing_max_;
} hm_rr_bs_tabla;
int hmrh_init(hm_rr_bs_tabla *ht, int num_cubetas) {
	ht->num_buckets_ = num_cubetas;
	ht->buckets_ = (hm_cubeta *) calloc(ht->num_buckets_, sizeof(hm_cubeta));
	ht->num_buckets_used_ = 0;
	ht->probing_max_ = num_cubetas;
	return 0;
}
int hmrh_fini(hm_rr_bs_tabla *ht) {
	if (ht->buckets_ != NULL) {
		for (uint32_t i = 0; i < ht->num_buckets_; i++) {
			if (ht->buckets_[i].entry != NULL) {
				free(ht->buckets_[i].entry);
				ht->buckets_[i].entry = NULL;
			}
		}
		free(ht->buckets_);
	}
	return 0;
}
static inline int hmrh_llena_distancia_a_indice_inicio(
		hm_rr_bs_tabla *ht, uint64_t index_stored, uint64_t *distance) {
	hm_cubeta cubeta = ht->buckets_[index_stored];
	*distance = 0;
	if (cubeta.entry == NULL)
		return -1;
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t index_init = cubeta.hash % num_cubetas;
	if (index_init <= index_stored) {
		*distance = index_stored - index_init;
	} else {
		*distance = index_stored + (num_cubetas - index_init);
	}
	return 0;
}
hm_iter hmrh_obten(hm_rr_bs_tabla *ht,
		const entero_largo key, entero_largo *value) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	uint64_t probe_distance = 0;
	uint64_t index_current;
	bool found = falso;
	uint32_t i;
	*value = HASH_MAP_VALOR_INVALIDO;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_entry *entrada = ht->buckets_[index_current].entry;
		if (entrada == NULL) {
			break;
		}
		hmrh_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			*value = entrada->valor;
			found = verdadero;
			break;
		}
	}
	if (found)
		return index_current;
	return HASH_MAP_VALOR_INVALIDO;
}
hm_iter hmrh_pon(hm_rr_bs_tabla *ht, entero_largo key,
		entero_largo value, bool *nuevo_entry) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_min_;
	hm_cubeta *cubetas = ht->buckets_;
	*nuevo_entry = verdadero;
	if (ht->num_buckets_used_ == num_cubetas) {
		*nuevo_entry = falso;
		return HASH_MAP_VALOR_INVALIDO;
	}
	ht->num_buckets_used_ += 1;
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	hm_entry *entry = (hm_entry *) calloc(1, sizeof(hm_entry));
	entry->llave = key;
	entry->valor = value;
	uint64_t index_current = index_init % num_cubetas;
	uint64_t probe_current = 0;
	uint64_t probe_distance;
	hm_entry *entry_temp;
	uint64_t hash_temp;
	uint64_t i;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_cubeta *cubeta = cubetas + index_current;
		if (cubeta->entry == NULL) {
			cubeta->entry = entry;
			cubeta->hash = hash;
			if (index_current > prob_max) {
				ht->probing_max_ = index_current;
			}
			if (index_current < prob_min) {
				ht->probing_min_ = index_current;
			}
			break;
		} else {
			if (cubeta->entry->llave == key) {
				free(entry);
				*nuevo_entry = falso;
				break;
			}
			hmrh_llena_distancia_a_indice_inicio(ht,
					index_current, &probe_distance);
			if (probe_current > probe_distance) {
				entry_temp = cubeta->entry;
				hash_temp = cubeta->hash;
				cubeta->entry = entry;
				cubeta->hash = hash;
				entry = entry_temp;
				hash = hash_temp;
				probe_current = probe_distance;
			}
		}
		probe_current++;
	}
	return index_current;
}
int hmrh_borra(hm_rr_bs_tabla *ht,
		const tipo_dato key) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_max_;
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	bool found = falso;
	uint64_t index_current = 0;
	uint64_t probe_distance = 0;
	hm_entry *entrada = NULL;
	for (uint64_t i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		entrada = ht->buckets_[index_current].entry;
		hmrh_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (entrada == NULL || i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			found = verdadero;
			break;
		}
	}
	if (found) {
		free(entrada);
		uint64_t i = 1;
		uint64_t index_previous = 0, index_swap = 0;
		for (i = 1; i < num_cubetas; i++) {
			index_previous = (index_current + i - 1) % num_cubetas;
			index_swap = (index_current + i) % num_cubetas;
			hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
			hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
			if (cubeta_swap->entry == NULL) {
				cubeta_previous->entry = NULL;
				break;
			}
			uint64_t distance;
			if (hmrh_llena_distancia_a_indice_inicio(
					ht, index_swap, &distance) != 0) {
				fprintf(stderr, "Error in FillDistanceToInitIndex()");
			}
			if (!distance) {
				cubeta_previous->entry = NULL;
				break;
			}
			cubeta_previous->entry = cubeta_swap->entry;
			cubeta_previous->hash = cubeta_swap->hash;
		}
		if (i < num_cubetas) {
			if (index_previous == prob_min) {
				prob_min++;
				if (prob_min >= num_cubetas) {
					prob_min = 0;
				} else {
					while (prob_min < num_cubetas
							&& ht->buckets_[prob_min].entry) {
						prob_min++;
					}
					if (prob_min >= num_cubetas) {
						prob_min = num_cubetas;
					}
				}
				ht->probing_min_ = prob_min;
			}
			if (index_previous == prob_max) {
				prob_max--;
				if (prob_max >= num_cubetas) {
					prob_max = num_cubetas;
				} else {
					while (((int64_t) prob_max) >= 0
							&& ht->buckets_[prob_max].entry) {
						prob_max--;
					}
					if (prob_max >= num_cubetas) {
						prob_max = 0;
					}
				}
				ht->probing_max_ = prob_max;
			}
		}
		ht->num_buckets_used_--;
		return 0;
	}
	return 1;
}

#endif

#if 1

#define HEAP_SHIT_MAX_NODOS (204|(204<<8))
#define HEAP_SHIT_MAX_LLAVES HV_MAX_LLAVE
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
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *tablon_llave_a_idx_heap;
#else
	natural mapa_llave_a_idx_heap[HEAP_SHIT_MAX_NODOS];
#endif
} heap_shit;

static inline heap_shit *heap_shit_init(bool es_min) {
	heap_shit *heap = calloc(1, sizeof(heap_shit));
	assert_timeout(heap);
	heap->heap_size = 0;
	heap->min = es_min;
	memset(heap->heap, HEAP_SHIT_VALOR_INVALIDO, sizeof(heap->heap));
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	heap->tablon_llave_a_idx_heap = calloc(1, sizeof(hm_rr_bs_tabla));
	assert_timeout(heap->tablon_llave_a_idx_heap);
	hmrh_init(heap->tablon_llave_a_idx_heap, HEAP_SHIT_MAX_NODOS);
#else
	for (int i = 0; i < HEAP_SHIT_MAX_NODOS; i++) {
		heap->mapa_llave_a_idx_heap[i] = HEAP_SHIT_VALOR_INVALIDO;
	}
#endif
	return heap;
}

void heap_shit_fini(heap_shit *heap_ctx) {
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hmrh_fini(heap_ctx->tablon_llave_a_idx_heap);
	free(heap_ctx->tablon_llave_a_idx_heap);
#endif
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

static inline void heap_shit_insert(heap_shit *heap_ctx,
		heap_shit_nodo *nodo_nuevo) {
	natural heap_size = heap_ctx->heap_size;
	heap_shit_nodo *heap = NULL;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *mapeo_inv = heap_ctx->tablon_llave_a_idx_heap;
#else
	natural *mapeo_inv = heap_ctx->mapa_llave_a_idx_heap;
#endif
	bool nuevo;

	heap = heap_ctx->heap;

	heap_size++;
	heap[heap_size] = *nodo_nuevo;
	natural now = heap_size;
	while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> nodo_nuevo->prioridad)
					|| (!heap_ctx->min
							&& heap[heap_shit_idx_padre(now)].prioridad
									< nodo_nuevo->prioridad))) {

		natural idx_padre = heap_shit_idx_padre(now);
		tipo_dato llave_padre = heap[idx_padre].llave;
		assert_timeout(llave_padre!= HEAP_SHIT_VALOR_INVALIDO);

		heap[now] = heap[idx_padre];
#ifdef HEAP_SHIT_MAPEO_COMPLETO
		hmrh_reemplazar(mapeo_inv, llave_padre, now);
#else
		mapeo_inv[llave_padre] = now;
#endif
		now = idx_padre;
	}

	heap[now] = *nodo_nuevo;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hmrh_insertar_nuevo(mapeo_inv, nodo_nuevo->llave,
			now);
#else
	mapeo_inv[nodo_nuevo->llave] = now;
#endif

	heap_ctx->heap_size = heap_size;
}

#define heap_shit_prioridad_en_idx_heap(heap,idx) ((heap)[idx].prioridad)
#define heap_shit_llave_en_idx_heap(heap,idx) ((heap)[idx].llave)

static inline void *heap_shit_delete(heap_shit *heap_ctx, natural idx_a_borrar) {
	natural heap_size = heap_ctx->heap_size;
	natural child, now;
	heap_shit_nodo minElement = { 0 };
	heap_shit_nodo lastElement = { 0 };
	heap_shit_nodo *heap;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *mapeo_inv = heap_ctx->tablon_llave_a_idx_heap;
#else
	natural *mapeo_inv = heap_ctx->mapa_llave_a_idx_heap;
#endif
	void *resultado;
	bool nuevo;

	heap = heap_ctx->heap;

	assert_timeout(heap_size >= idx_a_borrar);
	assert_timeout(idx_a_borrar);

	minElement = heap[idx_a_borrar];
	resultado = minElement.valor;
	lastElement = heap[heap_size--];

	now = idx_a_borrar;
	if (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> lastElement.prioridad)
					|| (!heap_ctx->min
							&& (heap[heap_shit_idx_padre(now)].prioridad
									< lastElement.prioridad)))) {
		while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
				&& ((heap_ctx->min
						&& heap[heap_shit_idx_padre(now)].prioridad
								> lastElement.prioridad)
						|| (!heap_ctx->min
								&& (heap[heap_shit_idx_padre(now)].prioridad
										< lastElement.prioridad)))) {
			natural idx_padre = heap_shit_idx_padre(now);
			tipo_dato llave_padre = heap[idx_padre].llave;

			assert_timeout(llave_padre != HEAP_SHIT_VALOR_INVALIDO);

			heap[now] = heap[idx_padre];

#ifdef HEAP_SHIT_MAPEO_COMPLETO
			hmrh_reemplazar(mapeo_inv, llave_padre,
					now);
#else
			mapeo_inv[llave_padre] = now;
#endif

			now = idx_padre;
		}
	} else {

		for (now = idx_a_borrar; heap_shit_idx_hijo_izq(now) <= heap_size; now =
				child) {
			child = heap_shit_idx_hijo_izq(now);
			if (child != heap_size
					&& ((heap_ctx->min
							&& heap[child + 1].prioridad < heap[child].prioridad)
							|| (!heap_ctx->min
									&& heap[child + 1].prioridad
											> heap[child].prioridad))) {
				child++;
			}
			if ((heap_ctx->min && lastElement.prioridad > heap[child].prioridad)
					|| (!heap_ctx->min
							&& lastElement.prioridad < heap[child].prioridad)) {
				heap[now] = heap[child];

#ifdef HEAP_SHIT_MAPEO_COMPLETO
				hmrh_reemplazar(mapeo_inv,
						heap[child].llave, now);
#else
				mapeo_inv[heap_shit_llave_en_idx_heap(heap, child)] = now;
#endif
			} else {
				break;
			}
		}
	}

#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hmrh_borra(mapeo_inv, minElement.llave);
#else
	mapeo_inv[minElement.llave] = HEAP_SHIT_VALOR_INVALIDO;
#endif
	if (heap_size && idx_a_borrar != heap_size + 1) {
		heap[now] = lastElement;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
		hmrh_reemplazar(mapeo_inv, lastElement.llave,
				now);
#else
		mapeo_inv[lastElement.llave] = now;
#endif
	}
	heap_ctx->heap_size = heap_size;
	return resultado;
}

static inline void *heap_shit_borrar_directo(heap_shit *heap_ctx,
		tipo_dato llave) {
	natural heap_size = heap_ctx->heap_size;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *indices_valores = heap_ctx->tablon_llave_a_idx_heap;
#else
	natural *indices_valores = heap_ctx->mapa_llave_a_idx_heap;
#endif
	entero_largo idx_a_borrar;

	assert_timeout(heap_size);

#ifdef HEAP_SHIT_MAPEO_COMPLETO
	natural idx_hm = hmrh_obten(indices_valores,
			llave, &idx_a_borrar);
	assert_timeout(idx_hm!=HASH_MAP_VALOR_INVALIDO);
#else
	idx_a_borrar = indices_valores[llave];
#endif
	assert_timeout(idx_a_borrar != HEAP_SHIT_VALOR_INVALIDO);

	return heap_shit_delete(heap_ctx, idx_a_borrar);
}

static inline void *heap_shit_borra_torpe(heap_shit *heap_ctx) {
	if (heap_ctx->heap_size) {
		return heap_shit_borrar_directo(heap_ctx, heap_ctx->heap[1].llave);
	} else {
		assert_timeout(heap_ctx->heap[1].llave==HEAP_SHIT_VALOR_INVALIDO);
		return NULL;
	}
}

static inline void heap_shit_sube_para_arriba(heap_shit *ctx,
		tipo_dato idx_a_subir
#ifdef HEAP_SHIT_MAPEO_COMPLETO
		, hm_iter idx_en_ht
#endif
		) {
	natural now = idx_a_subir;
	heap_shit_nodo *heap = ctx->heap;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *mapeo_inv = ctx->tablon_llave_a_idx_heap;
#else
	natural *mapeo_inv = ctx->mapa_llave_a_idx_heap;
#endif
	natural idx_padre = heap_shit_idx_padre(now);
	heap_shit_nodo *nodo_a_subir = &(heap_shit_nodo ) { 0 };

	*nodo_a_subir = heap[idx_a_subir];

	/*
	 while (heap_shit_nodo_valido(heap + idx_padre)
	 && ((ctx->min && heap_shit_prioridad_en_idx_heap(heap, idx_padre) > nodo_a_subir->prioridad
	 )
	 || (!ctx->min && ( heap_shit_prioridad_en_idx_heap(heap,
	 idx_padre) < nodo_a_subir->prioridad))))
	 */
	while (idx_padre
			&& heap_shit_prioridad_en_idx_heap(heap, idx_padre)
					> nodo_a_subir->prioridad) {

//		heap_shit_intercambia_nodos(ctx, idx_padre, now);

#ifdef HEAP_SHIT_MAPEO_COMPLETO
		hmrh_reemplazar(mapeo_inv,
				heap_shit_llave_en_idx_heap(heap, idx_padre), now);
#else
		mapeo_inv[heap_shit_llave_en_idx_heap(heap, idx_padre)] = now;
#endif
		heap[now] = heap[idx_padre];

		now = idx_padre;
		idx_padre = heap_shit_idx_padre(now);
	}

#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hmrh_indice_pon_valor(mapeo_inv, idx_en_ht, now);
#else
	mapeo_inv[nodo_a_subir->llave] = now;
#endif
	heap[now] = *nodo_a_subir;
}

static inline void heap_shit_altera_prioridad(heap_shit *ctx, tipo_dato llave,
		tipo_dato prioridad_nueva) {

	natural heap_size = ctx->heap_size;
#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_rr_bs_tabla *indices_valores = ctx->tablon_llave_a_idx_heap;
#else
	natural *indices_valores = ctx->mapa_llave_a_idx_heap;
#endif
	heap_shit_nodo *heap = ctx->heap;
	entero_largo idx_a_subir;
	heap_shit_nodo *minElement;

	assert_timeout(heap_size);

#ifdef HEAP_SHIT_MAPEO_COMPLETO
	hm_iter idx_hm = hmrh_obten(indices_valores,
			llave, &idx_a_subir);
	assert_timeout(idx_hm!=HASH_MAP_VALOR_INVALIDO);
#else
	idx_a_subir = indices_valores[llave];
#endif
	assert_timeout(idx_a_subir != HEAP_SHIT_VALOR_INVALIDO);

	minElement = heap + idx_a_subir;
	minElement->prioridad = prioridad_nueva;

	heap_shit_sube_para_arriba(ctx, idx_a_subir
#ifdef HEAP_SHIT_MAPEO_COMPLETO
			, idx_hm
#endif
			);
}

#endif

#define HV_VALOR_INVALIDO (CACA_COMUN_VALOR_INVALIDO-10)
#define HV_INVIABLE (HV_VALOR_INVALIDO-101)

typedef enum huaronverga_caso_familiar {
	vertical_huaronverga_caso_familiar = 0,
	horizontal_huaronverga_caso_familiar,
	esq_1_huaronverga_caso_familiar,
	esq_2_huaronverga_caso_familiar,
	esq_3_huaronverga_caso_familiar,
	esq_4_huaronverga_caso_familiar,
	final_huaronverga_caso_familiar,
	invalido_huaronverga_caso_familiar = HV_VALOR_INVALIDO
} huaronverga_caso_familiar;
#define inicial_huaronverga_caso_familiar vertical_huaronverga_caso_familiar

typedef enum huaronverga_movimientos_decendiente_directo_idx {
	arriba_huaronverga_movimientos_decendiente_directo_idx = 0,
	derecha_huaronverga_movimientos_decendiente_directo_idx,
	abajo_huaronverga_movimientos_decendiente_directo_idx,
	izquierda_huaronverga_movimientos_decendiente_directo_idx,
	ultimo_huaronverga_movimientos_decendiente_directo_idx,
	invalido_huaronverga_movimientos_decendiente_directo_idx = HV_VALOR_INVALIDO
} huaronverga_movimientos_decendiente_directo_idx;
#define primero_huaronverga_movimientos_decendiente_directo_idx arriba_huaronverga_movimientos_decendiente_directo_idx

#define HV_MAX_FILAS 200
#define HV_MAX_COLUMNAS 200
#define HV_MAX_ELEMENTOS (HV_MAX_FILAS*HV_MAX_COLUMNAS)
#define HV_MAX_ELEMENTOS_RODEADOS ((HV_MAX_FILAS+4)*(HV_MAX_COLUMNAS+4))
#define HV_CARACTER_BLOQUE_LIBRE '1'
#define HV_CARACTER_BLOQUE_BLOKEADO '0'

typedef struct huaronverga_ctx {
	byteme matrix[HV_MAX_FILAS][HV_MAX_COLUMNAS];
	byteme matrix_rodeada[HV_MAX_FILAS + 4][HV_MAX_COLUMNAS
			+ 4];
	tipo_dato matrix_chosto_brinco[final_huaronverga_caso_familiar][HV_MAX_FILAS][HV_MAX_COLUMNAS];
	tipo_dato matrix_chosto_minimo[HV_MAX_FILAS][HV_MAX_COLUMNAS];
	pcrd matrix_predecesores[HV_MAX_FILAS][HV_MAX_COLUMNAS];
	tipo_dato chosto_brinco;
	natural columnas_tam;
	natural filas_tam;
} huaronverga_ctx;

typedef struct huaronverga_args {
	pcrd *abuelo;
	pcrd *padre;
	pcrd *hijo;
	pcrd *mas_arriba;
	pcrd *mas_abajo;
	tipo_dato chosto_act_hijo;
	huaronverga_caso_familiar cacaso;
} huaronverga_args;

pcrd movimientos_decendiente_directo[] = { { 0, 1 }, { 1, 0 },
		{ 0, -1 }, { -1, 0 } };

pcrd movimientos_posible_abuelo[][2] = {
		[vertical_huaronverga_caso_familiar]= { { 1, 0 }, { -1, 0 } },
		[horizontal_huaronverga_caso_familiar ]= { { 0, 1 }, { 0, -1 } },
		[esq_1_huaronverga_caso_familiar ... final_huaronverga_caso_familiar
				]= { {
				HV_VALOR_INVALIDO,
				HV_VALOR_INVALIDO }, {
				HV_VALOR_INVALIDO,
				HV_VALOR_INVALIDO } } };
natural movimientos_posible_abuelo_tam[] = { [vertical_huaronverga_caso_familiar
		]= 2, [horizontal_huaronverga_caso_familiar ]=2,
		[esq_1_huaronverga_caso_familiar ... final_huaronverga_caso_familiar
				]= 0 };

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

static inline tipo_dato huaronverga_cc(pcrd *nodo) {
	tipo_dato coord_xy_compacta = 0;

	coord_xy_compacta = pcrd_compacta_a_corto(nodo)
			& ((tipo_dato) 0xffff);

	assert_timeout(coord_xy_compacta<=HEAP_SHIT_MAX_LLAVES);

	return coord_xy_compacta;
}

static inline tipo_dato huaronverga_cc_rodeada(
		pcrd *nodo) {
	tipo_dato coord_xy_compacta = 0;

	coord_xy_compacta = pcrd_compacta_a_corto(nodo)
			& ((tipo_dato) 0xffff);

	assert_timeout(coord_xy_compacta <= HV_MAX_LLAVE_RODEADA);

	return coord_xy_compacta;
}

static inline huaronverga_caso_familiar huaronverga_obten_caso_familiar(
		huaronverga_args *hvargs) {
	pcrd *abuelo = hvargs->abuelo;
	pcrd *padre = hvargs->padre;
	pcrd *hijo = hvargs->hijo;
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

		pcrd *movimiento_act = movimientos_decendiente_directo
				+ caso_abuelo_idx;

		pcrd *potencial_abuelo = &(pcrd )
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

		pcrd *movimiento_act = movimientos_decendiente_directo
				+ caso_hijo_idx;

		pcrd *potencial_hijo = &(pcrd )
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

static inline void huaronverga_genera_vecino_en_idx(pcrd *origen,
		pcrd *vecino,
		huaronverga_movimientos_decendiente_directo_idx idx) {
	pcrd *mov_act = movimientos_decendiente_directo + idx;
	vecino->coord_x = origen->coord_x + mov_act->coord_x;
	vecino->coord_y = origen->coord_y + mov_act->coord_y;
}

#define huaronverga_obten_valor_en_coord(matrix,puto) (matrix)[puto->coord_x][puto->coord_y]

#define huaronverga_genera_puto_rodeado_local(nodo) (&(pcrd ){.coord_x=(nodo)->coord_x+2,.coord_y=(nodo)->coord_y+2})
#define huaronverga_genera_puto_desrodeado_local(nodo) (&(pcrd ){.coord_x=(nodo)->coord_x-2,.coord_y=(nodo)->coord_y-2})

static inline bool huaronverga_va_pcrd(huaronverga_ctx *ctx,
		pcrd *puto) {
	bool valido = verdadero;
	if (puto->coord_x == HV_VALOR_INVALIDO
			|| puto->coord_y == HV_VALOR_INVALIDO || puto->coord_x < 0
			|| puto->coord_y < 0 || puto->coord_x >= ctx->filas_tam
			|| puto->coord_y >= ctx->columnas_tam) {
		valido = falso;
	}
	return valido;
}

static inline bool huaronverga_va_pcrd_rodeado(
		huaronverga_ctx *ctx, pcrd *puto) {
	bool valido = verdadero;
	if (puto->coord_x == HV_VALOR_INVALIDO
			|| puto->coord_y == HV_VALOR_INVALIDO || puto->coord_x < 0
			|| puto->coord_y < 0 || puto->coord_x >= ctx->filas_tam + 4
			|| puto->coord_y >= ctx->columnas_tam + 4) {
		valido = falso;
	}
	return valido;
}

static inline bool huaronverga_va_pcrd_valor_matrix(
		huaronverga_ctx *ctx, pcrd *puto) {
	bool valido = verdadero;
	if (!huaronverga_va_pcrd(ctx, puto) ||
	huaronverga_obten_valor_en_coord(ctx->matrix,puto)!=HV_CARACTER_BLOQUE_LIBRE
	) {
		valido = falso;
	}
	return valido;
}

static inline bool huaronverga_va_pcrd_valor_matrix_rodeada(
		huaronverga_ctx *ctx, pcrd *puto) {
	bool valido = verdadero;
	if (!huaronverga_va_pcrd_rodeado(ctx, puto) ||
	huaronverga_obten_valor_en_coord(ctx->matrix_rodeada,puto)!=HV_CARACTER_BLOQUE_LIBRE
	) {
		valido = falso;
	}
	return valido;
}

static inline natural huaronverga_genera_vecinos_validos(huaronverga_ctx *ctx,
		pcrd *caca, pcrd *vecinos, bool solo_validos) {
	natural vecinos_cnt = 0;

	for (huaronverga_movimientos_decendiente_directo_idx i =
	primero_huaronverga_movimientos_decendiente_directo_idx;
			i < ultimo_huaronverga_movimientos_decendiente_directo_idx; i++) {
		pcrd *vecino_act = vecinos + vecinos_cnt;
		pcrd *vecino_act_rodeado = NULL;

		huaronverga_genera_vecino_en_idx(caca, vecino_act, i);
		vecino_act_rodeado = huaronverga_genera_puto_rodeado_local(vecino_act);

		if (huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
				vecino_act_rodeado) || !solo_validos) {
			vecinos_cnt++;
		}
	}
	return vecinos_cnt;
}

static inline void huaronverga_pon_valor_en_coord_stack_byteme(
		byteme matrix[HV_MAX_FILAS + 4][HV_MAX_COLUMNAS + 4],
		pcrd *puto, byteme valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

static inline void huaronverga_pon_valor_en_coord_stack_pcrd(
		pcrd matrix[HV_MAX_FILAS][HV_MAX_COLUMNAS],
		pcrd *puto, pcrd *valor) {
	matrix[puto->coord_x][puto->coord_y] = *valor;
}

static inline void huaronverga_pon_valor_en_coord_stack_natural(
		natural matrix[HV_MAX_FILAS][HV_MAX_COLUMNAS],
		pcrd *puto, natural valor) {
	matrix[puto->coord_x][puto->coord_y] = valor;
}

typedef struct huaronverga_chosto_bfs_elem {
	pcrd puto;
	natural chosto;
} huaronverga_chosto_bfs_elem;

huaronverga_chosto_bfs_elem chostos_queue_elems[HV_MAX_ELEMENTOS_RODEADOS
		+ 1] = { HV_VALOR_INVALIDO };

natural chostos_bfs[HV_MAX_LLAVE_RODEADA + 1] = {
HV_VALOR_INVALIDO };

static inline tipo_dato huaronverga_chosto_por_busqueda_en_amplitud(
		huaronverga_ctx *ctx,
		byteme matrix[HV_MAX_FILAS + 4][HV_MAX_COLUMNAS + 4],
		pcrd *inicio, pcrd *fin,
		huaronverga_chosto_bfs_elem *chostos_queue_elems, natural *chostos,
		bitch_vector_ctx *bitch_map) {
	tipo_dato chosto = 0;
	queue_t *kha = NULL;
	bool se_encontro_fin = falso;
	natural putos_tmp_cnt = 0;
	bitch_vector_ctx *bvctx = NULL;
	pcrd *inicio_rodeado = huaronverga_genera_puto_rodeado_local(
			inicio);
	pcrd *fin_rodeado = huaronverga_genera_puto_rodeado_local(fin);
	bool todos_encontrados = falso;

	if (inicio->coord_xy == fin->coord_xy) {
		return 0;
	}

	if (!huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
			inicio_rodeado)
			|| !huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
					fin_rodeado)) {
		return HV_VALOR_INVALIDO;
	}

	bvctx = bitch_init(HV_MAX_LLAVE_RODEADA);

	kha = queue_construye(HV_MAX_FILAS * HV_MAX_COLUMNAS);

	huaronverga_chosto_bfs_elem *elem_inicial = chostos_queue_elems
			+ putos_tmp_cnt++;
	elem_inicial->puto = *inicio_rodeado;
	elem_inicial->chosto = 0;
	queue_encula(kha, elem_inicial);

	chostos[huaronverga_cc_rodeada(inicio_rodeado)] = 0;
	chostos[huaronverga_cc_rodeada(fin_rodeado)] =
	HV_VALOR_INVALIDO;

	while (!queue_vacia(kha)) {
		huaronverga_chosto_bfs_elem *act_elem = queue_decula(kha);
		pcrd *act = &act_elem->puto;
		if (act_elem->chosto >= ctx->chosto_brinco) {
			break;
		}

		for (huaronverga_movimientos_decendiente_directo_idx vecino_idx = 0;
				vecino_idx
						< ultimo_huaronverga_movimientos_decendiente_directo_idx;
				vecino_idx++) {
			huaronverga_chosto_bfs_elem *vecino_elem = chostos_queue_elems
					+ putos_tmp_cnt;
			pcrd *vecino = &vecino_elem->puto;
			tipo_dato vecino_compacto = 0;

			huaronverga_genera_vecino_en_idx(act, vecino, vecino_idx);

			vecino_compacto = huaronverga_cc_rodeada(vecino);

			if (huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
					vecino) && !bitch_checa(bvctx, vecino_compacto)) {

				putos_tmp_cnt++;
				vecino_elem->chosto = act_elem->chosto + 1;
				queue_encula(kha, vecino_elem);
				chostos[vecino_compacto] = vecino_elem->chosto;
				bitch_asigna(bvctx, vecino_compacto);
				if (vecino->coord_xy == fin_rodeado->coord_xy) {
					se_encontro_fin = verdadero;
				}
				if (bitch_map && bitch_checa(bitch_map, vecino_compacto)) {
					bitch_limpia(bitch_map, vecino_compacto);
				}
				if (bitch_map && !bitch_map->bitch_numeros_agregados_tam) {
					todos_encontrados = verdadero;
					break;
				}
			}
		}
		if (todos_encontrados) {
			break;
		}
	}

	if (se_encontro_fin) {
		chosto = chostos[huaronverga_cc_rodeada(fin_rodeado)];
	} else {
		chosto = ctx->chosto_brinco;
	}

	queue_destruye(kha);
	bitch_fini(bvctx);

	if (chosto > ctx->chosto_brinco) {
		chosto = ctx->chosto_brinco;
	}

	return chosto;
}

static inline tipo_dato huaronverga_chosto_por_busqueda_en_amplitud_brincando(
		huaronverga_ctx *ctx,
		byteme matrix[HV_MAX_FILAS + 4][HV_MAX_COLUMNAS + 4],
		pcrd *inicio, pcrd *invalido, pcrd *fin,
		huaronverga_chosto_bfs_elem *chostos_queue_elems, natural *chostos,
		bitch_vector_ctx *bitch_map) {

	tipo_dato chosto = 0;
	byteme valor_ant = 0;
	pcrd *invalido_rodeado = huaronverga_genera_puto_rodeado_local(
			invalido);
	pcrd *inicio_rodeado = huaronverga_genera_puto_rodeado_local(
			inicio);

	valor_ant = huaronverga_obten_valor_en_coord(matrix, invalido_rodeado);
	huaronverga_pon_valor_en_coord_stack_byteme(matrix, invalido_rodeado,
	HV_CARACTER_BLOQUE_BLOKEADO);

	chosto = huaronverga_chosto_por_busqueda_en_amplitud(ctx, matrix, inicio,
			fin, chostos_queue_elems, chostos, bitch_map);

	huaronverga_pon_valor_en_coord_stack_byteme(matrix, invalido_rodeado,
			valor_ant);

	return chosto;
}

static inline tipo_dato huaronverga_calcula_chosto_brinco(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	tipo_dato res = 0;
	pcrd *abuelo = hvargs->abuelo;
	pcrd *padre = hvargs->padre;
	pcrd *hijo = hvargs->hijo;
	pcrd *mas_abajo = &(pcrd ) { 0 };
	pcrd *mas_arriba = &(pcrd ) { 0 };
	pcrd vecinos[4] = { 0 };
	natural vecinos_cnt = 0;
	pcrd *padre_rodeado = huaronverga_genera_puto_rodeado_local(padre);
	pcrd *mas_abajo_rodeado = NULL;
	tipo_dato (*matrix_chosto_brinco)[final_huaronverga_caso_familiar][HV_MAX_FILAS][HV_MAX_COLUMNAS] =
			&ctx->matrix_chosto_brinco;
	huaronverga_caso_familiar cacaso = hvargs->cacaso;
	bitch_vector_ctx *bitch_map = bitch_init(HV_MAX_LLAVE_RODEADA);

	assert_timeout(abuelo->coord_xy!=padre->coord_xy);
	assert_timeout(padre->coord_xy!=hijo->coord_xy);
	assert_timeout(hijo->coord_xy!=abuelo->coord_xy);

	*mas_abajo = *hvargs->mas_abajo;
	*mas_arriba = *hvargs->mas_arriba;

	mas_abajo_rodeado = huaronverga_genera_puto_rodeado_local(mas_abajo);

	vecinos_cnt = huaronverga_genera_vecinos_validos(ctx, padre_rodeado,
			vecinos, falso);

	int i = 0;
	for (i = 0; i < vecinos_cnt; i++) {
		pcrd *vecino_act = vecinos + i;
		tipo_dato chosto_act = HV_VALOR_INVALIDO;
		tipo_dato coord_compacta = 0;

		coord_compacta = huaronverga_cc_rodeada(vecino_act);

		if (huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
				vecino_act) && vecino_act->coord_xy == padre_rodeado->coord_xy) {
			chosto_act = 0;
		} else {
			chosto_act = HV_VALOR_INVALIDO;
		}
		chostos_bfs[coord_compacta] = chosto_act;
		bitch_asigna(bitch_map, coord_compacta);
	}

	res = huaronverga_chosto_por_busqueda_en_amplitud_brincando(ctx,
			ctx->matrix_rodeada, mas_abajo, padre, mas_arriba,
			chostos_queue_elems, chostos_bfs, bitch_map);

	bitch_fini(bitch_map);

	for (huaronverga_movimientos_decendiente_directo_idx i =
	primero_huaronverga_movimientos_decendiente_directo_idx, vecinos_cnt = 0;
			i < ultimo_huaronverga_movimientos_decendiente_directo_idx;
			i++, vecinos_cnt++) {
		pcrd *vecino_act = vecinos + vecinos_cnt;
		if (huaronverga_va_pcrd_valor_matrix_rodeada(ctx,
				vecino_act)) {
			pcrd *mas_abajo_act = NULL;
			tipo_dato chosto_act = 0;

			huaronverga_caso_familiar cacaso_act =
					invalido_huaronverga_caso_familiar;

			chosto_act = chostos_bfs[huaronverga_cc_rodeada(
					vecino_act)];

			switch (cacaso) {
			case esq_1_huaronverga_caso_familiar:
			case esq_2_huaronverga_caso_familiar:
			case vertical_huaronverga_caso_familiar:
				mas_abajo_act = mas_abajo;
				switch (i) {
				case arriba_huaronverga_movimientos_decendiente_directo_idx:
					cacaso_act = vertical_huaronverga_caso_familiar;
					break;
				case izquierda_huaronverga_movimientos_decendiente_directo_idx:
					cacaso_act = esq_2_huaronverga_caso_familiar;
					break;
				case abajo_huaronverga_movimientos_decendiente_directo_idx:
					assert_timeout(
							mas_abajo->coord_xy==huaronverga_genera_puto_desrodeado_local(vecino_act)->coord_xy)
					;
					break;
				case derecha_huaronverga_movimientos_decendiente_directo_idx:
					cacaso_act = esq_1_huaronverga_caso_familiar;
					break;
				default:
					abort();
					break;
				}
				break;
			case esq_3_huaronverga_caso_familiar:
			case horizontal_huaronverga_caso_familiar:
				switch (i) {
				case arriba_huaronverga_movimientos_decendiente_directo_idx:
					mas_abajo_act = mas_abajo;
					cacaso_act = esq_3_huaronverga_caso_familiar;
					break;
				case izquierda_huaronverga_movimientos_decendiente_directo_idx:
					assert_timeout(
							mas_abajo->coord_xy==huaronverga_genera_puto_desrodeado_local(vecino_act)->coord_xy)
					;
					break;
				case abajo_huaronverga_movimientos_decendiente_directo_idx:
					mas_abajo_act = huaronverga_genera_puto_desrodeado_local(
							vecino_act);
					cacaso_act = esq_2_huaronverga_caso_familiar;
					break;
				case derecha_huaronverga_movimientos_decendiente_directo_idx:
					break;
				default:
					abort();
					break;
				}
				break;
			case esq_4_huaronverga_caso_familiar:
				switch (i) {
				case arriba_huaronverga_movimientos_decendiente_directo_idx:
					mas_abajo_act = mas_abajo;
					cacaso_act = cacaso;
					break;
				case izquierda_huaronverga_movimientos_decendiente_directo_idx:
					mas_abajo_act = huaronverga_genera_puto_desrodeado_local(
							vecino_act);
					cacaso_act = horizontal_huaronverga_caso_familiar;
					break;
				case abajo_huaronverga_movimientos_decendiente_directo_idx:
					mas_abajo_act = huaronverga_genera_puto_desrodeado_local(
							vecino_act);
					cacaso_act = esq_1_huaronverga_caso_familiar;
					break;
				case derecha_huaronverga_movimientos_decendiente_directo_idx:
					assert_timeout(
							mas_abajo->coord_xy==huaronverga_genera_puto_desrodeado_local(vecino_act)->coord_xy)
					;
					break;
				default:
					abort();
					break;
				}
				break;
			default:
				abort();
				break;
			}

			if (cacaso_act != invalido_huaronverga_caso_familiar) {
				if (chosto_act == HV_VALOR_INVALIDO
						|| chosto_act > ctx->chosto_brinco) {
					chosto_act = ctx->chosto_brinco;
				}
				(*matrix_chosto_brinco)[cacaso_act][mas_abajo_act->coord_x][mas_abajo_act->coord_y] =
						chosto_act;
			}
		}
	}

	return res;
}

static inline tipo_dato huaronverga_obten_chosto_brinco(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	huaronverga_caso_familiar cacaso = invalido_huaronverga_caso_familiar;
	pcrd *abuelo = hvargs->abuelo;
	pcrd *padre = hvargs->padre;
	pcrd *hijo = hvargs->hijo;
	pcrd *mas_abajo = NULL;
	pcrd *mas_arriba = NULL;
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

	if (res == HV_VALOR_INVALIDO) {
		res = huaronverga_calcula_chosto_brinco(ctx, hvargs);
		ctx->matrix_chosto_brinco[cacaso][mas_abajo->coord_x][mas_abajo->coord_y] =
				res;
	}

	return res;
}

static inline tipo_dato huaronverga_mejora_chosto_hijo(huaronverga_ctx *ctx,
		huaronverga_args *hvargs) {
	tipo_dato chosto = hvargs->chosto_act_hijo;
	huaronverga_caso_familiar cacaso = hvargs->cacaso;
	pcrd *abuelo = hvargs->abuelo;
	pcrd *padre = hvargs->padre;
	pcrd *hijo = hvargs->hijo;

	for (int i = 0; i < movimientos_posible_abuelo_tam[cacaso]; i++) {
		tipo_dato chosto_posible_bisabuelo_padre = 0;
		tipo_dato chosto_posible_abuelo_hijo = 0;
		tipo_dato chosto_posible_abuelo = 0;
		tipo_dato chosto_hijo = 0;
		pcrd *mov_act = &movimientos_posible_abuelo[cacaso][i];
		pcrd *posible_abuelo = &(pcrd ) { .coord_x =
				padre->coord_x + mov_act->coord_x, .coord_y =
				padre->coord_y + mov_act->coord_y };

		assert_timeout(posible_abuelo->coord_x!=HV_VALOR_INVALIDO);
		assert_timeout(posible_abuelo->coord_y!=HV_VALOR_INVALIDO);

		if (!huaronverga_va_pcrd_valor_matrix(ctx,
				posible_abuelo)) {
			continue;
		}

		chosto_posible_abuelo = huaronverga_obten_valor_en_coord(
				ctx->matrix_chosto_minimo, posible_abuelo);

		if (chosto_posible_abuelo == HV_VALOR_INVALIDO) {
			continue;
		}

		pcrd *posible_bisabuelo = &huaronverga_obten_valor_en_coord(
				ctx->matrix_predecesores, posible_abuelo);

		if (huaronverga_va_pcrd_valor_matrix(ctx,
				posible_bisabuelo) && posible_bisabuelo->coord_xy != padre->coord_xy) {
			assert_timeout(
					posible_bisabuelo->coord_y!=HV_VALOR_INVALIDO);

			chosto_posible_bisabuelo_padre = huaronverga_obten_chosto_brinco(
					ctx, &(huaronverga_args ) { .cacaso = cacaso, .abuelo =
									posible_bisabuelo, .padre = posible_abuelo,
									.hijo = padre });
			assert_timeout(
					chosto_posible_bisabuelo_padre!=HV_VALOR_INVALIDO);

			chosto_posible_bisabuelo_padre++;
		}

		chosto_posible_abuelo_hijo = huaronverga_obten_chosto_brinco(ctx,
				&(huaronverga_args ) { .cacaso = cacaso, .abuelo =
								posible_abuelo, .padre = padre, .hijo = hijo });

		assert_timeout(chosto_posible_abuelo_hijo!=HV_VALOR_INVALIDO);
		chosto_posible_abuelo_hijo++;

		chosto_hijo = chosto_posible_abuelo + chosto_posible_bisabuelo_padre
				+ chosto_posible_abuelo_hijo;

		if (chosto_hijo < chosto) {
			chosto = chosto_hijo;
		}
	}

	return chosto;
}

static inline tipo_dato huaronverga_dickstra(huaronverga_ctx *ctx,
		pcrd *cacao, pcrd *vacio, pcrd *salida) {
	pcrd (*matrix_predecesores)[HV_MAX_FILAS][HV_MAX_COLUMNAS] =
			&ctx->matrix_predecesores;
	tipo_dato (*matrix_chosto_minimo)[HV_MAX_FILAS][HV_MAX_COLUMNAS] =
			&ctx->matrix_chosto_minimo;
	pcrd vecinos_tmp[ultimo_huaronverga_movimientos_decendiente_directo_idx] =
			{ 0 };
	natural vecinos_tmp_cnt = 0;
	bool encontrada_salida = falso;
	tipo_dato chosto_final = HV_VALOR_INVALIDO;

	pcrd *putos_tmp = NULL;
	natural putos_tmp_cnt = 0;

	heap_shit *cola_prioridad = NULL;

	bitch_vector_ctx *bvctx = NULL;

	if (cacao->coord_xy == salida->coord_xy) {
		return 0;
	}

	if (!huaronverga_va_pcrd_valor_matrix(ctx, cacao)
			|| !huaronverga_va_pcrd_valor_matrix(ctx, vacio)
			|| !huaronverga_va_pcrd_valor_matrix(ctx, salida)) {
		return HV_VALOR_INVALIDO;
	}

	putos_tmp = calloc(HV_MAX_ELEMENTOS << 1, sizeof(putos_tmp));
	assert_timeout(putos_tmp);

	bvctx = bitch_init(HV_MAX_LLAVE);

	cola_prioridad = heap_shit_init(verdadero);

	for (int i = 0; i < ctx->filas_tam; i++) {
		for (int j = 0; j < ctx->columnas_tam; j++) {
			pcrd *puto_act = putos_tmp + putos_tmp_cnt++;
			puto_act->coord_y = j;
			puto_act->coord_x = i;
			if (huaronverga_obten_valor_en_coord(ctx->matrix,puto_act)==HV_CARACTER_BLOQUE_LIBRE && huaronverga_cc(puto_act)!=huaronverga_cc(cacao)) {
				heap_shit_insert(cola_prioridad, &(heap_shit_nodo ) {.prioridad =
					HV_VALOR_INVALIDO, .llave =
					huaronverga_cc(puto_act), .valor =
					puto_act});
			}
		}
	}

	bitch_asigna(bvctx, huaronverga_cc(cacao));
	huaronverga_pon_valor_en_coord_stack_natural(*matrix_chosto_minimo, cacao,
			0);

	vecinos_tmp_cnt = huaronverga_genera_vecinos_validos(ctx, cacao,
			vecinos_tmp, verdadero);

	for (int i = 0; i < vecinos_tmp_cnt; i++) {
		natural chosto_vacio_primer_mov = 0;
		pcrd *vecino_act = vecinos_tmp + i;

		if (cacao->coord_xy != vacio->coord_xy) {
			chosto_vacio_primer_mov =
					huaronverga_chosto_por_busqueda_en_amplitud_brincando(ctx,
							ctx->matrix_rodeada, vacio, cacao, vecino_act,
							chostos_queue_elems, chostos_bfs, NULL) + 1;
		} else {
			chosto_vacio_primer_mov = 2;
		}

		vecino_act = heap_shit_borrar_directo(cola_prioridad,
				huaronverga_cc(vecino_act));

		heap_shit_insert(cola_prioridad, &(heap_shit_nodo ) { .prioridad =
						chosto_vacio_primer_mov, .llave =
						huaronverga_cc(vecino_act), .valor =
						vecino_act });
		huaronverga_pon_valor_en_coord_stack_pcrd(*matrix_predecesores,
				vecino_act, cacao);
		huaronverga_pon_valor_en_coord_stack_natural(*matrix_chosto_minimo,
				vecino_act, chosto_vacio_primer_mov);
	}

	while (cola_prioridad->heap_size) {
		pcrd *padre = heap_shit_borra_torpe(cola_prioridad);
		assert_timeout(padre);
		tipo_dato chosto_padre = huaronverga_obten_valor_en_coord(
				*matrix_chosto_minimo, padre);

		if (padre->coord_xy == salida->coord_xy) {
			encontrada_salida = verdadero;
			break;
		}
		if (chosto_padre == HV_VALOR_INVALIDO) {
			break;
		}

		bitch_asigna(bvctx, huaronverga_cc(padre));

		pcrd *abuelo = &huaronverga_obten_valor_en_coord(
				*matrix_predecesores, padre);
		assert_timeout(abuelo);
		vecinos_tmp_cnt = huaronverga_genera_vecinos_validos(ctx, padre,
				vecinos_tmp, verdadero);

		for (int i = 0; i < vecinos_tmp_cnt; i++) {
			tipo_dato chosto_act = 0;
			pcrd *hijo = &(pcrd ) { 0 };
			*hijo = vecinos_tmp[i];

			if (bitch_checa(bvctx, huaronverga_cc(hijo))) {
				continue;
			}

			huaronverga_args *hvargs = &(huaronverga_args ) { .abuelo = abuelo,
							.padre = padre, .hijo = hijo };
			tipo_dato chosto_padre_hijo = huaronverga_obten_chosto_brinco(ctx,
					hvargs);

			tipo_dato chosto_hijo = chosto_padre + chosto_padre_hijo + 1;

			hvargs->chosto_act_hijo = chosto_hijo;
			chosto_hijo = huaronverga_mejora_chosto_hijo(ctx, hvargs);

			chosto_act = huaronverga_obten_valor_en_coord(*matrix_chosto_minimo,
					hijo);

			if (chosto_hijo < chosto_act) {
				huaronverga_pon_valor_en_coord_stack_pcrd(
						*matrix_predecesores, hijo, padre);
				huaronverga_pon_valor_en_coord_stack_natural(
						*matrix_chosto_minimo, hijo, chosto_hijo);
				/*
				 hijo = heap_shit_borrar_directo(cola_prioridad,
				 huaronverga_cc(hijo));
				 heap_shit_insert(cola_prioridad, &(heap_shit_nodo ) {
				 .prioridad = chosto_hijo, .llave =
				 huaronverga_cc(hijo),
				 .valor = hijo });
				 */

				heap_shit_altera_prioridad(cola_prioridad,
						huaronverga_cc(hijo), chosto_hijo);
			}
		}
	}

	if (encontrada_salida) {
		chosto_final = huaronverga_obten_valor_en_coord(
				ctx->matrix_chosto_minimo, salida);
	}

	bitch_fini(bvctx);
	heap_shit_fini(cola_prioridad);
	free(putos_tmp);

	return chosto_final;
}

static inline entero_largo_sin_signo huaronverga_compacta_caso(
		pcrd *cacao, pcrd *salida, pcrd *vacio) {
	entero_largo_sin_signo caso_compacto = 0;

	caso_compacto = huaronverga_cc(cacao);
	caso_compacto |= ((entero_largo_sin_signo) huaronverga_cc(
			salida)) << 16;
	caso_compacto |= ((entero_largo_sin_signo) huaronverga_cc(
			vacio)) << 32;

	return caso_compacto;
}

static inline void huaronverga_main() {
	huaronverga_ctx *ctx = NULL;
	natural consultas_tam = 0;
	natural filas_tam = 0;
	natural columnas_tam = 0;
	hm_rr_bs_tabla *tablon = &(hm_rr_bs_tabla ) { 0 };

	ctx = calloc(1, sizeof(huaronverga_ctx));
	assert_timeout(ctx);
	memset(ctx, HV_VALOR_INVALIDO, sizeof(huaronverga_ctx));
	memset(ctx->matrix_rodeada, HV_CARACTER_BLOQUE_BLOKEADO,
			sizeof(ctx->matrix_rodeada));
	memset(ctx->matrix, HV_CARACTER_BLOQUE_BLOKEADO,
			sizeof(ctx->matrix));
	for (huaronverga_caso_familiar i = inicial_huaronverga_caso_familiar;
			i < final_huaronverga_caso_familiar; i++) {
		for (int j = 0; j < HV_MAX_FILAS; j++) {
			for (int k = 0; k < HV_MAX_COLUMNAS; k++) {
				ctx->matrix_chosto_brinco[i][j][k] = HV_VALOR_INVALIDO;
				ctx->matrix_chosto_minimo[j][k] = HV_VALOR_INVALIDO;
				ctx->matrix_predecesores[j][k] = (pcrd ) { .coord_x =
						HV_VALOR_INVALIDO, .coord_y =
						HV_VALOR_INVALIDO };
			}
		}
	}

	scanf("%u %u %u %u\n", &filas_tam, &columnas_tam, &ctx->chosto_brinco,
			&consultas_tam);

	int i = 0;
	while (i < filas_tam) {
		int j = 0;
		while (j < columnas_tam) {
			char car_act = '\0';
			scanf("%c", &car_act);
			if (!isspace(car_act)) {
				ctx->matrix[i][j] = car_act;
				ctx->matrix_rodeada[i + 2][j + 2] = ctx->matrix[i][j];
				j++;
			}
		}
		i++;
	}
	ctx->columnas_tam = columnas_tam;
	ctx->filas_tam = filas_tam;

	hmrh_init(tablon, HEAP_SHIT_MAX_NODOS);

	for (int i = 0; i < consultas_tam; i++) {
		pcrd *cacao = &(pcrd ) { 0 };
		pcrd *vacio = &(pcrd ) { 0 };
		pcrd *salida = &(pcrd ) { 0 };
		tipo_dato resu = 0;
		hm_iter iter = 0;
		entero_largo resu_cacheado = 0;

		scanf("%u %u %u %u %u %u\n", &vacio->coord_x, &vacio->coord_y,
				&cacao->coord_x, &cacao->coord_y, &salida->coord_x,
				&salida->coord_y);
		if (!vacio->coord_x) {
			break;
		}

		vacio->coord_x--;
		vacio->coord_y--;
		cacao->coord_x--;
		cacao->coord_y--;
		salida->coord_x--;
		salida->coord_y--;

		iter = hmrh_obten(tablon,
				huaronverga_compacta_caso(cacao, salida, vacio),
				&resu_cacheado);

		if (iter != HASH_MAP_VALOR_INVALIDO) {
			resu = resu_cacheado;
		} else {
			bool nuevo = falso;
			resu = huaronverga_dickstra(ctx, cacao, vacio, salida);
			hmrh_pon(tablon,
					huaronverga_compacta_caso(cacao, salida, vacio), resu,
					&nuevo);
			assert_timeout(nuevo);
		}

		for (int j = 0; j < HV_MAX_FILAS; j++) {
			for (int k = 0; k < HV_MAX_COLUMNAS; k++) {
				ctx->matrix_chosto_minimo[j][k] =
				HV_VALOR_INVALIDO;
				ctx->matrix_predecesores[j][k] = (pcrd ) { .coord_x =
						HV_VALOR_INVALIDO, .coord_y =
						HV_VALOR_INVALIDO };
			}
		}
		if (resu == HV_VALOR_INVALIDO) {
			printf("-1\n");
		} else {
			printf("%d\n", resu);
		}
	}

	hmrh_fini(tablon);
	free(ctx);
}

int main() {
	huaronverga_main();
	return EXIT_SUCCESS;
}
