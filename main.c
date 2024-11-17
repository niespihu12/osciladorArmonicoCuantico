#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Constantes físicas (en unidades naturales: ℏ = m = ω = 1)
const double HBAR = 1.0;
const double MASS = 1.0;
const double OMEGA = 1.0;

// Parámetros de la cuadrícula
const double XMAX = 5.0;  // Aumentado para capturar mejor el decaimiento
const double XMIN = -5.0;
const int N = 1000;
const double DX = (XMAX - XMIN) / (N - 1);

// Energías esperadas: E_n = (n + 1/2)ℏω
double expected_energy(int n) {
    return (n + 0.5) * HBAR * OMEGA;
}

// Potencial armónico V(x) = 1/2 * k * x^2 = 1/2 * m * ω^2 * x^2
double V(double x) {
    return 0.5 * MASS * OMEGA * OMEGA * x * x;
}

// Función g(x) para el método de Numerov
double g(double x, double E) {
    return (2.0 * MASS / (HBAR * HBAR)) * (E - V(x));
}

// Factor f para la fórmula de Numerov
double f(double x, double E) {
    return 1.0 + (DX * DX / 12.0) * g(x, E);
}

// Método de Numerov mejorado
void numerov_method(double *psi, double E, int n_state) {
    // Inicialización
    double x[N];
    for (int i = 0; i < N; i++) {
        x[i] = XMIN + i * DX;
    }

    // Condiciones iniciales basadas en la paridad del estado
    if (n_state % 2 == 0) {  // Estado par
        psi[0] = exp(-x[0] * x[0] / 2);  // Aproximación gaussiana
        psi[1] = exp(-x[1] * x[1] / 2);
    } else {  // Estado impar
        psi[0] = 0.0;
        psi[1] = DX * x[1] * exp(-x[1] * x[1] / 2);  // Primera derivada
    }

    // Iteración de Numerov hacia adelante
    for (int i = 1; i < N - 1; i++) {
        double fn = f(x[i], E);
        double fn_prev = f(x[i-1], E);
        double fn_next = f(x[i+1], E);
        
        psi[i + 1] = ((12.0 - 10.0 * fn) * psi[i] - fn_prev * psi[i-1]) / fn_next;
    }
}

// Normalización mejorada usando el método del trapecio
void normalize(double *psi) {
    double integral = 0.0;
    
    // Integración por método del trapecio
    for (int i = 0; i < N - 1; i++) {
        integral += 0.5 * DX * (psi[i] * psi[i] + psi[i + 1] * psi[i + 1]);
    }
    
    double norm_factor = 1.0 / sqrt(integral);
    
    for (int i = 0; i < N; i++) {
        psi[i] *= norm_factor;
    }
}

// Verificar si la función de onda satisface las condiciones de contorno
double check_boundary(double *psi) {
    return fabs(psi[N-1]) + fabs(psi[0]);  // Debería ser cercano a 0
}

int main(void) {
    double *psi = (double *)malloc(N * sizeof(double));
    if (psi == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return EXIT_FAILURE;
    }

    // Calcular los primeros 4 estados
    for (int n = 0; n < 4; n++) {
        double E = expected_energy(n);
        char filename[50];
        sprintf(filename, "estado_%d.txt", n);
        
        numerov_method(psi, E, n);
        normalize(psi);
        
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", filename);
            free(psi);
            return EXIT_FAILURE;
        }
        
        // Guardar resultados
        fprintf(file, "# x\tPsi(x)\tPsi^2(x)\tV(x)\n");
        for (int i = 0; i < N; i++) {
            double x = XMIN + i * DX;
            fprintf(file, "%f\t%f\t%f\t%f\n", 
                    x, psi[i], psi[i] * psi[i], V(x));
        }
        
        fclose(file);
        printf("Estado n=%d: E=%.6f, Error en frontera=%.6e\n", 
               n, E, check_boundary(psi));
    }

    free(psi);
    return EXIT_SUCCESS;
}


// #include <stdio.h>
// #include <math.h>
// #include <stdlib.h>

