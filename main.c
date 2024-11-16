#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double h = 1.0;
const double o = 1.0;

const double xmax = 5.0;
const double xmin = -5.0;

const int n = 1000;
const double m = 1.0;

const double dx = (xmax - xmin) / (n - 1);

// Potencial armónico
double V(double xi) {
    double formula = 0.5 * m * pow(xi, 2);
    return formula;
}

// Método de Numerov
void metodoNumerov(double *psi, double epsilon) {
    double g[n];
    double x[n];

    // g(x)
    for (int i = 0; i < n; i++) {
        x[i] = xmin + i * dx;
        g[i] = 2 * (V(x[i]) - epsilon);
    }

    // Condiciones iniciales
    psi[0] = 0.0;
    psi[1] = 1e-5;

    // Iteración de Numerov
    for (int i = 1; i < n - 1; i++) {
        psi[i + 1] = (2 * psi[i] * (1 - dx * dx * g[i] / 12) - 
                      psi[i - 1] * (1 + dx * dx * g[i - 1] / 12)) / 
                     (1 + dx * dx * g[i + 1] / 12);
    }
}

// Normalizar el valor de onda
void normalizar(double *psi) {
    double s = 0;

    for (int i = 0; i < n; i++) {
        s += pow(psi[i], 2) * dx;
    }

    double f = sqrt(1.0 / s);
    for (int i = 0; i < n; i++) {
        psi[i] *= f;
    }
}

int main(void) {
    double psi[n];
    double epsilon = 0.5;

    metodoNumerov(psi, epsilon);

    normalizar(psi);

    FILE *archivo = fopen("data.txt", "w");

    for (int i = 0; i < n; i++) {
        double x = xmin + i * dx;
        fprintf(archivo, "%f %f\n", x, psi[i]);
    }

    fclose(archivo);

    return EXIT_SUCCESS;
}







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
