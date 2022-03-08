#include "getopt.c"

int main(int argc, char **argv)
{
	static struct getopt_option options[] = {
		{ "amend", 'a', 0 },
		{ "verbose", 'v', 0 },
		{ NULL, 'c', 1 }
	};

	static const char *args[] = {
		"...", "--amend=chris", "-vc", "test.o"
	};

	struct getopt_parser parser;
	int index;
	const char *value;

	getopt_init(&parser, args);

	index = getopt(&parser, &value, options, 3);
	printf("%s\n", value);

	index = getopt(&parser, &value, options, 3);
	printf("%c %s\n", options[index].flag, value);

	index = getopt(&parser, &value, options, 3);
	printf("%s\n", value);

	return 0;
}
