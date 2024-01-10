#include "main.h"

void *mem_realloc(void *ptr, unsigned int old_s, unsigned int new_s);
void update_lineptr(char **_lineptr, size_t *n, char *buffer, size_t buff_s);
ssize_t getline(char **buff_lineptr, size_t *s, FILE *strm);

/**
 * mem_realloc - Reallocates a memory block using malloc and free.
 * @ptr: A pointer to the memory previously allocated.
 * @old_s: The size in bytes of the allocated space for pointer.
 * @new_s: The size in bytes for the new memory block.
 *
 * Return: If new_size == old_size - ptr.
 *         If new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *mem_realloc(void *ptr, unsigned int old_s, unsigned int new_s)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_s == old_s)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_s);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_s == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_s);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (index = 0; index < old_s && index < new_s; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * update_lineptr - Reassigns the lineptr variable for getline.
 * @_lineptr: A buffer to store an input string.
 * @s: The size of lineptr.
 * @buffer: The string to assign to lineptr.
 * @buff_s: The size of buffer.
 */
void update_lineptr(char **_lineptr, size_t *s, char *buffer, size_t buff_s)
{
	if (*_lineptr == NULL)
	{
		if (buff_s > 120)
			*s = buff_s;
		else
			*s = 120;
		*_lineptr = buffer;
	}
	else if (*s < buff_s)
	{
		if (buff_s > 120)
			*s = buff_s;
		else
			*s = 120;
		*_lineptr = buffer;
	}
	else
	{
		_strcpy(*_lineptr, buffer);
		free(buffer);
	}
}

/**
 * getline - Reads input from a stream.
 * @buff_lineptr: A buffer to store the input.
 * @s: The size of lineptr.
 * @strm: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t getline(char **buff_lineptr, size_t *s, FILE *strm)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	if (input == 0)
		fflush(strm);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = mem_realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	update_lineptr(buff_lineptr, s, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}
