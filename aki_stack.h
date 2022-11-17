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