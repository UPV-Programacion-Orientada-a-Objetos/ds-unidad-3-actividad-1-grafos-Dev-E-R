"""
neuronet_gui.py - Interfaz Gráfica para NeuroNet
Visualización y análisis de grafos masivos usando Tkinter
"""

import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import time
import sys
import os

# Añadir el directorio raíz del proyecto al path para importar el módulo compilado
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if PROJECT_ROOT not in sys.path:
    sys.path.insert(0, PROJECT_ROOT)

# Intentar importar el módulo compilado
try:
    import grafo_wrapper
    CYTHON_AVAILABLE = True
except ImportError:
    CYTHON_AVAILABLE = False
    print("ADVERTENCIA: Módulo Cython no encontrado. Compile primero con 'python setup.py build_ext --inplace'")

# Importar NetworkX solo para visualización
try:
    import networkx as nx
    import matplotlib.pyplot as plt
    from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
    NETWORKX_AVAILABLE = True
except ImportError:
    NETWORKX_AVAILABLE = False
    print("ADVERTENCIA: NetworkX/Matplotlib no disponibles. Visualización deshabilitada.")


class NeuroNetGUI:
    """
    Interfaz gráfica principal de NeuroNet
    
    Permite cargar datasets, ejecutar análisis y visualizar resultados
    usando el núcleo C++ optimizado vía Cython.
    """
    
    def __init__(self, root):
        self.root = root
        self.root.title("NeuroNet - Análisis de Grafos Masivos")
        self.root.geometry("1000x700")
        
        self.grafo = None
        self.archivo_cargado = None
        self.tiempo_carga = 0
        
        self.crear_interfaz()
        
        if not CYTHON_AVAILABLE:
            messagebox.showwarning(
                "Módulo no compilado",
                "El módulo C++ no está compilado.\n\n"
                "Ejecute: python setup.py build_ext --inplace"
            )
    
    def crear_interfaz(self):
        """Crea todos los widgets de la interfaz"""
        
        # Frame superior - Carga de archivo
        frame_carga = ttk.LabelFrame(self.root, text="Carga de Dataset", padding=10)
        frame_carga.pack(fill=tk.X, padx=10, pady=5)
        
        ttk.Label(frame_carga, text="Archivo:").grid(row=0, column=0, sticky=tk.W)
        self.entry_archivo = ttk.Entry(frame_carga, width=50)
        self.entry_archivo.grid(row=0, column=1, padx=5)
        
        ttk.Button(frame_carga, text="Examinar...", command=self.seleccionar_archivo).grid(row=0, column=2, padx=5)
        ttk.Button(frame_carga, text="Cargar Dataset", command=self.cargar_dataset).grid(row=0, column=3, padx=5)
        
        # Frame de estadísticas
        frame_stats = ttk.LabelFrame(self.root, text="Estadísticas del Grafo", padding=10)
        frame_stats.pack(fill=tk.X, padx=10, pady=5)
        
        self.label_nodos = ttk.Label(frame_stats, text="Nodos: -")
        self.label_nodos.grid(row=0, column=0, padx=10, sticky=tk.W)
        
        self.label_aristas = ttk.Label(frame_stats, text="Aristas: -")
        self.label_aristas.grid(row=0, column=1, padx=10, sticky=tk.W)
        
        self.label_tiempo = ttk.Label(frame_stats, text="Tiempo de carga: -")
        self.label_tiempo.grid(row=0, column=2, padx=10, sticky=tk.W)
        
        self.label_memoria = ttk.Label(frame_stats, text="Memoria estimada: -")
        self.label_memoria.grid(row=0, column=3, padx=10, sticky=tk.W)
        
        self.label_critico = ttk.Label(frame_stats, text="Nodo más crítico: -")
        self.label_critico.grid(row=1, column=0, columnspan=2, padx=10, sticky=tk.W)
        
        # Frame de análisis
        frame_analisis = ttk.LabelFrame(self.root, text="Análisis y Recorridos", padding=10)
        frame_analisis.pack(fill=tk.X, padx=10, pady=5)
        
        # BFS
        ttk.Label(frame_analisis, text="Nodo inicio BFS:").grid(row=0, column=0, sticky=tk.W, padx=5)
        self.entry_nodo_bfs = ttk.Entry(frame_analisis, width=10)
        self.entry_nodo_bfs.grid(row=0, column=1, padx=5)
        self.entry_nodo_bfs.insert(0, "0")
        
        ttk.Label(frame_analisis, text="Profundidad:").grid(row=0, column=2, sticky=tk.W, padx=5)
        self.entry_prof_bfs = ttk.Entry(frame_analisis, width=10)
        self.entry_prof_bfs.grid(row=0, column=3, padx=5)
        self.entry_prof_bfs.insert(0, "2")
        
        ttk.Button(frame_analisis, text="Ejecutar BFS", command=self.ejecutar_bfs).grid(row=0, column=4, padx=5)
        
        # DFS
        ttk.Label(frame_analisis, text="Nodo inicio DFS:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=5)
        self.entry_nodo_dfs = ttk.Entry(frame_analisis, width=10)
        self.entry_nodo_dfs.grid(row=1, column=1, padx=5, pady=5)
        self.entry_nodo_dfs.insert(0, "0")
        
        ttk.Button(frame_analisis, text="Ejecutar DFS", command=self.ejecutar_dfs).grid(row=1, column=4, padx=5, pady=5)
        
        # Análisis de nodo específico
        ttk.Label(frame_analisis, text="Analizar nodo:").grid(row=2, column=0, sticky=tk.W, padx=5)
        self.entry_nodo_analisis = ttk.Entry(frame_analisis, width=10)
        self.entry_nodo_analisis.grid(row=2, column=1, padx=5)
        self.entry_nodo_analisis.insert(0, "0")
        
        ttk.Button(frame_analisis, text="Ver Información", command=self.analizar_nodo).grid(row=2, column=4, padx=5)
        
        # Frame de resultados
        frame_resultados = ttk.LabelFrame(self.root, text="Resultados", padding=10)
        frame_resultados.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # Área de texto con scrollbar
        scroll = ttk.Scrollbar(frame_resultados)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)
        
        self.text_resultados = tk.Text(frame_resultados, height=15, yscrollcommand=scroll.set)
        self.text_resultados.pack(fill=tk.BOTH, expand=True)
        scroll.config(command=self.text_resultados.yview)
        
        # Botón para visualizar
        ttk.Button(self.root, text="Visualizar Subgrafo (NetworkX)", command=self.visualizar_subgrafo).pack(pady=5)
        
        self.ultimo_resultado_bfs = None
    
    def seleccionar_archivo(self):
        """Abre diálogo para seleccionar archivo"""
        archivo = filedialog.askopenfilename(
            title="Seleccionar Dataset",
            filetypes=[("Archivos de texto", "*.txt"), ("Todos los archivos", "*.*")]
        )
        if archivo:
            self.entry_archivo.delete(0, tk.END)
            self.entry_archivo.insert(0, archivo)
    
    def cargar_dataset(self):
        """Carga el dataset usando el núcleo C++"""
        if not CYTHON_AVAILABLE:
            messagebox.showerror("Error", "Módulo Cython no disponible")
            return
        
        archivo = self.entry_archivo.get()
        if not archivo or not os.path.exists(archivo):
            messagebox.showerror("Error", "Archivo no válido")
            return
        
        self.log_resultado("=== Cargando Dataset ===")
        self.log_resultado(f"Archivo: {archivo}")
        
        try:
            # Crear instancia del grafo
            self.grafo = grafo_wrapper.PyGrafoDisperso()
            
            # Medir tiempo de carga
            inicio = time.time()
            exito = self.grafo.cargar_datos(archivo)
            fin = time.time()
            
            if exito:
                self.tiempo_carga = fin - inicio
                self.archivo_cargado = archivo
                
                # Actualizar estadísticas
                num_nodos = self.grafo.get_num_nodos()
                num_aristas = self.grafo.get_num_aristas()
                
                self.label_nodos.config(text=f"Nodos: {num_nodos:,}")
                self.label_aristas.config(text=f"Aristas: {num_aristas:,}")
                self.label_tiempo.config(text=f"Tiempo de carga: {self.tiempo_carga:.3f}s")
                
                # Estimar memoria
                memoria_mb = (num_aristas * 2 + num_nodos + 1) * 4 / (1024 * 1024)
                self.label_memoria.config(text=f"Memoria estimada: {memoria_mb:.2f} MB")
                
                # Encontrar nodo crítico
                nodo_critico, grado = self.grafo.obtener_nodo_mayor_grado()
                self.label_critico.config(text=f"Nodo más crítico: {nodo_critico} (grado: {grado})")
                
                self.log_resultado(f"\n✓ Carga exitosa!")
                self.log_resultado(f"  Nodos: {num_nodos:,}")
                self.log_resultado(f"  Aristas: {num_aristas:,}")
                self.log_resultado(f"  Tiempo: {self.tiempo_carga:.3f}s")
                
                messagebox.showinfo("Éxito", "Dataset cargado correctamente")
            else:
                messagebox.showerror("Error", "No se pudo cargar el dataset")
        
        except Exception as e:
            messagebox.showerror("Error", f"Error al cargar: {str(e)}")
            self.log_resultado(f"\n✗ Error: {str(e)}")
    
    def ejecutar_bfs(self):
        """Ejecuta BFS desde el nodo especificado"""
        if not self.verificar_grafo_cargado():
            return
        
        try:
            nodo_inicio = int(self.entry_nodo_bfs.get())
            profundidad = int(self.entry_prof_bfs.get())
            
            self.log_resultado(f"\n=== Ejecutando BFS ===")
            self.log_resultado(f"Nodo inicio: {nodo_inicio}")
            self.log_resultado(f"Profundidad máxima: {profundidad}")
            
            inicio = time.time()
            visitados = self.grafo.bfs(nodo_inicio, profundidad)
            fin = time.time()
            
            self.ultimo_resultado_bfs = (nodo_inicio, visitados)
            
            self.log_resultado(f"\nNodos alcanzados: {len(visitados)}")
            self.log_resultado(f"Tiempo de ejecución: {(fin-inicio)*1000:.4f}ms")
            
            # Mostrar primeros nodos
            muestra = visitados[:50] if len(visitados) > 50 else visitados
            self.log_resultado(f"\nPrimeros nodos visitados: {muestra}")
            if len(visitados) > 50:
                self.log_resultado(f"... y {len(visitados)-50} más")
            
        except ValueError:
            messagebox.showerror("Error", "Valores inválidos")
        except Exception as e:
            messagebox.showerror("Error", str(e))
    
    def ejecutar_dfs(self):
        """Ejecuta DFS desde el nodo especificado"""
        if not self.verificar_grafo_cargado():
            return
        
        try:
            nodo_inicio = int(self.entry_nodo_dfs.get())
            
            self.log_resultado(f"\n=== Ejecutando DFS ===")
            self.log_resultado(f"Nodo inicio: {nodo_inicio}")
            
            inicio = time.time()
            visitados = self.grafo.dfs(nodo_inicio)
            fin = time.time()
            
            self.log_resultado(f"\nNodos alcanzados: {len(visitados)}")
            self.log_resultado(f"Tiempo de ejecución: {(fin-inicio)*1000:.4f}ms")
            
            # Mostrar primeros nodos
            muestra = visitados[:50] if len(visitados) > 50 else visitados
            self.log_resultado(f"\nPrimeros nodos visitados: {muestra}")
            if len(visitados) > 50:
                self.log_resultado(f"... y {len(visitados)-50} más")
            
        except ValueError:
            messagebox.showerror("Error", "Valores inválidos")
        except Exception as e:
            messagebox.showerror("Error", str(e))
    
    def analizar_nodo(self):
        """Muestra información detallada de un nodo"""
        if not self.verificar_grafo_cargado():
            return
        
        try:
            nodo = int(self.entry_nodo_analisis.get())
            
            self.log_resultado(f"\n=== Análisis del Nodo {nodo} ===")
            
            grado_entrada = self.grafo.obtener_grado_entrada(nodo)
            grado_salida = self.grafo.obtener_grado_salida(nodo)
            vecinos = self.grafo.get_vecinos(nodo)
            
            self.log_resultado(f"Grado de entrada: {grado_entrada}")
            self.log_resultado(f"Grado de salida: {grado_salida}")
            self.log_resultado(f"Grado total: {grado_entrada + grado_salida}")
            self.log_resultado(f"Número de vecinos: {len(vecinos)}")
            
            if len(vecinos) > 0:
                muestra = vecinos[:20] if len(vecinos) > 20 else vecinos
                self.log_resultado(f"Vecinos: {muestra}")
                if len(vecinos) > 20:
                    self.log_resultado(f"... y {len(vecinos)-20} más")
            
        except ValueError:
            messagebox.showerror("Error", "Valor de nodo inválido")
        except Exception as e:
            messagebox.showerror("Error", str(e))
    
    def visualizar_subgrafo(self):
        """Visualiza el subgrafo resultante de BFS usando NetworkX"""
        if not NETWORKX_AVAILABLE:
            messagebox.showwarning("No disponible", "NetworkX/Matplotlib no instalados")
            return
        
        if not self.ultimo_resultado_bfs:
            messagebox.showinfo("Info", "Ejecute primero un BFS para visualizar")
            return
        
        nodo_inicio, visitados = self.ultimo_resultado_bfs
        
        if len(visitados) > 200:
            respuesta = messagebox.askyesno(
                "Advertencia",
                f"El subgrafo tiene {len(visitados)} nodos.\n"
                "La visualización puede ser lenta.\n¿Continuar?"
            )
            if not respuesta:
                return
        
        self.log_resultado("\n=== Generando visualización con NetworkX ===")
        
        # Crear grafo NetworkX solo para dibujo
        G = nx.DiGraph()
        
        # Agregar nodos visitados
        G.add_nodes_from(visitados[:200])  # Limitar a 200 para rendimiento
        
        # Agregar aristas entre nodos visitados
        for nodo in visitados[:200]:
            vecinos = self.grafo.get_vecinos(nodo)
            for vecino in vecinos:
                if vecino in visitados[:200]:
                    G.add_edge(nodo, vecino)
        
        # Crear ventana de visualización
        ventana_viz = tk.Toplevel(self.root)
        ventana_viz.title(f"Visualización BFS desde nodo {nodo_inicio}")
        ventana_viz.geometry("800x600")
        
        fig, ax = plt.subplots(figsize=(10, 8))
        
        # Calcular posiciones
        pos = nx.spring_layout(G, k=0.5, iterations=50)
        
        # Colorear nodo inicial diferente
        node_colors = ['red' if node == nodo_inicio else 'lightblue' for node in G.nodes()]
        
        # Dibujar
        nx.draw(G, pos, ax=ax, 
                node_color=node_colors,
                node_size=100,
                with_labels=len(G.nodes()) < 50,  # Etiquetas solo si hay pocos nodos
                font_size=8,
                arrows=True,
                arrowsize=10,
                edge_color='gray',
                alpha=0.7)
        
        ax.set_title(f"Subgrafo BFS - Nodo inicial: {nodo_inicio}\n"
                    f"Nodos mostrados: {len(G.nodes())} de {len(visitados)}")
        
        canvas = FigureCanvasTkAgg(fig, ventana_viz)
        canvas.draw()
        canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        
        self.log_resultado(f"✓ Visualización generada ({len(G.nodes())} nodos, {len(G.edges())} aristas)")
    
    def verificar_grafo_cargado(self):
        """Verifica si hay un grafo cargado"""
        if not self.grafo:
            messagebox.showwarning("Advertencia", "Primero debe cargar un dataset")
            return False
        return True
    
    def log_resultado(self, texto):
        """Agrega texto al área de resultados"""
        self.text_resultados.insert(tk.END, texto + "\n")
        self.text_resultados.see(tk.END)
        self.root.update()


def main():
    """Función principal"""
    root = tk.Tk()
    app = NeuroNetGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
