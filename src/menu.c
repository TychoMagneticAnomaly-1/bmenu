#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "menu.h"
#include "tio.h"

#define MAX_MENU_OPTIONS   10
#define MAX_MENU_LENGTH    100
#define MAX_COMMAND_LENGTH 1000

static char *menu_title = "Select Option";
static char *menu_config = ".bmenu";
static char *menu[MAX_MENU_OPTIONS];
static char *command[MAX_MENU_OPTIONS];
static int  menu_count = 0;

static void menu_create(char *);
static int  menu_exists(char *);

void menu_init(void) {
	tio_init_terminal();
}

void menu_end(void) {
	tio_restore_terminal();
}

void menu_set_title(char *title) {
	menu_title = malloc(strlen(title) + 1);
	strcpy(menu_title, title);
}

void menu_set_config(char *config) {
	menu_config = malloc(strlen(config) + 1);
	strcpy(menu_config, config);
}

/*
 * Load the menu config file. Return a non-zero result if anything goes
 * wrong.
 */
int menu_load(void) {
	int i, j;
	char *config = menu_config;
	char c;
	char *menuConfigPath;

	// Lets get the config file path. If it is the same as MENU_CONFIG (i.e. default)
	// then we need to build the full path from the $HOME env variable. Otherwise,
	// the full path should already be provided.
	if (*config == '/') {
		menuConfigPath = config;
	} else {

		char *homeDir = getenv("HOME");

		if (homeDir == NULL)
			return 1;

		menuConfigPath = malloc(strlen(homeDir) + strlen(config) + 2);

		if (menuConfigPath == NULL)
			return 3;

		strcpy(menuConfigPath, homeDir);
		strcat(menuConfigPath, "/");
		strcat(menuConfigPath, config);

		if (!menu_exists(menuConfigPath))
			menu_create(menuConfigPath);
	}

	// Open file
	FILE *menuConfig = fopen(menuConfigPath, "r");

	// If there was an issue opening the file, return to main()
	if (menuConfig == NULL)
		return 2;

	// Loop over config file and store menu items
	for (i = 0; i < MAX_MENU_OPTIONS; ++i) {
		
		// Allocate menumemory
		menu[i] = malloc(MAX_MENU_LENGTH);
		memset(menu[i], 0, MAX_MENU_LENGTH);
		
		// Allocate command memory
		command[i] = malloc(MAX_COMMAND_LENGTH);
		memset(command[i], 0, MAX_COMMAND_LENGTH);
		
		// Getting menu text
		for (j = 0; (c = fgetc(menuConfig)) != EOF && c != ':' && c != '\n'; ++j) {
			menu[i][j] = c;
		}
		if (c == '\n') {
			free(menu[i]);
			free(command[i]);
			--i;
			continue;
		}
		if (c == EOF) {
			break;
		}
		
		// Getting menu command
		for (j = 0; (c = fgetc(menuConfig)) != EOF && c != '\n'; ++j) {
			command[i][j] = c;
		}
		if (c == EOF) {
			break;
		}
	}
	
	// Keep track of menu count
	menu_count = i;

	return 0;
}

int menu_get_count(void) {
	return menu_count;
}

char *menu_get_config_path(void) {
	return menu_config;
}

void menu_show(char *version) {
	int i;
	int max_cols = tio_get_cols();
	
	tio_move_cursor(1, 2);
	
	tio_set_text_normal();
	tio_set_text_bold();
	printf("B-MENU v%s", version);
	tio_set_text_normal();
	
	tio_move_cursor(2, 1);
	
	for (i = 0; i < max_cols; ++i) {
		printf("=");
	}
}

void menu_free_all(void) {
	int i;

	for (i = 0; i <= menu_count; ++i) {
		free(menu[i]);
		free(command[i]);
	}
}

///////////////////
// STATIC FUNCTIONS
///////////////////

static void menu_create(char *path) {
	FILE *menu = fopen(path, "w");

	// Return if we can't open the file. menu_load() will
	// ultimately return an error code to main which will
	// terminate the program with an error message.
	if (menu == NULL)
		return;

	fprintf(menu, "Clear Screen:/bin/clear\n");
	fprintf(menu, "Dir Listing:/bin/ls -l");
	fclose(menu);
}

static int menu_exists(char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}
