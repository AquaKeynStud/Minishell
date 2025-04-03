/* La fonction qui va récupérer le prompt de l'utilisateur (leak normal) */

#include <stdlib.h> // Pour free
#include <stdio.h> // Celui là et tous les autres pour readline (ajouter -lreadline a la compil)
#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("tests > "); // Display le texte et récupère l'entrée en malloc. Doit être free.
		if (!prompt) // Retourne NULL en cas de Ctrl+D
			break ;
		printf("Prompt reçu : %s\n", prompt);
		free(prompt);
	}
	return (0);
}