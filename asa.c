#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE  1
#define NIL  -1

/************************** Estruturas *******************************/

/* #define Vertex int */
/* Os vertices (0,1,..,N-1) sao representados por um tipo Vertex. */
typedef int Vertex;

/* A lista de adjacência de um vértice v é composta por nós do tipo node. 
Cada nó da lista corresponde a um arco e contém um vizinho w de v e o endereço do nó seguinte da lista. 
Um link é um ponteiro para um node. */
typedef struct node *link;
struct node {
    Vertex w;
    link next;
};

/* O grafo armazena: o número de vertices (pessoas) V,
o número de arcos (partilhas) E, e um ponteiro para o vetor de listas de adjacência. */
typedef struct graph *Graph;
struct graph {
    int V;
    int E;
    link *adj;
    int *scc_number;
};

/* Uma estrutura SCC armazena: o número de SCC,
 um vector de vectores com os SCC e outro vector com o tamanho de cada SCCs.  */
typedef struct scc_result *Scc_result;
struct scc_result {
    int n_sets;
    Vertex **sets;
    int *set_sizes;
};

/************************** Cabeçalhos das Funções *******************************/

Graph GRAPHinit(int V);
void GRAPHinsertE(Graph G, Vertex v, Vertex w);
void GRAPHdestroy(Graph G);
void GRAPHshow(Graph G);

void DFSscc(Graph G, Vertex v, Scc_result result);
Scc_result GRAPHscc(Graph G);
void free_scc_result(Scc_result result);
void print_scc_result(Scc_result result);

link NEWnode(Vertex w, link next);

/************************** Representação de grafos *******************************/

/* A função NEWnode recebe um vértice w e o endereço next de um nó 
e devolve o endereço a de um novo nó tal que a->w == w e a->next == next. */
link NEWnode(Vertex w, link next) {
   link a = malloc(sizeof (struct node));
   a->w = w; 
   a->next = next;     
   return a;                         
}

/* A função GRAPHinit devolve (o endereço de) um novo grafo com vértices 0 1 ... V-1 e nenhum arco. */
Graph GRAPHinit(int V) {
    int k;

    Graph G = malloc(sizeof *G);
    G->V = V;
    G->E = 0;
    G->adj = malloc(V * sizeof (link));
    G->scc_number = malloc(V * sizeof (int));
    
    for(k = 0; k < G->V; k++){
        G->adj[k] = NULL;
        G->scc_number[k] = NIL;
    }
    
    return G;
}

/* A função GRAPHdestroy destroi o grafo. */
void GRAPHdestroy(Graph G){
    int v;
    
    /* Liberta memória dos nós da lista de adjacências */
    for(v = 0; v < G->V; v++){
        free(G->adj[v]);
    }
    
    /* Liberta memória do vector da lista de adjacências */
    free(G->adj);
    
    /* Liberta memória do grafo */
    free(G);
}

/* A função GRAPHinsertE insere um arco v-w no grafo G. */
void GRAPHinsertE(Graph G, Vertex v, Vertex w) {
    G->adj[v-1] = NEWnode(w, G->adj[v-1]);
    G->E++;
}

/* Para cada vértice v do grafo G, esta função imprime, numa linha,
todos os vértices adjacentes a v. */
void GRAPHshow (Graph G) {
    Vertex v;
    link a;
    for(v = 0; v < G->V; v++){
        printf("%2d:", v+1);
        for(a = G->adj[v]; a != NULL; a = a->next)
            printf(" %2d", a->w);
        printf("\n");
    }
}

/***************************** Algoritmo de Tarjan *******************************/

/* 
 L[v]: é uma pilha de vértices, l_size é o seu tamanho.
 d[v]: número de vértices visitados quando v é descoberto
 low[v]: o menor valor de d[] atingível por um arco para trás ou de cruzamento na sub-árvore de v
 */
static int *d;
static int *low;
static Vertex *L;
static int l_size;
static int visited;

