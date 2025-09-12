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

typedef struct {
    int vertex;
    int distance;
} HeapNode;

typedef struct {
    HeapNode* heap;
    int* position;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->heap = malloc(capacity * sizeof(HeapNode));
    heap->position = malloc(MAX_VERTICES * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    for (int i = 0; i < MAX_VERTICES; i++) {
        heap->position[i] = -1;
    }
    return heap;
}

void swap_nodes(MinHeap* heap, int i, int j) {
    heap->position[heap->heap[i].vertex] = j;
    heap->position[heap->heap[j].vertex] = i;
    HeapNode temp = heap->heap[i];
    heap->heap[i] = heap->heap[j];
    heap->heap[j] = temp;
}

void heapify_up(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;
    
    if (index > 0 && heap->heap[index].distance < heap->heap[parent].distance) {
        swap_nodes(heap, index, parent);
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heap->size && heap->heap[left].distance < heap->heap[smallest].distance) {
        smallest = left;
    }
    
    if (right < heap->size && heap->heap[right].distance < heap->heap[smallest].distance) {
        smallest = right;
    }
    
    if (smallest != index) {
        swap_nodes(heap, index, smallest);
        heapify_down(heap, smallest);
    }
}

void insert_heap(MinHeap* heap, int vertex, int distance) {
    if (heap->size >= heap->capacity) return;
    
    int index = heap->size;
    heap->heap[index].vertex = vertex;
    heap->heap[index].distance = distance;
    heap->position[vertex] = index;
    heap->size++;
    
    heapify_up(heap, index);
}

HeapNode extract_min(MinHeap* heap) {
    if (heap->size == 0) {
        HeapNode empty = {-1, INFINITY};
        return empty;
    }
    
    HeapNode min_node = heap->heap[0];
    heap->position[min_node.vertex] = -1; 
    heap->heap[0] = heap->heap[heap->size - 1];
    heap->position[heap->heap[0].vertex] = 0;
    heap->size--;
    
    if (heap->size > 0) {
        heapify_down(heap, 0);
    }
    
    return min_node;
}

void decrease_key(MinHeap* heap, int vertex, int new_distance) {
    int index = heap->position[vertex];
    if (index == -1) return;
    heap->heap[index].distance = new_distance;
    heapify_up(heap, index);
}

int is_in_heap(MinHeap* heap, int vertex) {
    return heap->position[vertex] != -1;
}

void free_heap(MinHeap* heap) {
    free(heap->heap);
    free(heap->position);
    free(heap);
}

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
        if (strlen(line) <= 1) continue;
        
        int vertex;
        char* token = strtok(line, "\t\n");
        if (!token) continue;
        
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

void dijkstra_heap(Graph* graph, int source, int* distances) {
    MinHeap* heap = create_heap(graph->num_vertices);
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        distances[i] = INFINITY;
    }
    distances[source] = 0;
    
    for (int v = 1; v <= graph->num_vertices; v++) {
        insert_heap(heap, v, distances[v]);
    }
    
    while (heap->size > 0) {
        HeapNode min_node = extract_min(heap);
        int u = min_node.vertex;
        
        if (min_node.distance == INFINITY) {
            break; 
        }
        Edge* edge = graph->adjacency_list[u];
        while (edge) {
            int v = edge->vertex;
            int weight = edge->weight;
            if (is_in_heap(heap, v) && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                decrease_key(heap, v, distances[v]);
            }
            edge = edge->next;
        }
    }
    
    free_heap(heap);
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
    
    printf("Running Dijkstra's algorithm with heap from vertex 1...\n");
    dijkstra_heap(graph, 1, distances);
    
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