#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Data structure for the Nodes in the Graph
typedef struct node
{
    int *array; // contains the integers
    struct node *previous;//previous node
    struct node* up; //what the node will look if the space goes up
    struct node* down; //what the node will look like if the space goes down
    struct node* right; //what the node will look like if the space goes right 
    struct node* left; //what the node will look like if the space goes left
    struct node* tableNext; // for hash table
    int move;

} Node;

// Node functions
Node *initializeNode(int *array, Node *prev, int k)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->array = malloc(sizeof(int) * k * k);
    memcpy(newNode->array, array, sizeof(int) * k * k);
    newNode->previous = prev;
    newNode->up = NULL;
    newNode->down = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->tableNext = NULL;

    return newNode;
}

// function to print array, for debugging purposes
void printBoard(int *board, int k)
{
    if (board == NULL){
        return;
    }
    for (int i = 0; i < (k * k); i++)
    {
        printf("%d ", board[i]);
    }
    printf("\n");
}
// Data structure for the hashtable, using code in PA3
// defining my open hash data structure
typedef struct openHashTable
{
    int size;
    Node **Table;
} OpenHashTable;

// hashtable functions

// function for initializing a hashtable
OpenHashTable *initializeHashTable(int size)
{
    OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
    hashTable->size = size;
    hashTable->Table = malloc(sizeof(Node *) * size);
    for (int i = 0; i < size; i++)
    {
        // sets each node as Null to make sure they are all empty
        // Runtime of O(n)
        hashTable->Table[i] = NULL;
    }
    // print statement to make sure the function works
    //  printf("HashTable has been made\nSize: %d\n", hashTable->size);
    return hashTable;
}


// calculate hashTable
unsigned long hashFunc(int *array, int k, int size)
{
    unsigned long hash = 0;
    for (int i = 0; i < (k * k); i++)
    {
        //printf("index %d has value %d\n", i, array[i]);
        hash = hash + array[i] * i;
        //printf("%ld hash value rn\n", hash);
    }
    hash = hash * 101;

    //printf("Before the final step in hash %ld\n", hash);
    return hash % size;
}

// lookup hastable
// function for looking up nodes in the Hash table
int isMember(OpenHashTable *hashTable, int *array, int k, unsigned long position)
{
    //unsigned long position = hashFunc(array, k, hashTable->size);
    //printf("checking %ld in has table\n", position);
    Node *ptr = hashTable->Table[position];
    while (ptr != NULL)
    {
        if (memcmp(ptr->array, array, sizeof(int) * k * k) == 0)
        {
            return 1; //present in the table
        }
        ptr = ptr->tableNext;
        
    }

    return 0;
}

void insertToHashTable(OpenHashTable *hashtable, Node *value, int k, unsigned long position)
{
    //unsigned long position = hashFunc(value->array, k, hashtable->size);
    //printf("%d\n", position);
    value->tableNext = hashtable->Table[position];
    hashtable->Table[position] = value;
}


// data structure for queue
typedef struct queue
{
    int maxsize;
    int front, rear;
    Node **items;
} Queue;

// functions for queue
Queue *initializeQueue(int maxsize)
{
    Queue *newQueue = malloc(sizeof(Queue));
    newQueue->maxsize = maxsize;
    newQueue->front = -1;
    newQueue->rear = -1; // circular queue
    newQueue->items = malloc(sizeof(Node *) * maxsize);
    return newQueue;
}

int size(Queue *myQueue)
{
    return myQueue->rear;
}
int isEmpty(Queue *Queue)
{
    return (Queue->front == -1 &&  Queue->rear == -1);
}

int isFull(Queue *myQueue)
{
    return (((myQueue->rear + 1) % myQueue->maxsize) == myQueue->front);
}
void enqueue(Queue *myQueue, Node *value)
{
    // check if full
    if (isFull(myQueue))
    {
        printf("Queue Overflow!!\n");
        exit(-1);
    }
    // add element and increase the top's index by 1
    if (isEmpty(myQueue))
    {
        myQueue->front = myQueue->rear = 0;
    }
    else
    {
        myQueue->rear = (myQueue->rear + 1) % myQueue->maxsize;
    }

    myQueue->items[myQueue->rear] = value;
}

Node *dequeue(Queue *myQueue)
{
    // check is the queue is empty
    if ((isEmpty(myQueue)))
    {
        printf("Trying to dequeue empty queue!!\n");
        exit(-1);
    }

    Node *data = myQueue->items[myQueue->front];

    if (myQueue->front == myQueue->rear)
    {
        myQueue->front = myQueue->rear = -1; // reset the queue
    }
    else
    {
        myQueue->front = (myQueue->front + 1) % myQueue->maxsize;
    }
    return data;
}
// functioins for NODES
///Linear search for finding the zero
int LinearSearch(int *h, int size)
{
    for (int x = 0; x < (size); x++)
    {
        if (h[x] == 0)
        {
            return x;
        }
    }
    return -1;
}
void swapIndex(int *Array, int index, int newIndex)
{
    int temp = Array[newIndex];
    Array[newIndex] = Array[index];
    Array[index] = temp;
}

