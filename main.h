#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define END_OF_FILE -2
#define EXIT -3

/* Global environemnt */
extern char **environ;
/* Global program name */
char *name;
/* Global history counter */
int hist;

/**
 * struct list_s - A new struct type defining a linked list.
 * @dir: A directory path.
 * @next: A pointer to another struct list_s.
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;

/**
 * struct builtin_s - A new struct type defining builtin commands.
 * @name: The name of the builtin command.
 * @f: A function pointer to the builtin command's function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/**
 * struct alias_s - A new struct defining aliases.
 * @name: The name of the alias.
 * @value: The value of the alias.
 * @next: A pointer to another struct alias_s.
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

/* Global aliases linked list */
alias_t *aliases;

/* Main Helpers */
ssize_t getline(char **buff_lineptr, size_t *s, FILE *strm);
void *mem_realloc(void *ptr, unsigned int old_s, unsigned int new_s);
char **_str_token(char *line, char *del);
char *get_cmd_location(char *cmd);
list_t *get_path_dir(char *path);
int run(char **args, char **front);
void free_list(list_t *head);
char *_itoa(int num);

/* Input Helpers */
void part_line(char **line, ssize_t read);
void replace_variable_(int *exe_ret);
char *fetch_args(char *line, int *exe_ret);
int part_args(char **args, char **front, int *exe_ret);
int execute_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int valid_args(char **args);
void set_args_free(char **args, char **front);
char **displace_aliases(char **args);

/* String functions */
int _strlen(const char *s);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);

/* Builtins */
int (*get_builtin(char *command))(char **args, char **front);
int exit_shell(char **args, char **front);
int env_shell(char **args, char __attribute__((__unused__)) **front);
int setenv_shell(char **args, char __attribute__((__unused__)) **front);
int unsetenv_shell(char **args, char __attribute__((__unused__)) **front);
int cd_shell(char **args, char __attribute__((__unused__)) **front);
int alias_shell(char **args, char __attribute__((__unused__)) **front);
int help_shell(char **args, char __attribute__((__unused__)) **front);

/* Builtin Helpers */
char **_duplicate_env(void);
void release_env(void);
char **_acqenv(const char *var);

/* Error Handling */
int make_error(char **args, int err);
char *error_env(char **args);
char *error_1(char **args);
char *error_2_exit(char **args);
char *error_2_cd(char **args);
char *error_2_syntax(char **args);
char *error_126(char **args);
char *error_127(char **args);

/* Linkedlist Helpers */
alias_t *appd_alias_end(alias_t **head, char *name, char *value);
void setfree_alias_list(alias_t *head);
list_t *appd_node_end(list_t **head, char *dir);
void set_free_list(list_t *head);

void print_help_all(void);
void print_help_alias(void);
void print_help_cd(void);
void print_help_exit(void);
void print_help_help(void);
void print_help_env(void);
void print_help_setenv(void);
void print_help_unsetenv(void);
void print_help_history(void);

int process_file_cmd(char *file_path, int *exe_ret);
int open_error(char *file_path);
#endif /* _MAIN_H_ */
