#include "main.h"

int token_len(char *str, char *del);
int count_tokens(char *str, char *del);
char **_str_token(char *line, char *del);

/**
 * token_len - Locates delimiter index marking the end of
 * the first token contained within a string.
 * @str: String to be searched.
 * @del: Delimiter character.
 * Return: The delimiter index marking the end of
 * the intitial token pointed to be str.
 */
int token_len(char *str, char *del)
{
	int index = 0, len = 0;

	while (*(str + index) && *(str + index) != *del)
	{
		len++;
		index++;
	}

	return (len);
}

/**
 * count_tokens - Counts number of delimited
 * words contained within a string.
 * @str: String to be searched.
 * @del: Delimiter character.
 * Return: Number of words contained within str.
 */
int count_tokens(char *str, char *del)
{
	int index, tokens = 0, len = 0;

	for (index = 0; *(str + index); index++)
		len++;

	for (index = 0; index < len; index++)
	{
		if (*(str + index) != *del)
		{
			tokens++;
			index += token_len(str + index, del);
		}
	}

	return (tokens);
}

/**
 * _str_token - Tokenizes a string.
 * @line: String.
 * @del: Delimiter character to tokenize the string.
 * Return: Points array containing the tokenized words.
 */
char **_str_token(char *line, char *del)
{
	char **ptr;
	int index = 0, tokens, t, letters, l;

	tokens = count_tokens(line, del);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < tokens; t++)
	{
		while (line[index] == *del)
			index++;

		letters = token_len(line + index, del);

		ptr[t] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[t])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[t][l] = line[index];
			index++;
		}

		ptr[t][l] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}
