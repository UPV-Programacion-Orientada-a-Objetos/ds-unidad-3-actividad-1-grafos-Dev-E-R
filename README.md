[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/KRv8GX-k)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21883322)
# Caso de Estudio: "NeuroNet: Análisis y Visualización de Propagación en Redes Masivas"

## Definición del Problema a Resolver

La organización de investigación "Global Connectivity Watch" necesita una herramienta de alto rendimiento para analizar la robustez de redes de comunicación masivas (Internet, redes sociales o redes eléctricas) ante fallos en cascada o propagación de malware.

Las herramientas actuales basadas puramente en Python (como NetworkX) son demasiado lentas para procesar grafos de millones de nodos en tiempo real, y las herramientas en C++ puro carecen de capacidades de visualización ágiles.


Por lo tanto se necesita desarrollar un sistema híbrido llamado **NeuroNet**. El núcleo del sistema (backend) debe ser un motor de procesamiento de grafos escrito en **C++** altamente optimizado, que implemente **matrices poco densas (Sparse Matrices)** para la gestión de memoria. Este núcleo debe exponerse mediante **Cython** a una interfaz gráfica en **Python**, la cual utilizará librerías como `NetworkX` o `Matplotlib` únicamente para visualizar sub-grafos o resultados específicos, pero **nunca** para el procesamiento de datos.

El sistema debe cargar un dataset masivo (Benchmark del proyecto SNAP de Stanford), almacenarlo eficientemente utilizando estructuras comprimidas (CSR - Compressed Sparse Row) hechas a mano, y permitir al usuario realizar análisis de centralidad y simulaciones de recorrido desde una interfaz visual.

-----

## Temas Relacionados y Necesarios

Para resolver este problema, se deberá utilizar e integrar los siguientes conceptos:

| Tema Principal | Concepto a Aplicar |
| :--- | :--- |
| **Grafos Masivos** | Manejo de millones de nodos y aristas. Diferencia entre grafos dirigidos y no dirigidos. |
| **Matrices Poco Densas** | Implementación manual del formato **CSR (Compressed Sparse Row)** o **COO (Coordinate List)** para representar la Matriz de Adyacencia sin desperdiciar memoria RAM. |
| **POO Avanzada en C++** | Uso de **Clases Abstractas** para definir la interfaz del Grafo (`IGraph`), y herencia para implementaciones específicas (`SparseGraph`). |
| **Recorridos de Grafos** | Implementación "a mano" de **BFS** (Búsqueda en Anchura) para encontrar el camino más corto y **DFS** (Profundidad) para detección de ciclos o componentes conexos. |
| **Cython** | Creación de archivos `.pxd` y `.pyx` para compilar código C++ como un módulo importable en Python. Interoperabilidad de punteros y memoria. |
| **Python GUI** | Uso de `Tkinter`, `PyQt` o `Streamlit` para invocar las funciones de C++ y visualizar los resultados. |

-----

## Definición y Detalles del Proceso a Desarrollar

### A. Arquitectura del Sistema

El sistema constará de tres capas:

1. **Capa Núcleo (C++):**
      * **Clase Abstracta `GrafoBase`:** Define métodos virtuales puros como `cargarDatos()`, `BFS()`, `obtenerGrado()`, `getVecinos()`.
      * **Clase Concreta `GrafoDisperso`:** Hereda de `GrafoBase`. Implementa la matriz de adyacencia utilizando vectores dinámicos (punteros) para simular el formato **CSR**. *No se permite una matriz de adyacencia clásica $N \times N$ debido al consumo de memoria.*
2. **Capa de Enlace (Cython):**
      * Un "Wrapper" que instancia la clase de C++, convierte los tipos de datos de Python a C++ y viceversa, y expone los métodos de cálculo.
3. **Capa de Presentación (Python):**
      * Interfaz Gráfica de Usuario (GUI) que permite seleccionar el archivo dataset, ejecutar algoritmos y visualizar los nodos críticos usando `NetworkX` (solo para dibujar el resultado final, no para calcular).

### B. Flujo de Trabajo

