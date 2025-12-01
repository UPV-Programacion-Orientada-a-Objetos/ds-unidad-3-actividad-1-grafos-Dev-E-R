/**
 * @file main.cpp
 * @brief Programa principal del sistema NeuroNet
 * @author Dev-E-R
 * @version 1.0
 * @date 2025
 * 
 * Este programa implementa el núcleo C++ del sistema NeuroNet para
 * análisis de grafos masivos usando estructuras dispersas CSR.
 */

#include <iostream>
#include <cstring>
#include "GrafoBase.h"
#include "GrafoDisperso.h"

/**
 * @brief Muestra el menú de opciones al usuario
 */
void mostrarMenu();

/**
 * @brief Prueba la funcionalidad de BFS desde un nodo
 * @param grafo Puntero al grafo
 */
void probarBFS(GrafoBase* grafo);

/**
 * @brief Prueba la funcionalidad de DFS desde un nodo
 * @param grafo Puntero al grafo
 */
void probarDFS(GrafoBase* grafo);

/**
 * @brief Muestra información detallada de un nodo
 * @param grafo Puntero al grafo
 */
void analizarNodo(GrafoBase* grafo);

/**
 * @brief Función principal del programa
 * @return 0 si la ejecución fue exitosa
 * 
 * Flujo del programa:
 * 1. Inicializa el grafo disperso
 * 2. Carga un dataset desde archivo
 * 3. Ofrece un menú interactivo para análisis
 * 4. Libera recursos al finalizar
 */
