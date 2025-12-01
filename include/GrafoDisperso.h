/**
 * @file GrafoDisperso.h
 * @brief Implementación de grafo masivo usando matriz dispersa CSR
 * @author Dev-E-R
 * @version 1.0
 * @date 2025
 */

#ifndef GRAFODISPERSO_H
#define GRAFODISPERSO_H

#include "GrafoBase.h"
#include <cstdlib>
#include <ctime>

/**
 * @class GrafoDisperso
 * @brief Grafo implementado con formato CSR (Compressed Sparse Row)
 * 
 * Esta clase implementa un grafo masivo usando representación dispersa CSR
 * para optimizar el uso de memoria. La estructura CSR usa tres vectores:
 * - values: valores de las aristas (en grafos ponderados, aquí siempre 1)
 * - column_indices: índices de columna de cada elemento
 * - row_ptr: punteros que indican dónde comienza cada fila
 * 
 * Para un grafo con millones de nodos donde la mayoría no están conectados,
 * CSR ahorra memoria al no almacenar los ceros de una matriz completa NxN.
 * 
 * @note Implementación manual sin uso de STL ni librerías de grafos
 */
class GrafoDisperso : public GrafoBase
{
private:
    // Estructura CSR (Compressed Sparse Row)
    int* values;          ///< Valores de las aristas (siempre 1 para grafos no ponderados)
    int* column_indices;  ///< Índices de columna de cada arista
    int* row_ptr;         ///< Punteros de inicio de cada fila
    
    int capacidadAristas; ///< Capacidad actual del array de aristas
    int capacidadNodos;   ///< Capacidad actual del array de nodos
    
    /**
     * @brief Redimensiona los arrays dinámicos si es necesario
     * @param nuevaCapacidadAristas Nueva capacidad para aristas
     * @param nuevaCapacidadNodos Nueva capacidad para nodos
     */
    void redimensionar(int nuevaCapacidadAristas, int nuevaCapacidadNodos);
    
    /**
     * @brief Convierte de lista de adyacencia temporal a formato CSR
     * @param listaAdyacencia Array temporal con las conexiones
     * @param contadores Array con el número de vecinos por nodo
     */
    void construirCSR(int** listaAdyacencia, int* contadores);
    
public:
    /**
     * @brief Constructor por defecto
     * 
     * Inicializa las estructuras CSR vacías.
     */
    GrafoDisperso();
    
    /**
     * @brief Destructor
     * 
     * Libera toda la memoria dinámica reservada.
     */
    ~GrafoDisperso();
    
    // Implementación de métodos virtuales
    bool cargarDatos(const char* nombreArchivo) override;
    void BFS(int nodoInicio, int profundidadMax, int*& visitados, int& cantidadVisitados) override;
    void DFS(int nodoInicio, int*& visitados, int& cantidadVisitados) override;
    int obtenerGradoEntrada(int nodo) override;
    int obtenerGradoSalida(int nodo) override;
    void getVecinos(int nodo, int*& vecinos, int& cantidadVecinos) override;
    int obtenerNodoMayorGrado(int& gradoMaximo) override;
    void imprimirEstadisticas() override;
    
    /**
     * @brief Método auxiliar para DFS recursivo
     * @param nodoActual Nodo que se está visitando
     * @param visitados Array booleano de nodos visitados
     * @param resultado Array donde se acumulan los nodos visitados
     * @param contador Contador de nodos visitados
     */
    void DFSRecursivo(int nodoActual, bool* visitados, int* resultado, int& contador);
};

/**
 * @brief Constructor - inicializa el grafo vacío
 */
GrafoDisperso::GrafoDisperso() : GrafoBase() {
    values = nullptr;
    column_indices = nullptr;
    row_ptr = nullptr;
    capacidadAristas = 0;
    capacidadNodos = 0;
    
    std::cout << "[C++ Core] Inicializando GrafoDisperso..." << std::endl;
}

/**
 * @brief Destructor - libera memoria
 */