// function to find the neighbors
void findNeighbors(Node *node, int k)
{
    int positionofzero = LinearSearch(node->array, (k * k)); //O(n)
    //printf("Linear Search found the zero at %d\n", positionofzero);
    int newpositionofzero = 0;

    //checking if left is possible
    if((positionofzero - 1 > 0) && (positionofzero % k != 0))
    {
        newpositionofzero = positionofzero - 1;
        // printf("Moving left, new zero Position: %d\n", newpositionofzero);
        // printf("Old Board\n");
        // printBoard(node->array, k);
        node->left = initializeNode(node->array, node, k);
        node->left->move = node->array[newpositionofzero]; // updating the move
        swapIndex(node->left->array, positionofzero, newpositionofzero);
        //printBoard(node->left->array, k);
    }

     //checking if right is possible
    if ((positionofzero) % k != (k-1))
    {
        newpositionofzero = positionofzero + 1;
        // printf("Right, New zero Position: %d\n", newpositionofzero);
        // printf("Old Board\n");
        // printBoard(node->array, k);
        node->right = initializeNode(node->array, node, k);
        node->right->move = node->array[newpositionofzero]; // updating the move
        swapIndex(node->right->array, positionofzero, newpositionofzero);
        //printBoard(node->right->array, k);
    }
    //checking if down is possible
    if ((positionofzero + k) < (k*k))
    {
        newpositionofzero = positionofzero + k;
        // printf("Down New zero Position: %d\n", newpositionofzero);
        // printf("Old Board\n");
        // printBoard(node->array, k);
        node->down = initializeNode(node->array, node, k);
        node->down->move = node->array[newpositionofzero]; // updating the move
        swapIndex(node->down->array, positionofzero, newpositionofzero);
        //printBoard(node->down->array, k);
    }

     //checking if up is possible
    if ((positionofzero - k) >= 0)
    {
        newpositionofzero = positionofzero - k;
        // printf("moving up, new zero Position: %d\n", newpositionofzero);
        // printf("Old Board\n");
        // printBoard(node->array, k);
        node->up = initializeNode(node->array, node, k);
        node->up->move = node->array[newpositionofzero];//updating the move
        swapIndex(node->up->array, positionofzero, newpositionofzero);
        //printBoard(node->up->array, k);
    }
}

// function to backtrack and get the answer
// function to calculate the answer
int *calAnswer(int k)
{
    int *answer = malloc(sizeof(int) * k * k);
    for (int i = 0; i < (k * k) - 1; i++)
    {
        answer[i] = i + 1;
    }

    answer[(k * k) - 1] = 0;
    return answer;
}

// function for BFS
Node *BFS(Node *StartingNode, OpenHashTable *Hashtable, int k)
{
    Queue *Q = initializeQueue((size_t)100000000);
    enqueue(Q, StartingNode);//enqueue starting node
    int *answer = calAnswer(k);
    int count =1;
    while (!isEmpty(Q))
    {
        //printf("Node %d:\n",count);
        Node *u = dequeue(Q); //declaration of u inside the loop
        //printBoard(u->array,k);
        unsigned long pos = hashFunc(u->array, k, Hashtable->size);
        // printf("Node: %d\n", count);
        // printBoard(u->array, k);
        // Check if u is the solution
        if (memcmp(u->array, answer, sizeof(int) * k*k) == 0)
        {
            //printf("BFS Worked\n");
            return u; // Found the solution, return u
        }

        findNeighbors(u, k);

        if (u->up != NULL)
        {
            //printBoard(u->up->array, k);
            if (isMember(Hashtable, u->up->array, k, pos) == 0)
            {
                insertToHashTable(Hashtable,u->up, k, pos);
                enqueue(Q, u->up);
            }
        }
        if (u->down != NULL)
        {
           //printBoard(u->down->array, k);
           if (isMember(Hashtable, u->down->array, k,pos) == 0)
           {
               insertToHashTable(Hashtable, u->down, k, pos);
               enqueue(Q, u->down);
           }
        }
        if (u->right != NULL)
        {
            //printBoard(u->right->array, k);
            if (isMember(Hashtable, u->right->array, k, pos) == 0)
            {
                insertToHashTable(Hashtable, u->right, k, pos);
                enqueue(Q, u->right);
            }
        }
        if (u->left != NULL)
        {
            //printBoard(u->left->array, k);
            if (isMember(Hashtable, u->left->array, k, pos) == 0)
            {
                insertToHashTable(Hashtable, u->left, k, pos);
                enqueue(Q, u->left);
            }
        }
        count++;
    }
    printf("BFS worked\n");

    return NULL; // No solution found
}

//data structure to get the moves, i am using a stack
typedef struct stack
{
    int maxsize;
    int top;
    int *moves;

}Moves;

