#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

void parse_adjacency_list(char *data) {
    char *data_copy = strdup(data);
    if (!data_copy) {
        perror("Failed to copy data");
        return;
    }

    char *line = strtok(data_copy, "\n");

    while (line != NULL) {
        if (strlen(line) == 0) {
            line = strtok(NULL, "\n");
            continue;
        }
        
        int source_node, neighbor;
        int offset = 0;
        int items_read = 0;

        if (sscanf(line, "%d %n", &source_node, &offset) == 1) {
            printf("Node %d is connected to: ", source_node);
            char* current_pos = line + offset;
            while (sscanf(current_pos, "%d %n", &neighbor, &items_read) == 1) {
                printf("%d ", neighbor);
                current_pos += items_read;
            }
            printf("\n");
        }

        line = strtok(NULL, "\n");
    }

    free(data_copy);
}

int main(void) {
    // For now, just read from a local file
    FILE *file = fopen("SCC.txt", "r");
    if (!file) {
        printf("Please download SCC.txt manually and place it in the current directory\n");
        printf("URL: https://d3c33hcgiwev3.cloudfront.net/_410e934e6553ac56409b2cb7096a44aa_SCC.txt\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *data = malloc(file_size + 1);
    fread(data, 1, file_size, file);
    data[file_size] = '\0';

    parse_adjacency_list(data);

    free(data);
    fclose(file);
    return 0;
}