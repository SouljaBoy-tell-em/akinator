int addAnswer (Node * lastNode, int * size) {

	printf ("Is it %s?(y/n)\n", lastNode->data);

	char answer [MAXLENANSWER] = "";
	scanf ("%s", answer);

	if (!strcmp (answer, YES)) {

		printf ("Yes, I won!\n\n");
		return ERROR_OFF;
	}

	else if (!strcmp (answer, NO)) {

		char trueAnswer [MAXLENTITLE] = "";
		char difference [MAXLENTITLE] = "";

		printf ("Who is it? Input: ");
		scanf ("%s", trueAnswer);
		printf ("\nHow does %s differ from %s? Input: ", trueAnswer, lastNode->data);
		scanf ("%s", difference);

		constructor (&(lastNode->left),  lastNode);
		constructor (&(lastNode->right), lastNode);

		strcpy ((lastNode->right)->data, lastNode->data);
		strcpy (lastNode->data,              difference);
		strcpy ((lastNode->left)->data,      trueAnswer);
		
		( * size)++;

		return ERROR_OFF;
	}

	else
		printf ("Input fake answer! Please input (y/n).\n");

	return ERROR_OFF;
}


int AddNode (Node * currentNode, int * size) {

	if (currentNode->left == NULL && currentNode->right == NULL) {

		addAnswer (currentNode, size);
		return ERROR_OFF;
	}

	printf ("%s(y/n):\n", currentNode->data);
	char answer [MAXLENANSWER]= "";
	scanf ("%s", answer);
		
	if (!strcmp (answer, YES))
		AddNode (currentNode->left, size);

	else if (!strcmp (answer, NO))
		AddNode (currentNode->right, size);

	else {

		printf ("Input fake answer! Please input (y/n).\n");
		return ERROR_ON;
	}

	return ERROR_OFF;
}


int Aki (void) {

	Tree tree = {};

	FILE * infoTree = NULL;
	CHECK_ERROR(InitializeTree (&tree, infoTree), "Problem with initializing tree.\n");

	char * mem 		= NULL;
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

	getDataFromFile (dumpFile, &tree, &mem);
	CHECK_ERROR(graphDump (tree.head),    "Problem with opening file graphDump.txt.\n");

	return ERROR_OFF;
}


int compareFeatures (Stack * object1, Stack * object2) {

	char * save = (char * ) calloc (MAXLENTITLE,    sizeof (char));
	CHECK_ERROR(!save, "Problem with allocating memory for save.");

	printf ("%s is similar to %s in that: ", object1->answer, object2->answer);
	while (object1->size != 0) {

		strcpy (save, object1->data[object1->size - 1]);

		if (!strcmp (save, StackPop (object2)))
			printf ("%s, ", StackPop (object1));

		else
			break;
	}

	printf ("\nand differs: ");
	while (object1->size != 0)
		printf ("%s, ", StackPop (object1));

	return ERROR_OFF;
}


int compare2Objects (Tree * tree) {

	Stack object1 = infoCharacter (tree);
	CHECK_ERROR(!object1.size, "Problem with explore character #1.\n");

	Stack object2 = infoCharacter (tree);
	CHECK_ERROR(!object2.size, "Problem with explore character #2.\n");

	compareFeatures (&object1, &object2);

	return ERROR_OFF;
}


int constructor (Node ** currentNode, Node * parentCurrentNode) {

	* currentNode = (Node * )          malloc 								(sizeof (Node));
	CHECK_ERROR(!( * currentNode), 		 "Problem with allocating memory for currentNode.");

	( * currentNode)->data = (char * ) malloc 				  (MAXLENTITLE * sizeof (char));
	CHECK_ERROR(!( * currentNode), "Problem with allocating memory for currentNode->data.\n");

	( * currentNode)->right  =               NULL;
	( * currentNode)->left   =               NULL;
	( * currentNode)->parent =  parentCurrentNode;

	return ERROR_OFF;
}


int dump (Tree * tree, FILE * dumpFile, FILE * infoFile) {

	dumpFile = fopen (DUMPFILE, "w");
	CHECK_ERROR(!dumpFile, "Problem with opening tree.txt\n");

	infoFile = fopen (INFOFILE, "w");
	CHECK_ERROR(!infoFile, "Problem with opening info.txt\n");
	
	fullPrint (tree->head, dumpFile, 4);
	fprintf (infoFile, "%d", tree->size);
	fclose (dumpFile);
	fclose (infoFile);

	return ERROR_OFF;
}


int existenceCheck (Tree * tree) {

	char * object = (char * ) calloc (MAXLENTITLE, sizeof (char));
	CHECK_ERROR(!object, "Problem with allocating memory for object.\n");

	bool flagExplore = false;
	Node * save 	 =  NULL;

	printf ("Input desired object:\n");
	scanf ("%s", object);

	exploreObject (tree->head, object, &flagExplore, &save);

	if (flagExplore)
		printf ("This object contains in the tree.     \n");

	else
		printf ("This object don't contain in the tree.\n");

	free (object);

	return ERROR_OFF;
}


