/*
This file is part of daemon.

daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Linux headers
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Misc headers
#include "xarray.h"

// UID and GID globals
static uid_t uid;
static gid_t gid;

// Checks if a path is a readable file
static bool checkFile(const char *path);

// Generates check paths from PATH
static const char *const *getCheckPaths(
	const char *argument,
	const char *pathEnv);

// Gets the path of the binary, given the user's requested name/path
static const char *getPath(const char *argument);

int main(const int argc, const char *const *const argv)
{
	// Initialise globals
	uid = getuid();
	gid = getgid();

	// Need at least 2 arguments
	if (argc < 2) {
		fputs("Expected at least one argument: binary to execute\n", stderr);
		exit(1);
	}

	// Get path of binary
	const char *const path = getPath(argv[1]);

	// Daemonize
	if (daemon(true, false) == -1) abort();

	// Create new argv
	char **newArgs = malloc(sizeof (char *) * argc);
	newArgs[0] = strcpy(malloc(strlen(path) + 1), path);
	memcpy(newArgs + 1, argv + 2, (argc - 1) * sizeof (char *));
	newArgs[argc - 1] = NULL;

	// exec program
	execv(path, newArgs);
	abort();
}

const char *getPath(const char *const argument)
{
	// Get PATH variable
	const char *const pathEnv = getenv("PATH");

	// Get paths to check, in order, from PATH
	const char *const *const paths = getCheckPaths(argument, pathEnv);

	// Check paths to see which one is readable
	const size_t nPaths = xa_size(paths);
	for (size_t i = 0; i < nPaths; ++i) {
		if (checkFile(paths[i])) {
			return paths[i];
		}
	}

	fprintf(stderr, "File not found: %s\n", argument);
	exit(1);
}

bool checkFile(const char *const path)
{
	// Ensure can open and read path
	int fileDesc = open(path, O_RDONLY);
	if (fileDesc == -1) return false;
	else {
		close(fileDesc);
		return true;
	}
}

const char *const *getCheckPaths(
	const char *const argument,
	const char *const pathEnv)
{
	const size_t argLength = strlen(argument);

	// xarray to store paths to try based on PATH
	const char **checkPaths = xa_alloc(sizeof (char *), 0);

	// Add argument on its own to try
	xa_extend(&checkPaths, 1);
	checkPaths[0] = argument;

	// Copy of path string to manipulate with strtok()
	char *const pathCpy = malloc(strlen(pathEnv) + 1);
	strcpy(pathCpy, pathEnv);

	// Substrings from PATH
	char *currentStr = strtok(pathCpy, ":");

	// While we have substrings to work with...
	while (currentStr) {
		// Add new string, currentStr/argument
		xa_extend(&checkPaths, 1);
		char *newPath = malloc(strlen(currentStr) + argLength + 2);
		sprintf(newPath, "%s/%s", currentStr, argument);
		checkPaths[xa_size(checkPaths) - 1] = newPath;

		// Load next string to work with
		currentStr = strtok(NULL, ":");
	}

	return checkPaths;
}
