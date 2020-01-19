//Structures

// Structure for edge list of each vertex
typedef struct AdjlistNode
{
	long location;
	float weight;// Length
	struct AdjlistNode *next;
} AlNode;

// Structure for edge vertex of graph
typedef struct vertex
{
	int x;
	int y;
	AlNode *head;
} Vertex;

// Structure for each node in min heap
typedef struct heapnode
{
	long  vertex;
	float weight; // weight of the edges connected to the node from the source, aka the length from source to current node
} hNode;


// Graph Functions
AlNode* createNode(long location,float weight);
void addCoord(Vertex* graph,long vertex,long x,long y);
void addEdge(Vertex* graph, long a, long b);
// Minheap Functions
hNode* createHeapNode(long vertex, float weight);
void swapHeapnodes(hNode** a, hNode** b);
void minHeapify(hNode ** minHeap, long index,long elements,long * pos);
hNode* getMin(hNode** minHeap,long *elements, long * pos);
void decreaseVal(hNode ** minHeap, long vertex,float weight,long elements,long * pos);
// Dijkstra’s Algorithm
float CalcWeight(int x1,int y1, int x2, int y2);
void Dijkstra(Vertex* graph, long source, long dest,long vertices);
//Print Functions
void printAdjlist(Vertex* graph,long vertices);
