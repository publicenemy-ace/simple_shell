#include "main.h"
int (*get_builtin(char *command))(char **args, char **front);
int exit_shell(char **args, char **front);
int cd_shell(char **args, char __attribute__((__unused__)) **front);
int help_shell(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin - Matches a command with a corresponding
 * shellby builtin function.
 * @command: Command to match.
 * Return: A function which points to corresponding builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", exit_shell },
		{ "env", env_shell },
		{ "setenv", setenv_shell },
		{ "unsetenv", unsetenv_shell },
		{ "cd", cd_shell },
		{ "alias", alias_shell },
		{ "help", help_shell },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * exit_shell - Terminates normal process for the shellby shell.
 * @args: Array of arguments containing the exit value.
 * @front: Double pointer to beginning of args.
 * Return: If no arguments - -3.
 * If given exit value is invalid - 2.
 * Otherwise - exits with the given status value.
 * Description: Upon returning -3, the program exits back in the main function.
 */
int exit_shell(char **args, char **front)
{
	int i, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			len_of_int++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= len_of_int && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (make_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (make_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	setfree_alias_list(aliases);
	exit(num);
}

/**
 * cd_shell - Changes current directory of shellby process.
 * @args: Array of arguments.
 * @front: Double pointer to beginning of args.
 * Return: If the given string is not a directory - 2.
 * If error occurs - -1.
 * Otherwise - 0.
 */
int cd_shell(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (getenv("OLDPWD") != NULL)
					(chdir(*getenv("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (make_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (make_error(args, 2));
			}
		}
	}
	else
	{
		if (getenv("HOME") != NULL)
			chdir(*(getenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
		return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = oldpwd;
	if (setenv_shell(dir_info, dir_info) == -1)
		return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	if (setenv_shell(dir_info, dir_info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(oldpwd);
	free(pwd);
	free(dir_info);
	return (0);
}

/**
 * help_shell - Displays information about shellby builtin commands.
 * @args: Array of arguments.
 * @front: Pointer to beginning of args.
 * Return: If error occurs - -1.
 * Otherwise - 0.
 */
int help_shell(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		print_help_all();
	else if (_strcmp(args[0], "alias") == 0)
		print_help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		print_help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		print_help_exit();
	else if (_strcmp(args[0], "env") == 0)
		print_help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		print_help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		print_help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		print_help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
