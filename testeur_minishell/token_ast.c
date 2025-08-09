#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libft.h"
#include <sys/stat.h>
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

// Affiche la liste de tokens avec leur type
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
            case TOKEN_AND:           type_str = "AND";         break;
            case TOKEN_OR:            type_str = "OR";          break;
            case TOKEN_LPAR:          type_str = "LEFT_PAR";    break;
            case TOKEN_RPAR:          type_str = "RIGHT_PAR";   break;
            default:                  type_str = "UNKNOWN";     break;
        }
        printf("\"%s\" -> %s : %s\n", head->value, type_str, head->expand);
        head = head->next;
    }
}

// Affiche l'AST en forme d'arbre ASCII
void print_ast_tree_rec(t_ast *node, const char *prefix, bool is_last)
{
    // Affiche le préfixe puis la branche et la valeur
    printf("%s", prefix);
    printf(is_last ? "└─ " : "├─ ");
    printf("%s [%s]\n", node->value ? node->value : "(null)", node->quotes ? node->quotes : "/");

    // Prépare le nouveau préfixe pour les enfants
    char new_prefix[1024];
    strcpy(new_prefix, prefix);
    strcat(new_prefix, is_last ? "   " : "│  ");

    for (int i = 0; i < node->sub_count; i++)
    {
        bool last_child = (i == node->sub_count - 1);
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
    for (int i = 0; i < root->sub_count; i++)
    {
        bool last = (i == root->sub_count - 1);
        print_ast_tree_rec(root->childs[i], "", last);
    }
}

void run_tests(const char *label, char **tests, int num_tests, t_ctx *ctx)
{
	t_ast *ast;

	for (int i = 0; i < num_tests; i++)
	{
		printf("_________________________________________________________|\n");
		printf("%s TEST %d: %s\n", label, i + 1, tests[i]);
		printf("_________________________________________________________|\n");

		// Tokenisation
		printf("Tokenisation :\n");
		t_token *tokens = tokenize(ctx, tests[i]);
		print_token_list(tokens);

		// Parsing en AST et affichage
		printf("_________________________________________________________|\n");
		printf("AST :\n");
		if (check_parenthesis(ctx, tokens))
			ast = parse_input(ctx, tokens);
		if (ast)
			print_ast_tree(ast);
		else
			printf("Erreur lors du parsing de la commande\n");

		printf("_________________________________________________________|\n\n");
	}
}

static void	init_context(t_ctx *ctx, char **argv, char **envp)
{
	struct stat	st;

	ctx->fds = NULL;
	ctx->ast = NULL;
	ctx->status = 0;
	ctx->input = NULL;
	ctx->tokens = NULL;
	ctx->allocs = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	ctx->env = init_env(ctx, argv, envp);
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	if (!stat("/proc/self", &st))
		ctx->uid = s_save(ctx, ft_itoa(st.st_uid));
	return ;
}

// ────────────────────────────────────────────────────────────────────────────────

int main(int argc, char **argv, char **envp)
{
	t_ctx ctx;
	init_context(&ctx, argv, envp);
	char *simple_tests[] = {
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
		"ps aux | grep ssh | awk '{print $2}' | xargs -I {} ls -lh /proc/{}/fd | grep sock | sort | uniq | head -n 5 | sed '1d' | wc -l"
	};

	char *pipe_redirect_tests[] = {
		"cat < input.txt | grep 'error'",
		"grep keyword < input.txt | sort > sorted.txt",
		"ls -l | tee files.txt > log.txt",
		"find . -name '*.c' | xargs grep main > results.txt",
		"cat << EOF | grep bar > result.txt\nfoo\nbar\nbaz\nEOF",
		"sort < unsorted.txt | uniq > unique.txt",
		"cut -d ':' -f1 /etc/passwd | sort > users.txt",
		"cat << END | sed 's/foo/bar/' > output.txt\nfoo\nbaz\nfoo again\nEND",
		"cat << DATA | awk '{print $1}' > col1.txt\none two\nthree four\nDATA",
		"grep TODO < notes.txt | wc -l > count.txt"
	};

	char *my_tests[] = {
		"cat Makefile > test.txt", // Test1
		"<< eof", // Test 2
		"Makefile < cat | echo > test.txt", // Test 3
		"< infile cmd", // Test 4
		"< in1 < in2 grep foo", // Test 5 
		"echo bar > out.txt", // Test 6
		"<< eof cat", // Test 7
		"<< eof < in1 > out1 grep foo", // Test 8
		"< input.txt > output.txt cat file1 file2", // Test 9
		"<< eof < in1 < in2 > out1 > out2 cat foo", // Test 10
		"cat | << eof < in1 | < in2 > out1 | grep foo", // Test 11
		"cat minishell.h | grep \");\"$",
		"env | sort | grep -v SHLVL | grep -v ^_",
		"grep hi \"<infile\" <         ./test_files/infile",
		"echo hi < ./test_files/infile bye bye",
		"echo <\"./test_files/infile\" \"bonjour       42\"",
		"cat <\"1\"\"2\"\"3\"\"4\"\"5\"",
		"cat < \"Hello\"\"$USER\"\"Wolrd !\"",
		"cat < \"Hello\"'$USER'\"Wolrd !\"",
		"cat <\"./test_files/infile_big\" | echo hi",
		">>>",
		"echo hi |  \"|\"",
		"echo ceci'$USER'estuntest",
		"echo ceci\"$USER\"estuntest",
		"echo '$PWD'",
		"echo \"$PWD\"",
		"echo ceci\"$USER\"est un test",
		"echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\"",
		"echo hi | echo >>./outfiles/outfile01 bye",
		"cat << eof > outfile.txt",
		// Cas avec erreurs syntaxiques qu'il faudra détecter :
		"cat | | grep foo",       // erreur de syntaxe : double pipe
		">",                      // erreur de syntaxe : redirection sans fichier
		"cat >",                  // erreur de syntaxe : redirection sans fichier
		"<",                      // erreur de syntaxe : redirection sans fichier
		"cat > fichier >",        // erreur de syntaxe : redirection sans fichier
		"cat || grep",            // erreur de syntaxe (pour toi pour l'instant, car tu ne fais pas ||)
		"cat ||| grep",           // erreur de syntaxe (triple pipe invalide)
	};

	char *bonus[] = {
		"( echo foo && ls | grep bar ) || cat > out.txt",
		"true && echo success",
		"false || echo fallback",
		"true && false || echo fallback",
		"(echo first && echo second) || echo failed",
		"((echo nested1 && echo nested2) && echo outer) || echo error",
		"mkdir test && cd test && touch file.txt",
		"(ls -l && echo done) || (echo fail && exit 1)",
		"(false && echo never) || (true && echo always)",
		"(echo start && (echo middle && echo end))",
		"(true || false) && (false || true)",

		// Cas avec redirections et pipes combinés
		"cat input.txt | grep 'error' && echo found > log.txt",
		"echo hello && cat << EOF | grep l\nhello\nworld\nEOF",
		"cat << HERE | grep foo && echo matched\nfoo\nbar\nbaz\nHERE",
		"(cat file.txt | sort) && echo sorted > sorted.txt",
		"ls -l | tee out.txt && echo done > done.txt",

		// Cas invalides à détecter par le parser
		"|| echo broken",
		"echo start ||| echo too_many_pipes",
		"echo &&&& echo too_many_ands",
		"(((echo deep)",
		"(echo unbalanced",
		"echo missing_paren)",
		"echo & echo &&& echo",
		"ls |||| grep txt",
		"true && || false",
		"((echo test) &&)",
		"()",
		"((()))",
		"(ls -l | grep txt) &&",
		"(ls -l | grep txt &&",
		"(ls -l | grep txt ||",
		"echo ok && (|| echo fail)",

		// Cas plus complexes
		"(true && (false || (echo maybe && echo still)))",
		"(echo one && (echo two || echo three)) && echo four",
		"(cat << END | grep 'bar' && echo ok\nfoo\nbar\nbaz\nEND)",
		"(cut -d ':' -f1 /etc/passwd | sort) && (echo done || echo fail)",
		"(grep TODO notes.txt || grep FIXME notes.txt) && echo checked",

		"(cat file.txt | grep hello) && echo found || echo not found",
		"(echo start && (echo mid || echo alt)) && echo end",
		"((ls -l | grep \".c\") && echo \"C file\") || echo \"No match\"",
		"echo A > a.txt && echo B >> a.txt || cat a.txt",
		"(((echo 1 && echo 2) || echo 3) && echo 4)",
		"|| echo fail",
		"echo start && || echo fail",
		"echo test >",
		"echo ok && (echo fine",
		"echo hello (|| echo world)",
		"echo $PATH",
		"echo src/*"
	};

	char *refonte[] = {
		"echo 'test'\"$USER*.\"c *rc"
	};
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [ 1 | 2 | 3 | 4 | 5 ]\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "1") == 0)
		run_tests("SIMPLE", simple_tests, sizeof(simple_tests) / sizeof(simple_tests[0]), &ctx);
	else if (strcmp(argv[1], "2") == 0)
		run_tests("PIPE+REDIRECT", pipe_redirect_tests, sizeof(pipe_redirect_tests) / sizeof(pipe_redirect_tests[0]), &ctx);
	else if (strcmp(argv[1], "3") == 0)
		run_tests("my tests", my_tests, sizeof(my_tests) / sizeof(my_tests[0]), &ctx);
	else if (strcmp(argv[1], "4") == 0)
		run_tests("bonus", bonus, sizeof(bonus) / sizeof(bonus[0]), &ctx);
	else if (strcmp(argv[1], "5") == 0)
		run_tests("refonte", refonte, sizeof(refonte) / sizeof(refonte[0]), &ctx);
	else
		fprintf(stderr, "Argument invalide. Utilise : 1, 2, 3 ou 4\n");

	secure_exit(&ctx);

	return 0;
}