GrafoDisperso::~GrafoDisperso() {
    if (values != nullptr) {
        delete[] values;
    }
    if (column_indices != nullptr) {
        delete[] column_indices;
    }
    if (row_ptr != nullptr) {
        delete[] row_ptr;
    }
    
    std::cout << "[C++ Core] GrafoDisperso destruido. Memoria liberada." << std::endl;
}

/**
 * @brief Carga un dataset desde archivo y construye la estructura CSR
 */
bool GrafoDisperso::cargarDatos(const char* nombreArchivo) {
    std::cout << "[C++ Core] Cargando dataset '" << nombreArchivo << "'..." << std::endl;
    
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "[ERROR] No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return false;
    }
    
    // Paso 1: Primera pasada - contar nodos y aristas
    int origen, destino;
    int nodoMaximo = -1;
    int contadorAristas = 0;
    
    while (archivo >> origen >> destino) {
        if (origen > nodoMaximo) nodoMaximo = origen;
        if (destino > nodoMaximo) nodoMaximo = destino;
        contadorAristas++;
    }
    
    numNodos = nodoMaximo + 1;
    numAristas = contadorAristas;
    
    std::cout << "[C++ Core] Primera pasada completa. Nodos detectados: " << numNodos 
              << " | Aristas: " << numAristas << std::endl;
    
    // Paso 2: Construir lista de adyacencia temporal
    // Usar arrays dinámicos para almacenar vecinos de cada nodo
    int** listaAdyacencia = new int*[numNodos];
    int* contadores = new int[numNodos];
    int* capacidades = new int[numNodos];
    
    for (int i = 0; i < numNodos; i++) {
        listaAdyacencia[i] = new int[10]; // Capacidad inicial
        contadores[i] = 0;
        capacidades[i] = 10;
    }
    
    // Segunda pasada - leer aristas y construir lista
    archivo.clear();
    archivo.seekg(0, std::ios::beg);
    
    while (archivo >> origen >> destino) {
        // Verificar si necesitamos expandir el array
        if (contadores[origen] >= capacidades[origen]) {
            int nuevaCapacidad = capacidades[origen] * 2;
            int* nuevoArray = new int[nuevaCapacidad];
            
            for (int i = 0; i < contadores[origen]; i++) {
                nuevoArray[i] = listaAdyacencia[origen][i];
            }
            
            delete[] listaAdyacencia[origen];
            listaAdyacencia[origen] = nuevoArray;
            capacidades[origen] = nuevaCapacidad;
        }
        
        listaAdyacencia[origen][contadores[origen]++] = destino;
    }
    
    archivo.close();
    
    std::cout << "[C++ Core] Lista de adyacencia temporal construida." << std::endl;
    
    // Paso 3: Convertir lista de adyacencia a formato CSR
    construirCSR(listaAdyacencia, contadores);
    
    // Liberar memoria temporal
    for (int i = 0; i < numNodos; i++) {
        delete[] listaAdyacencia[i];
    }
    delete[] listaAdyacencia;
    delete[] capacidades;
    delete[] contadores;
    
    std::cout << "[C++ Core] Carga completa. Nodos: " << numNodos 
              << " | Aristas: " << numAristas << std::endl;
    
    // Estimar memoria usada
    int memoriaBytes = (numAristas * 2 + numNodos + 1) * sizeof(int);
    float memoriaMB = memoriaBytes / (1024.0f * 1024.0f);
    std::cout << "[C++ Core] Estructura CSR construida. Memoria estimada: " 
              << memoriaMB << " MB." << std::endl;
    
    return true;
}

/**
 * @brief Construye la estructura CSR desde lista de adyacencia
 */
