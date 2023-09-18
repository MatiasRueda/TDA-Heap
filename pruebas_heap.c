#include "heap.h"
#include "testing.h"
#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <stdlib.h> // rand y RAND_MAX
#include <unistd.h> // getpid

#define TAM_ARR 7
#define VOLUMEN_PRUEBA 100000

int comparar_enteros(const void *a, const void *b) {
    int x = *(int*)a;
    int y = *(int*)b;
    if (x == y) {
        return 0;
    } else if (x < y) {
        return -1;
    } else {
        return 1;
    }
}

int comparar_enteros_null(const void *a, const void *b) {
    if(a == NULL && b == NULL) return 0;
    if(a == NULL) return 1;
    if(b == NULL) return -1;
    return comparar_enteros(a,b);
}

// Devuelve un número aleatorio entre minimo y maximo, incluyendo a minimo y maximo
int aleatorio_en_rango(int minimo, int maximo){
   return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

// Agrega enteros pedidos con malloc a un arreglo, de manera ascendente, desde 0 a n-1
void agregar_enteros(int* arr[], int n) {
    for(int i = 0; i < n; i++) {
        arr[i] = malloc(sizeof(int));
        *arr[i] = i;
    }
}

// Destruye los enteros de un arreglo que fueron pedidos con malloc
void destruir_arr(void* arr[], int n) {
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
}

// Distribuye aleatoriamente los elementos en un arreglo de int*
void desordenar_arr(int* arr[], int n) {
    srand(getpid());
    for (int i = 0; i < n; i++) {
        int indiceAleatorio = aleatorio_en_rango(0, n-1);
        int* aux = arr[i];
        arr[i] = arr[indiceAleatorio];
        arr[indiceAleatorio] = aux;
    }
}

static void prueba_crear_heap() {
    printf("PRUEBA CREAR HEAP\n");
    heap_t* heap = heap_crear(comparar_enteros);
    print_test("Prueba heap de arreglo creado correctamente", heap != NULL);
    print_test("Prueba heap no esta vacio", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
}

static void prueba_crear_heap_arr() {
    printf("PRUEBA CREAR HEAP CON ARREGLO\n");
    int* arr[TAM_ARR];
    agregar_enteros(arr, TAM_ARR);
    heap_t* heap = heap_crear_arr((void*)arr, TAM_ARR, comparar_enteros);
    print_test("Prueba heap de arreglo creado correctamente", heap != NULL);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    heap_destruir(heap, free);
}


static void prueba_heap_comportamiento() {
    printf("PRUEBA COMPORTAMIENDO DEL HEAP\n");

    heap_t* heap = heap_crear(comparar_enteros);

    int numero1 = 1;
    int numero2 = 2;
    int numero3 = 3;
    int numero10 = 10;
    int numero20 = 20;

    print_test("Prueba heap comportamiento se agrego el numero 3", heap_encolar(heap, &numero3));
    print_test("Prueba heap comportamiento hay 1 elemento en el heap", heap_cantidad(heap) == 1);
    print_test("Prueba heap comportamiento el maximo sigue siendo el numero 3", heap_ver_max(heap) == &numero3);

    print_test("Prueba heap comportamiento se agrego el numero 2", heap_encolar(heap, &numero2));
    print_test("Prueba heap comportamiento hay 2 elementos en el heap", heap_cantidad(heap) == 2);
    print_test("Prueba heap comportamiento el maximo sigue siendo el numero 3", heap_ver_max(heap) == &numero3);

    print_test("Prueba heap comportamiento se agrego el numero 1", heap_encolar(heap, &numero1));
    print_test("Prueba heap comportamiento hay 3 elementos en el heap", heap_cantidad(heap) == 3);
    print_test("Prueba heap comportamiento el maximo sigue siendo el numero 3", heap_ver_max(heap) == &numero3);

    print_test("Prueba heap comportamiento se agrego el numero 20", heap_encolar(heap, &numero20));
    print_test("Prueba heap comportamiento hay 4 elementos en el heap", heap_cantidad(heap) == 4);
    print_test("Prueba heap comportamiento el 3 dejo de ser el maximo", heap_ver_max(heap) != &numero3);
    print_test("Prueba heap comportamiento el maximo ahora es el numero 20", heap_ver_max(heap) == &numero20);

    print_test("Prueba heap comportamiento se agrego el numero 10", heap_encolar(heap, &numero10));
    print_test("Prueba heap comportamiento hay 5 elementos en el heap", heap_cantidad(heap) == 5);
    print_test("Prueba heap comportamiento el maximo sigue siendo el numero 20", heap_ver_max(heap) == &numero20);

    print_test("Prueba heap comportamiento saco el maximo que es 20", heap_ver_max(heap) == heap_desencolar(heap));
    print_test("Prueba heap comportamiento hay 4 elementos en el heap", heap_cantidad(heap) == 4);
    print_test("Prueba heap comportamiento verifico que el maximo sea 10", heap_ver_max(heap) == &numero10);

    print_test("Prueba heap comportamiento saco el maximo que es 10", heap_ver_max(heap) == heap_desencolar(heap));
    print_test("Prueba heap comportamiento hay 3 elementos en el heap", heap_cantidad(heap) == 3);
    print_test("Prueba heap comportamiento verifico que el maximo sea 3", heap_ver_max(heap) == &numero3);

    print_test("Prueba heap comportamiento saco el maximo que es 3", heap_ver_max(heap) == heap_desencolar(heap));
    print_test("Prueba heap comportamiento hay 2 elementos en el heap", heap_cantidad(heap) == 2);
    print_test("Prueba heap comportamiento verifico que el maximo sea 2", heap_ver_max(heap) == &numero2);

    print_test("Prueba heap comportamiento saco el maximo que es 2", heap_ver_max(heap) == heap_desencolar(heap));
    print_test("Prueba heap comportamiento hay 1 elemento en el heap", heap_cantidad(heap) == 1);
    print_test("Prueba heap comportamiento verifico que el maximo sea 1", heap_ver_max(heap) == &numero1);

    print_test("Prueba heap comportamiento saco el maximo que es 1", heap_ver_max(heap) == heap_desencolar(heap));
    print_test("Prueba heap comportamiento verifico que el heap ahora esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_volumen(int largo) {
    printf("PRUEBAS HEAP VOLUMEN\n");

    heap_t* heap = heap_crear(comparar_enteros);
    int* primer_valor = malloc(sizeof(int));

    int i = 0;
    bool ok = true; 
    int** orden_heap = malloc(sizeof(int*) * largo);

    *primer_valor = largo;
    heap_encolar(heap,primer_valor);
    orden_heap[0] = primer_valor;

    for (i = 1 ;i < largo; i++) {
        int* valor = malloc(sizeof(int));
        *valor = i;
        orden_heap[i] = valor;
        ok &= heap_encolar(heap,valor);
        ok &= (heap_ver_max(heap) == primer_valor);
    }

    printf("Prueba heap volumen se agregaron %i elementos ",largo);
    print_test("correctamente",ok);

    ok = true;
    i = largo-1;

    int* primer_valor_max = heap_ver_max(heap);
    ok &= (primer_valor_max == orden_heap[0]);
    ok &= (heap_desencolar(heap) == primer_valor_max);
    free(primer_valor_max);


    while (!heap_esta_vacio(heap)) {
        int* valor_maximo = heap_ver_max(heap);
        int* valor_desencolado = heap_desencolar(heap);
        ok &= (orden_heap[i] == valor_desencolado);
        ok &= (valor_desencolado == valor_maximo);
        free(valor_desencolado);
        i--;
    }

    free(orden_heap);

    printf("Prueba heap volumen se pudieron desencolar %i elementos", largo);
    print_test("correctamente", ok);
    print_test("Prueba heap volumen el heap  vacio", heap_esta_vacio(heap));
    print_test("Prueba heap volumen NO se puede desencolar en un heap en el cual se encolo y desencolo", heap_desencolar(heap) == NULL);
    print_test("Prueba heap voluemn NO se puede desencolar un heap vacio en la cual se encolo y se desencolo", heap_ver_max(heap) == NULL);
    heap_destruir(heap, NULL);
}

static void prueba_heap_vacio_desencolar_y_ver_max(void) {
    printf("PRUEBA HEAP VACIO DESENCOLAR Y VER MAX\n");
    heap_t *heap = heap_crear(comparar_enteros);
    print_test("Prueba heap vacio se pudo crea un heap", heap != NULL);
    print_test("prueba heap vacio verifico que el heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap vacio NO se puede desencolar en una heap recien creado", heap_desencolar(heap) == NULL);
    print_test("Prueba heap vacio NO se puede ver maximo en una heap recien creado", heap_ver_max(heap) == NULL);
    heap_destruir(heap, NULL);
}

static void prueba_destruir_heap_vacio_con_NULL(void) { 
    printf("PRUEBA DESTRUIR HEAP VACIO USANDO NULL\n");

    heap_t* heap = heap_crear(comparar_enteros);

    print_test("Prueba heap destruir creo el heap correctamente", heap != NULL);
    print_test("Prueba heap destruir con NULL", true);

    heap_destruir(heap, NULL);
}

static void prueba_destruir_heap_con_NULL(void) { 
    printf("PRUEBA DESTRUIR HEAP NO VACIO USANDO NULL\n");
    heap_t* heap = heap_crear(comparar_enteros);
    int numero = 1; 
    print_test("Prueba heap destruir creo el heap correctamente", heap != NULL);
    print_test("Prueba heap destruir con un dato el heap",heap_encolar(heap, &numero));
    print_test("Prueba heap destruir con NULL", true);

    heap_destruir(heap, NULL);
}

static void prueba_destruir_heap_vacio_con_FREE(void) { 
    printf("PRUEBA DESTRUIR HEAP VACIO USANDO NULL\n");

    heap_t* heap = heap_crear(comparar_enteros);

    print_test("Prueba heap destruir creo el heap correctamente", heap != NULL);
    print_test("Prueba heap destruir con FREE", true);

    heap_destruir(heap, free);
}



static void prueba_invariante_heap_arr() {
    printf("PRUEBA INVARIANTE HEAP CON ARREGLO\n");
    int* arr[TAM_ARR];
    agregar_enteros(arr, TAM_ARR);
    heap_t* heap = heap_crear_arr((void*)arr, TAM_ARR, comparar_enteros);

    for (int i = 0; i < TAM_ARR; i++) {
        char *buffer = (char*)malloc(50 * sizeof(char));
        sprintf(buffer, "Prueba ver maximo es %d", TAM_ARR-1-i);
        print_test(buffer, *(int*)heap_ver_max(heap) == TAM_ARR-1-i);
        void* dato_maximo_actual = heap_ver_max(heap);
        void* dato_desencolado = heap_desencolar(heap);
        print_test("Prueba ver maximo es el valor desencolado", dato_maximo_actual == dato_desencolado);
        free(dato_desencolado);
        free(buffer);
    }
    print_test("Prueba el heap esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_sort() {
    printf("PRUEBA ORDENAR UN ARREGLO USANDO HEAP\n");
    int* arr[TAM_ARR];
    agregar_enteros(arr, TAM_ARR);
    desordenar_arr(arr, TAM_ARR);
    heap_sort((void*)arr, TAM_ARR, comparar_enteros);
    for (int i = 0; i < TAM_ARR; i++) {
        char *buffer = (char*)malloc(50 * sizeof(char));
        sprintf(buffer, "Prueba elemento %d del arreglo es %d", i, i);
        print_test(buffer, *(int*)arr[i] == i);
        free(buffer);
    }
    destruir_arr((void*)arr, TAM_ARR);
}

static void prueba_encolar_null() {
    printf("PRUEBA ENCOLAR NULL\n");
    heap_t* heap = heap_crear(comparar_enteros_null);
    int v = 3;
    print_test("Se encolo el primer elemento", heap_encolar(heap, &v));

    print_test("Se encolo NULL", heap_encolar(heap, NULL));
    print_test("Ver max es NULL", heap_ver_max(heap) == NULL);
    print_test("Se desencolo NULL", heap_desencolar(heap) == NULL);
    print_test("Hay un solo elemento", heap_cantidad(heap) == 1);

    print_test("Volvi a encolar NULL", heap_encolar(heap, NULL));
    print_test("Se encolo un valor", heap_encolar(heap, &v));
    print_test("Ver max es NULL", heap_ver_max(heap) == NULL);
    print_test("Se desencolo NULL", heap_desencolar(heap) == NULL);
    print_test("Hay 2 elementos", heap_cantidad(heap) == 2);
    print_test("Se desencolo el valor", *(int*)heap_desencolar(heap) == v);
    print_test("Ver max es valor", *(int*)heap_ver_max(heap) == v);
    print_test("Se desencolo el valor", *(int*)heap_desencolar(heap) == v);

    print_test("No hay mas elementos", heap_cantidad(heap) == 0);
    print_test("El heap esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

void pruebas_heap_estudiante() {
    prueba_crear_heap();
    prueba_crear_heap_arr();
    prueba_heap_comportamiento();
    prueba_heap_volumen(VOLUMEN_PRUEBA);
    prueba_heap_vacio_desencolar_y_ver_max();
    prueba_destruir_heap_vacio_con_NULL();
    prueba_destruir_heap_con_NULL();
    prueba_destruir_heap_vacio_con_FREE();
    prueba_invariante_heap_arr();
    prueba_heap_sort();
    prueba_encolar_null();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_heap_estudiante();
    return 0;
}

#endif