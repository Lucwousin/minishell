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

t_sh_env	g_env;

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

static int	run_test(char *file, DIR *test_dir, bool gen)
{
	char	*files[3];
	int32_t	fds[3];

	pid_t	pid = fork();
	if (pid == -1)
		return (2);
	if (pid != 0)
		return (0);

	files[0] = get_file_name("./test/cases/", file, NULL);
	files[1] = get_file_name("./test/output/", file, gen ? "-expected" : "-output");
	files[2] = VALGRIND ? get_file_name("./test/output/", file, "-valgrind") : NULL;

	if (VALGRIND)
		setenv("VG_LOG", files[2], true);
	fds[0] = open(files[0], O_RDONLY);
	fds[1] = open(files[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	fds[2] = VALGRIND ? open(files[2], O_WRONLY | O_TRUNC | O_CREAT, 0644) : -1;

	free(files[0]);
	free(files[1]);
	free(files[2]);

	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	dup2(fds[1], STDERR_FILENO);
	dup2(fds[2], 3);

	close(fds[0]);
	close(fds[1]);
	close(fds[2]);

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
	int			stat;

	if (generate)
		--argc, ++argv;
	init_environment();
	rv = run_tests(--argc, ++argv, generate);
	printf("run_tests returned: %d\n", rv);
	clean_environment();
	while (wait(&stat) > 0)
	{
		if (WIFEXITED(stat))
			printf("test returned: %d\n", WEXITSTATUS(stat));
		if (WIFSIGNALED(stat))
			printf("test signaled: %d\n", WSTOPSIG(stat));
	}
	return (rv);
}