void GrafoDisperso::construirCSR(int** listaAdyacencia, int* contadores) {
    // Alocar memoria para CSR
    values = new int[numAristas];
    column_indices = new int[numAristas];
    row_ptr = new int[numNodos + 1];
    
    // Construir row_ptr
    row_ptr[0] = 0;
    for (int i = 0; i < numNodos; i++) {
        row_ptr[i + 1] = row_ptr[i] + contadores[i];
    }
    
    // Llenar values y column_indices
    int indice = 0;
    for (int i = 0; i < numNodos; i++) {
        for (int j = 0; j < contadores[i]; j++) {
            values[indice] = 1; // Peso de la arista (1 para grafos no ponderados)
            column_indices[indice] = listaAdyacencia[i][j];
            indice++;
        }
    }
    
    std::cout << "[C++ Core] Formato CSR construido exitosamente." << std::endl;
}

/**
 * @brief Implementación de BFS (Breadth-First Search)
 */
void GrafoDisperso::BFS(int nodoInicio, int profundidadMax, int*& visitados, int& cantidadVisitados) {
    std::cout << "[C++ Core] Ejecutando BFS nativo..." << std::endl;
    
    clock_t inicio = clock();
    
    if (nodoInicio < 0 || nodoInicio >= numNodos) {
        std::cout << "[ERROR] Nodo de inicio inválido: " << nodoInicio << std::endl;
        cantidadVisitados = 0;
        return;
    }
    
    // Crear arrays auxiliares
    bool* marcados = new bool[numNodos];
    int* distancias = new int[numNodos];
    int* cola = new int[numNodos];
    int* resultado = new int[numNodos];
    
    for (int i = 0; i < numNodos; i++) {
        marcados[i] = false;
        distancias[i] = -1;
    }
    
    // Inicializar BFS
    int frente = 0;
    int final = 0;
    int contador = 0;
    
    cola[final++] = nodoInicio;
    marcados[nodoInicio] = true;
    distancias[nodoInicio] = 0;
    resultado[contador++] = nodoInicio;
    
    // Ejecutar BFS
    while (frente < final) {
        int nodoActual = cola[frente++];
        int distanciaActual = distancias[nodoActual];
        
        // Verificar límite de profundidad
        if (profundidadMax != -1 && distanciaActual >= profundidadMax) {
            continue;
        }
        
        // Explorar vecinos usando CSR
        int inicioVecinos = row_ptr[nodoActual];
        int finVecinos = row_ptr[nodoActual + 1];
        
        for (int i = inicioVecinos; i < finVecinos; i++) {
            int vecino = column_indices[i];
            
            if (!marcados[vecino]) {
                marcados[vecino] = true;
                distancias[vecino] = distanciaActual + 1;
                cola[final++] = vecino;
                resultado[contador++] = vecino;
            }
        }
    }
    
    // Copiar resultado
    visitados = new int[contador];
    for (int i = 0; i < contador; i++) {
        visitados[i] = resultado[i];
    }
    cantidadVisitados = contador;
    
    // Liberar memoria temporal
    delete[] marcados;
    delete[] distancias;
    delete[] cola;
    delete[] resultado;
    
    clock_t fin = clock();
    double tiempoMs = ((double)(fin - inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    std::cout << "[C++ Core] Nodos encontrados: " << cantidadVisitados 
              << ". Tiempo ejecución: " << tiempoMs << "ms." << std::endl;
}

/**
 * @brief Implementación de DFS (Depth-First Search)
 */
void GrafoDisperso::DFS(int nodoInicio, int*& visitados, int& cantidadVisitados) {
    std::cout << "[C++ Core] Ejecutando DFS nativo..." << std::endl;
    
    if (nodoInicio < 0 || nodoInicio >= numNodos) {
        std::cout << "[ERROR] Nodo de inicio inválido: " << nodoInicio << std::endl;
        cantidadVisitados = 0;
        return;
    }
    
    bool* marcados = new bool[numNodos];
    int* resultado = new int[numNodos];
    
    for (int i = 0; i < numNodos; i++) {
        marcados[i] = false;
    }
    
    int contador = 0;
    DFSRecursivo(nodoInicio, marcados, resultado, contador);
    
    visitados = new int[contador];
    for (int i = 0; i < contador; i++) {
        visitados[i] = resultado[i];
    }
    cantidadVisitados = contador;
    
    delete[] marcados;
    delete[] resultado;
    
    std::cout << "[C++ Core] DFS completo. Nodos visitados: " << cantidadVisitados << std::endl;
}

/**
 * @brief Método auxiliar recursivo para DFS
 */
void GrafoDisperso::DFSRecursivo(int nodoActual, bool* visitados, int* resultado, int& contador) {
    visitados[nodoActual] = true;
    resultado[contador++] = nodoActual;
    
    // Explorar vecinos usando CSR
    int inicioVecinos = row_ptr[nodoActual];
    int finVecinos = row_ptr[nodoActual + 1];
    
    for (int i = inicioVecinos; i < finVecinos; i++) {
        int vecino = column_indices[i];
        if (!visitados[vecino]) {
            DFSRecursivo(vecino, visitados, resultado, contador);
        }
    }
}

/**
 * @brief Obtiene el grado de salida de un nodo (aristas salientes)
 */
int GrafoDisperso::obtenerGradoSalida(int nodo) {
    if (nodo < 0 || nodo >= numNodos) {
        return 0;
    }
    return row_ptr[nodo + 1] - row_ptr[nodo];
}

/**
 * @brief Obtiene el grado de entrada de un nodo (aristas entrantes)
 */
int GrafoDisperso::obtenerGradoEntrada(int nodo) {
    if (nodo < 0 || nodo >= numNodos) {
        return 0;
    }
    
    // Contar cuántas veces aparece el nodo en column_indices
    int contador = 0;
    for (int i = 0; i < numAristas; i++) {
        if (column_indices[i] == nodo) {
            contador++;
        }
    }
    return contador;
}

/**
 * @brief Obtiene los vecinos de un nodo
 */
void GrafoDisperso::getVecinos(int nodo, int*& vecinos, int& cantidadVecinos) {
    if (nodo < 0 || nodo >= numNodos) {
        cantidadVecinos = 0;
        return;
    }
    
    int inicioVecinos = row_ptr[nodo];
    int finVecinos = row_ptr[nodo + 1];
    cantidadVecinos = finVecinos - inicioVecinos;
    
    if (cantidadVecinos > 0) {
        vecinos = new int[cantidadVecinos];
        for (int i = 0; i < cantidadVecinos; i++) {
            vecinos[i] = column_indices[inicioVecinos + i];
        }
    }
}

/**
 * @brief Encuentra el nodo con mayor grado (más conectado)
 */
int GrafoDisperso::obtenerNodoMayorGrado(int& gradoMaximo) {
    std::cout << "[C++ Core] Buscando nodo con mayor grado..." << std::endl;
    
    int nodoMaximo = -1;
    gradoMaximo = 0;
    
    for (int i = 0; i < numNodos; i++) {
        int grado = obtenerGradoSalida(i);
        if (grado > gradoMaximo) {
            gradoMaximo = grado;
            nodoMaximo = i;
        }
    }
    
    std::cout << "[C++ Core] Nodo más crítico: " << nodoMaximo 
              << " con grado: " << gradoMaximo << std::endl;
    
    return nodoMaximo;
}

/**
 * @brief Imprime estadísticas del grafo
 */
void GrafoDisperso::imprimirEstadisticas() {
    std::cout << "\n=== Estadísticas del Grafo ===" << std::endl;
    std::cout << "Número de nodos: " << numNodos << std::endl;
    std::cout << "Número de aristas: " << numAristas << std::endl;
    std::cout << "Tipo: " << (esDirigido ? "Dirigido" : "No Dirigido") << std::endl;
    
    // Calcular grado promedio
    float gradoPromedio = (numNodos > 0) ? (float)numAristas / numNodos : 0;
    std::cout << "Grado promedio: " << gradoPromedio << std::endl;
    std::cout << "==============================\n" << std::endl;
}

#endif
