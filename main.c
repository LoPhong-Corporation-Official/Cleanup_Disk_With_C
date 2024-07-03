// Code created by LoPhong Corporation
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>

// Hàm để xóa tệp
void delete_files_in_directory(const char* path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char search_path[MAX_PATH];

    snprintf(search_path, sizeof(search_path), "%s\\*", path);
    hFind = FindFirstFile(search_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Cannot open this folder: %s\n", path);
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
                        printf("Deleted: %s\n", full_path);
                    } else {
                        printf("Cannot delete: %s\n", full_path);
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
}

void cleanup() {
    char temp_path[MAX_PATH];
    char local_temp_path[MAX_PATH];
    char prefetch_path[MAX_PATH] = "C:\\Windows\\Prefetch";

    // Lấy đường dẫn thư mục Temp
    if (GetTempPath(MAX_PATH, temp_path) == 0) {
        printf("Cannot find the folder Temp in your Windows\n");
        return;
    }

    // Lấy đường dẫn thư mục Local Temp
    if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_temp_path) != S_OK) {
        printf("Cannot find the folder Local Temp in your Windows\n");
        return;
    }
    strcat(local_temp_path, "\\Temp");

    printf("Cleanup your Temp folder: %s\n", temp_path);
    delete_files_in_directory(temp_path);

    printf("Cleanup your Local Temp folder: %s\n", local_temp_path);
    delete_files_in_directory(local_temp_path);

    printf("Cleanup your Prefetch: %s\n", prefetch_path);
    delete_files_in_directory(prefetch_path);

    printf("Cleanup finish. Press key 'E' to exit this tool or press 'P' to go to my github!\n");
}

int main() {
    
    printf("Press key 'S' to start cleanup\n");
    while (1) {
        if (GetAsyncKeyState('S') & 0x8000) {
            printf("Starting cleanup...\n");
            cleanup();
        } else if (GetAsyncKeyState('E') & 0x8000) {
            Sleep(1000); // Đợi 1000ms trước khi exit
            printf("Exit this tool.\n");
            printf("Thank you to using this tool.\n");
            break;
        }
        if (GetAsyncKeyState('P') & 0x8000) {
            printf("Please wait...\n");
            ShellExecute(NULL, "open", "http://github.com/LoPhong-Corporation-Official", NULL, NULL, SW_SHOWNORMAL);
            break;
        }
        Sleep(100); // Đợi 100ms trước khi kiểm tra phím nhấn lần nữa
    }
    return 0;
}
