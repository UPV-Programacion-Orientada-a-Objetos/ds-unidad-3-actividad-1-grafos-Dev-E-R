#!/bin/bash
# Script para ejecutar NeuroNet GUI
# Asegura que el módulo Cython compilado pueda ser encontrado

# Cambiar al directorio del proyecto
cd "$(dirname "$0")"

# Ejecutar el GUI con el Python del entorno virtual
./venv/bin/python3 python/neuronet_gui.py
