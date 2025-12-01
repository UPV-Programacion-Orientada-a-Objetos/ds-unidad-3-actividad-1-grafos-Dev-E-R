"""
setup.py - Script de compilación para NeuroNet
Compila el código C++ con Cython y genera la extensión Python
"""

from setuptools import setup, Extension
from Cython.Build import cythonize
import os

# Rutas a los directorios
INCLUDE_DIR = os.path.join(os.path.dirname(__file__), "include")
PYTHON_DIR = os.path.join(os.path.dirname(__file__), "python")

# Configurar la extensión Cython
extensions = [
    Extension(
        name="grafo_wrapper",
        sources=[os.path.join(PYTHON_DIR, "grafo_wrapper.pyx")],
        include_dirs=[INCLUDE_DIR],
        language="c++",
        extra_compile_args=["-std=c++11", "-O3"],  # Optimización nivel 3
        extra_link_args=["-std=c++11"],
    )
]

setup(
    name="NeuroNet",
    version="1.0",
    description="Sistema híbrido C++/Python para análisis de grafos masivos",
    author="Dev-E-R",
    ext_modules=cythonize(
        extensions,
        compiler_directives={
            'language_level': "3",
            'boundscheck': False,  # Desactivar verificación de límites para mayor velocidad
            'wraparound': False,   # Desactivar índices negativos
        }
    ),
    zip_safe=False,
)
