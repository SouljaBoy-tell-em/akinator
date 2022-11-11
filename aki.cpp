#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


enum error_code {

    ERROR_OFF,
    ERROR_ON
};


#define MAXLENANSWER				10
#define MAXLENTITLE 			   100
#define HEADOBJECT 		 "poltorashka"
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

} Tree;


void addAnswer (Node * head);
int AddNode (Node * head);
int AddObject (Tree * tree, FILE * dump);
unsigned long FileSize (FILE * infoTree);
void fullExplore (Node * x);
int getMainInfoFile (Tree * tree, FILE * infoTree);
int InitializeTree (Tree * tree, FILE * infoTree);


int main (void) {

	Tree tree = {};

	FILE * infoTree = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");
	
	FILE * dump = NULL;

	while (true)
		AddObject (&tree, dump);

	return ERROR_OFF;
}


void addAnswer (Node * head) {

	printf ("Is it %s?(y/n)\n", head->data);

	char answer [MAXLENANSWER] = " ";
	scanf ("%s", answer);

	if (!strcmp (answer, "y")) {

		printf ("Yes, I won!\n\n");
		return;
	}

	else if (!strcmp (answer, "n")) {

		char trueAnswer [MAXLENTITLE];
		char difference [MAXLENTITLE];

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

		return;
	}

	else
		printf ("Input fake answer! Please input (y/n).\n");
}


int AddNode (Node * currentNode) {

	if (currentNode->left == NULL && currentNode->right == NULL) {

		addAnswer (currentNode);
		return ERROR_OFF;
	}

	printf ("%s(y/n):\n", currentNode->data);
	char answer [MAXLENANSWER]= " ";
	scanf ("%s", answer);
		
	if (!strcmp (answer, "y")) 
		AddNode (currentNode->left);

	else if (!strcmp (answer, "n"))
		AddNode (currentNode->right);

	else {

		printf ("ANS: %s\n\n\n", answer);
		printf ("Input fake answer! Please input (y/n).\n");
		return ERROR_ON;
	}

	return ERROR_OFF;
}


int AddObject (Tree * tree, FILE * dump) {

	AddNode (tree->head);
	tree->size++;

	return ERROR_OFF;
}


unsigned long FileSize (FILE * infoTree) {

    struct stat buf = {};
    if (fstat (fileno (infoTree), &buf) == 0)
        return buf.st_size;

    return 0;
}


void fullExplore (Node * currentNode) {

	if (currentNode != NULL) {

		printf ("Title: ");
		printf ("%s\n", currentNode->data);

		fullExplore (currentNode->left );
		fullExplore (currentNode->right);
	}
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d", &(tree->size));

	return ERROR_OFF;
}


int InitializeTree (Tree * tree, FILE * infoTree) {

	CHECK_ERROR(getMainInfoFile (tree, infoTree), "Problem with getting data from file info.txt .\n");

	Node * memObject = (Node * ) malloc (sizeof (Node));
	memObject->data  = (char * ) malloc (MAXLENTITLE * sizeof (char));

	strcpy (memObject->data, HEADOBJECT);
	memObject->left = NULL;
	memObject->right = NULL;
	tree->head = memObject;
	tree->size++;

	return ERROR_OFF;
}

