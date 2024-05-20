#include <stdio.h>
#include <string.h>

typedef enum {Sun, HSun, Shade} Location;
typedef enum {false, true} boolean;


typedef struct Tree {
	char* name;
	Location loc;
	int price;
	int* myTrees; //Array of integers representing different trees
	int numT; //Amount of trees
} Tree;


typedef struct treeItem {
	Tree* myT;
	struct treeItem* prev, * next;
} treeItem;


treeItem* createList(int n);
int addTreeType(treeItem* list, const char* name, Location loc, int price, int x);
Tree* createTree(const char* name, Location loc, int prince);

int main() {

}

treeItem* createList(int n) {
	//The function gets an integer and returns a treeItem list of NULL in size n
	treeItem* list, * help, * last;

	if (n < 1) //Invalid size of list;
		return NULL;

	//Creating the first Tree
	list = (treeItem*)malloc(sizeof(treeItem));
	list->myT = NULL;
	list->prev = NULL;
	list->next = NULL;

	if (n == 1)
		return list;

	help = (treeItem*)malloc(sizeof(treeItem));
	help->myT = NULL;
	help->next = NULL;

	list->next = help;
	help->prev = list;

	for (int i = 3; i <= n; i++) {
		last = (treeItem*)malloc(sizeof(treeItem));
		last->myT = NULL;
		last->next = NULL;

		last->prev = help;
		help->next = last;
		help = last;
	}
	return list;
}

int addTreeType(treeItem* list, const char* name, Location loc, int price, int x) {
	/*
	The function gets description of a new tree and adds it to the first open spot in the list, if the list is full
	it adds x elements of treeItem and adds the tree in the first cell open
	*/
	
	treeItem* p = list; //To keep list on the same element
	treeItem* newList = NULL;
	boolean isThereSpace = false;

	while (p) {
		if (p->myT && !strcmp(p->myT->name, name))
			return -1;
		p = p->next;
	}

	p = list;
	while (p->next) {
		if (!p->myT) {
			isThereSpace = true;
			break;
		}
		p = p->next;
	}
	

	if (!p->myT)
		isThereSpace = true;


	if (isThereSpace) {
		p->myT = createTree(name, loc, price);
		return 0;
	}

	newList = createList(x);
	newList->prev = p;
	p->next = newList;

	newList->myT = createTree(name, loc, price);
	return 0;	
}

Tree* createTree(const char* name, Location loc, int price) {
	Tree* myTree = (Tree*)malloc(sizeof(Tree));
	myTree->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	myTree->loc = loc;
	myTree->price = price;
	myTree->numT = 0;
	myTree->myTrees = NULL;
	return myTree;
}