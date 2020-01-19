#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<limits.h>


// Structure for edge list of each vertex
typedef struct AdjlistNode
{
	long dest;
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

// A minheap 
typedef struct MinHeap {
long size; // The current size of the min heap
long cap;// capacity of the min heap
int *location; // Required for the Decrease value function
hNode** heaparray; // Array of node pointers 
}minheap;

// Graph Functions
AlNode* createNode(long dest,float weight);
void addCoord(Vertex* graph,long dictionary[][3],long vertices);
void addEdge(Vertex* graph, long a, long b);
// Minheap Functions
hNode* createHeapNode(long vertex, float weight);
minheap* makeMinHeap(long vertices);
void swapHeapnodes(hNode** a, hNode** b);
void minHeapify(minheap* minHeap, int index);
int isEmpty(minheap* minHeap);
int inMinHeap(minheap* minHeap,int vertex);
hNode* getMin(minheap* MinHeap);
void decreaseVal(minheap* minHeap, long vertex,float weight);
// Dijkstra’s Algorithm
float CalcWeight(int x1,int y1, int x2, int y2);
void Dijkstra(Vertex* graph, long source, long dest,long vertices);
//Print Functions
void printAdjlist(Vertex* graph,long vertices);

//Function to create a new node
AlNode* createNode(long dest,float weight)
{
    AlNode* newNode = malloc(sizeof(newNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
} 
//Function to add the x and y to the current vertex
void addCoord(Vertex* graph,long dictionary[][3],long vertices){
    long i;
    for(i = 0; i<vertices; i++)
    {
    graph->x = dictionary[i][2];
    graph->y = dictionary[i][3];
    }
}
//Function to add a edge(s) to a node aka connect two nodes together 
void addEdge(Vertex* graph, long a, long b)
{
   float w = CalcWeight(graph[a].x,graph[a].y,graph[b].x,graph[b].y);
   AlNode *newnode = createNode(b, w); 
	if(graph[a].head == NULL)
	{
		graph[a].head = newnode;
	}
	else if(graph[a].head->next == NULL) 
	{
		if(b > graph[a].head->dest)
		{
			graph[a].head->next = newnode;
		}	
		else
		{
			newnode->next = graph[a].head;
		}
	
	}
	else if (newnode->dest < graph[a].head->dest)
	{
			newnode->next = graph[a].head;
			graph[a].head = newnode;
	}
	else
	{
		AlNode *temp;	
		temp = graph[a].head;
		while(temp->next != NULL)
		{
			AlNode *temp2 = temp->next;
			if (newnode->dest < temp2->dest) break;
			temp = temp -> next;
		}
			
		if (temp -> next == NULL)
		{
			temp->next = newnode;
		}
		else
		{
			newnode->next = temp->next;
			temp->next = newnode;
		}
	}
}
//Function to create a heapnode based on the given vertex and weight value
hNode* createHeapNode(long vertex, float weight)
{
    hNode* newnode = malloc(sizeof(newnode));
    newnode->vertex = vertex;
    newnode->weight = weight;
    return newnode;
}
// Standard Swap Function
void swapHeapnodes(hNode** a, hNode** b){
    hNode* temp = *a;
    *a = *b;
    *b = temp;
}
minheap* makeMinHeap(long vertices){
    minheap* minHeap = malloc(sizeof(hNode) * vertices);
    minHeap->cap = vertices;
    minHeap->size = 0;
    minHeap->location = malloc(sizeof(int)* vertices);
    minHeap->heaparray = malloc(vertices * sizeof(hNode*));
    return minHeap;
}
// The minheapify function that is required to make sure the heap properties are not violated
void minHeapify(minheap* minHeap, int index) 
{ 
    int smallest = index; 
    int left = 2 * index + 1; 
    int right = 2 * index + 2; 
  
    if (left < minHeap->size && minHeap->heaparray[left]->weight < minHeap->heaparray[smallest]->weight) 
        smallest = left; 
  
    if (right < minHeap->size && minHeap->heaparray[right]-> weight < minHeap->heaparray[smallest]->weight) 
        smallest = right; 
  
    if (smallest != index) { 

        hNode* smallestNode = minHeap->heaparray[smallest];
        hNode* curNode = minHeap->heaparray[index];

        // Swaping the positions of the two nodes
        minHeap->location[smallestNode->vertex] = index;
        minHeap->location[curNode->vertex] = smallest;

        // Swaping the Nodes
        swapHeapnodes(&minHeap->heaparray[smallest], 
                        &minHeap->heaparray[index]); 
        minHeapify(minHeap, smallest); 
    } 
} 
//Standard function to extract the min node from the heap
hNode* getMin(minheap* minHeap){
      hNode* temp = minHeap->heaparray[0]; 
      hNode* lastNode = minHeap->heaparray[minHeap->size - 1]; 
      temp = lastNode;

      //update the postion of the lastNode
      minHeap->location[temp->vertex] = minHeap-> size - 1;
      minHeap->location[lastNode->vertex] = 0;
    
    --minHeap->size; 
    minHeapify(minHeap, 0); 
    return temp; 
}

//Function to decrease the weight value of a vertex because Dijkstra algo requires the distance(weight) values 
// each vertex to be updated accordly. 
void decreaseVal(minheap* minHeap, long vertex,float weight){
    
    long i = minHeap->location[vertex];
    printf("\ni has a val of %ld",i);
    minHeap->heaparray[i]->weight = weight;
    //Travel up the tree(minheap) while it not fully heapified
    while(i && minHeap->heaparray[i]->weight < minHeap->heaparray[(i - 1) / 2]->weight){

        //swap the current node with its parent node
        minHeap->location[minHeap->heaparray[i]->vertex] = (i-1)/2;
        minHeap->location[minHeap->heaparray[(i-1)/2]->vertex] = i;
        swapHeapnodes(&minHeap->heaparray[i],&minHeap->heaparray[(i-1)/2]);

    // Move up the index from child to parent
        i = (i-1)/2;
    }
}
// Function to get the weight value for the heapnode
float CalcWeight(int x1,int y1, int x2, int y2){
    int result;
    result = sqrt(pow(2,x2 - x1) + pow(2,y2-y1));
    return result;
}
// Function to find the shortest path from source to dest
void Dijkstra(Vertex* graph, long source, long dest,long vertices)
{
   
    float* lengths = malloc(sizeof(float) * vertices);
    long* forward = malloc(sizeof(long) * vertices);
    long* backward = malloc(sizeof(long) * vertices);
    long v,elements, i;

	for(i = 0 ; i < vertices ; i++)
	{    	
		lengths[i] = INT_MAX;
    		backward[i] = -1;

	}
    minheap* minHeap = makeMinHeap(vertices);
    // Initialize the min heap
    minHeap->size = vertices;
    for(v = 0; v< vertices; v++){
        lengths[v] = INT_MAX;
        minHeap->heaparray[v] = createHeapNode(v,lengths[v]);
        minHeap->location[v] = v;
       // printf("\nheap node %ld has a val of %f",v,minHeap->heaparray[v]->weight);
    }
    lengths[source] = 0;
    // set the source weight to zero 
    minHeap->heaparray[source] = createHeapNode(source,lengths[source]);
    minHeap->location[source] = source;
    decreaseVal(minHeap,source,lengths[source]);

    elements = vertices;
    
    while(lengths[dest] == INT_MAX && elements > 0)
    {
        //Extract the vertex that has the smallest weight
        hNode* mhNode = getMin(minHeap);
        elements = elements -1;
        printf("\nThe extracted vertex is %ld",mhNode->vertex);
		AlNode * edge = graph[mhNode->vertex].head;
		while (edge != NULL)
		{
           // printf("\nhere4");
			if (minHeap->location[edge->dest] < elements  && (edge->weight + lengths[mhNode->vertex] < lengths[edge->dest]) && lengths[mhNode->vertex] != INT_MAX)
			{
				lengths[edge->dest] = lengths[mhNode->vertex] + edge->weight;
 				backward[edge->dest] = mhNode->vertex;
				decreaseVal(minHeap, edge->dest, lengths[edge->dest]);
			}
			edge = edge->next;
		
        }
    }
// Print shortest path
	long j;
	printf("%ld\n", (long int)lengths[dest]);
	long p = dest;
	j = 0;
	while(p >= 0)
	{
		forward[j++] = p;
		p = backward[p];
	}
	while(j > 0)
	{
		printf("%ld ", forward[--j]);
	}
	printf("\n");
}
//Function to print a graph's adjlists
void printAdjlist(Vertex* graph,long vertices)
{
   long i;
	AlNode *temp;
	for(i = 0; i < vertices; i++)
	{
		printf("\n%ld: ", i);
		temp = graph[i].head;
		while(temp != NULL)
		{
			printf("%ld ", temp->dest);
			temp = temp->next;
		}
		printf("\n");
	}
}
int main(int argc, char**argv)
{
    long vertices,edges,vertex,a,b,num_queries,i;
    int x,y;
    FILE* input;
    input = fopen(argv[1],"r");
    if(input == NULL)
    {
        return 0;
    }
    fscanf(input,"%ld %ld",&vertices,&edges);
    long dictionary[vertices][3];
    //printf("\n There are %ld vertices and %ld edges",vertices,edges);
    Vertex graph[100000];
    for(i = 0; i < vertices; i++)
    {
        fscanf(input,"%ld" "%d" "%d",&vertex,&x,&y);
        dictionary[vertex][2] = x;
        dictionary[vertex][3] = y;
       // printf("\n The x and y  of vertex %ld are %d and %d",vertex,x,y);
    }
    for(i = 0; i < edges; i++)
    {
        fscanf(input,"%ld" "%ld",&a,&b);
       // printf("\n The two edges to connect are %ld and %ld",a,b);
       addEdge(graph,a,b);
       addEdge(graph,b,a);
    }
    // Adding the x and y values to each vertex
    addCoord(graph,dictionary,vertices); // This works correctly
    //printAdjlist(graph,vertices);
    
    // Reading in the Query File
    FILE* input2; 
    input2 = fopen(argv[2],"r");
    if(input2 == NULL)
    {
        return 0;
    }
     fscanf(input2, "%ld", &num_queries);
     for(i = 0; i < num_queries; i++)
     {
         fscanf(input2,"%ld %ld", &a,&b);
         Dijkstra(graph,a,b,vertices);
     }
     
    return 0;
}