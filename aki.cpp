#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


enum error_code {

    ERROR_OFF,
    ERROR_ON
};

enum graph_dump_commands {

	HEAD,
	FIRSTLY
};


const char * YES = "y";
const char * NO =  "n";

#define STACKSIZE										  30
#define FRAME "############################################"
#define POISON										   "###"
#define CANARY									   "##END##"
#define MAXLENVARIANT         						      10
#define MAXLENANSWER							     	  10
#define MAXLENTITLE 			   						 100
#define HEADOBJECT 							   "poltorashka"
#define DUMPFILE   		 					      "tree.txt"
#define INFOFILE 								  "info.txt"
#define SIZECOEFFICIENT 								  2
#define STARTSIZETREE   								  3
#define CHECK_ERROR(condition, message_error) 		      \
            do {                                          \
               if  (condition) {                          \
                   printf ("%s", message_error);          \
                   return ERROR_ON;                       \
               }                                          \
            } while(false)

#define DEF_CMD(name, num, ...)							  \
            CMD_##name = num,


enum COMMANDS {

	#include "commands.h"
};
#undef DEF_CMD


typedef struct {

	char * answer;
	char **  data;
	int 	 size;
	int  capacity;
} Stack;

typedef struct node {

	char        *   data;
	struct node * parent;
	struct node *   left;
	struct node *  right;
} Node;

typedef struct {

	Node *      head;
	Stack      stack;
	Stack  nextStack;
	int         size;
} Tree;


void AddObject (Tree * tree);
int addAnswer (Node * lastNode, int * size, Stack * stack);
int AddNode (Node * currentNode, int * size, Stack * stack);
int compare2Characters (Tree * tree);
int dump (Tree * tree, FILE * dumpFile, FILE * infoFile);
int existenceCheck (Tree * tree);
void exploreObject (Node * currentNode, char * object, bool * flagExplore);
unsigned long FileSize (FILE * infoTree);
void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces);
int getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem);
int getMainInfoFile (Tree * tree, FILE * infoTree);
int graphDump (Node * head);
void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump);
void infoLastCharacter (Tree * tree);
int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);
int InitializeTree (Tree * tree, FILE * infoTree);
int menu (Tree * tree);


int StackCtor (Stack * stack, int capacity);
void StackPush (Stack * stack, const char * sign, char * answer);
void StackClean (Stack * stack);
char * StackPop (Stack * stack);


int main (void) {

	Tree tree = {};
	FILE * infoTree = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");

	char * mem = NULL;
	FILE * dumpFile = NULL;
	getDataFromFile (dumpFile, &tree, &mem);

	int answer = 0;
	while ((answer = menu (&tree))) {

		#define DEF_CMD(name, num, ...)		 \
			case num:						 \
				__VA_ARGS__					 \
				break;

		switch (answer) {

			#include "commands.h"
			#undef DEF_CMD

			default: break;
		}
	}
	CHECK_ERROR(dump (&tree, dumpFile, infoTree), "Problem with record in the tree.\n");

	graphDump (tree.head);

	return ERROR_OFF;
}


void AddObject (Tree * tree) {

	if ((tree->stack).size) {

		if ((tree->nextStack).size) {

			tree->stack = tree->nextStack;
			StackClean(&(tree->nextStack));
		}

		AddNode (tree->head, &(tree->size), &(tree->nextStack));
		return;
	}

	AddNode (tree->head, &(tree->size), &(tree->stack));
}


int compare2Characters (Tree * tree) {

	if (!(tree->stack).size || !(tree->stack).size) {

		printf ("You didn't input 2 objects.\n");
		return ERROR_OFF;
	}

	int minAmountFeatures = 		((tree->stack).size <= (tree->nextStack).size) ? 
									 (tree->stack).size :  (tree->nextStack).size  ;

	char ** listFeatures  = (char ** ) calloc (minAmountFeatures, sizeof (char * ));
	CHECK_ERROR(!listFeatures, "Problem with allocating memory for listFeatures. ");

	printf ("Similarities:\n");
	int i = 0;
	for (i = minAmountFeatures; i >= 0; i--)
		if (!strcmp ((tree->stack).data[i], (tree->nextStack).data[i]))
			printf ("%s\n", (tree->stack).data[i]);

	StackClean (&(tree->stack));
	StackClean (&(tree->nextStack));
	free (listFeatures);

	return ERROR_OFF;
}


