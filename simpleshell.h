#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#include  <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

/*using system getline() */
#define USE_STRTOK 0
#define USE_GETLINE 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/* for command chaining */
#define CMD_NORM		0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN		3


/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/**
 *struct passinfo - contains arguments to pass into a function and
 *allowing uniform prototype for function pointer struct
 *@status: the return status of the last executed command
 *@arg: a string generated from getline containing args
*@err_num: error code for exit()s
 *@argv: an array of strings generated from argument
 *@path: a string path for the current command
 *@argc: argument count
 *@line_count: error count
 *@fname: program filename
 *@env: linked list local copy of environ
 *@history: the history node
 *@alias: alias node
 *@env_changed: on if environ was changed
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
*@linecount_flag: if on count this line of input
 *@environ: custom modified copy of environ from LL env
 */
typedef struct passinfo
{
	int status;
	char *arg;
	int err_num;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	int env_changed;
char **cmd_buf; 
	int cmd_buf_type; 
	int readfd;
int histcount;
int linecount_flag;
char **environ;

	
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/* shloop.c */
int hsh(info_t *, char **);
void find_cmd(info_t *);
void fork_cmd(info_t *);
int find_builtin(info_t *);

/* loophsh.c */
int loophsh(char **);

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* parser.c */
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* _vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

/* getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* getline.c */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* errors1.c */
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* memory.c */
int bfree(void **);

/* history.c */
int build_history_list(info_t *info, char *buf, int linecount);
char *get_history_file(info_t *info);
int read_history(info_t *info);
int renumber_history(info_t *info);
int write_history(info_t *info);

/* builtin.c */
int _mycd(info_t *);
int _myhelp(info_t *);
int _myexit(info_t *);

/* lists.c */
size_t print_list_str(const list_t *);
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
void free_list(list_t **);
int delete_node_at_index(list_t **, unsigned int);

/* environ.c */
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);
char *_getenv(info_t *, const char *);

/* builtin1.c */
int _myalias(info_t *);
int _myhistory(info_t *);

/* string.c */
char *_strcat(char *, char *);
int _strcmp(char *, char *);
int _strlen(char *);
char *starts_with(const char *, const char *);

/* atoi.c */
int is_delim(char, char *);
int interactive(info_t *);
int _atoi(char *);
int _isalpha(int);

/* tokenizer.c */
char **strtow2(char *, char);
char **strtow(char *, char *);

#endif
