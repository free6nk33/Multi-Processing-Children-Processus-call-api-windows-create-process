#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>

void simulate_av_interaction(int process_id) {
    std::cout << "Processus enfant " << process_id << " interagit avec l'AV" << std::endl;

    std::string file_name = "temp_simulated_file_" + std::to_string(process_id) + ".txt";

    HANDLE hFile = CreateFileA(file_name.c_str(),
                                GENERIC_WRITE, 
                                0,
                                NULL,
                                CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        const char* content = "Ceci est un fichier simule pour tromper l'AV\n";
        DWORD bytes_written;

        // Added NULL as the last argument for WriteFile
        if (WriteFile(hFile, content, strlen(content), &bytes_written, NULL)) {
            std::cout << "Processus enfant " << process_id << " a cree un fichier simule." << std::endl;
        } else {
            std::cerr << "Erreur lors de l'écriture dans le fichier." << std::endl;
        }
        
        CloseHandle(hFile);

        Sleep(2000);
        DeleteFileA(file_name.c_str());
        std::cout << "Processus enfant " << process_id << " a supprime le fichier simule" << std::endl;

    } else {
        std::cout << "Erreur lors de la creation du fichier." << std::endl;
    }
}

// Execute notepad.exe
void execute_process() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    if (CreateProcessA("C:\\Windows\\system32\\notepad.exe", 
                       NULL, 
                       NULL, 
                       NULL, 
                       FALSE,
                       0,
                       NULL,
                       NULL,
                       &si,
                       &pi)
        ) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

    } else {
        std::cout << "Erreur" << std::endl;
    }
}

DWORD WINAPI child_process(LPVOID param) {
    int process_id = *(int*)param;
    std::cout << "Processus enfant " << process_id << " démarré." << std::endl;

    for (int i = 0; i < 5; i++) {
        Sleep(rand() % 3000 + 1000);
        simulate_av_interaction(process_id);
    }

    std::cout << "Processus enfant " << process_id << " terminé." << std::endl;

    execute_process();
    return 0;
}

int main() {
    const int num_children = 3;
    std::vector<HANDLE> child_process_handles(num_children);
    std::vector<int> process_id(num_children);

    for (int i = 0; i < num_children; ++i) {
        process_id[i] = i + 1; // ID du processus
        child_process_handles[i] = CreateThread(NULL, 0, child_process, &process_id[i], 0, NULL); 
        if (child_process_handles[i] == NULL) {
            std::cerr << "Erreur lors de la création du processus enfant " << i + 1 << std::endl;
        }
    }

    WaitForMultipleObjects(num_children, child_process_handles.data(), TRUE, INFINITE);

    std::cout << "Tous les processus enfants ont terminé." << std::endl;
    return 0;
}