Scc_result GRAPHscc(Graph G){
    Vertex v;

    /* inicializar o algoritmo Tarjan */
    visited = 0;
    l_size = 0;
    L = malloc(G->V * sizeof(Vertex));
    d = malloc(G->V * sizeof (int ));
    low = malloc(G->V * sizeof(int));
    for(v = 1; v <= G->V; v++){
        d[v-1] = NIL;
    }

    Scc_result result = malloc(sizeof(*result));
    result->n_sets = 0;
    result->sets = NULL;
    result->set_sizes = NULL;
    
    /* visitar todos os vértices */
    for(v = 1; v <= G->V; v++){
        if (d[v-1] == NIL){
            DFSscc(G, v, result);
        }
    }
    
    /* libertar memória */
    free(d);
    free(low);
    free(L);

    return result;
}

void DFSscc(Graph G, Vertex v, Scc_result result){
    link a;
    
    /* configura o indice de profundidade de v */
    d[v-1] = visited++;
    low[v-1] = d[v-1];
    /* insere v no topo da pilha (push) */
    L[l_size++] = v;
    
    /* considerar os sucessores de v */
    for(a = G->adj[v-1]; a != NULL; a = a->next) {
        Vertex w = a->w;
        
        if (d[w-1] == NIL)
            /* successor w ainda não foi visitado */
            DFSscc(G, w, result);
        
        /* teste dos low menores */
        if (low[w-1] < low[v-1])
            low[v-1] = low[w-1];
    }

    if (low[v-1] < d[v-1])
        return;
    
    /* v é raíz de um SCC, isto é, v é o primeiro vértice do SCC a ser descoberto */
    if(d[v-1] == low[v-1]){
        
        Vertex w;

        result->n_sets++;
        result->sets = realloc(result->sets, result->n_sets * sizeof(Vertex*));
        result->set_sizes = realloc(result->set_sizes, result->n_sets * sizeof(int));
        Vertex *set = (Vertex*) malloc(G->V * sizeof(Vertex));

        int set_size = 0;
        
        do {
            w = L[--l_size]; /* retira w do topo da pilha (pop) */
            /* vertices retirados definem SCC */

            set[set_size] = w;
            G->scc_number[w-1] = result->n_sets-1;
            set_size++;
            
            /* low fica com o tamanho do grafo para chumbar no teste dos low menores */
            low[w-1] = G->V;
        } while (w != v);
        
        set = realloc(set, set_size * sizeof(Vertex));

        result->sets[result->n_sets-1] = set;
        result->set_sizes[result->n_sets-1] = set_size;
    }

}

/***************************** Operações SCCs *******************************/

void free_scc_result(Scc_result result){
    int k;
    
    for(k = 0; k < result->n_sets; k++) {
        free(result->sets[k]);
    }
    free(result->sets);
    free(result->set_sizes);
}

void print_scc_result(Scc_result result){
    int k;
    int j;
    
    for(k = 0; k < result->n_sets; k++){
        for(j = 0; j<result->set_sizes[k]; j++){
            printf("%d ", result->sets[k][j]);
        }
        printf("\n");
    }
}

int biggest_set(Scc_result result){
    int k;
    int max = 0;

    for(k = 0; k < result->n_sets; k++){
        if(max < result->set_sizes[k]){
            max = result->set_sizes[k];
        }
    }
    return max;
}


int single_share(Vertex *set, int set_size, Graph G, int scc_number){
    int k;
    link a;

    for(k = 0; k < set_size; k++){
        Vertex v = set[k];
        for(a = G->adj[v-1]; a != NULL; a = a->next){
            if (G->scc_number[(a->w)-1] != scc_number){
                return FALSE;
            }
        }
        
    }
    return TRUE;
}

int num_single_share(Scc_result result, Graph G){
    int k;
    int single = 0;

    for(k = 0; k < result->n_sets; k++){
        if(single_share(result->sets[k], result->set_sizes[k], G, k) == TRUE){
            single++;
        }
    }
    return single;
}

/***************************** Função principal *******************************/

int main(int argc, char *argv[]){
    int N;
    int P;
    int k;

    scanf("%d %d", &N, &P);
    
    Graph G = GRAPHinit(N);
    
    for(k = 0; k < P; k++){
        Vertex v, w;
        scanf("%d %d",&v,&w);
        GRAPHinsertE(G, v, w);
    }

    Scc_result result = GRAPHscc(G);
    printf("%d\n", result->n_sets);
    printf("%d\n", biggest_set(result));
    printf("%d\n", num_single_share(result, G));

    free_scc_result(result);
    free(result);
    
    GRAPHdestroy(G);

    return 0;
}
