#include "getopt.h"
#include "libc/libc.h"

void save(struct getopt_parser *parser, const char *needle, int index, int state);
const struct getopt_option *find_name(const char *name, size_t length,
                                      const struct getopt_option *opts, int optslength,
				      int *index);
const struct getopt_option *find_flag(int flag, const struct getopt_option *opts, int length,
                                      int *index);

extern int optdash;
extern int optend;
extern int optshort;

#ifndef GETOPT_C
#define GETOPT_C

#define START_STATE 0
#define END_STATE 1
#define SHORT_STATE 2

void save(struct getopt_parser *parser, const char *needle, int index, int state)
{
	parser->needle = needle;
	parser->index = index;
	parser->state = state;
}

void getopt_init(struct getopt_parser *parser, const char **argv)
{
	parser->argv = argv;
	parser->needle = NULL;
	parser->index = 1;
	parser->state = START_STATE;
}

void getopt_reset(struct getopt_parser *parser)
{
	parser->index = 1;
	parser->state = START_STATE;
}

const struct getopt_option *find_name(const char *name, size_t length,
                                      const struct getopt_option *opts, int optslength,
				      int *index)
{
	int i;
	const struct getopt_option *opt;

	for (i = 0; i < optslength; i++)
	{
		opt = &opts[i];
		if (opt->name) {
			if (strncmp(name, opt->name, length) == 0) {
				*index = i;
				return opt;
			}
		}
	}

	return NULL;
}

const struct getopt_option *find_flag(int flag, const struct getopt_option *opts, int length,
                                      int *index)
{
	int i;
	const struct getopt_option *opt;

	for (i = 0; i < length; i++)
	{
		opt = &opts[i];
		if (opt->flag > 0 &&
		    opt->flag == flag)
		{
			*index = i;
			return opt;
		}
	}

	return NULL;
}

int getopt(struct getopt_parser *parser, const char **value,
           const struct getopt_option *opts, int length)
{
	const char *needle, *name;
	size_t name_length;
	const char **argv;
	char ch;
	int index, optindex;
	const struct getopt_option *opt;

	/* Restore locals */
	argv = parser->argv;
	needle = parser->needle;
	index = parser->index;

	/* Jump to previous state */
	switch (parser->state) {
	case START_STATE: goto start_state;
	case END_STATE: goto end_state;
	case SHORT_STATE: goto short_state;
	default: goto end_state;
	}

	/* Find option that starts with a dash */
start_state:
	needle = argv[index];
	while (needle)
	{
		/* Is an argument? */
		ch = *needle++;
		if (ch == '-') {
			/* Is a long argument? */
			ch = *needle;
			if (ch == '-') {
				/* Is end of arguments? */
				needle += 1;
				ch = *needle;
				if (ch == '\0') {
					/* Set end state and return */
					parser->state = END_STATE;
					return getopt_end;
				}

				/* Find the end of argument name */
				name = needle;
				while (1)
				{
					/* The long option has a value */
					ch = *needle;
					if (ch == '=') {
						name_length = (size_t)(needle - name);
						opt = find_name(name, name_length, opts, length, &optindex);
						if (!opt)
							return getopt_ambiguous;

						/* Save and return */
						save(parser, needle, index + 1, START_STATE);
						*value = needle + 1;
						return optindex;
					}
					/* The long option does not have a value */
					else if (ch == '\0') {
						name_length = (size_t)(needle - name);
						opt = find_name(name, name_length, opts, length, &optindex);
						if (!opt)
							return getopt_ambiguous;

						/* Save and return */
						save(parser, needle, index + 1, START_STATE);
						*value = "";
						return optindex;
					}

					needle += 1;
				}
				/* While loop should never exit.
				 * This should never be hit */
			}

			/* Set short state and jump */
			parser->state = SHORT_STATE;
			goto short_state;
		}

		index += 1;
		needle = argv[index];
	}
	/* No more arguments to search */
	save(parser, needle, index, END_STATE);
	return getopt_end;

short_state:
	/* Check if flag or end of argument */
	ch = *needle;
	if (ch == '\0') {
		index += 1;
		goto start_state;
	}
	
	/* Return ambiguous, but allow next iteration */
	opt = find_flag(ch, opts, length, &optindex);
	if (!opt) {
		save(parser, needle + 1, index, SHORT_STATE);
		return getopt_ambiguous;
	}

	/* Need to check for value? */
	if (!opt->required) {
		save(parser, needle + 1, index, SHORT_STATE);
		*value = "";
		return optindex;
	}

	/* Find value */
	needle += 1;
	ch = *needle;
	if (ch != '\0') {
		save(parser, needle, index + 1, START_STATE);
		return getopt_required;
	}

	index += 1;
	needle = argv[index];
	if (!needle) {
		save(parser, needle, index, END_STATE);
		return getopt_required;
	}

	*value = needle;
	save(parser, needle, index + 1, START_STATE);
	return optindex;

end_state:
	return getopt_end;
}


#endif
