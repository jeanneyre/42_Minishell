#include "minishell.h"

int	built_in_cd(char *path, t_env **env)
{
	char *pwd;
	char *var_pwd;
	char **split;
	int i;
	char *user;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	var_pwd = ft_strjoin("OLDPWD=", pwd);
	if (!var_pwd)
		return (free(pwd), 1);
	if	(lst_set_var(env, var_pwd))
		return (free(pwd), free(var_pwd), 1);
	free(var_pwd);
	if (!path || !ft_strncmp(path, "~", 1))
	{
		user = NULL;
		if(lst_get_var(*env, "USER", &user))
			return (free(pwd), 1);
		split = ft_split_set(pwd, "/");
		if (!split)
			return (free(pwd), free(user), 1);
		i = 0;
		while (split[i])
			i++;
		i--;
		while (ft_strncmp(split[i], user, ft_strlen(user)))
		{
			free_array(split);
			free(pwd);
			if (chdir(".."))
				return (free(user), -1);			
			pwd = getcwd(NULL, 0);
			if (!pwd)
				return (free(user), 1);
			split = ft_split_set(pwd, "/");
			if (!split)
				return (free(pwd), free(user), 1);
			i = 0;
			while (split[i])
				i++;
			i--;
		}
		free(user);
		free_array(split);
	}	
	else
	{
		if (chdir(path))
			return (free(pwd), -1);
	}
	free(pwd);	
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	var_pwd = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (!var_pwd)
		return (1);
	if (lst_set_var(env, var_pwd))
		return (free(var_pwd), 1);
	free(var_pwd);
	return (0);
}