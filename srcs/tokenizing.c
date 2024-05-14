/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:14:56 by sabakar-          #+#    #+#             */
/*   Updated: 2024/05/14 15:18:16 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./tokenizing.h"

int main (int ac, char *av[], char *envp[])
{
  ft_tokenize('args'); // taking the args after checking it if it's closed correctly
  return (0);
}

void ft_tokenize (char *sr)
{
  int x;
  char *token;
  char flag_value[20]; // This need to be allocated or something I'm not sure. I was struggling so I just used in it's simple form.
  int j;

  x = 0;
  while ((*sr >= 9 && *sr <= 13) || *sr == 32) // Skipping the white spaces
    sr++;
  while (*sr)
  {
    if (*sr == '-' || ft_isalpha(*sr))
    {
      while (ft_isalnum(*sr) || *sr == '-')
        token[x++] = *sr++;
      token[x] = '\0';
      if (x > 1 && *sr == '=') // Flag with argument (e.g., -f=value)
      {
        sr++; // skippinf the char
        j = 0;
        while (ft_isalnum(*sr) || *sr == '_')
          flag_value[j++] = *sr++;
        flag_value[j] = '\0';
        ft_add_token(2, ft_strcat(token, "=")); // Flag with value (e.g., -f=value)
        ft_add_token(2, flag_value);
      }
      else
        ft_add_token(0, token); // Command or single flag but I'm not sure if just a flag is even allowed.
    }
    else if (*sr == '|')
      ft_add_token(3, "|"); // Add pipe token
    else if (*sr == '<' || *sr == '>')
    {
      char redir[2];
      redir[0] = *sr;
      sr++; // Check for next character (e.g., '<') for redirection filename
      if (*sr != '\0' && ft_isalnum(*sr) || *sr == '_')
        redir[1] = *sr;
      else
      {
        redir[1] = '\0';
        sr--; // Move back if no filename provided
      }
      ft_add_token(4, redir); // Add redirection token
    }
    else
    {
      printf("Error: Unknown character '%c'\n", *sr);
      exit(EXIT_FAILURE);
    }
    sr++;
  }
}

void ft_add_token (int type, char *cmd)
{
  t_tokens *new_token;

  new_token = (t_tokens *)malloc(sizeof(t_tokens));
  if (!new_token)
    return ;
  new_token->type = type;
  ft_strcpy(new_token->token, cmd);
  new_token->next = new_token;
}

t_cmds_tree *ft_parse_tokens(t_tokens *tokens)
{
  t_cmds_tree *tokens_tree;

  tokens_tree = (t_cmds_tree *)malloc(sizeof(t_cmds_tree));
  if (!tokens_tree)
    return NULL;
  if (tokens->type == 0)
  {
    tokens_tree->type = 0;
    tokens_tree->left_kid = NULL;
    tokens_tree->right_kid = NULL;
    tokens_tree->cmd = (char *)malloc(ft_strlen(tokens->token) + 1);
    ft_strcpy(tokens_tree->cmd, tokens->token);
    return (tokens_tree);
  }
  else if (tokens->type == 3)
  {
    tokens_tree->type = 1;
    tokens_tree->left_kid = ft_parse_tokens(tokens->next);
    tokens_tree->right_kid = ft_parse_tokens(tokens->next->next);
    return (tokens_tree);
  }
  else if (tokens->type == 4)
  {
    tokens_tree->type = 0; // Assuming redirection modifies the command 
    tokens_tree->left_kid = NULL;
    tokens_tree->right_kid = NULL;
    int value_len = strlen(tokens->token) + 1; // Include redirection symbol

    // Allocate memory for value considering redirection symbol and filename
    tokens_tree->cmd = malloc(value_len + strlen(tokens->next->token) + 1);
    ft_strcpy(tokens_tree->cmd, tokens->token);

    // Append filename after redirection symbol
    strcat(tokens_tree->cmd, tokens->next->token);

    return tokens_tree;
  }
  else
  {
    printf("Error: Unexpected token type\n");
    exit(1);
  }
}
