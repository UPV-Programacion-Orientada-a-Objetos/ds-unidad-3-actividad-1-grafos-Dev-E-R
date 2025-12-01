# Instrucciones de Instalación

## Requisitos Previos

- **C++ Compiler**: `g++` con soporte para C++11 o superior
- **Python**: 3.8 o superior
- **CMake**: 3.10 o superior (opcional, para compilación standalone)

## Instalación Paso a Paso

### 1. Clonar el Repositorio

```bash
git clone <URL-del-repositorio>
cd ds-unidad-3-actividad-1-grafos-Dev-E-R
```

### 2. Crear Entorno Virtual Python

```bash
python3 -m venv venv
source venv/bin/activate  # En Linux/Mac
# o en Windows: venv\Scripts\activate
```

### 3. Instalar Dependencias Python

```bash
pip install -r requirements.txt
```

### 4. Compilar el Módulo Cython

```bash
python setup.py build_ext --inplace
```

Este comando genera el archivo `grafo_wrapper.cpython-312-x86_64-linux-gnu.so` (o similar según tu sistema).

### 5. Ejecutar la Aplicación

#### Usando el script de lanzamiento:
```bash
./run_gui.sh
```

#### O manualmente:
```bash
./venv/bin/python3 python/neuronet_gui.py
```

## Verificar la Instalación

```bash
python3 -c "import sys; sys.path.insert(0, '.'); import grafo_wrapper; print('Módulo Cython cargado correctamente')"
```

## Estructura del Proyecto

```
ds-unidad-3-actividad-1-grafos-Dev-E-R/
include/              # Headers C++ (GrafoBase.h, GrafoDisperso.h)
src/                  # Código fuente C++
python/               # Código Python
- grafo_wrapper.pyx # Wrapper Cython
- neuronet_gui.py   # Interfaz gráfica
data/                 # Datasets de ejemplo
setup.py              # Script de compilación
requirements.txt      # Dependencias Python
run_gui.sh           # Script de lanzamiento
README.md            # Documentación principal
```
