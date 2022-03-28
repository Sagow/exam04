#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void	ft_putstr(char *str)
{
	while (*str)
		write(2, str++, 1);
}

int		ft_execve(int tmp_fd, char **argv, int i, char **env)
{
	argv[i] = NULL;
	close(tmp_fd);
	execve(argv[0], argv, env);
	ft_putstr("erreur\n");
	return (1);
}

int		main(int argc, char **argv, char **env)
{
	int tmp_fd, i = 0, pid = 0;
	int	fd[2];
	(void)argc;

	tmp_fd = dup(STDIN_FILENO);

	while (argv[i] && argv[i + 1])
	{
		argv = argv + i + 1;
		i = 0;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (!strcmp(argv[0], "cd"))
		{
			if (i != 2)
				ft_putstr("I call bullshit\n");
			else if (chdir(argv[1]))
				ft_putstr("chdir a dit grrrr\n");
		}
		else if (i && (argv[i] == NULL || !strcmp(argv[i], ";")))
		{
			pid = fork();
			if (!pid)
			{
				dup2(tmp_fd, STDIN_FILENO);
				return (ft_execve(tmp_fd, argv, i, env));
			}
			else
			{
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if (i && !strcmp(argv[i], "|"))
		{
			pipe(fd);
			pid = fork();
			if (!pid)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				return (ft_execve(tmp_fd, argv, i , env));
			}
			else
			{
				close(tmp_fd);
				close(fd[1]);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(fd[0]);
				close(fd[0]);
			}
		}
	}
	close(tmp_fd);
	return (0);
}