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
const char * GREEN = "green";
const char * RED = "red";
const char * ORANGE = "orange";


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


typedef struct node {

	char        *   data;
	struct node * parent;
	struct node *   left;
	struct node *  right;
} Node;

typedef struct {

	Node *      head;
	int         size;
} Tree;


int Aki (void);
int addAnswer (Node * lastNode, int * size, Stack * stack);
int AddNode (Node * currentNode, int * size);
int compareFeatures (Stack * object1, Stack * object2);
int compare2Objects (Tree * tree);
int constructor (Node ** currentNode, Node * parentCurrentNode);
int dump (Tree * tree, FILE * dumpFile, FILE * infoFile);
int existenceCheck (Tree * tree);
void exploreObject (Node * currentNode, char * object, bool * flagExplore, Node ** save);
unsigned long FileSize (FILE * infoTree);
void fullPrint (Node * currentNode, FILE * dumpFile, int amountSpaces);
int getDataFromFile (FILE * dumpFile, Tree * tree, char ** mem);
int getMainInfoFile (Tree * tree, FILE * infoTree);
int graphDump (Node * head);
void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump);
Stack infoCharacter (Tree * tree);
int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);
int InitializeTree (Tree * tree, FILE * infoTree);
int menu (Tree * tree);
int outputInfoCharacter (Tree * tree);
void remainingSkillsObject (Stack * object);
int startConstructor (Tree * tree);
