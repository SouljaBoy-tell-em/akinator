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
int AddObject (Tree * tree);
void consistentAllocatingMemory (Node ** currentNode);
int dump (Node * currentNode, FILE * dumpFile);
unsigned long FileSize (FILE * infoTree);
void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces);
int getMainInfoFile (Tree * tree, FILE * infoTree);
int InitializeTree (Tree * tree, FILE * infoTree);


int main (void) {

	Tree tree = {};

	FILE * infoTree = NULL;
	FILE * dumpFile = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");


	while (true)
		if (AddObject (&tree) == 228)
			break;


	CHECK_ERROR(dump (tree.head, dumpFile), "Problem with record in the tree.\n");


	return ERROR_OFF;
}


void addAnswer (Node * lastNode) {

	printf ("Is it %s?(y/n)\n", lastNode->data);

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
		printf ("\nHow does %s differ from %s? Input: ", trueAnswer, lastNode->data);
		scanf ("%s", difference);

		lastNode->left =  (Node * ) malloc (sizeof (Node));
		(lastNode->left)->data = (char * ) malloc (MAXLENTITLE * sizeof (char));
		lastNode->right = (Node * ) malloc (sizeof (Node));
		(lastNode->right)->data = (char * ) malloc (MAXLENTITLE * sizeof (char));

		strcpy ((lastNode->right)->data, lastNode->data);
		strcpy (lastNode->data         , difference);
		strcpy ((lastNode->left)->data , trueAnswer);
		(lastNode->left)->left   = NULL;
		(lastNode->left)->right  = NULL;
		(lastNode->right)->left  = NULL;
		(lastNode->right)->right = NULL;

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

	else if (!strcmp (answer, "#"))
		return 228;

	else {

		printf ("ANS: %s\n\n\n", answer);
		printf ("Input fake answer! Please input (y/n).\n");
		return ERROR_ON;
	}

	return ERROR_OFF;
}


int AddObject (Tree * tree) {

	return AddNode (tree->head);

	//tree->size++;
}


int dump (Node * currentNode, FILE * dumpFile) {

	dumpFile = fopen (DUMPFILE, "w");
	CHECK_ERROR(!dumpFile, "Problem with opening tree.txt");
	
	fullPrint (currentNode, dumpFile, 4);
	fclose (dumpFile);

	return ERROR_OFF;
}


unsigned long FileSize (FILE * infoTree) {

    struct stat buf = {};
    if (fstat (fileno (infoTree), &buf) == 0)
        return buf.st_size;

    return 0;
}


void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces) {

		fprintf (dumpFile, "\n%*s", amountSpaces, "{");
		fprintf (dumpFile, " %s", currentNode->data);

		if (currentNode->left)
			fullPrint (currentNode->left,  dumpFile, amountSpaces + 4);

		if (currentNode->right)
			fullPrint (currentNode->right, dumpFile, amountSpaces + 4);

		if (!currentNode->left && !currentNode->right)
			amountSpaces = 2;

		fprintf (dumpFile , "%*s\n", amountSpaces, "}");
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d", &(tree->size));

	return ERROR_OFF;
}


int InitializeTree (Tree * tree, FILE * infoTree) {

	CHECK_ERROR(getMainInfoFile (tree, infoTree), "Problem with getting data from file info.txt .\n");
	
	if (tree->size == 0) {

		Node * memObject = (Node * ) malloc (sizeof (Node));
		memObject->data  = (char * ) malloc (MAXLENTITLE * sizeof (char));

		strcpy (memObject->data, HEADOBJECT);
		memObject->left = NULL;
		memObject->right = NULL;
		tree->head = memObject;
		tree->size++;
	}

	return ERROR_OFF;
}