// const double h = 1.0;
// const double o = 1.0;

// const double xmax = 2.0;
// const double xmin = -2.0;

// const int n = 1000;
// const double m = 1.0;

// const double dx = (xmax - xmin) / (n - 1);

// // Potencial armónico
// double V(double xi) {
//     double formula = 0.5 * m * pow(xi, 2);
//     return formula;
// }

// // Método de Numerov
// void metodoNumerov(double *psi, double epsilon) {
//     double g[n];
//     double x[n];

//     // g(x)
//     for (int i = 0; i < n; i++) {
//         x[i] = xmin + i * dx;
//         g[i] = 2 * (V(x[i]) - epsilon);
//     }

//     // Condiciones iniciales
//     psi[0] = 0.0;
//     psi[1] = 1e-5;

//     // Iteración de Numerov
//     for (int i = 1; i < n - 1; i++) {
//         psi[i + 1] = (2 * psi[i] * (1 - dx * dx * g[i] / 12) - 
//                       psi[i - 1] * (1 + dx * dx * g[i - 1] / 12)) / 
//                      (1 + dx * dx * g[i + 1] / 12);
//     }
// }

// // Normalizar el valor de onda
// void normalizar(double *psi) {
//     double s = 0;

//     for (int i = 0; i < n; i++) {
//         s += pow(psi[i], 2) * dx;
//     }

//     double f = sqrt(1.0 / s);
//     for (int i = 0; i < n; i++) {
//         psi[i] *= f;
//     }
// }

// int main(void) {
//     double psi[n];
//     double epsilon = 0.5;

//     metodoNumerov(psi, epsilon);

//     normalizar(psi);

//     FILE *archivo = fopen("data.txt", "w");

//     for (int i = 0; i < n; i++) {
//         double x = xmin + i * dx;
//         fprintf(archivo, "%f %f\n", x, psi[i]);
//     }

//     fclose(archivo);

//     return EXIT_SUCCESS;
// }







// #include <stdio.h>
// #include <math.h>

// // Constantes físicas
// const double h_bar = 1.0; // Constante reducida de Planck (puedes ajustar el valor)
// const double m = 1.0;     // Masa de la partícula

// // Función del potencial V(x)
// double V(double x) {
//     double omega = 1.0; // Frecuencia angular
//     return 0.5 * omega * omega * x * x;
// }

// // Función para calcular k^2(x)
// double k_squared(double x, double E) {
//     return (2 * m / (h_bar * h_bar)) * (E - V(x));
// }

// // Implementación del método de Numerov
// void numerov_method(double *psi, double *x, int n_points, double h, double E) {
//     double k2_i_minus_1, k2_i, k2_i_plus_1;

//     for (int i = 1; i < n_points - 1; ++i) {
//         k2_i_minus_1 = k_squared(x[i-1], E);
//         k2_i = k_squared(x[i], E);
//         k2_i_plus_1 = k_squared(x[i+1], E);

//         psi[i+1] = (2 * (1 - 5.0 * h * h * k2_i / 12.0) * psi[i] - 
//                     (1 + h * h * k2_i_minus_1 / 12.0) * psi[i-1]) / 
//                    (1 + h * h * k2_i_plus_1 / 12.0);
//     }
// }

// int main() {
//     int n_points = 1000;
//     double x_start = -5.0, x_end = 5.0;
//     double h = (x_end - x_start) / (n_points - 1);
//     double E = 1.0; // Energía a probar

//     double x[n_points];
//     double psi[n_points];

//     // Inicialización de x y psi
//     for (int i = 0; i < n_points; ++i) {
//         x[i] = x_start + i * h;
//         psi[i] = 0.0;
//     }

//     // Condiciones iniciales
//     psi[0] = 0.0;
//     psi[1] = 0.001; // Valor pequeño para empezar la iteración

//     numerov_method(psi, x, n_points, h, E);

//     // Imprimir resultados
//     for (int i = 0; i < n_points; ++i) {
//         printf("%f %f\n", x[i], psi[i]);
//     }

//     return 0;
// }
