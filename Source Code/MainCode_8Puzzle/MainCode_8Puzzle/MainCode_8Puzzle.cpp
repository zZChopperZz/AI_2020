// MainCode_8Puzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<ctime>
using namespace std;

enum class Actions { 
    left = 0,
    right = 1,
    up = 2,
    down = 3
};
string action_names[] = { "left","right","up","down" };
#define SIDE 3 // Length of the board 
typedef struct Node_array//should be tile
{
    int state[SIDE][SIDE];
    Actions action;
    struct Node_array* parent; // tree search

    int path_cost;
    int depth;
    struct Node_array* nextNode; // list


} Node;

typedef struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    Node* head;            //pointer to the first node in the list
    Node* tail;            //pointer to the last node in the list
};


void set_zero(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = 0;
}

void copy_array(int a[][SIDE], int b[][SIDE])//copy b to a
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = b[i][j];
}

int is_equal(int a[][SIDE], int b[][SIDE])
{
    int i, j, flag = 1;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            if (a[i][j] != b[i][j])
                flag = 0;
    return flag;
}

void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void print_array(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            printf("%d  | ", a[i][j]);
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
}

void print_array2(int a[][SIDE], int x, int y)
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
        {
            if ((i == x && j == y) || a[i][j] == 0)
            {
                printf("%d  | ", a[i][j]);
            }
            else
            {
                //printf("X | ");
                std::cout << "X  | ";
            }
        }
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
}

bool isLegalAction(Node* node, Actions action) {
    int index_i, index_j;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {
                index_i = i;
                index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    if (action == Actions::left && index_j == 0)  return false;
    if (action == Actions::right && index_j == 2)  return false;

    switch (index_i)
    {
    case 0:
        if (action == Actions::up) return false;
        return true;
    case 2:
        if (action == Actions::down) return false;
        return true;
    default:
        return true;
    }
    return true;
}

Node* Child_node(Node* node, Actions action) {
    Node* child = (Node*)malloc(sizeof(Node));
    int index_i, index_j;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {
                index_i = i;
                index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    copy_array(child->state, node->state);
    child->action = action;
    child->parent = node;
    child->path_cost = node->path_cost + 1;
    if (action == Actions::left) {
        swap(child->state[index_i][index_j - 1], child->state[index_i][index_j]);
    }
    else if (action == Actions::right) {
        swap(child->state[index_i][index_j + 1], child->state[index_i][index_j]);
    }
    else if (action == Actions::up) {
        swap(child->state[index_i - 1][index_j], child->state[index_i][index_j]);
    }
    else if (action == Actions::down) {
        swap(child->state[index_i + 1][index_j], child->state[index_i][index_j]);
    }

    return child;

    //can't free memory
}

NodeList* FIFO_initial() {
    NodeList* list;
    list = (NodeList*)malloc(sizeof(NodeList));
    list->nodeCount = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
    //can't free memory
}
void FIFO_add(NodeList* list, Node* node) {
    if (list->nodeCount <= 0) {
        list->head = node;
        list->tail = node;
        list->nodeCount += 1;
        return;
    }

    list->tail->nextNode = node;


    list->tail = node;
    list->nodeCount += 1;
}
Node* FIFO_pop(NodeList* list) {
    if (list->nodeCount <= 0) {
        return NULL;
    }
    Node* temp = list->head;
    list->nodeCount -= 1;
    if (list->nodeCount <= 0) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        list->head = temp->nextNode;
    }
    return temp;
}

bool Goal_test(Node* node, Node* goal)
{
    return is_equal(node->state, goal->state);
}

bool checkExist(NodeList* list, Node* node) {
    Node* _node;
    int i = 0;
    _node = list->head;
    while (i < list->nodeCount) {
        if (Goal_test(node, _node)) {
            return true;
        }
        _node = _node->nextNode;
        i++;
    }
    return false;
}


void Solution(Node* node) {
    printf("=======================");
    printf("\nSolution\n");
    while (node->parent != NULL) {

        print_array(node->state);
        printf("\n----------^---------\n");
        std::cout << "\naction: " << action_names[int(node->action)] << std::endl;
        printf("----------------------\n");
        node = node->parent;
    }
    print_array(node->state);

}

void breadthFirstSearch(Node* root, Node* goal) {
    Node* node = root;
    int Path_cost = 0;
    if (Goal_test(node, goal)) {
        Solution(node);
        return;
    }
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    int action;
    FIFO_add(frontier, node);
    do {
        node = FIFO_pop(frontier);
        FIFO_add(explorer, node);
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action)) {
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false || checkExist(frontier, child) == false) {
                    if (Goal_test(child, goal)) {
                        Solution(child);
                        return;
                    }
                    FIFO_add(frontier, child);
                    //free(child);
                }
            }
        }
    } while (frontier->nodeCount > 0);
}

