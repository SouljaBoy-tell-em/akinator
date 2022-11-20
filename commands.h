DEF_CMD(q, 0, {

	printf      		("The end!\n");
})
DEF_CMD(p, 1, {

	AddNode (tree.head, &(tree.size));
})


DEF_CMD(i, 2, {

	outputInfoCharacter 	  (&tree);
})


DEF_CMD(e, 3, {

	existenceCheck    		  (&tree);
})

DEF_CMD(c, 4, {

	compare2Objects 	      (&tree);
})
