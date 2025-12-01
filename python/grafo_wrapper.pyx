# distutils: language = c++
# cython: language_level = 3

"""
Wrapper Cython para el núcleo C++ de NeuroNet
Expone la funcionalidad del grafo disperso a Python
"""

from libcpp cimport bool
from libcpp.string cimport string

# Declaración de la clase C++ GrafoBase
cdef extern from "../include/GrafoBase.h":
    cdef cppclass GrafoBase:
        GrafoBase() except +
        bool cargarDatos(const char* nombreArchivo)
        void BFS(int nodoInicio, int profundidadMax, int*& visitados, int& cantidadVisitados)
        void DFS(int nodoInicio, int*& visitados, int& cantidadVisitados)
        int obtenerGradoEntrada(int nodo)
        int obtenerGradoSalida(int nodo)
        void getVecinos(int nodo, int*& vecinos, int& cantidadVecinos)
        int obtenerNodoMayorGrado(int& gradoMaximo)
        int getNumNodos()
        int getNumAristas()
        bool esGrafoDirigido()
        void imprimirEstadisticas()

# Declaración de la clase C++ GrafoDisperso
cdef extern from "../include/GrafoDisperso.h":
    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso() except +

# Clase Python que envuelve el grafo C++
cdef class PyGrafoDisperso:
    """
    Wrapper Python para la clase GrafoDisperso de C++
    
    Esta clase permite usar el grafo disperso optimizado de C++
    desde Python, manejando la conversión de tipos automáticamente.
    """
    cdef GrafoDisperso* c_grafo  # Puntero al objeto C++
    
    def __cinit__(self):
        """Constructor - crea la instancia C++"""
        self.c_grafo = new GrafoDisperso()
        print("[Cython] Instancia de GrafoDisperso creada")
    
    def __dealloc__(self):
        """Destructor - libera memoria C++"""
        del self.c_grafo
        print("[Cython] Instancia de GrafoDisperso destruida")
    
    def cargar_datos(self, str ruta_archivo):
        """
        Carga un dataset desde archivo
        
        Args:
            ruta_archivo: Ruta al archivo con formato edge list
            
        Returns:
            bool: True si la carga fue exitosa
        """
        cdef bytes py_bytes = ruta_archivo.encode('utf-8')
        cdef const char* c_ruta = py_bytes
        return self.c_grafo.cargarDatos(c_ruta)
    
    def bfs(self, int nodo_inicio, int profundidad_max=-1):
        """
        Ejecuta búsqueda en anchura (BFS) desde un nodo
        
        Args:
            nodo_inicio: ID del nodo inicial
            profundidad_max: Profundidad máxima (-1 = sin límite)
            
        Returns:
            list: Lista de nodos visitados
        """
        print(f"[Cython] Solicitud recibida: BFS desde Nodo {nodo_inicio}, Profundidad {profundidad_max}.")
        
        cdef int* visitados = NULL
        cdef int cantidad = 0
        
        self.c_grafo.BFS(nodo_inicio, profundidad_max, visitados, cantidad)
        
        # Convertir array C++ a lista Python
        resultado = []
        if visitados != NULL and cantidad > 0:
            for i in range(cantidad):
                resultado.append(visitados[i])
            # La memoria será liberada por C++
        
        print(f"[Cython] Retornando lista de {cantidad} nodos a Python.")
        return resultado
    
    def dfs(self, int nodo_inicio):
        """
        Ejecuta búsqueda en profundidad (DFS) desde un nodo
        
        Args:
            nodo_inicio: ID del nodo inicial
            
        Returns:
            list: Lista de nodos visitados
        """
        print(f"[Cython] Solicitud recibida: DFS desde Nodo {nodo_inicio}.")
        
        cdef int* visitados = NULL
        cdef int cantidad = 0
        
        self.c_grafo.DFS(nodo_inicio, visitados, cantidad)
        
        # Convertir array C++ a lista Python
        resultado = []
        if visitados != NULL and cantidad > 0:
            for i in range(cantidad):
                resultado.append(visitados[i])
        
        print(f"[Cython] Retornando lista de {cantidad} nodos a Python.")
        return resultado
    
    def obtener_grado_entrada(self, int nodo):
        """
        Obtiene el grado de entrada de un nodo
        
        Args:
            nodo: ID del nodo
            
        Returns:
            int: Grado de entrada
        """
        return self.c_grafo.obtenerGradoEntrada(nodo)
    
    def obtener_grado_salida(self, int nodo):
        """
        Obtiene el grado de salida de un nodo
        
        Args:
            nodo: ID del nodo
            
        Returns:
            int: Grado de salida
        """
        return self.c_grafo.obtenerGradoSalida(nodo)
    
    def get_vecinos(self, int nodo):
        """
        Obtiene los vecinos de un nodo
        
        Args:
            nodo: ID del nodo
            
        Returns:
            list: Lista de IDs de vecinos
        """
        cdef int* vecinos = NULL
        cdef int cantidad = 0
        
        self.c_grafo.getVecinos(nodo, vecinos, cantidad)
        
        resultado = []
        if vecinos != NULL and cantidad > 0:
            for i in range(cantidad):
                resultado.append(vecinos[i])
        
        return resultado
    
    def obtener_nodo_mayor_grado(self):
        """
        Encuentra el nodo con mayor grado
        
        Returns:
            tuple: (id_nodo, grado)
        """
        cdef int grado_max = 0
        cdef int nodo = self.c_grafo.obtenerNodoMayorGrado(grado_max)
        return (nodo, grado_max)
    
    def get_num_nodos(self):
        """Retorna el número de nodos"""
        return self.c_grafo.getNumNodos()
    
    def get_num_aristas(self):
        """Retorna el número de aristas"""
        return self.c_grafo.getNumAristas()
    
    def es_dirigido(self):
        """Verifica si el grafo es dirigido"""
        return self.c_grafo.esGrafoDirigido()
    
    def imprimir_estadisticas(self):
        """Imprime estadísticas del grafo"""
        self.c_grafo.imprimirEstadisticas()
