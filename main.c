#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double hbar = 1.0;
const double masa = 1.0;
const double omega = 1.0;

const double x_max = 5.0;
const double x_min = -5.0;
const int n_puntos = 1000;
const double dx = (x_max - x_min) / (n_puntos - 1);

// Energías esperadas: E_n = (n + 1/2)ℏω
double energia_esperada(int n) {
    return (n + 0.5) * hbar * omega;
}

// Potencial armónico V(x) = 1/2 * k * x^2 = 1/2 * m * ω^2 * x^2
double potencial(double x) {
    return 0.5 * masa * omega * omega * x * x;
}

// Función g(x) para el método de Numerov
double funcion_g(double x, double energia) {
    return (2.0 * masa / (hbar * hbar)) * (energia - potencial(x));
}

// Factor f para la fórmula de Numerov
double factor_f(double x, double energia) {
    return 1.0 + (dx * dx / 12.0) * funcion_g(x, energia);
}

// Método de Numerov 
void metodo_numerov(double *funcion_onda, double energia, int estado_n) {
    double x[n_puntos];
    for (int i = 0; i < n_puntos; i++) {
        x[i] = x_min + i * dx;
    }


    if (estado_n % 2 == 0) {

        funcion_onda[0] = exp(-x[0] * x[0] / 2);
        funcion_onda[1] = exp(-x[1] * x[1] / 2);
    } else {

        funcion_onda[0] = 0.0;
        funcion_onda[1] = dx * x[1] * exp(-x[1] * x[1] / 2);
    }

    // Iteración de Numerov hacia adelante
    for (int i = 1; i < n_puntos - 1; i++) {
        double fn = factor_f(x[i], energia);
        double fn_prev = factor_f(x[i-1], energia);
        double fn_next = factor_f(x[i+1], energia);
        funcion_onda[i + 1] = ((12.0 - 10.0 * fn) * funcion_onda[i] - fn_prev * funcion_onda[i-1]) / fn_next;
    }
}

// Normalización usando el método del trapecio
void normalizar(double *funcion_onda) {
    double integral = 0.0;
    
    for (int i = 0; i < n_puntos - 1; i++) {
        integral += 0.5 * dx * (funcion_onda[i] * funcion_onda[i] + 
                               funcion_onda[i + 1] * funcion_onda[i + 1]);
    }
    
    double factor_norm = 1.0 / sqrt(integral);
    for (int i = 0; i < n_puntos; i++) {
        funcion_onda[i] *= factor_norm;
    }
}

// Verificar si la función de onda satisface las condiciones de contorno
double verificar_frontera(double *funcion_onda) {
    return fabs(funcion_onda[n_puntos-1]) + fabs(funcion_onda[0]);
}

int main(void) {
    double *funcion_onda = (double *)malloc(n_puntos * sizeof(double));
    if (funcion_onda == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return EXIT_FAILURE;
    }

    for (int n = 0; n < 4; n++) {
        double energia = energia_esperada(n);
        char nombre_archivo[50];
        sprintf(nombre_archivo, "estado_%d.txt", n);
        
        metodo_numerov(funcion_onda, energia, n);
        normalizar(funcion_onda);

        FILE *archivo = fopen(nombre_archivo, "w");

        fprintf(archivo, "# x\tPsi(x)\tPsi^2(x)\tV(x)\n");
        for (int i = 0; i < n_puntos; i++) {
            double x = x_min + i * dx;
            fprintf(archivo, "%f\t%f\t%f\t%f\n", 
                    x, funcion_onda[i], funcion_onda[i] * funcion_onda[i], potencial(x));
        }
        fclose(archivo);

        printf("Estado n=%d: E=%.6f, Error en frontera=%.6e\n", 
               n, energia, verificar_frontera(funcion_onda));
    }

    free(funcion_onda);
    return EXIT_SUCCESS;
}