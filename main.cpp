#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <time.h>
#include "resource.h"

HINSTANCE hInst;
using namespace std;
int N;
int M[16][1000];
int Tiempo;
long long iteracionesQuickSort = 0;
long long iteracionesMergeSort = 0;
double tiempoQuickSort = 0;
double tiempoMergeSort = 0;

void Inicia(int *A, int NN, int T) {
    N = NN;
    Tiempo = T;
    for (int i = 0; i < N; i++)
        M[0][i] = M[1][i] = M[2][i] = M[3][i] = M[4][i] = M[5][i] = M[6][i] = M[7][i] = M[8][i] = A[i];
}

void Muestra(HDC hdc, int *A, float xi, float yi, float xf, float yf) {
    Rectangle(hdc, xi, yi, xf, yf);
    TextOut(hdc, 130, 8, "Quick Sort", 10);
    TextOut(hdc, 430, 8, "Merge Sort", 10);
    int i;
    int Mayor = A[0];
    for (i = 1; i < N; i++)
        if (A[i] > Mayor)
            Mayor = A[i];
    float ancho = (xf - xi) / N;
    float alto = (yf - yi) / Mayor;
    for (i = 0; i < N; i++)
        Rectangle(hdc, xi + i * ancho, yf - A[i] * alto, xi + (i + 1) * ancho - 1, yf);
}

class OrdenaQuickSort {
public:
    void quickSort(int arr[], int inicio, int fin, HWND hwndDlg) {
        if (inicio < fin) {
            int pivote = particion(arr, inicio, fin, hwndDlg);
            quickSort(arr, inicio, pivote - 1, hwndDlg);
            quickSort(arr, pivote + 1, fin, hwndDlg);
        }
    }

    int particion(int arr[], int inicio, int fin, HWND hwndDlg) {
        int pivote = arr[fin];
        int i = inicio - 1;
        for (int j = inicio; j < fin; j++) {
            iteracionesQuickSort++;
            if (arr[j] <= pivote) {
                i++;
                swap(arr[i], arr[j]);
                RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);
                Sleep(Tiempo);
            }
        }
        swap(arr[i + 1], arr[fin]);
        return i + 1;
    }

    void operator()(HWND hwndDlg) {
        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        iteracionesQuickSort = 0;
        quickSort(M[5], 0, N - 1, hwndDlg);

        QueryPerformanceCounter(&end);
        tiempoQuickSort = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        cout << "Quick Sort \t Tiempo: " << tiempoQuickSort << " s,\t Iteraciones: " << iteracionesQuickSort << endl;
    }
};

class OrdenaMergeSort {
public:
    void merge(int arr[], int l, int m, int r, HWND hwndDlg) {
        int n1 = m - l + 1;
        int n2 = r - m;

        int L[n1], R[n2];
        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            iteracionesMergeSort++;
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);
            Sleep(Tiempo);
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

    void mergeSort(int arr[], int l, int r, HWND hwndDlg) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(arr, l, m, hwndDlg);
            mergeSort(arr, m + 1, r, hwndDlg);
            merge(arr, l, m, r, hwndDlg);
        }
    }

    void operator()(HWND hwndDlg) {
        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        iteracionesMergeSort = 0;
        mergeSort(M[7], 0, N - 1, hwndDlg);

        QueryPerformanceCounter(&end);
        tiempoMergeSort = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        cout << "Merge Sort \t Tiempo: " << tiempoMergeSort << " s,\t Iteraciones: " << iteracionesMergeSort << endl;
    }
};

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG: {
        int N = 200, i, j, T, Salir;
        int AA[200];
        srand(time(NULL));

        for (i = 0; i < N; i++) {
            do {
                Salir = 1;
                T = rand() % N + 1;
                for (j = 0; j < i; j++) {
                    if (AA[j] == T) {
                        Salir = 0;
                        break;
                    }
                }
                AA[i] = T;
            } while (Salir == 0);
        }
        Inicia(AA, N, 50);
    }
        return TRUE;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
        return TRUE;
    case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwndDlg, &ps);
        Muestra(hdc, M[5], 10, 30, 300, 220);
        Muestra(hdc, M[7], 310, 30, 600, 220);
        EndPaint(hwndDlg, &ps);
    }
        return TRUE;
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case B_PROBAR:
            thread((OrdenaQuickSort()), hwndDlg).detach();
            thread((OrdenaMergeSort()), hwndDlg).detach();
            return TRUE;
        }
    }
        return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    hInst = hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
