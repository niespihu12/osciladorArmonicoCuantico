import matplotlib.pyplot as plt
import numpy as np

# Configurar el estilo de la gráfica - usando un estilo básico de matplotlib
plt.style.use('https://github.com/dhaitz/matplotlib-stylesheets/raw/master/pitayasmoothie-light.mplstyle')

# Crear la figura y subplots
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
axes = [ax1, ax2, ax3, ax4]

# Configurar estilo general
plt.rcParams.update({
    'font.size': 10,
    'axes.grid': True,
    'grid.alpha': 0.3,
    'lines.linewidth': 2
})

for n in range(4):
    # Cargar datos
    data = np.loadtxt(f'estado_{n}.txt', skiprows=1)
    x = data[:, 0]
    psi = data[:, 1]
    psi2 = data[:, 2]
    V = data[:, 3]
    
    ax = axes[n]
    
    # Graficar función de onda
    ax.plot(x, psi, color='#1f77b4', label='Ψ(x)', linewidth=1.5)
    # Graficar densidad de probabilidad
    ax.plot(x, psi2, color='#d62728', label='|Ψ(x)|²', linewidth=1.5)
    # Graficar potencial (escalado)
    ax.plot(x, V/max(V)*max(abs(psi)), 'k--', 
           label='V(x) (escalado)', alpha=0.5)
    
    ax.set_title(f'Estado n={n}', pad=10)
    ax.grid(True, alpha=0.3)
    ax.legend(frameon=True, fancybox=True, shadow=True)
    ax.set_xlabel('x')
    ax.set_ylabel('Amplitud')
    
    # Ajustar límites y aspecto
    ax.set_xlim(min(x), max(x))
    margin = 0.1 * (max(psi) - min(psi))
    ax.set_ylim(min(psi) - margin, max(psi) + margin)

# Ajustar el espaciado entre subplots
plt.tight_layout(pad=2.0)

# Añadir un título general
fig.suptitle('Funciones de Onda del Oscilador Armónico Cuántico', 
            fontsize=14, y=1.02)

# Guardar la figura
plt.savefig('graficas.png', 
            dpi=300, bbox_inches='tight')
plt.close()


# import matplotlib.pyplot as plt;
# import pandas as pd
# import numpy as np

# archivo = "data.txt"
# x = []
# psi = []

# with open(archivo, 'r') as a:
#     for i in a.readlines():
#         valor = i.strip().split(' ')
#         x.append(float(valor[0]))
#         psi.append(float(valor[1]))

# print(x)
# print(psi)

# plt.plot(x, psi, label="hola mundo desde python")
# plt.xlabel("x")
# plt.ylabel("Psi")
# plt.title("oscilador")
# plt.legend()
# plt.grid()
# plt.savefig('grafica.png')
# plt.close()



# data = np.loadtxt(archivo)


# x = data[:, 0]
# psi = data[:, 1]



    