bool Win = false;
void DFSChild(NodeList* explorer, Node* node, Node* goal)
{
    int action;
    for (action = 0; action < 4; action++) {
        if (isLegalAction(node, (Actions)action)) {
            Node* child = (Child_node(node, (Actions)action));
            if (checkExist(explorer, child) == false)
            {
                if (Goal_test(child, goal))
                {
                    Solution(child);
                    Win = true;
                    return;
                }
                FIFO_add(explorer, child);
                
                DFSChild(explorer, child, goal);
            }
            else
            {
                free(child);
            }

        }
        if (Win) break;
    }
}
void depthFirstSearch(Node* root, Node* goal) {
    Node* node = root;
    int Path_cost = 0;
    if (Goal_test(node, goal)) {
        Solution(node);
        return;
    }
    NodeList* frontier;
    NodeList* explorer;
    //frontier = FIFO_initial();
    explorer = FIFO_initial();
    //FIFO_add(frontier, node);
   // do {
        //node = FIFO_pop(frontier);
        FIFO_add(explorer, node);
        //action
        //for (action = 0; action < 4; action++) {
            DFSChild( explorer, node, goal);
        //}
        
        
    //} while (frontier->nodeCount > 0);
}


void LIFO_add(NodeList* list, Node* node)
{
    if (list->nodeCount <= 0)
    {
        list->head = node;
        list->tail = node;
        list->nodeCount += 1;
        return;
    }
    node->nextNode = list->head;
    list->head = node;
    list->nodeCount += 1;
}

Node* LIFO_pop(NodeList* list)
{
    if (list->nodeCount <= 0)
    {
        return NULL;
    }
    Node* temp = list->head;
    if (list->nodeCount <= 0)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = temp->nextNode;
    }
    list->nodeCount -= 1;
    return temp;
}

Node* RemoveNode(NodeList* list, Node* node)
{
    Node* n1 = list->head;
    Node* n2 = n1->nextNode;
    if (list->nodeCount <= 0)
    {
        return NULL;
    }
    for (int i = 0; i < list->nodeCount; i++)
    {
        if (is_equal(n2->state, node->state))
        {
            n1->nextNode = n2->nextNode;
        }
        n1 = n1->nextNode;
    }
}

int manhattanDist(Node* node, Node* goal)
{
    int x0, y0; //used for indexing each symbol in `curr`
    int x1, y1; //correspoinding row and column of symbol from curr[y0, x0] at `goal`
    int dx, dy; //change in x0 and x1, and y0 and y1, respectively
    int sum = 0;

    //for each symbol in `curr`
    for (y0 = 0; y0 < 3; ++y0) {
        for (x0 = 0; x0 < 3; ++x0) {
            //find the coordinates of the same symbol in `goal`
            for (y1 = 0; y1 < 3; ++y1) {
                for (x1 = 0; x1 < 3; ++x1) {
                    if (node->state[y0][x0] == goal->state[y1][x1]) {
                        dx = (x0 - x1 < 0) ? x1 - x0 : x0 - x1;
                        dy = (y0 - y1 < 0) ? y1 - y0 : y0 - y1;
                        sum += dx + dy;
                    }
                }
            }
        }
    }

    return sum;
}

