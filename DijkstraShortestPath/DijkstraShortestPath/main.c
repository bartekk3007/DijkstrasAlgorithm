#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct ListHeadNode
{
	char value;
	struct ListHeadNode* left;
	struct ListHeadNode* right;
	int distance;
	struct ListHeadNode* down;
};

struct TreeNode
{
	char value;
	struct TreeNode* left;
	struct TreeNode* right;
};

struct distanceNode
{
	char value;
	int distance;
	char previous;
	struct distanceNode* next;
};

struct distanceNode* createDistanceNode(char n)
{
	struct distanceNode* vertex = (struct distanceNode*) malloc(sizeof(struct distanceNode));
	if (vertex)
	{
		vertex->value = n;
		vertex->distance = INT_MAX;
		vertex->previous = '-';
		vertex->next = NULL;
	}
	return vertex;
}

void insertDistanceNode(struct distanceNode** distArray, char n)
{
	if (*distArray == NULL)
	{
		*distArray = createDistanceNode(n);
	}
	else
	{
		insertDistanceNode(&(*distArray)->next, n);
	}
}

struct distanceNode* findDistanceNode(struct distanceNode** distArray, char n)
{
	if (*distArray == NULL)
	{
		return NULL;
	}
	else if ((*distArray)->value == n)
	{
		return *distArray;
	}
	else
	{
		findDistanceNode(&(*distArray)->next, n);
	}
}

void updateDistanceNode(struct distanceNode** distArray, char n, int dist)
{
	if (*distArray == NULL)
	{
		return;
	}
	else if ((*distArray)->value == n)
	{
		(*distArray)->distance = dist;
	}
	else
	{
		updateDistanceNode(&(*distArray)->next, n, dist);
	}
}

void updatePreviousNode(struct distanceNode** distArray, char n, char prev)
{
	if (*distArray == NULL)
	{
		return;
	}
	else if ((*distArray)->value == n)
	{
		(*distArray)->previous = prev;
	}
	else
	{
		updatePreviousNode(&(*distArray)->next, n, prev);
	}
}

struct TreeNode* createTreeNode(char value)
{
	struct TreeNode* vertex = (struct TreeNode*) malloc(sizeof(struct TreeNode));
	if (vertex)
	{
		vertex->value = value;
		vertex->left = NULL;
		vertex->right = NULL;
	}
	return vertex;
}

void insertVisited(struct TreeNode** visited, char value)
{
	if (*visited == NULL)
	{
		*visited = createTreeNode(value);
	}
	else
	{
		if (value <= (*visited)->value)
		{
			insertVisited(&(*visited)->left, value);
		}
		else
		{
			insertVisited(&(*visited)->right, value);
		}
	}
}

bool isVisited(struct TreeNode** visited, char value)
{
	if (*visited == NULL)
	{
		return false;
	}
	else if ((*visited)->value == value)
	{
		return true;
	}
	else
	{
		if (value <= (*visited)->value)
		{
			isVisited(&(*visited)->left, value);
		}
		else
		{
			isVisited(&(*visited)->right, value);
		}
	}
}

struct ListHeadNode* createNode(char n)
{
	struct ListHeadNode* vertex = (struct ListHeadNode*) malloc(sizeof(struct ListHeadNode));
	if (vertex)
	{
		vertex->value = n;
		vertex->left = NULL;
		vertex->right = NULL;
		vertex->distance = 0;
		vertex->down = NULL;
	}
	return vertex;
}

void insertNode(struct ListHeadNode** array, char n)
{
	if (*array == NULL)
	{
		*array = createNode(n);
	}
	else if ((*array)->right == NULL)
	{
		(*array)->right = createNode(n);
		(*array)->right->left = (*array);
	}
	else
	{
		insertNode(&(*array)->right, n);
	}
}

struct ListHeadNode* createChild(char n, int dist)
{
	struct ListHeadNode* vertex = (struct ListHeadNode*) malloc(sizeof(struct ListHeadNode));
	if (vertex)
	{
		vertex->value = n;
		vertex->left = NULL;
		vertex->right = NULL;
		vertex->distance = dist;
		vertex->down = NULL;
	}
	return vertex;
}

void insertChild(struct ListHeadNode** array, char m, int distance)
{
	if ((*array)->down == NULL)
	{
		(*array)->down = createChild(m, distance);
	}
	else
	{
		insertChild(&(*array)->down, m, distance);
	}
}

struct ListHeadNode* findHeadNode(struct ListHeadNode** array, char n)
{
	while ((*array)->left)
	{
		(*array) = (*array)->left;
	}

	while ((*array))
	{
		if ((*array)->value == n)
		{
			return *array;
		}
		else
		{
			(*array) = (*array)->right;
		}
	}

