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
		prompt = readline("tests > ");
		if (!prompt)
			break ;
		add_history(prompt); // Ajoute le prompt à l'historique pour le retrouver avec ↑ et ↓
		printf("Prompt reçu : %s\n", prompt);
		free(prompt);
	}
	rl_clear_history(); // Permet de libérer l'historique
	return (0);
}