int main() {
    std::cout << "=== NeuroNet: Sistema de Análisis de Grafos Masivos ===" << std::endl;
    std::cout << "========================================================\n" << std::endl;
    
    // Crear instancia del grafo disperso (polimorfismo)
    GrafoBase* grafo = new GrafoDisperso();
    
    // Solicitar archivo de dataset
    std::cout << "Ingrese la ruta del archivo dataset (ej: data/grafo_pequeno.txt):" << std::endl;
    char rutaArchivo[256];
    std::cin.getline(rutaArchivo, 256);
    
    // Cargar datos
    if (!grafo->cargarDatos(rutaArchivo)) {
        std::cout << "[ERROR] No se pudo cargar el dataset. Abortando..." << std::endl;
        delete grafo;
        return 1;
    }
    
    // Mostrar estadísticas iniciales
    grafo->imprimirEstadisticas();
    
    // Encontrar nodo más crítico
    int gradoMaximo;
    int nodoCritico = grafo->obtenerNodoMayorGrado(gradoMaximo);
    
    // Menú interactivo
    bool continuar = true;
    while (continuar) {
        mostrarMenu();
        
        int opcion;
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer
        
        switch(opcion) {
            case 1:
                grafo->imprimirEstadisticas();
                break;
                
            case 2:
                std::cout << "\nNodo más crítico (mayor grado): " << nodoCritico 
                          << " con grado: " << gradoMaximo << std::endl;
                break;
                
            case 3:
                probarBFS(grafo);
                break;
                
            case 4:
                probarDFS(grafo);
                break;
                
            case 5:
                analizarNodo(grafo);
                break;
                
            case 0:
                continuar = false;
                std::cout << "\nSaliendo del sistema..." << std::endl;
                break;
                
            default:
                std::cout << "[ERROR] Opción inválida. Intente nuevamente." << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    // Liberar memoria
    delete grafo;
    
    std::cout << "=== Sistema finalizado correctamente ===" << std::endl;
    return 0;
}

/**
 * @brief Muestra el menú de opciones
 */
void mostrarMenu() {
    std::cout << "\n--- Menú Principal ---" << std::endl;
    std::cout << "1. Mostrar estadísticas del grafo" << std::endl;
    std::cout << "2. Identificar nodo más crítico" << std::endl;
    std::cout << "3. Ejecutar BFS (Búsqueda en anchura)" << std::endl;
    std::cout << "4. Ejecutar DFS (Búsqueda en profundidad)" << std::endl;
    std::cout << "5. Analizar nodo específico" << std::endl;
    std::cout << "0. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

/**
 * @brief Prueba BFS desde un nodo dado
 */
void probarBFS(GrafoBase* grafo) {
    std::cout << "\n=== Búsqueda en Anchura (BFS) ===" << std::endl;
    
    int nodoInicio;
    std::cout << "Ingrese el nodo de inicio (0-" << grafo->getNumNodos()-1 << "): ";
    std::cin >> nodoInicio;
    
    int profundidadMax;
    std::cout << "Ingrese la profundidad máxima (-1 para sin límite): ";
    std::cin >> profundidadMax;
    
    int* visitados = nullptr;
    int cantidadVisitados = 0;
    
    grafo->BFS(nodoInicio, profundidadMax, visitados, cantidadVisitados);
    
    std::cout << "\nNodos alcanzados desde " << nodoInicio 
              << " (Total: " << cantidadVisitados << "):" << std::endl;
    
    // Mostrar primeros nodos (máximo 20 para no saturar consola)
    int limite = (cantidadVisitados > 20) ? 20 : cantidadVisitados;
    for (int i = 0; i < limite; i++) {
        std::cout << visitados[i];
        if (i < limite - 1) std::cout << ", ";
    }
    
    if (cantidadVisitados > 20) {
        std::cout << " ... (y " << (cantidadVisitados - 20) << " más)";
    }
    std::cout << std::endl;
    
    // Liberar memoria
    if (visitados != nullptr) {
        delete[] visitados;
    }
}

/**
 * @brief Prueba DFS desde un nodo dado
 */
void probarDFS(GrafoBase* grafo) {
    std::cout << "\n=== Búsqueda en Profundidad (DFS) ===" << std::endl;
    
    int nodoInicio;
    std::cout << "Ingrese el nodo de inicio (0-" << grafo->getNumNodos()-1 << "): ";
    std::cin >> nodoInicio;
    
    int* visitados = nullptr;
    int cantidadVisitados = 0;
    
    grafo->DFS(nodoInicio, visitados, cantidadVisitados);
    
    std::cout << "\nNodos alcanzados desde " << nodoInicio 
              << " (Total: " << cantidadVisitados << "):" << std::endl;
    
    // Mostrar primeros nodos (máximo 20)
    int limite = (cantidadVisitados > 20) ? 20 : cantidadVisitados;
    for (int i = 0; i < limite; i++) {
        std::cout << visitados[i];
        if (i < limite - 1) std::cout << ", ";
    }
    
    if (cantidadVisitados > 20) {
        std::cout << " ... (y " << (cantidadVisitados - 20) << " más)";
    }
    std::cout << std::endl;
    
    // Liberar memoria
    if (visitados != nullptr) {
        delete[] visitados;
    }
}

/**
 * @brief Analiza un nodo específico mostrando su información
 */
void analizarNodo(GrafoBase* grafo) {
    std::cout << "\n=== Análisis de Nodo ===" << std::endl;
    
    int nodo;
    std::cout << "Ingrese el ID del nodo a analizar (0-" << grafo->getNumNodos()-1 << "): ";
    std::cin >> nodo;
    
    if (nodo < 0 || nodo >= grafo->getNumNodos()) {
        std::cout << "[ERROR] Nodo inválido." << std::endl;
        return;
    }
    
    std::cout << "\nInformación del Nodo " << nodo << ":" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    
    // Grado de salida
    int gradoSalida = grafo->obtenerGradoSalida(nodo);
    std::cout << "Grado de salida (aristas salientes): " << gradoSalida << std::endl;
    
    // Grado de entrada
    int gradoEntrada = grafo->obtenerGradoEntrada(nodo);
    std::cout << "Grado de entrada (aristas entrantes): " << gradoEntrada << std::endl;
    
    // Grado total
    int gradoTotal = gradoSalida + gradoEntrada;
    std::cout << "Grado total: " << gradoTotal << std::endl;
    
    // Vecinos
    int* vecinos = nullptr;
    int cantidadVecinos = 0;
    grafo->getVecinos(nodo, vecinos, cantidadVecinos);
    
    std::cout << "\nVecinos directos (nodos conectados): " << cantidadVecinos << std::endl;
    
    if (cantidadVecinos > 0) {
        std::cout << "IDs de vecinos: ";
        int limite = (cantidadVecinos > 15) ? 15 : cantidadVecinos;
        for (int i = 0; i < limite; i++) {
            std::cout << vecinos[i];
            if (i < limite - 1) std::cout << ", ";
        }
        if (cantidadVecinos > 15) {
            std::cout << " ... (y " << (cantidadVecinos - 15) << " más)";
        }
        std::cout << std::endl;
        
        delete[] vecinos;
    }
}
