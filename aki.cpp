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
	struct node * parent;
	struct node *  left;
	struct node * right;
} Node;

typedef struct {

	Node *  head;
	int     size;

} Tree;


void getDataFromFile (FILE * dumpFile, Tree * tree, char * mem);
void addAnswer (Node * head);
int AddNode (Node * head);
int AddObject (Tree * tree);
void consistentAllocatingMemory (Node ** currentNode);
int dump (Tree * tree, FILE * dumpFile, FILE * infoFile);
unsigned long FileSize (FILE * infoTree);
void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces);
void getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem);
int getMainInfoFile (Tree * tree, FILE * infoTree);
void InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);
int InitializeTree (Tree * tree, FILE * infoTree);
void fullExplore (Node * x);


int main (void) {

	Tree tree = {};

	FILE * infoTree = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");

	char * mem = NULL;
	FILE * dumpFile = NULL;
	getDataFromFile (dumpFile, &tree, &mem);

	while (true)
		if (AddObject (&tree) == 228)
			break;

	CHECK_ERROR(dump (&tree, dumpFile, infoTree), "Problem with record in the tree.\n");

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

	tree->size++;
	return AddNode (tree->head);
}


int dump (Tree * tree, FILE * dumpFile, FILE * infoFile) {

	dumpFile = fopen (DUMPFILE, "w");
	CHECK_ERROR(!dumpFile, "Problem with opening tree.txt");
	infoFile = fopen (INFOFILE, "w");
	CHECK_ERROR(!infoFile, "Problem with opening info.txt");
	
	fullPrint (tree->head, dumpFile, 4);
	fprintf (infoFile, "%d", tree->size);
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


void getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem) {

	dumpFile = fopen ("tree.txt", "r");
	int fileSize = FileSize (dumpFile);

	InitializeNode (&(tree->head), dumpFile, NULL);
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d", &(tree->size));

	return ERROR_OFF;
}


void InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char bracketBuffer [MAXLENTITLE], answerBuffer [MAXLENTITLE];

	fscanf (dumpFile, "%s", bracketBuffer);

	if (!strcmp (bracketBuffer, "{")) {

		* currentNode = (Node * ) malloc (sizeof (Node));

		( * currentNode)->data = (char * ) malloc (MAXLENTITLE * sizeof (char));
		fscanf (dumpFile, "%s", answerBuffer);

		( * currentNode)->right = NULL;
		( * currentNode)->left = NULL;
		( * currentNode)->parent = parentCurrentNode;
		strcpy (( * currentNode)->data, answerBuffer);

		InitializeNode (&( * currentNode)->left,  dumpFile, * currentNode);
		InitializeNode (&( * currentNode)->right, dumpFile, * currentNode);
	}

	if (!strcmp (bracketBuffer, "}")) {

		ungetc ('}', dumpFile);
		return;
	}

	fscanf (dumpFile, "%s", bracketBuffer);
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

