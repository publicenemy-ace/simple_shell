#include "main.h"

char **_duplicate_env(void);
void release_env(void);
char **_acqenv(const char *var);

/**
 * _duplicate_env - Creates a copy of the environment.
 * Return: If an error occurs - NULL.
 * Otherwise - a double pointer to the new copy.
 */
char **_duplicate_env(void)
{
	char **new_environ;
	size_t size;
	int index;

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);

	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(_strlen(environ[index]) + 1);

		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}
		_strcpy(new_environ[index], environ[index]);
	}
	new_environ[index] = NULL;

	return (new_environ);
}

/**
 * release_env - Frees the environment copy.
 */
void release_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _acqenv - Gets an environmental variable from the PATH.
 * @var: Name of environmental variable to get.
 * Return: If environmental variable does not exist - NULL.
 * Otherwise - a pointer to the environmental variable.
 */
char **_acqenv(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
