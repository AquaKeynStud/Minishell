/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:27:23 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 16:16:48 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef struct s_ctx {
	int     status;
	char    **env; // tableau NULL-terminé de chaînes "VAR=VALUE"
} t_ctx;

// Fonction utilitaire : récupère la valeur d'une variable d'environnement
static char *get_from_env(char **env, const char *key) {
	size_t key_len = strlen(key);
	for (int i = 0; env && env[i]; i++) {
		if (strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=') {
			return env[i] + key_len + 1;
		}
	}
	return NULL;
}

// Convertit un entier en chaîne (itoa minimale)
static char *itoa_simple(int n) {
	char buffer[12];
	int i = 11;
	int neg = (n < 0);
	buffer[i--] = '\0';
	if (n == 0)
		buffer[i--] = '0';
	else {
		unsigned int un = (neg ? -n : n);
		while (un > 0) {
			buffer[i--] = '0' + (un % 10);
			un /= 10;
		}
		if (neg)
			buffer[i--] = '-';
	}
	return strdup(&buffer[i + 1]);
}

// Fonction utilitaire : join + free de l'ancien
static char *sjf(char *s1, const char *s2) {
	size_t len1 = s1 ? strlen(s1) : 0;
	size_t len2 = strlen(s2);
	char *res = malloc(len1 + len2 + 1);
	if (!res) {
		free(s1);
		return NULL;
	}
	if (s1)
		memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2 + 1); // +1 pour le '\0'
	free(s1);
	return res;
}

// Fonction principale
char *expand_args(t_ctx *ctx, char *s) {
	char *res;
	size_t i = 0;

	res = strdup("");
	if (!res)
		return NULL;

	while (s && s[i]) {
		if (s[i] == '$' && s[i + 1]) {
			if (s[i + 1] == '?') {
				char *val = itoa_simple(ctx->status);
				if (!val)
					return free(res), NULL;
				res = sjf(res, val);
				free(val);
				if (!res)
					return NULL;
				i += 2;
				continue;
			}
			size_t start = ++i;
			while (isalnum((unsigned char)s[i]) || s[i] == '_')
				i++;
			size_t key_len = i - start;
			char *key = malloc(key_len + 1);
			if (!key)
				return free(res), NULL;
			strncpy(key, s + start, key_len);
			key[key_len] = '\0';
			char *val = get_from_env(ctx->env, key);
			free(key);
			res = sjf(res, val ? val : "");
			if (!res)
				return NULL;
			continue;
		}
		char tmp[2] = { s[i++], '\0' };
		res = sjf(res, tmp);
		if (!res)
			return NULL;
	}
	return res;
}
