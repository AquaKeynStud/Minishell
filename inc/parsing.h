/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:20:15 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 11:35:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

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

/* -- Functions -- */

#endif