1. **Ingesta de Datos:** El usuario carga un archivo del repositorio **SNAP** (ej. [`web-Google.txt`](https://snap.stanford.edu/data/web-Google.html) o [`amazon0601.txt`](https://snap.stanford.edu/data/amazon0601.html)). El motor C++ lee el archivo línea por línea y construye la estructura CSR.
2. **Análisis Topológico:** El usuario solicita "Identificar el Nodo más crítico (Mayor Grado)". C++ recorre la estructura dispersa y retorna el ID del nodo.
3. **Simulación de Recorrido:** El usuario selecciona un nodo de inicio y una profundidad máxima. El motor C++ ejecuta un **BFS** "a mano" y retorna la lista de nodos visitados y sus aristas.
4. **Visualización:** Python recibe la lista de nodos/aristas del subgrafo resultante y lo dibuja en pantalla.

-----

## Requerimientos Funcionales y No Funcionales

### Requisitos Funcionales

1.  **Carga de Benchmarks:** El sistema debe leer archivos de texto plano formato "Edge List" (NodoOrigen NodoDestino).
2.  **Representación Dispersa:** Implementar manualmente la Matriz de Adyacencia usando formato CSR (3 vectores: valores, índices de columnas, punteros de fila) o Listas de Adyacencia optimizadas.
3.  **Algoritmos Nativos:**
      * **Grado de Nodos:** Calcular qué nodo tiene más conexiones (entrada/salida).
      * **BFS (Breadth-First Search):** Para encontrar la distancia entre dos nodos seleccionados.
4.  **Interfaz Gráfica:**
      * Botón para cargar archivo.
      * Visualización de métricas (Tiempo de carga, Memoria usada, Nodo con mayor grado).
      * Canvas para dibujar el subgrafo resultante de una búsqueda.
5.  **Interoperabilidad:** El cálculo pesado debe ocurrir en C++, la visualización en Python.

### Requisitos No Funcionales

1.  **Eficiencia de Memoria:** El sistema debe ser capaz de cargar un grafo de al menos **500,000 nodos** sin desbordar la memoria de una laptop estándar (8-16GB RAM), gracias al uso de matrices dispersas.
2.  **Prohibición de Librerías de Grafo en el Núcleo:** No se puede usar `boost::graph` en C++ ni `networkx` para los algoritmos de búsqueda. Todo algoritmo debe ser lógica de punteros propia.
3.  **Arquitectura Orientada a Objetos:** Uso estricto de herencia y polimorfismo en el código C++.
4.  **Compilación Híbrida:** Se debe entregar un `setup.py` que compile el código C++ y genere la extensión de Python.

-----

## Ejemplo de Entradas y Salidas en Consola (Backend Debugging)

Aunque tendrá GUI, el backend debe mostrar logs de operación:

**Entrada (Archivo `web-Google.txt` - Fragmento):**

```text
0 11342
0 8754
0 456
1 0
1 456
... (800,000 líneas más)
```

**Salida (Consola de C++ invocada desde Python):**

```text
[C++ Core] Inicializando GrafoDisperso...
[C++ Core] Cargando dataset 'web-Google.txt'...
[C++ Core] Carga completa. Nodos: 875,713 | Aristas: 5,105,039
[C++ Core] Estructura CSR construida. Memoria estimada: 45 MB.

[Cython] Solicitud recibida: BFS desde Nodo 0, Profundidad 2.
[C++ Core] Ejecutando BFS nativo...
[C++ Core] Nodos encontrados: 25. Tiempo ejecución: 0.004ms.
[Cython] Retornando lista de adyacencia local a Python.
```

**Salida (Interfaz Gráfica - Visual):**

  * Se muestra un gráfico de "estrella" donde el nodo 0 está en el centro, conectado a sus vecinos directos, y estos a los suyos (nivel 2). .

-----

## Temas Adicionales de Investigación Necesarios

Para resolver este problema se deberá investigar:

1.  **Formato CSR (Compressed Sparse Row):** Entender cómo comprimir una matriz llena de ceros en tres vectores unidimensionales (`values`, `column_indices`, `row_ptr`) para ahorrar memoria.
2.  **Wrappers en Cython (`cdef`, `cpdef`):** Cómo declarar clases de C++ dentro de archivos `.pyx` para que Python pueda instanciarlas como objetos.
3.  **Gestión de Memoria Python vs C++:** Entender quién es dueño de la memoria cuando se pasa un arreglo de C++ a Python (uso de `memoryviews` o conversión a listas de Python).
4.  **Datasets SNAP:** Estructura y características de los datasets de Stanford (Large Network Dataset Collection).
5.  **Bibliotecas de Visualización de Grafos en Python:** Cómo usar `networkx` solo como motor de dibujo (`nx.draw`) recibiendo una lista de aristas crudas, o librerías más avanzadas como `PyVis` para visualización interactiva web.

---

## 🚀 IMPLEMENTACIÓN COMPLETADA

### ✅ Sistema NeuroNet Desarrollado

Este repositorio contiene la implementación completa del sistema **NeuroNet** según las especificaciones del README.

#### Estructura del Proyecto

```
ds-unidad-3-actividad-1-grafos-Dev-E-R/
│
├── include/                    # Cabeceras C++
│   ├── GrafoBase.h            # Clase abstracta (interfaz)
│   └── GrafoDisperso.h        # Implementación CSR
│
├── src/                        # Código fuente
│   └── main.cpp               # Programa principal C++
│
├── python/                     # Capa Python
│   ├── grafo_wrapper.pyx      # Wrapper Cython
│   └── neuronet_gui.py        # Interfaz gráfica (Tkinter)
│
├── data/                       # Datasets de prueba
│   ├── grafo_pequeno.txt      # 16 nodos
│   └── grafo_mediano.txt      # 50 nodos
│
├── CMakeLists.txt             # Configuración CMake
├── setup.py                   # Compilación Cython
├── requirements.txt           # Dependencias Python
├── MANUAL_TECNICO.md          # Documentación completa
└── INICIO_RAPIDO.md           # Guía rápida de uso
```

#### Características Implementadas

✅ **Capa Núcleo C++**
- Clase abstracta `GrafoBase` con métodos virtuales puros
- Clase concreta `GrafoDisperso` con formato CSR
- BFS y DFS implementados manualmente (sin STL de grafos)
- Cálculo de grado de nodos y análisis de centralidad
- Logs detallados de operación

✅ **Capa de Enlace Cython**
- Wrapper completo `grafo_wrapper.pyx`
- Conversión automática de tipos C++ ↔ Python
- Gestión de memoria segura

✅ **Capa de Presentación Python**
- Interfaz gráfica con Tkinter
- Visualización con NetworkX (solo para dibujo)
- Métricas en tiempo real
- Análisis interactivo de nodos

✅ **Requisitos Cumplidos**
- ✅ Formato CSR implementado manualmente
- ✅ Sin uso de boost::graph
- ✅ Arquitectura orientada a objetos (herencia y polimorfismo)
- ✅ BFS/DFS implementados desde cero
- ✅ Gestión eficiente de memoria
- ✅ Interfaz gráfica funcional

#### Compilación e Instalación

**1. Instalar dependencias:**
```bash
# Compilador C++
sudo apt-get install build-essential cmake g++ python3-dev

# Paquetes Python
pip install -r requirements.txt
```

**2. Compilar núcleo C++:**
```bash
mkdir build && cd build
cmake ..
make
```

**3. Compilar extensión Cython:**
```bash
python setup.py build_ext --inplace
```

#### Uso del Sistema

**Interfaz Gráfica (Recomendado):**
```bash
cd python
python neuronet_gui.py
```

**Programa de Consola:**
```bash
cd build
./neuronet
# Ingresar ruta: ../data/grafo_pequeno.txt
```

**Uso desde Python:**
```python
import grafo_wrapper

grafo = grafo_wrapper.PyGrafoDisperso()
grafo.cargar_datos("data/grafo_pequeno.txt")
visitados = grafo.bfs(0, 2)
print(f"Nodos alcanzados: {len(visitados)}")
```

#### Documentación

- **`MANUAL_TECNICO.md`**: Documentación completa con detalles de implementación
- **`INICIO_RAPIDO.md`**: Guía rápida de inicio
- **Este README**: Especificaciones del proyecto

#### Datasets de Prueba

Los archivos en `data/` siguen el formato Edge List de SNAP:
```
nodoOrigen nodoDestino
0 1
0 2
1 3
...
```

Para datasets masivos reales, visitar: https://snap.stanford.edu/data/

#### Autor

**Dev-E-R**  
Implementación siguiendo estilo de código propio  
UPV - Programación Orientada a Objetos  
2025

---

### 📊 Ejemplo de Salida del Sistema

```
=== NeuroNet: Sistema de Análisis de Grafos Masivos ===
[C++ Core] Inicializando GrafoDisperso...
[C++ Core] Cargando dataset 'grafo_pequeno.txt'...
[C++ Core] Carga completa. Nodos: 16 | Aristas: 27
[C++ Core] Estructura CSR construida. Memoria estimada: 0.0003 MB.

[Cython] Solicitud recibida: BFS desde Nodo 0, Profundidad 2.
[C++ Core] Ejecutando BFS nativo...
[C++ Core] Nodos encontrados: 8. Tiempo ejecución: 0.001ms.
[Cython] Retornando lista de adyacencia local a Python.

Nodos alcanzados: [0, 1, 2, 3, 4, 5, 6, 7]
```

---