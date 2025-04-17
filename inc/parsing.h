/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2025/04/12 10:04:09 by arocca            #+#    #+#             */
/*   Updated: 2025/04/13 09:32:24 by arocca           ###   ########.fr       */
=======
/*   Created: 2025/04/09 10:20:15 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 11:35:08 by arocca           ###   ########.fr       */
>>>>>>> 586d43a6751d0ef030d679f06864200fcf4b1458
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

<<<<<<< HEAD
/* -- Includes -- */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* -- Structures -- */
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_REDIR,
	AST_PIPE
}			t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			*value;
	struct s_ast	**childs;
	int				child_count;
}				t_ast;
=======
/* -- Structures -- */
typedef enum	e_redir {
	INPUT_REDIR;	// <
	OUTPUT_REDIR;	// >
	HEREDOC_REDIR;	// <<
	APPEND_REDIR	// >>
}				t_redir;

typedef struct	s_redirection {
	t_redir					type;
	char					*file;	// Nom du fichier où rediriger
	struct	s_redirection	*next;	// Chaine des redirections
}				t_redirection;

typedef struct	s_command {
	char				*name;		// Nom de la commande (ex : ls)
	char				**args;		// Tableau d'arguments
	t_redirection		*redirs;	// Liste des redirections
	struct s_command	*next;		// Commande suivante si pipe
}				t_command;
>>>>>>> 586d43a6751d0ef030d679f06864200fcf4b1458

/* -- Functions -- */

#endif