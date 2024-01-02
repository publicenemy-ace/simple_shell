#include "main.h"

char *fetch_args(char *line, int *exit_stat);
int part_args(char **args, char **front, int *exit_stat);
int execute_args(char **args, char **front, int *exit_stat);
int handle_args(int *exe_ret);
int valid_args(char **args);

/**
 * fetch_args - Fetches a command from standard input.
 * @line: A buffer to store the command.
 * @exit_stat: The return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *fetch_args(char *line, int *exit_stat)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (fetch_args(line, exit_stat));
	}

	line[read - 1] = '\0';
	replace_variable(&line, exit_stat);
	handle_line(&line, read);

	return (line);
}

/**
 * part_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exit_stat: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int part_args(char **args, char **front, int *exit_stat)
{
	int ret, index;

	if (!args[0])
		return (*exit_stat);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = displace_aliases(args);
			ret = run_args(args, front, exit_stat);
			if (*exit_stat != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = displace_aliases(args);
			ret = execute_args(args, front, exe_ret);
			if (*exit_stat == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = displace_aliases(args);
	ret = execute_args(args, front, exit_stat);
	return (ret);
}

/**
 * execute_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exit_stat: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int execute_args(char **args, char **front, int *exit_stat)
{
	int ret, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exit_stat = ret;
	}
	else
	{
		*exit_stat = execute(args, front);
		ret = *exit_stat;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (ret);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @exit_stat: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int handle_args(int *exit_stat)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = fetch_args(line, exit_stat;
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (valid_args(args) != 0)
	{
		*exit_stat = 2;
		free_args(args, args);
		return (*exit_stat);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = part_args(args, front, exit_stat);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = part_args(args, front, exit_stat);

	free(front);
	return (ret);
}

/**
 * valid_args - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and arguments.
 *
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *	   Otherwise - 0.
 */
int valid_args(char **args)
{
	size_t i;
	char *cur, *nex;

	for (i = 0; args[i]; i++)
	{
		cur = args[i];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (i == 0 || cur[1] == ';')
				return (create_error(&args[i], 2));
			nex = args[i + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[i + 1], 2));
		}
	}
	return (0);
}
