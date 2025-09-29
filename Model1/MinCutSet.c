#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_VERTICES 201
#define MAX_EDGES 10000

typedef struct {
    int u, v;
} Edge;

typedef struct {
    int vertices[MAX_VERTICES];
    int vertex_count;
    Edge edges[MAX_EDGES];
    int edge_count;
} Graph;

// Function to read the graph from file
Graph* readGraph() {
    FILE *file = fopen("MinCutSet.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return NULL;
    }
    
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->vertex_count = 200;
    g->edge_count = 0;
    
    // Initialize vertices
    for (int i = 0; i < 200; i++) {
        g->vertices[i] = i + 1;
    }
    
    char line[2000];
    while (fgets(line, sizeof(line), file)) {
        int vertex, neighbor;
        char *token = strtok(line, "\t ");
        vertex = atoi(token);
        
        while ((token = strtok(NULL, "\t \n")) != NULL) {
            neighbor = atoi(token);
            // Only add edge once (when vertex < neighbor to avoid duplicates)
            if (vertex < neighbor) {
                g->edges[g->edge_count].u = vertex;
                g->edges[g->edge_count].v = neighbor;
                g->edge_count++;
            }
        }
    }
    
    fclose(file);
    return g;
}

// Function to copy a graph
Graph* copyGraph(Graph *original) {
    Graph *copy = (Graph*)malloc(sizeof(Graph));
    copy->vertex_count = original->vertex_count;
    copy->edge_count = original->edge_count;
    
    memcpy(copy->vertices, original->vertices, sizeof(original->vertices));
    memcpy(copy->edges, original->edges, sizeof(Edge) * original->edge_count);
    
    return copy;
}

// Function to contract an edge
void contractEdge(Graph *g, int edgeIndex) {
    int u = g->edges[edgeIndex].u;
    int v = g->edges[edgeIndex].v;
    
    // Merge v into u (replace all occurrences of v with u)
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].u == v) g->edges[i].u = u;
        if (g->edges[i].v == v) g->edges[i].v = u;
    }
    
    // Remove self-loops and the contracted edge
    int newEdgeCount = 0;
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].u != g->edges[i].v) {
            g->edges[newEdgeCount] = g->edges[i];
            newEdgeCount++;
        }
    }
    g->edge_count = newEdgeCount;
    g->vertex_count--;
}

// Karger's randomized contraction algorithm
int kargerMinCut(Graph *original) {
    Graph *g = copyGraph(original);
    
    // Contract until only 2 vertices remain
    while (g->vertex_count > 2) {
        if (g->edge_count == 0) break;
        
        // Pick a random edge
        int randomEdge = rand() % g->edge_count;
        contractEdge(g, randomEdge);
    }
    
    int result = g->edge_count;
    free(g);
    return result;
}

int main() {
    srand(time(NULL));
    printf("Random seed initialized\n");
    Graph *graph = readGraph();
    if (!graph) return 1;
    
    printf("Graph loaded: %d vertices, %d edges\n", graph->vertex_count, graph->edge_count);
    
    int minCut = INT_MAX;
    int iterations = 1000;  // Run many times for better accuracy
    
    for (int i = 0; i < iterations; i++) {
        int cut = kargerMinCut(graph);
        if (cut < minCut) {
            minCut = cut;
        }
        
        if (i % 100 == 0) {
            printf("Iteration %d, current min cut: %d\n", i, minCut);
        }
    }
    
    printf("Minimum cut found: %d\n", minCut);
    
    free(graph);
    return 0;
}