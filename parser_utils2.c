/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichaiq <ichaiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 03:30:54 by ichaiq            #+#    #+#             */
/*   Updated: 2023/06/03 02:11:10 by ichaiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token_info *next_word(char *str, char *delimiter)
{
    t_token_info *info;
    int i;
    int flag;
    char *c;

    flag = 0;
    if (!str)
        return (NULL);
    info = ft_calloc(1, sizeof(t_token_info));
    i = 0;
    while (str[i] != '\0')
    {
        // c = ft_strchr(delimiter, str[i]);
        if ((c = ft_strchr("<>", str[i]) ) != 0)
        {
            printf("test : %s\n", &str[i]);
            
            info->word = ft_substr(str, 0, i);
            while (str[i + 1] == *c && !ft_strchr("<>",*c))
                i++;
            // printf("str[i] %c\n",str[i]);
            // printf("str[i + 1] %c\n",str[i + 1]);
            if (str[i] == str[i + 1])
                info->limiter = ft_substr(str, i, 2);
            else if (ft_isdigit(str[i-1]))
            {
                // while (ft_isdigit(str[--i]))
                //     ;
                
                info->limiter = ft_substr(str, i, 2);
                info->word = ft_substr(str+1, 0, i);
            }
            else {
                info->limiter = &str[i];
                info->word = ft_substr(str+1, 0, i);
                // info->word = ft_substr(str, 0, i);
            }
            info->next_start = &str[i +1 ];
            return info;
        }
        if ((c = ft_strchr(delimiter, str[i]) ))
        {
            // puts("delimiter found\n");
            info->word = ft_substr(str, 0, i);
            while (str[i + 1] == *c && !ft_strchr("<>", *c))
                i++;
            info->limiter = &str[i];
            info->next_start = &str[i + 1];
            return info;
        }
        i++;
        if (!str[i])
        {
            info->word = str;
            info->limiter = NULL;
            info->next_start =NULL;
            return info;
        }
    }
    return NULL;
}

int insert_to_lexer(char *str, t_parse_utils *u)
{
    t_list *node;
    t_lex *lex;
    t_lex *last_lex;

    lex = ft_calloc(1, sizeof(t_lex));
    // t_list *last_node;
    
    // node = u->list_cmds;
    node = ft_lstlast(u->list_cmds);
    if (!node )
    {
        lex->type = CMD;
        lex->command_name = str;
        node = ft_lstnew(lex);
        ft_lstadd_back(&u->list_cmds, node);
        printf("CMD %s in lexer\n",str);
        return (1);
    }
    
    last_lex = (t_lex*)node->content;
    if (*str == '|')
    {
        lex->type = PIPE;
        ft_lstadd_back(&u->list_cmds, ft_lstnew(lex));
        printf("PIPE in lexer\n");
        return (1);

    }
    else if (*str == '>' || (ft_strlen(str) == 2 && (str[1] == '>')))
    {
        ///     ***IMPORTANT***
        //   create a new fd in case there are multiple outfiles
        // ex: ls -al | grep .c < Makefile > outfiles >oj
        lex->type = REDIRECTION_OUTPUT;
        lex->command_name = last_lex->command_name;
        // printf("str is : %s\n",str);

        if ((ft_strlen(str) == 2 && (str[1] == '>')))
        {
            str[1] = '\0';
            lex->fd = ft_atoi(str);
        }
        ft_lstadd_back(&u->list_cmds, ft_lstnew(lex));
        printf("REDIRECTION_OUTPUT %d in lexer\n",lex->fd);

        return (1);
        
    }
    else if (*str == '<' || (ft_strlen(str) == 2 && str[1] == '<'))
    { 
        ///     ***IMPORTANT***
        //   create a new fd in case there are multiple outfiles
        // ex: ls -al | grep .c < Makefile > outfiles >oj
        lex->type = REDIRECTION_INPUT;
        lex->command_name = last_lex->command_name;
        
        
        if ((ft_strlen(str) == 2 && (str[1] == '<' )))
        {
            str[1] = '\0';
            lex->fd = ft_atoi(str);

        }
        ft_lstadd_back(&u->list_cmds, ft_lstnew(lex));
        printf("REDIRECTION_INPUT %d in lexer\n",lex->fd);
        return (1);

        
    }
    else if (last_lex->type == PIPE)
    {
        lex->type = CMD;
        lex->command_name = str;
        node = ft_lstnew(lex);
        ft_lstadd_back(&u->list_cmds, node);
        printf("CMD '%s' in lexer\n",str);
        return (1);
    }
    else if (last_lex->type == CMD || last_lex->type == ARG)
    {
        lex->type = ARG;
        // lex->last
        lex->variable = str;
        lex->command_name = last_lex->command_name;
        ft_lstadd_back(&u->list_cmds, ft_lstnew(lex));
        printf("ARGS '%s' in lexer\n",str);
        return (1);

    }
    else if ((last_lex->type == REDIRECTION_OUTPUT || last_lex->type == REDIRECTION_INPUT )
         && !(last_lex->fd == -1 && !last_lex->filename))
    {

        last_lex->filename = str;
        lex->variable = str;
        // ft_lstadd_back(&u->list_cmds, ft_lstnew(lex));
        printf("modified REDIRECT OF %s TO '%s' in lexer\n", last_lex->command_name, str);
        return (1);

    }
    
    // if (last_lex->type == CMD)
    //     printf("last is cmd \n");
    return (0);
}