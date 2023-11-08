#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

/* Points to an array of pointers to strings called the "envirment" */
extern char **envir;


/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @in: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
 * @_envir: envirment variable
 * @pid: process ID of the shell
 */
typedef struct dt
{
	char **av;
	char *in;
	char **args;
	int status;
	int counter;
	char **_envir;
	char *pid;
} dt_shell;

/**
 * struct list_sep_s - single linked list
 * @separator: ; | &
 * @next: next node
 * Description: single linked list to store separators
 */
typedef struct list_sep_s
{
	char separator;
	struct list_sep_s *next;
} list_sep;

/**
 * struct list_line_s - single linked list
 * @line: command line
 * @next: next node
 * Description: single linked list to store command lines
 */
typedef struct list_line_s
{
	char *line;
	struct list_line_s *next;
} list_line;

/**
 * struct list_var - single linked list
 * @len_var: length of the variable
 * @val: value of the variable
 * @len_val: length of the value
 * @next: next node
 * Description: single linked list to store variables
 */
typedef struct list_var
{
	int len_var;
	char *val;
	int len_val;
	struct list_var *next;
} l_var;

/**
 * struct inbuilt_s - Builtin struct for command args.
 * @name: The name of the command builtin i.e cd, exit, env
 * @f: dt type pointer function.
 */
typedef struct inbuilt_s
{
	char *name;
	int (*f)(dt_shell *dtsh);
} inbuilt_t;

/* aux_lists.c */
list_sep *add_sep_node_end(list_sep **head, char sep);
void free_list_sep(list_sep **head);
list_line *add_line_node_end(list_line **head, char *line);
void free_list_line(list_line **head);

/* aux_lists2.c */
l_var *add_rvar_node(l_var **head, int lvar, char *var, int lval);
void free_rvar_list(l_var **head);

/* aux_str functions */
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);

/* aux_mem.c */
void _memcpy(void *newptr, const void *ptr, unsigned int size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);

/* aux_str2.c */
char *_strdup(const char *s);
int _strlen(const char *s);
int cmp_chars(char str[], const char *delim);
char *_strtok(char str[], const char *delim);
int _isdigit(const char *s);

/* aux_str3.c */
void rev_string(char *s);

/* check_syntax_error.c */
int repeated_char(char *in, int m);
int error_sep_op(char *in, int m, char last);
int first_char(char *in, int *m);
void print_syntax_error(dt_shell *sh_data, char *in, int m, int bool);
int check_syntax_error(dt_shell *sh_data, char *in);

/* shell_loop.c */
char *without_comment(char *in);
void shell_loop(dt_shell *sh_data);

/* read_line.c */
char *read_line(int *m_eof);

/* split.c */
char *swap_char(char *in, int bool);
void add_nodes(list_sep **head_s, list_line **head_l, char *in);
void go_next(list_sep **list_s, list_line **list_l, dt_shell *sh_data);
int split_commands(dt_shell *sh_data, char *in);
char **split_line(char *in);

/* rep_var.c */
void check_env(l_var **h, char *in, dt_shell *data);
int check_vars(l_var **h, char *in, char *st, dt_shell *data);
char *replaced_in(l_var **head, char *in, char *new_in, int nlen);
char *rep_var(char *in, dt_shell *sh_data);

/* get_line.c */
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

/* exec_line */
int exec_line(dt_shell *sh_data);

/* cmd_exec.c */
int ms_cdir(char *path, int *m);
char *_which(char *cmd, char **_envir);
int ms_executable(dt_shell *sh_data);
int check_error_cmd(char *dir, dt_shell *sh_data);
int cmd_exec(dt_shell *sh_data);

/* env1.c */
char *get_envir(const char *name, char **_envir);
int _envir(dt_shell *sh_data);

/* env2.c */
char *copy_info(char *name, char *value);
void set_env(char *name, char *value, dt_shell *sh_data);
int _setenv(dt_shell *sh_data);
int _unsetenv(dt_shell *sh_data);

/* cd.c */
void cd_dot(dt_shell *sh_data);
void cd_to(dt_shell *sh_data);
void cd_previous(dt_shell *sh_data);
void cd_to_home(dt_shell *sh_data);

/* cd_shell.c */
int cd_shell(dt_shell *sh_data);

/* get_builtin */
int (*get_builtin(char *cmd))(dt_shell *sh_data);

/* _exit.c */
int exit_shell(dt_shell *sh_data);

/* aux_stdlib.c */
int get_len(int n);
char *aux_itoa(int n);
int _atoi(char *s);

/* aux_error1.c */
char *strcat_cd(dt_shell *, char *, char *, char *);
char *error_get_cd(dt_shell *sh_data);
char *error_not_found(dt_shell *sh_data);
char *error_exit_shell(dt_shell *sh_data);

/* aux_error2.c */
char *error_get_alias(char **args);
char *error_env(dt_shell *sh_data);
char *error_syntax(char **args);
char *error_permission(char **args);
char *error_path_126(dt_shell *sh_data);


/* get_error.c */
int get_error(dt_shell *sh_data, int eval);

/* get_sigint.c */
void get_sigint(int sig);

/* aux_help.c */
void aux_help_env(void);
void aux_help_setenv(void);
void aux_help_unsetenv(void);
void aux_help_general(void);
void aux_help_exit(void);

/* aux_help2.c */
void aux_help(void);
void aux_help_alias(void);
void aux_help_cd(void);

/* get_help.c */
int get_help(dt_shell *sh_data);

#endif