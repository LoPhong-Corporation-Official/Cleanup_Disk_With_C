#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>
#include <psapi.h>

// Định nghĩa màu chữ cho console
enum ConsoleColor {
    CC_DEFAULT = 7,     // Màu chữ mặc định
    CC_GREEN = 10,      // Màu chữ xanh lá cây
    CC_RED = 12,        // Màu chữ đỏ
    CC_YELLOW = 14,     // Màu chữ vàng
    CC_WHITE = 15       // Màu chữ trắng
};

// Hàm đổi màu chữ trong console
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Hàm để xóa tệp
void delete_files_in_directory(const char* path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char search_path[MAX_PATH];

    snprintf(search_path, sizeof(search_path), "%s\\*", path);
    hFind = FindFirstFile(search_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        setColor(CC_RED); // Đổi màu chữ thành đỏ
        printf("Cannot open this folder: %s\n", path);
        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
        return;
    } else {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                char full_path[MAX_PATH];
                snprintf(full_path, sizeof(full_path), "%s\\%s", path, findFileData.cFileName);

                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    delete_files_in_directory(full_path); // Đệ quy cho các thư mục con
                    RemoveDirectory(full_path);
                } else {
                    if (DeleteFile(full_path)) {
                        setColor(CC_YELLOW);
                        printf("[CLEANUP] ");
                        setColor(CC_DEFAULT);
                        setColor(CC_GREEN); // Đổi màu chữ thành xanh lá cây
                        printf("Deleted: ");
                        setColor(CC_WHITE); // Đổi màu chữ của phần nội dung file bị xóa thành trắng
                        printf("%s\n", full_path);
                        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
                    } else {
                        setColor(CC_YELLOW);
                        printf("[CLEANUP] ");
                        setColor(CC_DEFAULT);
                        setColor(CC_RED); // Đổi màu chữ thành đỏ
                        printf("Cannot delete: %s\n", full_path);
                        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
}

// Hàm dọn dẹp các thư mục
void cleanup() {
    char temp_path[MAX_PATH] = "C:\\Windows\\Temp";
    char local_temp_path[MAX_PATH];
    char prefetch_path[MAX_PATH] = "C:\\Windows\\Prefetch";

    // Lấy đường dẫn thư mục Temp
    if (GetTempPath(MAX_PATH, temp_path) == 0) {
        setColor(CC_RED); // Đổi màu chữ thành đỏ
        printf("Cannot find the folder Temp in your Windows\n");
        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
        return;
    }

    // Lấy đường dẫn thư mục Local Temp
    if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_temp_path) != S_OK) {
        setColor(CC_RED); // Đổi màu chữ thành đỏ
        printf("Cannot find the folder Local Temp in your Windows\n");
        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
        return;
    }
    strcat(local_temp_path, "\\Temp");

    setColor(CC_YELLOW); // Đổi màu chữ thành vàng

    printf("Cleanup your Temp folder: %s\n", temp_path);
    setColor(CC_DEFAULT); // Đổi về màu chữ mặc định

    delete_files_in_directory("C:\\Windows\\Temp");

    setColor(CC_YELLOW); // Đổi màu chữ thành vàng

    printf("Cleanup your Local Temp folder: %s\n", local_temp_path);
    setColor(CC_DEFAULT); // Đổi về màu chữ mặc định

    delete_files_in_directory(local_temp_path);

    setColor(CC_YELLOW); // Đổi màu chữ thành vàng

    printf("Cleanup your Prefetch: %s\n", prefetch_path);
    setColor(CC_DEFAULT); // Đổi về màu chữ mặc định

    delete_files_in_directory(prefetch_path);

    setColor(CC_GREEN); // Đổi màu chữ thành xanh lá cây
    printf("Cleanup finish.\n");
    setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
    setColor(CC_GREEN);
    
    printf("Press key 'S' to start cleanup\n");
    printf("Press key 'R' to reduce memory\n");
    printf("Press key 'E' to exit\n");
    printf("Press key 'P' to go to my github\n");
    setColor(CC_DEFAULT);
}

void reduce_memory() {
    DWORD processes[1024], count;
    if (!EnumProcesses(processes, sizeof(processes), &count)) {
        setColor(CC_RED); // Đổi màu chữ thành đỏ
        printf("Failed to enumerate processes.\n");
        setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
        return;
    }

    count /= sizeof(DWORD);
    for (DWORD i = 0; i < count; i++) {
        if (processes[i] != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
            if (hProcess) {
                if (EmptyWorkingSet(hProcess)) {
                    setColor(CC_YELLOW);
                    printf("[REDUCE] ");
                    setColor(CC_GREEN); // Đổi màu chữ thành xanh lá cây
                    printf("Reduced memory for process ID %lu\n", processes[i]);
                } else {
                    setColor(CC_RED); // Đổi màu chữ thành đỏ
                    printf("Failed to reduce memory for process ID %lu\n", processes[i]);
                }
                setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
                CloseHandle(hProcess);
            }
        }
    }
}

int main() {
    setColor(CC_GREEN);
    printf("Please run at administrator if you didn't run to cleanup your system.\n");
    printf("Press key 'S' to start cleanup\n");
    printf("Press key 'R' to reduce memory\n");
    printf("Press key 'E' to exit\n");
    printf("Press key 'P' to go to my github\n");
    setColor(CC_DEFAULT);
    while (1) {
        if (GetAsyncKeyState('S') & 0x8000) {
            setColor(CC_YELLOW); // Đổi màu chữ thành vàng
            printf("Starting cleanup...\n");
            setColor(CC_DEFAULT); // Đổi về màu chữ mặc định

            cleanup();
        } else if (GetAsyncKeyState('R') & 0x8000) {
            setColor(CC_YELLOW); // Đổi màu chữ thành vàng
            printf("Reducing memory...\n");
            setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
            reduce_memory();
            setColor(CC_GREEN);
            printf("Memory reduced successfully.\n");
            setColor(CC_DEFAULT);
            setColor(CC_GREEN);

            printf("Press key 'S' to start cleanup\n");
            printf("Press key 'R' to reduce memory\n");
            printf("Press key 'E' to exit\n");
            printf("Press key 'P' to go to my github\n");
            setColor(CC_DEFAULT);
        } else if (GetAsyncKeyState('E') & 0x8000) {
            Sleep(1000); // Đợi 1000ms trước khi exit
            setColor(CC_WHITE); // Đổi màu chữ thành trắng
            printf("Exit this tool.\n");
            printf("Thank you to using this tool.\n");
            Sleep(1000);
            setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
            break;
        }
        if (GetAsyncKeyState('P') & 0x8000) {
            setColor(CC_YELLOW); // Đổi màu chữ thành vàng
            printf("Please wait...\n");
            setColor(CC_DEFAULT); // Đổi về màu chữ mặc định
            ShellExecute(NULL, "open", "http://github.com/LoPhong-Corporation-Official", NULL, NULL, SW_SHOWNORMAL);
            break;
        }
        Sleep(100); // Đợi 100ms trước khi kiểm tra phím nhấn lần nữa
    };
    return 0;
}