	return NULL;
}

void insertSingleEdge(struct ListHeadNode** array, char n, char m, int distance)
{
	if (*array == NULL)
	{
		return;
	}
	else if ((*array)->value == n)
	{
		insertChild(array, m, distance);
	}
	else
	{
		insertSingleEdge(&(*array)->right, n, m, distance);
	}
}

void insertEdge(struct ListHeadNode** array, char n, char m, int distance)
{
	insertSingleEdge(array, n, m, distance);
	insertSingleEdge(array, m, n, distance);
}

void printList(struct ListHeadNode* array)
{
	while (array != NULL)
	{
		printf("Wierzcholek %c: ", array->value);
		struct ListHeadNode* bottom = array->down;
		while (bottom != NULL)
		{
			printf("%c ", bottom->value);
			printf("(%d) ", bottom->distance);
			bottom = bottom->down;
		}
		printf("\n");
		array = array->right;
	}
}

void appendDistArray(struct ListHeadNode** array, struct distanceNode** distArray)
{
	struct ListHeadNode* temp = *array;
	while (temp)
	{
		insertDistanceNode(distArray, temp->value);
		temp = temp->right;
	}
}

void fillStaringNode(struct ListHeadNode** array, struct distanceNode** distArray)
{
	if ((*array)->value == (*distArray)->value)
	{
		(*distArray)->distance = 0;
	}
	else
	{
		fillStaringNode(array, &(*distArray)->next);
	}
}

struct ListHeadNode* smallestDistanceNode(struct ListHeadNode** array, struct distanceNode** distArray, struct TreeNode** visited)
{
	struct ListHeadNode* smallestVertex = NULL;
	int maxdist = INT_MAX;

	struct distanceNode* distanceTable = *distArray;
	while (distanceTable)
	{
		if (!isVisited(visited, distanceTable->value) && distanceTable->distance <= maxdist)
		{
			smallestVertex = findHeadNode(array, distanceTable->value);
			maxdist = distanceTable->distance;
		}
		distanceTable = distanceTable->next;
	}

	return smallestVertex;
}

void updateDistances(struct ListHeadNode* smallestVertex, struct distanceNode** distArray, struct TreeNode** visited)
{
	struct ListHeadNode* parentNode = smallestVertex;
	while (parentNode->down)
	{
		parentNode = parentNode->down;
		if (!isVisited(visited, parentNode->value))
		{
			int dist = findDistanceNode(distArray, smallestVertex->value)->distance + parentNode->distance;
			if (dist < findDistanceNode(distArray, parentNode->value)->distance)
			{
				updateDistanceNode(distArray, parentNode->value, dist);
				updatePreviousNode(distArray, parentNode->value, smallestVertex->value);
			}
		}
	}
}

void dijkstraSingle(struct ListHeadNode** array, struct distanceNode** distArray, struct TreeNode** visited)
{
	struct ListHeadNode* smallestVertex = smallestDistanceNode(array, distArray, visited);
	if (smallestVertex)
	{
		updateDistances(smallestVertex, distArray, visited);
		insertVisited(visited, smallestVertex->value);
		dijkstraSingle(array, distArray, visited);
	}
}

void printDistanceArray(struct distanceNode* distArray)
{
	struct distanceNode* headDist = distArray;
	while (headDist)
	{
		printf("%c ", headDist->value);
		printf("%3d ", headDist->distance);
		printf("%c ", headDist->previous);
		printf("\n");
		headDist = headDist->next;
	}
}

void dijkstra(struct ListHeadNode** array)
{
	struct TreeNode* visited = NULL;
	struct distanceNode* distArray = NULL;
	appendDistArray(array, &distArray);
	fillStaringNode(array, &distArray);

	dijkstraSingle(array, &distArray, &visited);

	printDistanceArray(distArray);
}

int main(void)
{
	struct ListHeadNode* array = NULL;

	insertNode(&array, 'A');
	insertNode(&array, 'B');
	insertNode(&array, 'C');
	insertNode(&array, 'D');
	insertNode(&array, 'E');

	/*
	insertEdge(&array, 'B', 'C', 5);
	insertEdge(&array, 'D', 'E', 7);
	insertEdge(&array, 'C', 'D', 10);
	insertEdge(&array, 'C', 'A', 8);
	*/

	insertEdge(&array, 'A', 'B', 6);
	insertEdge(&array, 'A', 'D', 1);
	insertEdge(&array, 'B', 'C', 5);
	insertEdge(&array, 'B', 'D', 2);
	insertEdge(&array, 'B', 'E', 2);
	insertEdge(&array, 'C', 'E', 5);
	insertEdge(&array, 'D', 'E', 1);
	
	printList(array);

	dijkstra(&array);

	return 0;
}