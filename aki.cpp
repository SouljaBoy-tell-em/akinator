#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


enum error_code {

    ERROR_OFF,
    ERROR_ON
};


#define MAXLENTITLE 			   100
#define HEADOBJECT 		 		 "pet"
#define DUMPFILE   		    "tree.txt"
#define INFOFILE 			"info.txt"
#define SIZECOEFFICIENT 			 2
#define STARTSIZETREE   			 3
#define CHECK_ERROR(condition, message_error) 		      \
            do {                                          \
               if  (condition) {                          \
                   printf ("%s", message_error);          \
                   return ERROR_ON;                       \
               }                                          \
            } while(false)


typedef struct node {

	char        * data;
	struct node *  left;
	struct node * right;
} Node;

typedef struct {

	Node *  head;
	int     size;
	int capacity;

} Tree;


void addAnswer (Node * head);
int AddNode (Node * head);
int AddObject (Tree * tree, FILE * dump);
char answer (char * currentNodeData);
unsigned long FileSize (FILE * infoTree);
void fullExplore (Node * x);
int getMainInfoFile (Tree * tree, FILE * infoTree);
int InitializeTree (Tree * tree, FILE * infoTree);


int main (void) {

	Tree tree = {};

	FILE * infoTree = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");
	
	FILE * dump = NULL;


	AddObject (&tree, dump);
	AddObject (&tree, dump);
	AddObject (&tree, dump);
	AddObject (&tree, dump);
	AddObject (&tree, dump);


	//fullExplore (tree.head);

	/*

	Node * LEFT = tree.head;

	while (LEFT != NULL) {

		printf ("%s\n", LEFT->data);
		LEFT = LEFT->left;
	}

	*/

	return 0;
}


void addAnswer (Node * head) {

	char trueAnswer [MAXLENTITLE];
	char difference [MAXLENTITLE];
	char answer = '\0';

	answer = getchar ();
	while (getchar () != '\n')
		continue;

	if (answer == 'y') {

		printf ("Yes, I won!\n\n");
		return;
	}

	if (answer == 'n') {

		printf ("Who is it? Input: ");
		scanf ("%s", trueAnswer);
		printf ("\nHow does %s differ from %s? Input: ", trueAnswer, head->data);
		scanf ("%s", difference);

		head->left =  (Node * ) malloc (sizeof (Node));
		(head->left)->data = (char * ) malloc (MAXLENTITLE * sizeof (char));
		head->right = (Node * ) malloc (sizeof (Node));
		(head->right)->data = (char * ) malloc (MAXLENTITLE * sizeof (char));

		strcpy ((head->right)->data, head->data);
		strcpy (head->data         , difference);
		strcpy ((head->left)->data , trueAnswer);
		(head->left)->left   = NULL;
		(head->left)->right  = NULL;
		(head->right)->left  = NULL;
		(head->right)->right = NULL;
	}

}


int AddNode (Node * head) {

	if (answer (head->data) == 'y') {

		if (head->left == NULL && head->right == NULL) {

			addAnswer (head);
			return 0;
		}

		else 
			AddNode (head->left);
	}

	if (answer (head->data) == 'n') {

		if (head->right == NULL && head->left == NULL) {

			addAnswer (head);
			return 0;
		}

		else
			AddNode (head->right);
	}

	return 0;
}


int AddObject (Tree * tree, FILE * dump) {

	dump = fopen (DUMPFILE, "w");
	CHECK_ERROR(!dump, "Problem with opening file tree.txt .\n");


	if (tree->size == 0) {

		Node * memObject = (Node * ) malloc (sizeof (Node));
		memObject->data  = (char * ) malloc (MAXLENTITLE * sizeof (char));
		Node * memObject1 = (Node * ) malloc (sizeof (Node));
		memObject1->data  = (char * ) malloc (MAXLENTITLE * sizeof (char));
		Node * memObject2 = (Node * ) malloc (sizeof (Node));
		memObject2->data  = (char * ) malloc (MAXLENTITLE * sizeof (char));

		strcpy (memObject->data, HEADOBJECT);
		memObject->left = NULL;
		memObject->right = NULL;

		strcpy (memObject1->data, "poltorashka");
		memObject1->left = NULL;
		memObject1->right = NULL;
		
		strcpy (memObject2->data, "man");
		memObject2->left = NULL;
		memObject2->right = NULL;

		tree->head = memObject;
		(tree->head)->left = memObject1;
		(tree->head)->right = memObject2;

		tree->size += 3;

		return ERROR_OFF;
	}

	else {

		AddNode (tree->head);
		tree->size++;
	}

	return ERROR_OFF;
}


char answer (char * currentNodeData) {

	char answer = '\0';
	printf ("%s (y/n):\n", currentNodeData);
	answer = getchar ();
	while (getchar () != '\n')
		continue;

	return answer;
}


void dump (FILE * dumpFile) {



}


unsigned long FileSize (FILE * infoTree) {

    struct stat buf = {};
    if (fstat (fileno (infoTree), &buf) == 0)
        return buf.st_size;

    return 0;
}


void fullExplore (Node * x) {

	if (x != NULL) {

		printf ("Title: ");
		puts (x->data);
		putchar ('\n');

		fullExplore (x->left);
		fullExplore (x->right);
	}
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d %d", &(tree->size), &(tree->capacity));

	return ERROR_OFF;
}


int InitializeTree (Tree * tree, FILE * infoTree) {

	CHECK_ERROR(getMainInfoFile (tree, infoTree), "Problem with getting data from file info.txt .\n");

	if (tree->size == 0) {

		tree->capacity = STARTSIZETREE;
		tree->head = NULL;
	}

	return ERROR_OFF;
}
