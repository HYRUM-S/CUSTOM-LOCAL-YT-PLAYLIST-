#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_FILES 150  // Increased max file count
#define MAX_FILENAME 260  // Max path length in Windows

void moveFiles(const char *sourceFolder, const char *destFolder, char fileList[MAX_FILES][MAX_FILENAME], int fileCount) {
    char oldPath[MAX_FILENAME], newPath[MAX_FILENAME];

    for (int i = 0; i < fileCount; i++) {
        snprintf(oldPath, sizeof(oldPath), "%s\\%s", sourceFolder, fileList[i]);
        snprintf(newPath, sizeof(newPath), "%s\\%s", destFolder, fileList[i]);

        if (MoveFile(oldPath, newPath)) {
            printf("Moved: %s -> %s\n", fileList[i], newPath);
        } else {
            DWORD errorCode = GetLastError();
            char errorMessage[256];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMessage, 256, NULL);
            printf("Error moving file: %s (%d) - %s\n", fileList[i], errorCode, errorMessage);
        }
    }
}

int main() {
    char sourceFolder[MAX_FILENAME] = "c:\\Users\\HANHAN\\Documents\\play downloader"; // Change to your source folder
    char destFolder[MAX_FILENAME] = "C:\\Users\\HANHAN\\Videos\\VideoProc Converter AI\\JAVA PROGRAMMING NESCO"; // Change to your destination folder
    char orderFile[MAX_FILENAME] = "order.txt"; // Text file with sorting order
    char fileList[MAX_FILES][MAX_FILENAME];
    int fileCount = 0;

    // Create destination folder if it doesn't exist
    if (!CreateDirectory(destFolder, NULL)) {
        DWORD errorCode = GetLastError();
        char errorMessage[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMessage, 256, NULL);
        printf("Error creating destination folder: %s (%d) - %s\n", destFolder, errorCode, errorMessage);
        return 1;
    }

    // Open order.txt
    FILE *fp = fopen(orderFile, "r");
    if (!fp) {
        perror("Error opening order.txt");
        return 1;
    }

    // Read filenames from order.txt
    while (fgets(fileList[fileCount], MAX_FILENAME, fp)) {
        fileList[fileCount][strcspn(fileList[fileCount], "\n")] = 0;  // Remove newline
        fileCount++;
    }
    fclose(fp);

    // Move files according to order
    moveFiles(sourceFolder, destFolder, fileList, fileCount);

    return 0;
}