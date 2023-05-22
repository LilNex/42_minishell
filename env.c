/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahallali <ahallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:06:09 by ahallali          #+#    #+#             */
/*   Updated: 2023/05/22 02:07:03 by ahallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

t_node *create_node(char *value,char *variable)
{
  	t_node*new=malloc (sizeof(t_node));
  	if (!new)
		return (NULL);
	new->value=strdup(value);
	new->variable = strdup(variable);
	new->next = NULL;
	return (new);
}
void  insert_node(t_node **head,char *value,char *variable)
{
 	t_node *new=create_node(value,variable);
  	t_node *current;
  	if (*head == NULL)
    	*head=new;
	else
	{
 	current= *head;
  		while (current->next != NULL)
  		{
    		current=current->next;
  		}
  	current->next = new;
	}
}
void ft_env(char **env)
{
  t_node *head = NULL;
  // printf("here");
  while (*env)
  {
    char* tmp =strchr(*env,'=');
      if (tmp!=NULL)
        *tmp='\0';
      char *value = tmp+1;
      char *variable = *env;      
       insert_node(&head,value,variable);
      env++;
  }
print_list(head);
}