void exploreObject (Node * currentNode, char * object, bool * flagExplore, Node ** save) {

	if (currentNode != NULL) {

		if (!strcmp (currentNode->data, object)) {

			* flagExplore =        true;
			* save        = currentNode;
			return;
		}

		exploreObject (currentNode->left , object, flagExplore, save);
		exploreObject (currentNode->right, object, flagExplore, save);
	}
}


unsigned long FileSize (FILE * infoTree) {

	struct stat buf = {};
	if (fstat (fileno (infoTree), &buf) == 0)
		return buf.st_size;

	return 0;
}


void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces) {

	fprintf (dumpFile, "\n%*s", amountSpaces, "{");
	fprintf (dumpFile, " %s",   currentNode->data);

	if (currentNode->left)
		fullPrint (currentNode->left,  dumpFile, amountSpaces + 4);

	if (currentNode->right)
		fullPrint (currentNode->right, dumpFile, amountSpaces + 4);

	if (!currentNode->left && !currentNode->right)
		amountSpaces = 2;

	fprintf (dumpFile , "%*s\n", amountSpaces, "}");
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
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",  
				(currentNode->parent)->data, ORANGE, (currentNode->parent)->data);

			if ((currentNode->parent)->left == currentNode)
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",
				currentNode->data, 			  GREEN,           currentNode->data);

			if ((currentNode->parent)->right == currentNode)
				fprintf (graphDumpFile, "block%s [shape=record, color=\"%s\", label=\"%s\"];\n",  
				currentNode->data, 				RED,    	   currentNode->data);

			fprintf (graphDumpFile, "block%s -> block%s\n",
				(currentNode->parent)->data, 				   currentNode->data);

		}

		( * commandGraphDump)++                                                ;
		graphDumpDrawNode (currentNode->left, graphDumpFile,  commandGraphDump);
		graphDumpDrawNode (currentNode->right, graphDumpFile, commandGraphDump);
	}
}


int getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem) {

	dumpFile = fopen ("tree.txt", "r");
	CHECK_ERROR(!dumpFile, "Problem with opening tree.txt");

	int fileSize = FileSize (dumpFile);
	InitializeNode (&(tree->head), dumpFile, NULL);

	return ERROR_OFF;
}


void getSkills (Stack * stack, Node * explore_object, Node * head) {

	if (explore_object == head) 
		return;

	if ((explore_object->parent)->left == explore_object)
		StackPush (stack, YES, (explore_object->parent)->data);

	if ((explore_object->parent)->right == explore_object)
		StackPush (stack,  NO, (explore_object->parent)->data);

	getSkills (stack, explore_object->parent, head);
}


int getMainInfoFile (Tree * tree, FILE * infoTree) {

	infoTree = fopen (INFOFILE, "r");
	CHECK_ERROR(!infoTree, "Problem with opening file info.txt .\n");
	fscanf (infoTree, "%d", &(tree->size));

	return ERROR_OFF;
}


Stack infoCharacter (Tree * tree) {

	Node * explore_object =  NULL;
	bool flag 		      = false;
	Stack stack = {};

	char * object = (char * ) calloc (MAXLENTITLE, sizeof (char));
	scanf ("%s", object);

	exploreObject (tree->head, object, &flag, &explore_object);

	if (flag) {

		StackCtor (&stack, tree->size);
		getSkills (&stack, explore_object, tree->head);

		stack.answer = (char * ) calloc (MAXLENTITLE, sizeof (char));
		strcpy (stack.answer, object);
	}

	else {

		printf ("So character not found.\n");
		stack.size = 0;
	}

	return stack;
}


int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char bracketBuffer [MAXLENTITLE] = "", 
		  answerBuffer [MAXLENTITLE] = "";

	fscanf (dumpFile, "%s", bracketBuffer);
	if (!strcmp (bracketBuffer, "{")) {

		constructor (currentNode, 						parentCurrentNode);
		fscanf (dumpFile, "%s",    				   ( * currentNode)->data);
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
	
	if (tree->size == 0)
		CHECK_ERROR(startConstructor (tree), "Problem with function startConstructor().");

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
	printf ("c) Compare 2 last objects;			 			 \n");
	printf ("q) Quit;                                        \n");
	printf ("						 						 \n");
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

int outputInfoCharacter (Tree * tree) {

	Stack object = infoCharacter (tree);
	CHECK_ERROR(!object.size, "Problem with explore character.\n");

	printf ("%s is: ", object.answer);
	while (object.size != 0)
		printf ("%s, ", StackPop (&object));

	return ERROR_OFF;
}


int startConstructor (Tree * tree) {

	Node * memObject = (Node * ) malloc									   (sizeof (Node));
	CHECK_ERROR(!memObject, 			"Problem with allocating memory for memObject.\n");

	memObject->data  = (char * ) malloc					     (MAXLENTITLE * sizeof (char));
	CHECK_ERROR(!memObject->data, "Problem with allocating memory for memObject->data.\n");

	strcpy (memObject->data,  HEADOBJECT);
	memObject->left  = 			 	 NULL;
	memObject->right = 			     NULL;
	tree->head       =		    memObject;
	tree->size++                         ;

	return ERROR_OFF;
}