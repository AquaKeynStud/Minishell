#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libft.h"
#include "lexing.h"
#include "parsing.h"

// Affiche la liste de tokens avec leur type
void print_token_list(t_token *head)
{
    while (head)
    {
        const char *type_str;
        switch (head->type)
        {
            case TOKEN_WORD:          type_str = "WORD";        break;
            case TOKEN_PIPE:          type_str = "PIPE";        break;
            case TOKEN_REDIR_IN:      type_str = "REDIR_IN";    break;
            case TOKEN_REDIR_OUT:     type_str = "REDIR_OUT";   break;
            case TOKEN_REDIR_APPEND:  type_str = "APPEND";      break;
            case TOKEN_HEREDOC:       type_str = "HEREDOC";     break;
            default:                  type_str = "UNKNOWN";     break;
        }
        printf("\"%s\" -> %s\n", head->value, type_str);
        head = head->next;
    }
}

// Affiche l'AST en forme d'arbre ASCII
void print_ast_tree_rec(t_ast *node, const char *prefix, bool is_last)
{
    // Affiche le préfixe puis la branche et la valeur
    printf("%s", prefix);
    printf(is_last ? "└─ " : "├─ ");
    printf("%s\n", node->value ? node->value : "(null)");

    // Prépare le nouveau préfixe pour les enfants
    char new_prefix[1024];
    strcpy(new_prefix, prefix);
    strcat(new_prefix, is_last ? "   " : "│  ");

    for (int i = 0; i < node->child_count; i++)
    {
        bool last_child = (i == node->child_count - 1);
        print_ast_tree_rec(node->childs[i], new_prefix, last_child);
    }
}

void print_ast_tree(t_ast *root)
{
    if (!root)
        return;
    // Affiche la racine
    printf("%s\n", root->value ? root->value : "(null)");
    // Affiche récursivement les enfants
    for (int i = 0; i < root->child_count; i++)
    {
        bool last = (i == root->child_count - 1);
        print_ast_tree_rec(root->childs[i], "", last);
    }
}

int main(void)
{
    const char *tests[] = {
        "ls -l",
        "echo hello world",
        "grep main *.c",
        "cat file.txt | grep error | wc -l",
        "sort < unsorted.txt > sorted.txt",
        "echo \"I love $SHELL\"",
        "find . -type f | xargs grep TODO",
        "tar -czf archive.tar.gz folder/",
        "du -sh * | sort -h | head -n 10",
        "cat << EOF\nfoo\nbar\nEOF > heredoc_output.txt",
        "echo start >> log.txt",
        "awk '{print $1}' data.csv | uniq | wc -l",
        "sed -e 's/foo/bar/g' < input.txt | tee modified.txt",
        "ps aux | grep sshd | awk '{print $2}' | xargs -I {} ls -lh /proc/{}/fd",
        // Pipeline complexe avec 10 pipes
        "ps aux | grep ssh | awk '{print $2}' | xargs -I {} ls -lh /proc/{}/fd | grep sock | sort | uniq | head -n 5 | sed '1d' | wc -l"
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        printf("_________________________________________________________|\n");
        printf("TEST %d: %s\n", i + 1, tests[i]);
        printf("_________________________________________________________|\n");

        // Tokenisation
        printf("Tokenisation :\n");
        t_token *tokens = tokenize(tests[i]);
        print_token_list(tokens);

        // Parsing en AST et affichage en arbre
        printf("_________________________________________________________|\n");
        printf("AST :\n");
        t_ast *ast = parse_input(tokens);
        if (ast)
            print_ast_tree(ast);
        else
            printf("Erreur lors du parsing de la commande\n");

        printf("_________________________________________________________|\n\n");
    }
    return 0;
}
