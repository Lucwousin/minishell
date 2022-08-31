#include "test.h"
#include "minishell.h"
#include <environ.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#ifndef VALGRIND
# define VALGRIND 0
#endif

t_sh_env	g_globals;

static int	ignore(struct dirent *entry, int argc, char **argv)
{
	for (int i = 0; i++ < argc; argv++)
		if (strcmp(entry->d_name, *argv) == 0)
			return (1);
	return (0);
}

static char	*get_file_name(const char *pre, const char *test, const char *suf)
{
	size_t	pl, tl, sl;
	char	*name;

	pl = strlen(pre);
	tl = strlen(test);
	sl = suf ? strlen(suf) : 0;
	name = calloc(pl + tl + sl + 1, sizeof(char));
	memcpy(name, pre, pl);
	memcpy(name + pl, test, tl);
	if (suf)
		memcpy(name + pl + tl, suf, sl);
	return (name);
}

static void	redirect(const char *test, bool gen)
{
	int		fd;
	char	*file;

	file = get_file_name("./test/cases/", test, NULL);
	fd = open(file, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	free(file);
	file = get_file_name("./test/output/", test, gen ? "-expected" : "-output");
	fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	free(file);
	if (!VALGRIND)
		return ;
	file = get_file_name("./test/output/", test, "-valgrind");
	setenv("valgrind_log", file, true);
}

static int	run_test(const char *file, DIR *test_dir, bool generate)
{
	pid_t	pid = fork();
	if (pid == -1)
		return (2);
	if (pid != 0)
		return (0);
	redirect(file, generate);
	closedir(test_dir);
	perform_test();
	return (69);
}

static int	run_tests(int argc, char **argv, bool generate)
{
	DIR				*test_dir = opendir("./test/cases");
	struct dirent	*entry;

	if (test_dir == NULL) {
		perror("Can't find test dir!");
		return (2);
	}
	while (1) {
		errno = 0;
		entry = readdir(test_dir);
		if (entry == NULL)
			break ;
		if (entry->d_name[0] == '.' || ignore(entry, argc, argv))
			continue ;
		if (run_test(entry->d_name, test_dir, generate))
			break ;
	}
	if (errno != 0) {
		perror("run_tests");
		closedir(test_dir);
		return (2);
	}
	closedir(test_dir);
	return (0);
}

int	main(int argc, char **argv)
{
	const bool	generate = argc > 1 && strcmp(argv[1], "--generate") == 0;
	int			rv;

	if (generate)
		--argc, ++argv;
	init_environment();
	rv = run_tests(--argc, ++argv, generate);
	clean_environment();
	while (wait(NULL) > 0);
	return (rv);
}