int existenceCheck (Tree * tree) {

	char * object = (char * ) calloc (MAXLENTITLE, sizeof (char));
	CHECK_ERROR(!object, "Problem with allocating memory for object.");

	bool flagExplore = false;
	printf ("Input desired object:\n");
	scanf ("%s", object);
	exploreObject (tree->head, object, &flagExplore);

	if (flagExplore)
		printf ("This object contains in the tree.     \n");

	else
		printf ("This object don't contain in the tree.\n");

	free (object);

	return ERROR_OFF;
}


void exploreObject (Node * currentNode, char * object, bool * flagExplore) {

	if (currentNode != NULL) {

		if (!strcmp (currentNode->data, object)) {

			* flagExplore = true;
			return;
		}

		exploreObject (currentNode->left , object, flagExplore);
		exploreObject (currentNode->right, object, flagExplore);
	}
}


int graphDump (Node * head) {

	FILE * graphDumpFile = fopen ("graphDump.txt", "w");
	CHECK_ERROR(!graphDumpFile, "Problem with opening graphDump.txt.\n");

	int commandGraphDump = HEAD;
	fprintf (graphDumpFile, "digraph G {\n rankdir=L\n");
	graphDumpDrawNode (head, graphDumpFile, &commandGraphDump);
	fprintf (graphDumpFile, "}");

	fclose (graphDumpFile);

	return ERROR_OFF;
}


void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump) {

	if (currentNode != NULL) {

		if (( * commandGraphDump) != HEAD) {

			if (( * commandGraphDump) == FIRSTLY)
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",  (currentNode->parent)->data, "orange", (currentNode->parent)->data);

			if ((currentNode->parent)->left == currentNode)
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",  currentNode->data, "green",  currentNode->data);

			if ((currentNode->parent)->right == currentNode)
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",  currentNode->data, "red",    currentNode->data);

			fprintf (graphDumpFile, "block%s -> block%s\n", (currentNode->parent)->data, currentNode->data);

		}

		( * commandGraphDump)++;
		graphDumpDrawNode (currentNode->left, graphDumpFile,  commandGraphDump);
		graphDumpDrawNode (currentNode->right, graphDumpFile, commandGraphDump);
	}
}


void infoLastCharacter (Tree * tree) {

	if ((tree->nextStack).size) {

		printf ("%s is:\n", (tree->nextStack).answer);
		while ((tree->nextStack).size != 0)
			printf ("%s\n", StackPop (&(tree->nextStack)));

		StackClean (&(tree->stack));
		return;
	}

	if ((tree->stack).size) {

		printf ("%s is:\n", (tree->stack).answer);
		while ((tree->stack).size != 0)
			printf ("%s\n", StackPop (&(tree->stack)));

		return;
	}

	printf ("You didn't guess character.\n");
}


int StackCtor (Stack * stack, int capacity) {

	stack->size = 0;
	stack->capacity = capacity;

	stack->data = (char ** ) calloc (capacity, sizeof (char * ));
	CHECK_ERROR(!stack->data, "Problem with allocating memory for stack->data.");

	int i = 0, j = 0;
	for (i = 0; i < capacity; i++) {

		stack->data[i] = (char * ) calloc (MAXLENTITLE, sizeof (char));
		CHECK_ERROR(!stack->data, "Problem with allocating memory for stack->data[...].");
	}

	for (i = 0; i < stack->capacity; i++)
		strcpy (stack->data[i], POISON);
	strcpy (stack->data[0], CANARY);

	stack->answer = (char * ) calloc (MAXLENTITLE, sizeof (char));
	CHECK_ERROR(!stack->answer, "Problem with allocating memory for stack->answer.");
	strcpy (stack->answer, POISON);

	return ERROR_OFF;
}