void aStarSearch(Node* root, Node* goal)
{
    Node* node = root;
    int action;
    if (Goal_test(node, goal)) //kiển tra xem state hiện tại có bằng goal srate không
    {
        Solution(node);     // nếu bằng thì in ra
        return;
    }
    // nếu chưa bằng:
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    LIFO_add(frontier, node);   
    while (frontier->nodeCount > 0)
    {
        node = LIFO_pop(frontier);
        LIFO_add(explorer, node);       //Đổ từng node của frontier sang explorer
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action)) { // nếu di chuyển được
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false)   //kiểm tra có di chuyển lại bước trước đó
                {
                    if (checkExist(frontier, child) == false)   //kiểm tra có di chuyển lại bước ban đầu
                    {
                        if (Goal_test(child, goal))
                        {
                            Solution(child);
                            return;
                        }
                        FIFO_add(frontier, child);
                    }
                    else
                    {
                        int i = 0;
                        Node* temp = frontier->head;
                        while (i <= frontier->nodeCount - 1)
                        {
                            if (is_equal(temp->state, child->state) == 1 && (manhattanDist(child, goal) < manhattanDist(temp, goal)))
                            {
                                LIFO_add(frontier, child);
                                RemoveNode(frontier, temp);
                            }
                            temp = temp->nextNode;
                            i++;
                        }
                    }
                }
            }
        }
    } while (frontier->nodeCount > 0);
    printf("No solution");
}

void random(int& x1, int& x2, int& x3, int& x4, int& x5, int& x6, int& x7, int& x8)
{
    //x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 0;
    srand(time(NULL));
    x1 = rand() % 8 + 1;
    while (x2 == x1 || x2 == x3 || x2 == x4 || x2 == x5 || x2 == x6 || x2 == x7 || x2 == x8)
    {
        x2 = rand() % 8 + 1;
    }
    while (x3 == x1 || x3 == x2 || x3 == x4 || x3 == x5 || x3 == x6 || x3 == x7 || x3 == x8)
    {
        x3 = rand() % 8 + 1;
    }
    while (x4 == x1 || x4 == x2 || x4 == x3 || x4 == x5 || x4 == x6 || x4 == x7 || x4 == x8)
    {
        x4 = rand() % 8 + 1;
    }
    while (x5 == x1 || x5 == x2 || x5 == x3 || x5 == x4 || x5 == x6 || x5 == x7 || x5 == x8)
    {
        x5 = rand() % 8 + 1;
    }
    while (x6 == x1 || x6 == x2 || x6 == x3 || x6 == x4 || x6 == x5 || x6 == x7 || x6 == x8)
    {
        x6 = rand() % 8 + 1;
    }
    while (x7 == x1 || x7 == x2 || x7 == x3 || x7 == x4 || x7 == x5 || x7 == x6 || x7 == x8)
    {
        x7 = rand() % 8 + 1;
    }
    while (x8 == x1 || x8 == x2 || x8 == x3 || x8 == x4 || x8 == x5 || x8 == x6 || x8 == x7 || x8 == 0)
    {
        x8 = rand() % 8 + 1;
    }
}

int main()
{
    int x0 = 0, x1 = 4, x2 = 3, x3 = 2, x4 = 1, x5 = 5, x6 = 6, x7 = 7, x8 = 8;
    //random(x1, x2, x3, x4, x5, x6, x7, x8);
    int a[SIDE][SIDE] =
    {
        {x0,x1,x2},
        {x3,x4,x5},
        {x6,x7,x8}
    };

    Node* Goal, * Start;
    int i, j ,r = 0;
    int goalstate[9] = { 0,1,2,3,4,5,6,7,8 };
    Goal = (Node*)malloc(sizeof(Node));
    Start = (Node*)malloc(sizeof(Node));
    printf("The goal state\n");
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
        {
            Goal->state[i][j] = goalstate[r];
            r++;
        }
    printf("Goal state:\n");
    print_array(Goal->state);

    Goal->parent = NULL;
    Goal->depth = 0;



    printf("Enter the current state\n");
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            Start->state[i][j] = a[i][j];
    Start->parent = NULL;
    Start->path_cost = 0;
    printf("Start state:\n");
    print_array2(Start->state,0,1);
    cout << endl;
    cout << "Start state Add:" << endl;
    print_array(Start->state);

    //call search function
    //breadthFirstSearch(Start, Goal);
    //depthFirstSearch(Start, Goal);
    aStarSearch(Start, Goal);

    // free 
    free(Goal);
    free(Start);
    return 0;
}