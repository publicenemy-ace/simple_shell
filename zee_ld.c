#include "main.h"

alias_t *appd_alias_end(alias_t **head, char *name, char *value);
void setfree_alias_list(alias_t *head);
list_t *appd_node_end(list_t **head, char *dir);
void set_free_list(list_t *head);

/**
 * appd_alias_end - Adds/Append a node to the end of a alias_t linked list.
 * @h: A pointer to the head of the list_t list.
 * @n: The name of the new alias to be added.
 * @v: The value of the new alias to be added.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
alias_t *appd_alias_end(alias_t **h, char *n, char *v)
{
	alias_t *new_node = malloc(sizeof(alias_t));
	alias_t *last;

	if (!new_node)
		return (NULL);

	new_node->next = NULL;
	new_node->n = malloc(sizeof(char) * (_strlen(n) + 1));
	if (!new_node->n)
	{
		free(new_node);
		return (NULL);
	}
	new_node->v = v;
	_strcpy(new_node->n, n);

	if (*h)
	{
		last = *h;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*h = new_node;

	return (new_node);
}

/**
 * appd_node_end - Adds/Append a node to the end of a list_t linked list.
 * @h: A pointer to the head of the list_t list.
 * @d: The directory path for the new node to contain.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
list_t *appd_node_end(list_t **h, char *d)
{
	list_t *new_node = malloc(sizeof(list_t));
	list_t *last;

	if (!new_node)
		return (NULL);

	new_node->d = d;
	new_node->next = NULL;

	if (*h)
	{
		last = *h;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*h = new_node;

	return (new_node);
}

/**
 * setfree_alias_list - Frees a alias_t linked list.
 * @h: The head of the alias_t list.
 */
void setfree_alias_list(alias_t *h)
{
	alias_t *next;

	while (h)
	{
		next = hd->next;
		free(hd->n);
		free(h->v);
		free(h);
		h = next;
	}
}

/**
 * set_free_list - Frees a list_t linked list.
 * @h: The head of the list_t list.
 */
void set_free_list(list_t *h)
{
	list_t *next;

	while (h)
	{
		next = h->next;
		free(h->dir);
		free(h);
		h = next;
	}
}
