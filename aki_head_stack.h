typedef struct {

	char * answer;
	char **  data;
	int 	 size;
	int  capacity;
} Stack;


int StackCtor (Stack * stack, int capacity);
void StackPush (Stack * stack, const char * sign, char * answer);
void StackClean (Stack * stack);
char * StackPop (Stack * stack);