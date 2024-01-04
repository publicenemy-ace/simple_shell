#include "main.h"

void sig_handler(int sig);
int run(char **args, char **front);

/**
 * sig_handler - Prints new prompt upon a signal.
 * @sig: Signal.
 */
void sig_handler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * run - Executes command in a child process.
 * @args: Array of arguments.
 * @front: Double pointer to the beginning of args.
 * Return: If error occurs - a corresponding error code.
 * Otherwise - Exit value of last executed command.
 */
int run(char **args, char **front)
{
	pid_t child_pid;
	int status, flag = 0, ret = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = get_cmd_location(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (make_error(args, 126));
		else
			ret = (make_error(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (flag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(command, args, environ);
			if (errno == EACCES)
				ret = (make_error(args, 126));
			release_env();
			set_args_free(args, front);
			setfree_alias_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(command);
	return (ret);
}

/**
 * main - Runs simple UNIX command interpreter.
 * @argc: Number of arguments supplied to program.
 * @argv: Array of pointers to the arguments.
 * Return: Return value of last executed command.
 */
int main(int argc, char *argv[])
{
	int ret = 0, retn;
	int *exe_ret = &retn;
	char *prompt = "$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _duplicate_env();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		ret = process_file_cmd(argv[1], exe_ret);
		release_env();
		setfree_alias_list(aliases);
		return (*exe_ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_args(exe_ret);
		release_env();
		setfree_alias_list(aliases);
		return (*exe_ret);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		ret = handle_args(exe_ret);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			release_env();
			setfree_alias_list(aliases);
			exit(*exe_ret);
		}
	}

	release_env();
	setfree_alias_list(aliases);
	return (*exe_ret);
}