void StackPush (Stack * stack, const char * sign, char * answer) {

	if (!strcmp (sign, YES)) {

		strcpy (stack->data [stack->size], answer);
		stack->size++;
	}

	if (!strcmp (sign, NO)) {

		strcpy (stack->data [stack->size], "not_");
		strcpy (stack->data [stack->size] + 4, answer);
		stack->size++;
	}

	strcpy (stack->data [stack->size], CANARY);
}

void StackClean (Stack * stack) {

	int i = 0;
	for (i = 0; i < stack->capacity; i++)
		strcpy (stack->data[i], POISON);

	strcpy (stack->answer, "NULL");
	strcpy (stack->data[0], CANARY);
	stack->size = 0;
}


char * StackPop (Stack * stack) {

	char * save = (char * ) calloc (MAXLENTITLE, sizeof (char));

	strcpy (save,    stack->data[stack->size - 1]);
	strcpy (stack->data [stack->size - 1], POISON);
	strcpy (stack->data [stack->size],     CANARY);
	stack->size--;

	return save;
}


int addAnswer (Node * lastNode, int * size, Stack * stack) {

	printf ("Is it %s?(y/n)\n", lastNode->data);

	char answer [MAXLENANSWER] = " ";
	scanf ("%s", answer);

	if (!strcmp (answer, YES)) {

		strcpy (stack->answer, lastNode->data);
		printf ("Yes, I won!\n\n");
		return ERROR_OFF;
	}

	else if (!strcmp (answer, NO)) {

		char trueAnswer [MAXLENTITLE];
		char difference [MAXLENTITLE];

		printf ("Who is it? Input: ");
		scanf ("%s", trueAnswer);
		printf ("\nHow does %s differ from %s? Input: ", trueAnswer, lastNode->data);
		scanf ("%s", difference);

		StackPush (stack, YES, difference);
		strcpy (stack->answer, trueAnswer);

		lastNode->left          = (Node * ) malloc                  			(sizeof (Node));
		CHECK_ERROR(!stack->data, "Problem with allocating memory for lastNode->left.         ");

		(lastNode->left)->data  = (char * ) malloc (MAXLENTITLE * 			     sizeof (char));
		CHECK_ERROR(!stack->data, "Problem with allocating memory for (lastNode->left)->data. ");

		lastNode->right 		= (Node * ) malloc 								(sizeof (Node));
		CHECK_ERROR(!stack->data, "Problem with allocating memory for lastNode->right.        ");

		(lastNode->right)->data = (char * ) malloc (MAXLENTITLE * 				 sizeof (char));
		CHECK_ERROR(!stack->data, "Problem with allocating memory for (lastNode->right)->data.");

		strcpy ((lastNode->right)->data, lastNode->data);
		strcpy (lastNode->data         , difference);
		strcpy ((lastNode->left)->data , trueAnswer);
		(lastNode->left)->left   = NULL;
		(lastNode->left)->right  = NULL;
		(lastNode->right)->left  = NULL;
		(lastNode->right)->right = NULL;
		( * size)++;

		return ERROR_OFF;
	}

	else
		printf ("Input fake answer! Please input (y/n).\n");

	return ERROR_OFF;
}


