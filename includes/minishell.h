/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 15:12:55 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <struct_utils.h>
# include <get_next_line_samu.h>
# include <ft_global.h>
# include <memhandl.h>
# include <lexer.h>
# include <libft.h>
# include <sys/errno.h>
# include <pipex.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <ft_signals.h>
# include <ft_history.h>

//============================================================================//
//================================GLOBAL======================================//
int			g_flag;
//============================================================================//
//================================EXPORT======================================//
size_t		compare_var(char *var, char *node);
void		rl_replace_line(const char *text, int clear_undo);
int			execve_with_error_check(char **argv, char **env);
int			ft_built_in(char **input);
void		ft_free_input(char **input);
int			ft_excuter(char **input, char **env);
void		ft_unset(char **input);
void		ft_echo(char **input);
//============================================================================//
//================================EXPORT======================================//
int			ft_parsing(char	*str);
int			error_ft_parsing(char *str);
size_t		upper_str(char *str1, char *str2);
int			in_the_list(char *new, char **list, int len);
char		*get_max(t_list	*env);
void		print_min(t_list *env, char **list, int len);
void		print_exp(void);
size_t		have_any_equal(char *str);
void		print_env_cpy(void);
void		ft_env(char **input);
int			ft_export_var(char *input);
void		ft_export(char **input);
//============================================================================//
//============================================================================//
t_static	*ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);
char		*ft_get_var(char *var);
void		handler(int signal);
void		ft_remove_node(t_list *list, int index);
//============================================================================//
//=================================HISTORY====================================//
int			len_2_fill(int x, int new);
void		save_and_clear(void *content);
char		*ft_get_history_path(char **env);
void		ft_save_history_in_list(int fd, t_list **list);
int			fd_old_history_4_list(char **env, t_save_history *his);
void		ft_save_history(void);
//============================================================================//
//============================================================================//
int			ft_static_proccess(int fd_n, int i);
char		*ft_get_info_from_env(char **env, char *var_name);
void		change_content_env(t_list *list, char *content);
t_list		*search_node_env(char *str, t_list *env);
char		*get_var_from_node(t_list *node);
int			shlvl_err(t_list *shlvl, char *str1);
int			shlvl_sum(t_static *s);
void		init_default_env(t_static *s);
void		copy_external_env(t_static	*s, char **env);
t_static	*ft_setup_struct(char **env, t_static *s);
void		*error_init_static(int x, int *i, t_static *s);
t_static	*init_static_struct(char **env);
//===========================================================================//
//================================EXECUTER===================================//
int			ft_exc_execution(char *cmd, char **env);
int			err_unexpected(int minor, int major, char first);
void		ft_init_exc_lex(t_exc_lex *lex);
int			ft_check_parse_normal_char(t_exc_lex *lex, int *space);
int			ft_check_parse_redirs(t_exc_lex *lex, int *space);
int			ft_check_dup_redir(char *input);
void		ft_put_status_redir(t_exc_lex *lex, char *input);
size_t		ft_strlen2(char *str);
char		*cut_input(char *str, int *i);
int			ft_exc_here_doc(t_argument *content, t_exc_lex *lex);
void		get_redir(t_argument *content);
int			ft_exc_change_input(t_argument *content, t_exc_lex *lex);
int			ft_exc_change_output_trc(t_argument *content, t_exc_lex *lex);
int			ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex);
int			ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex);
void		ft_exc_free_content(void *cnt_ptr);
void		*ft_exc_clear_content(t_list **result);
t_list		*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex);
char		*ft_exc_make_word(char *input);
int			ft_exc_lex_word(t_list **result, t_exc_lex *lex);
void		*ft_error_make_list(t_list **result, t_exc_lex *lex, int error);
t_list		*ft_make_list(t_exc_lex *lex);
void		*ft_not_closed_pipe(char **env);
t_list		*ft_exc_lex_input(char *input, int std[2]);
char		*ft_good_strjoin(char *s1, char*s2);
char		*ft_exc_make_cmd(t_list **input);
int			ft_exc_make_redir(char *cmd, t_input *line);
int			ft_executer_exec(t_input *input, char **env);
int			executer(char *cmd, t_input *input);
//============================================================================//
//===========================PROCCESS_MAKER===================================//
int			pipex_error_make_redir(int pipe[2]);
size_t		get_next_index_pipex(char *str);
size_t		count_pipes(char *str);
int			ft_first_child(char *cmd, int pipe[2]);
void		ft_is_valid_in(int fd, t_input *input);
int			ft_mid_child(char *cmd, int fd);
void		ft_wait_all_children(int pid);
int			ft_last_child(char *cmd, int fd);
char		*ft_strndup(const char *s1, size_t n);
char		*ft_get_cmd_pipex(char **cmd);
void		pipex(char *cmd);
void		ft_procces_maker(char *cmd, char **env);
//============================================================================//
//============================================================================//
void		ft_get_old_history(char **env, int *fd);
char		*get_var_info(char *var_name);
void		add_list_and_sort(t_list **list, t_list *new);
char		**list_to_matrix(t_list *list);
//============================================================================//
//===============================BUILT-IN=====================================//
void		ft_pwd(void);
void		ft_cd(char *path);
void		ft_exit(char **input);
int			is_valid_option(char *str);
void		not_valid_iden_error(char *str);
void		unset_var(char *var);
void		ft_err_msg(char *s1, char *path, char *s2);
void		ft_go_somewhere(t_static *s, char *somewhere);
void		ft_front_insert(t_list **front_node, t_list **new);
void		ft_print_int(int n, int *print_len);
void		not_numeric_error(char *str);
void		exit_with_no_argc(char **input);
void		ft_free_exit(char **input, int n);
//============================================================================//
//============================================================================//
int			shell_mode(char **env);

#endif
