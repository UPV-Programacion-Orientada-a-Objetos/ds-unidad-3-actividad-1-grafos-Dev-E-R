/**
 * @file GrafoBase.h
 * @brief Clase abstracta base para implementaciones de grafos masivos
 * @author Dev-E-R
 * @version 1.0
 * @date 2025
 */

#ifndef GRAFOBASE_H
#define GRAFOBASE_H

#include <iostream>
#include <fstream>
#include <cstring>

/**
 * @class GrafoBase
 * @brief Interfaz abstracta para estructuras de grafos de alto rendimiento
 * 
 * Esta clase define la interfaz que toda implementación de grafo debe cumplir.
 * Utiliza métodos virtuales puros para forzar la implementación en clases derivadas.
 * 
 * @note Prohibido el uso de librerías de grafos como boost::graph
 */
class GrafoBase
{
protected:
    int numNodos;       ///< Número total de nodos en el grafo
    int numAristas;     ///< Número total de aristas en el grafo
    bool esDirigido;    ///< Indica si el grafo es dirigido o no dirigido
    
public:
    /**
     * @brief Constructor por defecto
     * 
     * Inicializa el grafo con valores por defecto.
     */
    GrafoBase() : numNodos(0), numAristas(0), esDirigido(true) {}
    
    /**
     * @brief Destructor virtual para permitir polimorfismo correcto
     */
    virtual ~GrafoBase() {}
    
    /**
     * @brief Carga un dataset desde un archivo formato Edge List
     * @param nombreArchivo Ruta al archivo a cargar
     * @return true si la carga fue exitosa, false en caso contrario
     * 
     * Formato esperado: cada línea contiene "nodoOrigen nodoDestino"
     * Ejemplo: "0 11342"
     */
    virtual bool cargarDatos(const char* nombreArchivo) = 0;
    
    /**
     * @brief Realiza una búsqueda en anchura (BFS) desde un nodo inicial
     * @param nodoInicio ID del nodo desde donde iniciar el recorrido
     * @param profundidadMax Profundidad máxima del recorrido (-1 = sin límite)
     * @param visitados Array dinámico donde se almacenarán los nodos visitados
     * @param cantidadVisitados Número de nodos visitados (output)
     * 
     * El array visitados debe ser liberado por el llamador
     */
    virtual void BFS(int nodoInicio, int profundidadMax, int*& visitados, int& cantidadVisitados) = 0;
    
    /**
     * @brief Realiza una búsqueda en profundidad (DFS) desde un nodo inicial
     * @param nodoInicio ID del nodo desde donde iniciar el recorrido
     * @param visitados Array dinámico donde se almacenarán los nodos visitados
     * @param cantidadVisitados Número de nodos visitados (output)
     * 
     * Útil para detección de ciclos y componentes conexos
     */
    virtual void DFS(int nodoInicio, int*& visitados, int& cantidadVisitados) = 0;
    
    /**
     * @brief Obtiene el grado de entrada de un nodo
     * @param nodo ID del nodo
     * @return Número de aristas que llegan al nodo
     */
    virtual int obtenerGradoEntrada(int nodo) = 0;
    
    /**
     * @brief Obtiene el grado de salida de un nodo
     * @param nodo ID del nodo
     * @return Número de aristas que salen del nodo
     */
    virtual int obtenerGradoSalida(int nodo) = 0;
    
    /**
     * @brief Obtiene los vecinos (nodos adyacentes) de un nodo dado
     * @param nodo ID del nodo
     * @param vecinos Array dinámico con los IDs de los vecinos (output)
     * @param cantidadVecinos Número de vecinos encontrados (output)
     * 
     * El array vecinos debe ser liberado por el llamador
     */
    virtual void getVecinos(int nodo, int*& vecinos, int& cantidadVecinos) = 0;
    
    /**
     * @brief Encuentra el nodo con mayor grado (más crítico)
     * @param gradoMaximo Grado del nodo más conectado (output)
     * @return ID del nodo con mayor grado
     */
    virtual int obtenerNodoMayorGrado(int& gradoMaximo) = 0;
    
    /**
     * @brief Obtiene el número total de nodos del grafo
     * @return Número de nodos
     */
    virtual int getNumNodos() const { return numNodos; }
    
    /**
     * @brief Obtiene el número total de aristas del grafo
     * @return Número de aristas
     */
    virtual int getNumAristas() const { return numAristas; }
    
    /**
     * @brief Verifica si el grafo es dirigido
     * @return true si es dirigido, false si no lo es
     */
    virtual bool esGrafoDirigido() const { return esDirigido; }
    
    /**
     * @brief Imprime estadísticas básicas del grafo
     */
    virtual void imprimirEstadisticas() = 0;
};

#endif
