DEF_CMD(q, 0, {

	printf      ("The end!\n");
})
DEF_CMD(p, 1, {

	AddObject          (&tree);
})

DEF_CMD(i, 2, {

	infoLastCharacter  (&tree);
})

DEF_CMD(e, 3, {

	existenceCheck     (&tree);
})

DEF_CMD(c, 4, {

	compare2Characters (&tree);
})