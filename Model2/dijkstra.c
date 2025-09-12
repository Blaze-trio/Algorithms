#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTICES 201
#define INFINITY 1000000

typedef struct Edge {
    int vertex;
    int weight;
    struct Edge* next;
} Edge;

typedef struct {
    Edge* adjacency_list[MAX_VERTICES];
    int num_vertices;
} Graph;

Graph* init_graph() {
    Graph* graph = malloc(sizeof(Graph));
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjacency_list[i] = NULL;
    }
    graph->num_vertices = 200;
    return graph;
}

void add_edge(Graph* graph, int u, int v, int weight) {
    Edge* edge1 = malloc(sizeof(Edge));
    edge1->vertex = v;
    edge1->weight = weight;
    edge1->next = graph->adjacency_list[u];
    graph->adjacency_list[u] = edge1;
    Edge* edge2 = malloc(sizeof(Edge));
    edge2->vertex = u;
    edge2->weight = weight;
    edge2->next = graph->adjacency_list[v];
    graph->adjacency_list[v] = edge2;
}

int parse_dijkstra_file(const char* filename, Graph* graph) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    char line[2000];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) <= 1){
            continue;
        }
        int vertex;
        char* token = strtok(line, "\t\n");
        if (!token){
            continue;
        }
        vertex = atoi(token);
        while ((token = strtok(NULL, "\t\n")) != NULL) {
            char* comma_pos = strchr(token, ',');
            if (comma_pos) {
                *comma_pos = '\0'; 
                int neighbor = atoi(token);
                int weight = atoi(comma_pos + 1);
                add_edge(graph, vertex, neighbor, weight);
            }
        }
    }
    fclose(file);
    return 1;
}

int find_min_distance_vertex(int* distances, int* visited, int num_vertices) {
    int min_dist = INFINITY;
    int min_vertex = -1;
    for (int v = 1; v <= num_vertices; v++) {
        if (!visited[v] && distances[v] < min_dist) {
            min_dist = distances[v];
            min_vertex = v;
        }
    }
    return min_vertex;
}

void dijkstra(Graph* graph, int source, int* distances) {
    int* visited = calloc(MAX_VERTICES, sizeof(int));
    for (int i = 0; i < MAX_VERTICES; i++) {
        distances[i] = INFINITY;
    }
    distances[source] = 0;
    for (int count = 0; count < graph->num_vertices; count++) {
        int u = find_min_distance_vertex(distances, visited, graph->num_vertices);
        if (u == -1){
            break;
        }
        visited[u] = 1;
        Edge* edge = graph->adjacency_list[u];
        while (edge) {
            int v = edge->vertex;
            int weight = edge->weight;
            if (!visited[v] && distances[u] != INFINITY && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
            edge = edge->next;
        }
    }
    
    free(visited);
}

void free_graph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        Edge* edge = graph->adjacency_list[i];
        while (edge) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph);
}

int main() {
    Graph* graph = init_graph();
    
    printf("Parsing dijkstraData.txt...\n");
    
    if (!parse_dijkstra_file("dijkstraData.txt", graph)) {
        printf("Failed to parse file\n");
        free_graph(graph);
        return 1;
    }
    
    printf("Successfully parsed graph!\n");
    
    int* distances = malloc(MAX_VERTICES * sizeof(int));
    
    printf("Running Dijkstra's algorithm from vertex 1...\n");
    dijkstra(graph, 1, distances);
    
    int targets[] = {7, 37, 59, 82, 99, 115, 133, 165, 188, 197};
    int num_targets = 10;
    
    printf("\nShortest distances from vertex 1:\n");
    for (int i = 0; i < num_targets; i++) {
        int vertex = targets[i];
        printf("Vertex %d: %d\n", vertex, distances[vertex]);
    }
    
    printf("\nAnswer: ");
    for (int i = 0; i < num_targets; i++) {
        int vertex = targets[i];
        printf("%d", distances[vertex]);
        if (i < num_targets - 1) {
            printf(",");
        }
    }
    printf("\n");
    
    free(distances);
    free_graph(graph);
    return 0;
}
