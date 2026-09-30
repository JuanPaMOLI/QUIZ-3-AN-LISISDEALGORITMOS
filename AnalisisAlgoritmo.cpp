#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ==========================================
// ALGORITMO: MERGESORT
// ==========================================
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// ==========================================
// ALGORITMO: BÚSQUEDA BINARIA
// ==========================================
int busquedaBinaria(const vector<int>& arr, int objetivo) {
    int izquierda = 0;
    int derecha = arr.size() - 1;

    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) return medio;
        if (arr[medio] < objetivo) izquierda = medio + 1;
        else derecha = medio - 1;
    }
    return -1; // No encontrado
}

// ==========================================
// BENCHMARK (ANÁLISIS EMPÍRICO)
// ==========================================
int main() {
    // Tamaños de arreglos (N) a evaluar
    vector<int> tamanos = {1000, 5000, 10000, 50000, 100000, 250000, 500000, 1000000};
    
    // Configuración de números aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, 10000000);

    cout << "N\tTiempo Mergesort (ms)\tTiempo Busq. Binaria (microsegundos)\n";

    for (int n : tamanos) {
        // 1. Generar arreglo con valores aleatorios
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = distribucion(gen);
        }

        // 2. Benchmark de Mergesort
        vector<int> arrMerge = arr; // Copia para ordenar
        auto inicio_ms = high_resolution_clock::now();
        mergeSort(arrMerge, 0, n - 1);
        auto fin_ms = high_resolution_clock::now();
        double tiempo_mergesort = duration_cast<duration<double, milli>>(fin_ms - inicio_ms).count();

        // 3. Benchmark de Búsqueda Binaria
        // El arreglo debe estar ordenado previamente
        vector<int> arrBusqueda = arrMerge; 
        
        // Repetimos la búsqueda 10,000 veces para obtener un promedio medible
        int repeticiones = 10000; 
        double tiempo_total_busqueda = 0;

        for (int r = 0; r < repeticiones; r++) {
            // Buscamos un valor que sí exista en el arreglo
            int objetivo = arrBusqueda[distribucion(gen) % n]; 
            
            auto inicio_bb = high_resolution_clock::now();
            busquedaBinaria(arrBusqueda, objetivo);
            auto fin_bb = high_resolution_clock::now();
            
            tiempo_total_busqueda += duration_cast<duration<double, std::micro>>(fin_bb - inicio_bb).count();
        }
        
        double tiempo_promedio_busqueda = tiempo_total_busqueda / repeticiones;

        // 4. Imprimir resultados tabulados
        cout << n << "\t" 
             << fixed << setprecision(4) << tiempo_mergesort << "\t\t\t" 
             << fixed << setprecision(4) << tiempo_promedio_busqueda << "\n";
    }

    return 0;
}