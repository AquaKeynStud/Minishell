/*
** ============================================================================
** 4. Fonction d'affichage de l'AST pour vérification (simplifiée)
**
** La fonction print_ast affiche de manière récursive les nœuds de l'AST.
** ============================================================================
*/
void	print_ast(t_ast *node, int indent)
{
	int	i;

	if (!node)
		return ;
	while (indent-- > 0)
		printf("  ");
	printf("Type: %d, Value: %s, Children: %d\n",
		node->type, (node->value) ? node->value : "NULL", node->child_count);
	i = 0;
	while (i < node->child_count)
	{
		print_ast(node->children[i], indent + 2);
		i++;
	}
}


/*
** ============================================================================
** 5. Fonction main
**
** Exemple d'utilisation :
** On construit une liste chaînée de tokens représentant la commande suivante :
**
**    ls -l | grep minishell > output.txt
**
** Schéma du pipeline construit :
**
**                AST_PIPE ("|")
**                   /         \
**  AST_COMMAND ("ls")      AST_COMMAND ("grep")
**         |                       |
**  Argument: "-l"          Argument: "minishell"
**                                 └── AST_REDIRECTION (">")
**                                      └── Target: "output.txt"
**
** ============================================================================
*/
int	main(void)
{
	t_token	t1;
	t_token	t2;
	t_token	t3;
	t_token	t4;
	t_token	t5;
	t_token	t6;
	t_token	t7;

	/* Construction manuelle de la liste de tokens */
	t1.type = TOKEN_WORD;
	t1.value = "ls";
	t1.next = &t2;

	t2.type = TOKEN_WORD;
	t2.value = "-l";
	t2.next = &t3;

	t3.type = TOKEN_PIPE;
	t3.value = "|";
	t3.next = &t4;

	t4.type = TOKEN_WORD;
	t4.value = "grep";
	t4.next = &t5;

	t5.type = TOKEN_WORD;
	t5.value = "minishell";
	t5.next = &t6;

	t6.type = TOKEN_REDIRECT_OUT;
	t6.value = ">";
	t6.next = &t7;

	t7.type = TOKEN_WORD;
	t7.value = "output.txt";
	t7.next = NULL;

	/* Lancement du parsing */
	t_ast *ast = parse_input(&t1);
	if (!ast)
	{
		fprintf(stderr, "Échec du parsing\n");
		return (EXIT_FAILURE);
	}
	/* Affichage de l'AST */
	print_ast(ast, 0);
	return (EXIT_SUCCESS);
}