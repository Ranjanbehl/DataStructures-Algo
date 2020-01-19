#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<limits.h>
#include "pathfinder.h"

//Function to create a new node
AlNode* createNode(long location,float weight)
{
    AlNode* newNode = malloc(sizeof(newNode));
    newNode->location = location;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
} 
//Function to add the x and y to the current vertex
void addCoord(Vertex* graph,long vertex,long x,long y){
    graph[vertex].x = x;
	graph[vertex].y = y;
}
//Function to add a edge(s) to a node aka connect two nodes together 
void addEdge(Vertex* graph, long a, long b)
{
   float w = CalcWeight(graph[a].x,graph[a].y,graph[b].x,graph[b].y);
  // printf("\n W is %f",w);
   AlNode *newnode = createNode(b, w); 
	if(graph[a].head == NULL)
	{
		graph[a].head = newnode;
	}
	else if(graph[a].head->next == NULL) 
	{
		if(b > graph[a].head->location)
		{
			graph[a].head->next = newnode;
		}	
		else
		{
			newnode->next = graph[a].head;
		}
	
	}
	else if (newnode->location < graph[a].head->location)
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
			if (newnode->location < temp2->location) break;
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

// The minheapify function that is required to make sure the heap properties are not violated
void minHeapify(hNode ** minHeap, long index,long elements,long * pos) 
{ 
    long smallest = index; 
    long left = 2 * index + 1; 
    long right = 2 * index + 2; 
  
    if (left < elements && minHeap[left]->weight < minHeap[smallest]->weight ) {
    smallest = left;
    }
 	if (right< elements && minHeap[right]->weight < minHeap[smallest]->weight) {
    smallest = right;
     }
	if (smallest != index){ 

     // Swaping the positions of the two node
        pos[minHeap[smallest]->vertex] = index;
 		pos[minHeap[index]->vertex] = smallest;
        // Swaping the Nodes
        swapHeapnodes(&minHeap[smallest], &minHeap[index]); 
        minHeapify(minHeap, smallest,elements,pos); 
    } 
} 
//Standard function to extract the min node from the heap
hNode* getMin(hNode** minHeap,long *elements, long * pos){
    
        if(*elements == 0){
            return NULL;
        }
      hNode* temp = minHeap[0]; 
      hNode* lastNode = minHeap[*elements - 1]; 
      minHeap[0] = lastNode;
      //update the postion of the lastNode
      pos[temp->vertex] = *elements - 1;
      pos[lastNode->vertex] = 0;
      *elements = *elements -1;
    minHeapify(minHeap, 0,*elements,pos); 
    return temp; 
}

//Function to decrease the weight value of a vertex because Dijkstra algo requires the distance(weight) values 
// each vertex to be updated accordly. 
void decreaseVal(hNode ** minHeap, long vertex,float weight,long elements,long * pos){
    
    long i = pos[vertex];
    //printf("\ni has a val of %ld",i);
    minHeap[i]->weight = weight;
    //Travel up the tree(minheap) while it not fully heapified
    while(i > 0 && minHeap[i]->weight < minHeap[(i - 1) / 2]->weight){

        //swap the current node with its parent node
        pos[minHeap[i]->vertex] = (i-1)/2;
        pos[minHeap[(i-1)/2]->vertex] = i;
        swapHeapnodes(&minHeap[i],&minHeap[(i-1)/2]);
    // Move up the index from child to parent
        i = (i-1)/2;
    }
}
// Function to get the weight value for the heapnode
float CalcWeight(int x1,int y1, int x2, int y2){
    int result;
    result  = sqrt((x2- x1)*(x2 - x1) +  (y2 - y1)*(y2 - y1));
    return result;
}
// Function to find the shortest path from source to a set location
void Dijkstra(Vertex* graph, long source, long dest,long vertices)
{
   
    float* lengths = malloc(sizeof(float) * vertices);
    long* forset = malloc(sizeof(long) * vertices);
    long* backset = malloc(sizeof(long) * vertices);
    long *position = malloc(sizeof(long) * 100000);
    long v,elements = 0, i;
	for(i = 0 ; i < vertices ; i++)
	{    	
		lengths[i] = INT_MAX;
    		backset[i] = -1;

	}
    hNode** minHeap = malloc(sizeof(hNode*) * 100000);
    // Initialize the min heap
    for(v = 0; v< vertices; v++){
        if(v == source) continue;
        minHeap[v] = createHeapNode(v,lengths[v]);
        position[v] = v;
       // printf("\nheap node %ld has a val of %f",v,minHeap[v]->weight);
    }
    lengths[source] = 0;
    // set the source weight to zero 
    minHeap[source] = createHeapNode(source,lengths[source]);
    position[source] = source;
    decreaseVal(minHeap,source,lengths[source],elements,position);

    elements = vertices;
    
    while(lengths[dest] == INT_MAX && elements > 0)
    {
        //Extract the vertex that has the smallest weight
        hNode* mhNode = getMin(minHeap,&elements,position);
       //printf("\nThe extracted vertex is %ld",mhNode->vertex);
		AlNode * edge = graph[mhNode->vertex].head;
		while (edge != NULL)
		{
           // printf("\nhere4");
			if (position[edge->location] < elements  && (edge->weight + lengths[mhNode->vertex] < lengths[edge->location]) && lengths[mhNode->vertex] != INT_MAX)
			{
				lengths[edge->location] = lengths[mhNode->vertex] + edge->weight;
 				backset[edge->location] = mhNode->vertex;
				decreaseVal(minHeap, edge->location, lengths[edge->location],elements,position);
			}
			edge = edge->next;
        }
    }
   
	long j;
	printf("%ld\n", (long int)lengths[dest]);
	long p = dest;
	j = 0;
	while(p >= 0)
	{
		forset[j++] = p;
		p = backset[p];
	}
	while(j > 0)
	{
		printf("%ld ", forset[--j]);
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
			printf("%ld ", temp->location);
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
    //printf("\n There are %ld vertices and %ld edges",vertices,edges);
    Vertex graph[100000];
	for(i = 0; i < 100000; i++)
	{
		graph[i].x = -1;
		graph[i].y = -1;
		graph[i].head = NULL;		
	}
    for(i = 0; i < vertices; i++)
    {
        fscanf(input,"%ld" "%d" "%d",&vertex,&x,&y);
        // Adding the x and y values to each vertex
        addCoord(graph,vertex,x,y); // This works correctly
       // printf("\n The x and y  of vertex %ld are %d and %d",vertex,x,y);
    }
    for(i = 0; i < edges; i++)
    {
        fscanf(input,"%ld" "%ld",&a,&b);
       // printf("\n The two edges to connect are %ld and %ld",a,b);
       addEdge(graph,a,b);
       addEdge(graph,b,a);
    }
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