/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esimpson <esimpson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 01:32:13 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 17:51:31 by esimpson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "parsing.h"
# include "tokenizing.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef enum e_err_msg
{
	ERRMSG_CMD_NOT_FOUND,
	ERRMSG_NO_SUCH_FILE,
	ERRMSG_PERM_DENIED,
	ERRMSG_AMBIGUOUS,
	ERRMSG_TOO_MANY_ARGS,
	ERRMSG_NUMERIC_REQUI
}					t_err_msg;

typedef enum e_err_no
{
	ENO_SUCCESS,
	ENO_GENERAL,
	ENO_CANT_EXEC = 126,
	ENO_NOT_FOUND,
	ENO_EXEC_255 = 255
}					t_err_no;

typedef enum e_ast_direction
{
	TD_LEFT,
	TD_RIGHT
}					t_ast_direction;

typedef struct s_err
{
	t_err_no		no;
	t_err_msg		msg;
	char			*cause;
}					t_err;

typedef struct s_path
{
	t_err			err;
	char			*path;
}					t_path;

typedef struct s_minishell
{
	char			*line;
	t_token			*tokens;
	t_token			*curr_token;
	t_node			*ast;
	int				exit_s;
	bool			signint_child;
	t_parse_err		parse_err;
	int				stdin;
	int				stdout;
	char			**environ;
	t_env			*envlst;
	bool			heredoc_sigint;
	struct termios	original_term;
}					t_minishell;

/* ****************************   BUILTINS   ****************************** */
// * cd.c ********************************************************************
int					ft_cd(char **argv, int *exit_code, t_env *env_list);

// * echo.c ******************************************************************
int					ft_echo(char **av);

// * env.c *******************************************************************
int					ft_env(t_minishell *myshell);

// * exit.c ******************************************************************
void				ft_exit(char **argv, int *exit_code);

// * export.c ****************************************************************
int					ft_export(char **argv, t_env **env_list, int *exit_status);

// * pwd.c *******************************************************************
int					ft_pwd(int *exit_status);

// * unset.c *****************************************************************
int					ft_unset(char **argv, t_env **env_list, int *exit_status);
int					is_valid_key(char *key);

/* ****************************   ENV   ****************************** */
// * init_env.c **********************************************************
void				init_env_list(char **envp, t_env **env_list);
t_env				*new_env_node(char *key, char *value);
void				env_sort_insert(t_env **env_list, t_env *new_node);

// * utils_env.c ************************************

char				*get_env_value(t_env *env_list, char *key);
void				free_env_list(t_env *env_list);
void				free_env_node(t_env *node);
char				*get_env_value(t_env *env_list, char *key);

/* ****************************   CLEANING   ****************************** */
// * ft_clean_ms.c ***********************************************************
void				ft_clean_ms(t_minishell *myshell);

/* ******************************   EXEC   ******************************** */
// * error_msg.c *************************************************************
int					ft_err_msg(t_err err);

// * exec_builtin.c **********************************************************
int					ft_exec_builtin(char **args, t_minishell *myshell);
bool				ft_is_builtin(char *arg);

// * exec_redirect.c *********************************************************
int					ft_out(t_io_node *io_list, int *status);
int					ft_in(t_io_node *io_list, int *status);
int					ft_append(t_io_node *io_list, int *status);

// * exec_utils.c ************************************************************
void				*ft_garbage_collector(void *ptr, bool clean);
bool				ft_is_delimiter(char *delimiter, char *str);

// * exec.c ******************************************************************
int					ft_get_exit_status(int status);
int					ft_exec_node(t_node *tree, bool piped,
						t_minishell *myshell);

// * exist_check.c ***********************************************************
t_err				ft_check_exec(char *file, bool cmd);
t_err				ft_check_read(char *file);
t_err				ft_check_write(char *file);

// * ft_exec_simple_cmd.c ****************************************************
int					ft_check_redirection(t_node *node);
void				ft_reset_stds(bool piped, t_minishell *myshell);
int					ft_exec_simple_cmd(t_node *node, bool piped,
						t_minishell *myshell);

// * ft_get_path.c ***********************************************************
t_path				ft_get_path(char *cmd, t_env *env_list);

// * init_tree.c *************************************************************
void				ft_init_tree(t_node *node);
void				ft_heredoc(t_io_node *io, int p[2]);

/* ****************************   EXPANDER   ****************************** */

// * ft_clean_empty_strs.c ***************************************************
char				*ft_clean_empty_strs(char *str);

// * ft_expand_utils.c *******************************************************
bool				ft_is_valid_var_char(char c);
char				*ft_handle_normal_str(char *str, size_t *i);
char				*ft_handle_squotes(char *str, size_t *i);
char				*ft_handle_dquotes(char *str, size_t *i);

// * ft_expand.c *************************************************************
char				*ft_handle_dollar(char *str, size_t *i);
char				**ft_expand(char *str);

// * ft_expander_split.c *****************************************************
char				**ft_expander_split(char const *s);

// * ft_strip_quotes.c *******************************************************
char				*ft_strip_quotes(char *str);

// * heredoc_expander.c ******************************************************
void				ft_heredoc_expander(char *str, int fd);

/* ****************************   MINISHELL   ***************************** */
// * main_signals.c **********************************************************
void				ft_sigquit_handler(int num);
void				ft_init_signals(void);

#endif
