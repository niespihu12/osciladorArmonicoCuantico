import matplotlib.pyplot as plt;
import pandas as pd
import numpy as np

archivo = "data.txt"
x = []
psi = []

with open(archivo, 'r') as a:
    for i in a.readlines():
        valor = i.strip().split(' ')
        x.append(float(valor[0]))
        psi.append(float(valor[1]))

print(x)
print(psi)

plt.plot(x, psi, label="hola mundo desde python")
plt.xlabel("x")
plt.ylabel("Psi")
plt.title("oscilador")
plt.legend()
plt.grid()
plt.savefig('grafica.png')
plt.close()



# data = np.loadtxt(archivo)


# x = data[:, 0]
# psi = data[:, 1]



    

