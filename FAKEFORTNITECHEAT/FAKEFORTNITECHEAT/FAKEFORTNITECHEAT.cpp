#include <windows.h>
#include <commctrl.h>
#include <cstdlib> // per rand() e srand()
#include <ctime>   // per time()
#include <string>
#include <thread>  // per std::this_thread::sleep_for
#pragma comment(lib, "comctl32.lib")

// Variabili di stato
HWND hwndInject = NULL; // Finestra di inject
HWND hwndCheat = NULL;  // Finestra dei cheat

// Funzione per generare un nome casuale
std::wstring generaNomeCasuale(int lunghezza) {
    const std::wstring caratteri = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::wstring nomeCasuale;
    for (int i = 0; i < lunghezza; ++i) {
        nomeCasuale += caratteri[rand() % caratteri.length()];
    }
    return nomeCasuale;
}

// Prototipi
LRESULT CALLBACK InjectProc(HWND, UINT, WPARAM, LPARAM); // Finestra di inject
LRESULT CALLBACK CheatProc(HWND, UINT, WPARAM, LPARAM);  // Finestra dei cheat

// Entry point Unicode
int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    // Inizializza il generatore di numeri casuali
    srand(static_cast<unsigned int>(time(0)));

    // Inizializza i controlli comuni
    InitCommonControls();

    // Mostra la finestra di inject all'avvio
    {
        WNDCLASSW wc = { 0 };
        wc.lpfnWndProc = InjectProc;
        wc.hInstance = GetModuleHandleW(NULL);
        wc.lpszClassName = L"InjectClass";
        RegisterClassW(&wc);

        hwndInject = CreateWindowW(L"InjectClass", L"Inject Tool", WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,
            100, 100, 300, 150, NULL, NULL, GetModuleHandleW(NULL), NULL);

        ShowWindow(hwndInject, SW_SHOW);
    }

    // Loop messaggi
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// Finestra di inject
LRESULT CALLBACK InjectProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // Pulsante "Inject"
        CreateWindowW(L"Button", L"Inject", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            100, 50, 100, 30, hwnd, (HMENU)1, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) { // Pulsante "Inject" cliccato
            // Nascondi la finestra di inject
            ShowWindow(hwndInject, SW_HIDE);

            // Attendi 5 secondi prima di mostrare la GUI
            std::this_thread::sleep_for(std::chrono::seconds(5));

            // Mostra la finestra dei cheat
            WNDCLASSW wc = { 0 };
            wc.lpfnWndProc = CheatProc;
            wc.hInstance = GetModuleHandleW(NULL);
            wc.lpszClassName = L"CheatClass";
            RegisterClassW(&wc);

            hwndCheat = CreateWindowW(L"CheatClass", L"Cheat Menu", WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,
                100, 100, 300, 200, NULL, NULL, GetModuleHandleW(NULL), NULL);

            ShowWindow(hwndCheat, SW_SHOW);
        }
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Finestra dei cheat
LRESULT CALLBACK CheatProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // Checkbox e controlli
        CreateWindowW(L"Button", L"Aimbot", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 20, 20, 120, 30, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowW(L"Button", L"ESP", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 20, 60, 120, 30, hwnd, (HMENU)2, NULL, NULL);
        CreateWindowW(L"Button", L"FOV Changer", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 20, 100, 120, 30, hwnd, (HMENU)3, NULL, NULL);
        CreateWindowW(L"Edit", L"90", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 100, 50, 25, hwnd, (HMENU)4, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        // Gestione checkbox
        if (LOWORD(wParam) == 1 || LOWORD(wParam) == 2 || LOWORD(wParam) == 3) {
            bool isChecked = IsDlgButtonChecked(hwnd, LOWORD(wParam));
            CheckDlgButton(hwnd, LOWORD(wParam), isChecked ? BST_UNCHECKED : BST_CHECKED);
        }
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}