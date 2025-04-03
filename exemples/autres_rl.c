/* La fonction qui va récupérer le prompt de l'utilisateur (leak normal) */

#include <stdlib.h> // Pour free
#include <stdio.h> // Celui là et tous les autres pour readline (ajouter -lreadline a la compil)
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>

/*
	rl_on_new_line();
		Utilisée pour signaler une nouvelle ligne.
		Pas obligatoire avant modification
*/

/*
	rl_replace_line(const char *text, int clear_undo);
		Mofifie le prompt pré-enregistré avec deux options possibles:
			- clear_undo = 0 : Remplace le prompt et efface l'historique d'annulation.
			- clear_undo = 1 : Remplace le prompt mais conserve l'historique d'annulation.

	L'historique d'annulation permet les undo et redo pendant l'écriture du prompt avec :
		- Ctrl + _ : undo
		- Ctrl + Y : redo

	ATTENTION :  Cette fonction modifie le buffer de readline càd ce qui s'affiche à l'écran lorsque l'utilisateur tape.
				 Cela ne modifie pas la valeur de retour de readline qui reste exclusivement ce que l'utilisateur a tapé.
*/

/*
	rl_redisplay();
		Met a jour l'affichage du prompt actuellement tapé par l'utilisateur.
*/

int main(void)
{
	char *prompt;
	int prompt_count;

	prompt_count = 0;
	while (++prompt_count < 3)
	{
		prompt = readline("tests > ");
		if (!prompt)
			break ;

		if (prompt_count == 1)
		{
			rl_on_new_line(); // Force à passer à la ligne suivante
			rl_replace_line("Attendez 1 seconde...\n", 0); // Remplacement complet du prompt
			rl_redisplay(); // Mise à jour immédiate de l'affichage
			sleep(1); // Attente de 1 seconde avant de modifier le prompt

			/*
				La taille du buffer interne de readline n'ayant pas changée, sans retour a la ligne, l'affichage sera bizarre
				si le prompt est moins grand que "Attendez 1 seconde...\n".
			*/
			rl_replace_line("\n", 0);
			rl_redisplay();
		}

		add_history(prompt);
		printf("Prompt %i reçu : %s\n", prompt_count, prompt);
		free(prompt);
	}
    rl_clear_history();
    return (0);
}