Moves* newStack(int size)
{
    Moves* myStack = malloc(sizeof(Moves));
    myStack->maxsize = size;
    myStack->top = -1;
    myStack->moves = malloc(sizeof(int) *size);
    return myStack;
}
int stacksize(Moves* myStack)
{
    return myStack->top + 1;
}

int stackEmpty(Moves* mystack)
{
    return (mystack->top == -1);
}

int stackFull(Moves* mystack)
{
    return(mystack->top == mystack->maxsize - 1);
}

void push(Moves* mystack, int x)
{
    if(stackFull(mystack)){
        printf("Stack Overflow!!\n");
        exit(-1);
    }

    mystack->top++;
    mystack->moves[mystack->top] = x;
}

int pop(Moves* stack){
    if (stackEmpty(stack)){
        printf("Stack is empty!!\n");
        exit(-1);
    }

    int x = stack->moves[stack->top];
    stack->top--;

    return x;
}

Moves* getMoves(Node* moves)
{
    Node* current = moves;
    Moves* answer = newStack(15);
    //if current.prev == NULL you are at the starting node
    while(current->previous != NULL){
        push(answer, current->move);
        current = current->previous;
    }

    // int stackSize = stacksize(answer);
    // for(int i = 0; i < stackSize; i++){
    //     printf("%d ", pop(answer));
    // }
    return answer;
}
int countInversions(int *puzzle, int k)
{
    int inversions = 0;
    for (int i = 0; i < k * k - 1; i++)
    {
        for (int j = i + 1; j < k * k; j++)
        {
            if (puzzle[i] && puzzle[j] && puzzle[i] > puzzle[j])
            {
                inversions++;
            }
        }
    }
    return inversions;
}

int isSolvable(int *arr, int k)
{
    int inv_count = 0;
    for (int i = 0; i < (k * k) - 1; i++)
        for (int j = i + 1; j < (k * k); j++)
            if (arr[i] > arr[j] && arr[j] && arr[i])
                inv_count++;

    // printf("invs: %d",inv_count);
    if (k % 2 != 0)
    {
        // printf("invs: %d",inv_count);
        return !(inv_count % 2 == 0);
    }
    else
    {
        int pos = floor(LinearSearch(arr, k * k) / k);
        return !((inv_count + pos) % 2 != 0);
    }
}

int main(int argc, char **argv)
{
    FILE *fp_in, *fp_out;

    fp_in = fopen(argv[1], "r");
    if (fp_in == NULL)
    {
        printf("Could not open a file.\n");
        return -1;
    }

    fp_out = fopen(argv[2], "w");
    if (fp_out == NULL)
    {
        printf("Could not open a file.\n");
        return -1;
    }

    char *line = NULL;
    size_t lineBuffSize = 0;
    size_t lineSize;
    size_t HashMemory = 5000;
    int k; // size of the board

    getline(&line, &lineBuffSize, fp_in); // ignore the first line in file, which is a comment
    fscanf(fp_in, "%d\n", &k);            // read size of the board
    //printf("k = %d\n", k);                // make sure you read k properly for DEBUG purposes
    getline(&line, &lineBuffSize, fp_in); // ignore the second line in file, which is a comment

    int initial_board[k * k]; // get kxk memory to hold the initial board
    for (int i = 0; i < k * k; i++){
        fscanf(fp_in, "%d ", &initial_board[i]);
    }
    // printBoard(initial_board, k);//Assuming that I have a function to print the board, print it here to make sure I read the input board properly for DEBUG purposes

    fclose(fp_in);

    ////////////////////
    // do the rest to solve the puzzle
    OpenHashTable *HashTable = initializeHashTable(HashMemory);
    Node *node = initializeNode(initial_board, NULL, k);
    node->move = -1;//no move was made at the starting point
    //printBoard(node->array, k);
    

    //check if a puzzle is solvable
    int check = isSolvable(node->array, k);
    //printf("%d\n", check);
    //
    //printBoard(moves->array, k);
    //printBoard(moves->previous->array, k);

    //
    ////////////////////

    // once you are done, you can use the code similar to the one below to print the output into file
    // if the puzzle is NOT solvable use something as follows
    // fprintf(fp_out, "#moves\n");
    // fprintf(fp_out, "no solution\n");
    if (check == 1)
    {
        fprintf(fp_out, "#moves\n");
        fprintf(fp_out, "no solution\n");
    }
    else
    {
        fprintf(fp_out, "#moves\n");
        Node *moves = BFS(node, HashTable, k);
        //printBoard(moves->array, k);
        //printf("last move made before solution: %d\n", moves->move);
        Moves* pMoves = getMoves(moves);
        int numberOfMoves = stacksize(pMoves);
        for (int i = 0; i < numberOfMoves; i++)
        {
            fprintf(fp_out, "%d ", pop(pMoves));
        }
    }

    // // if it is solvable, then use something as follows:
    //
    // // probably within a loop, or however you stored proper moves, print them one by one by leaving a space between moves, as below
    // // 
    // //     

    fclose(fp_out);
    free(HashTable);

    return 0;
}