int AddNode (Node * currentNode, int * size, Stack * stack) {

	if (currentNode->left == NULL && currentNode->right == NULL) {

		addAnswer (currentNode, size, stack);
		return ERROR_OFF;
	}

	printf ("%s(y/n):\n", currentNode->data);
	char answer [MAXLENANSWER]= " ";
	scanf ("%s", answer);
		
	if (!strcmp (answer, YES)) {

		StackPush (stack, YES, currentNode->data);
		AddNode (currentNode->left, size, stack);
	}

	else if (!strcmp (answer, NO)) {

		StackPush (stack, NO, currentNode->data);
		AddNode (currentNode->right, size, stack);
	}

	else {

		printf ("ANS: %s\n\n\n", answer);
		printf ("Input fake answer! Please input (y/n).\n");
		return ERROR_ON;
	}

	return ERROR_OFF;
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


int getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem) {

	dumpFile = fopen ("tree.txt", "r");
	CHECK_ERROR(!dumpFile, "Problem with opening tree.txt");

	int fileSize = FileSize (dumpFile);
	InitializeNode (&(tree->head), dumpFile, NULL);

	return ERROR_OFF;
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d", &(tree->size));

	return ERROR_OFF;
}


int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char bracketBuffer [MAXLENTITLE], 
		  answerBuffer [MAXLENTITLE];

	fscanf (dumpFile, "%s", bracketBuffer);
	if (!strcmp (bracketBuffer, "{")) {

		* currentNode = (Node * )          malloc 								(sizeof (Node));
		CHECK_ERROR(!( * currentNode), 		 "Problem with allocating memory for currentNode.");

		( * currentNode)->data = (char * ) malloc 				  (MAXLENTITLE * sizeof (char));
		CHECK_ERROR(!( * currentNode), "Problem with allocating memory for currentNode->data.");

		( * currentNode)->right  =               NULL;
		( * currentNode)->left   =               NULL;
		( * currentNode)->parent =  parentCurrentNode;

		fscanf (dumpFile, "%s",         answerBuffer);
		strcpy (( * currentNode)->data, answerBuffer);

		InitializeNode (&( * currentNode)->left,  dumpFile, * currentNode);
		InitializeNode (&( * currentNode)->right, dumpFile, * currentNode);
	}

	if (!strcmp (bracketBuffer, "}")) {

		ungetc ('}', dumpFile);
		return ERROR_OFF;
	}

	fscanf (dumpFile, "%s", bracketBuffer);

	return ERROR_OFF;
}


int InitializeTree (Tree * tree, FILE * infoTree) {

	CHECK_ERROR(getMainInfoFile (tree, infoTree), "Problem with getting data from file info.txt .\n");

	if (tree->size == 0) {

		Node * memObject = (Node * ) malloc									 (sizeof (Node));
		CHECK_ERROR(!memObject, 			"Problem with allocating memory for memObject.");

		memObject->data  = (char * ) malloc					   (MAXLENTITLE * sizeof (char));
		CHECK_ERROR(!memObject->data, "Problem with allocating memory for memObject->data.");

		strcpy (memObject->data,  HEADOBJECT);
		memObject->left  = 			 	 NULL;
		memObject->right = 			     NULL;
		tree->head       =		    memObject;
		tree->size++;
	}

	StackCtor (&(tree->stack),     STACKSIZE);
	StackCtor (&(tree->nextStack), STACKSIZE);

	return ERROR_OFF;
}


int menu (Tree * tree) {

	char * variant = (char * ) calloc (MAXLENVARIANT, sizeof (char));
	CHECK_ERROR(!variant,           "Problem with opening tree.txt");

	printf ("\n\n\n%s\n", FRAME                                 );
	printf ("                     MENU:                      \n");
	printf ("                                                \n");
	printf ("p) Play;              							 \n");
	printf ("i) Info last character;   						 \n");
	printf ("e) Explore object in the tree;					 \n");
	printf ("c) Compare 2 last objects;						 \n");
	printf ("q) Quit;                                        \n");
	printf ("												 \n");
	printf ("%s\n", FRAME                                       );
	printf ("\n"                                                );
	printf ("Amount of elements in the tree: %d\n",   tree->size);
	puts ("Please, input one of the variant menu:"              );

	scanf ("%s", variant);

	#define DEF_CMD(name, num, ...)								\
		if (!strcmp (variant, #name))							\
			return num;							     			

	#include "commands.h"
	#undef DEF_CMD

	return -1;
}
