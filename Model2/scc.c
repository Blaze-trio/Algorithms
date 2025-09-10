#pragma comment(linker, "/STACK:268435456")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *neighbors; 
    int count;       
    int capacity; 
} AdjacencyList;

typedef struct {
    AdjacencyList *adj_list;
    int num_vertices;
    int max_vertex;
} Graph;

typedef struct {
    int vertex;
    int finish_time;
} VertexInfo;

void init_adjacency_list(AdjacencyList *list) {
    list->neighbors = NULL;
    list->count = 0;
    list->capacity = 0;
}

Graph* init_graph(int max_vertices) {
    Graph *graph = malloc(sizeof(Graph));
    if (!graph) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    graph->adj_list = malloc((max_vertices + 1) * sizeof(AdjacencyList));
    if (!graph->adj_list) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    for (int i = 0; i <= max_vertices; i++) {
        init_adjacency_list(&graph->adj_list[i]);
    }
    
    graph->num_vertices = 0;
    graph->max_vertex = max_vertices;
    return graph;
}

void add_neighbor(AdjacencyList *list, int neighbor) {
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 2 : list->capacity * 2;
        list->neighbors = realloc(list->neighbors, list->capacity * sizeof(int));
        if (!list->neighbors) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    list->neighbors[list->count++] = neighbor;
}

int parse_scc_file(const char *filename, Graph **original_graph, Graph **reverse_graph) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    int max_vertex = 875714;
    printf("Maximum vertex: %d\n", max_vertex);
    *original_graph = init_graph(max_vertex);
    *reverse_graph = init_graph(max_vertex);
    int tail, head;
    int edges_processed = 0;
    while (fscanf(file, "%d %d", &tail, &head) == 2) {
        add_neighbor(&(*original_graph)->adj_list[tail], head);
        add_neighbor(&(*reverse_graph)->adj_list[head], tail);
        
        edges_processed++;
        
        if (edges_processed % 100000 == 0) {
            printf("Processed %d edges...\n", edges_processed);
        }
    }
    
    printf("Total edges processed: %d\n", edges_processed);
    (*original_graph)->num_vertices = max_vertex;
    (*reverse_graph)->num_vertices = max_vertex;
    
    fclose(file);
    return 1;
}

void free_graph(Graph *graph) {
    if (!graph) return;
    
    for (int i = 0; i <= graph->max_vertex; i++) {
        if (graph->adj_list[i].neighbors) {
            free(graph->adj_list[i].neighbors);
        }
    }
    free(graph->adj_list);
    free(graph);
}

void dfs_pass1_iterative(Graph *graph, int start_vertex, int *visited, VertexInfo *finish_order, int *time) {
    int *stack = malloc(graph->max_vertex * sizeof(int));
    int *finish_stack = malloc(graph->max_vertex * sizeof(int));
    int stack_top = -1;
    int finish_top = -1;
    
    stack[++stack_top] = start_vertex;
    
    while (stack_top >= 0) {
        int vertex = stack[stack_top--];
        
        if (visited[vertex] == 0) {
            visited[vertex] = 1;
            finish_stack[++finish_top] = vertex;
            for (int i = 0; i < graph->adj_list[vertex].count; i++) {
                int neighbor = graph->adj_list[vertex].neighbors[i];
                if (!visited[neighbor]) {
                    stack[++stack_top] = neighbor;
                }
            }
        }
    }
    
    while (finish_top >= 0) {
        int vertex = finish_stack[finish_top--];
        finish_order[*time].vertex = vertex;
        finish_order[*time].finish_time = *time;
        (*time)++;
    }
    
    free(stack);
    free(finish_stack);
}

void dfs_pass2_iterative(Graph *graph, int start_vertex, int *visited, int *scc_size) {
    int *stack = malloc(graph->max_vertex * sizeof(int));
    int stack_top = -1;
    
    stack[++stack_top] = start_vertex;
    
    while (stack_top >= 0) {
        int vertex = stack[stack_top--];
        
        if (!visited[vertex]) {
            visited[vertex] = 1;
            (*scc_size)++;
            for (int i = 0; i < graph->adj_list[vertex].count; i++) {
                int neighbor = graph->adj_list[vertex].neighbors[i];
                if (!visited[neighbor]) {
                    stack[++stack_top] = neighbor;
                }
            }
        }
    }
    
    free(stack);
}

int compare_scc_sizes(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int main() {
    Graph *original_graph = NULL;
    Graph *reverse_graph = NULL;
    
    printf("Parsing SCC.txt...\n");
    
    if (!parse_scc_file("SCC.txt", &original_graph, &reverse_graph)) {
        printf("Failed to parse file\n");
        return 1;
    }
    
    printf("Successfully created graphs!\n");
    printf("Vertices: %d, Max vertex: %d\n", original_graph->num_vertices, original_graph->max_vertex);

    int max_vertex = 875714;
    int *visited = calloc(max_vertex + 1, sizeof(int));
    VertexInfo *finish_order = malloc((max_vertex + 1) * sizeof(VertexInfo));
    int time = 0;
    
    printf("Starting first DFS pass on reverse graph...\n");
    int components_processed = 0;
    
    for (int i = 1; i <= max_vertex; i++) {
        if (!visited[i] && reverse_graph->adj_list[i].count > 0) {
            dfs_pass1_iterative(reverse_graph, i, visited, finish_order, &time);
            components_processed++;
            
            if (components_processed % 1000 == 0) {
                printf("Processed %d components, %d vertices so far...\n", components_processed, time);
            }
        }
    }
    
    printf("First pass completed. Total vertices processed: %d\n", time);
    
    // Reset visited array for second pass
    free(visited);
    visited = calloc(max_vertex + 1, sizeof(int));
    
    int *scc_sizes = malloc((max_vertex + 1) * sizeof(int));
    int scc_count = 0;
    
    printf("Starting second DFS pass on original graph...\n");
    
    for (int i = time - 1; i >= 0; i--) {
        int vertex = finish_order[i].vertex;
        if (!visited[vertex]) {
            int scc_size = 0;
            dfs_pass2_iterative(original_graph, vertex, visited, &scc_size);
            scc_sizes[scc_count++] = scc_size;
            
            if (scc_count % 1000 == 0) {
                printf("Found %d SCCs so far...\n", scc_count);
            }
        }
    }
    
    printf("Found %d strongly connected components\n", scc_count);
    
    qsort(scc_sizes, scc_count, sizeof(int), compare_scc_sizes);
    
    printf("\nTop 5 largest SCCs:\n");
    printf("\nAnswer: ");
    for (int i = 0; i < 5; i++) {
        if (i < scc_count) {
            printf("%d", scc_sizes[i]);
        } else {
            printf("0");
        }
        if (i < 4) printf(",");
    }
    printf("\n");
    
    // Clean up
    free(visited);
    free(finish_order);
    free(scc_sizes);
    free_graph(original_graph);
    free_graph(reverse_graph);
    
    return 0;
}
