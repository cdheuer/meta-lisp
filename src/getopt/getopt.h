#ifndef GETOPT_H
#define GETOPT_H

#ifdef __cplusplus
extern "C" {
#endif

#define getopt_end -1
#define getopt_ambiguous -2
#define getopt_required -3

struct getopt_parser {
	const char **argv;
	const char *needle;
	int index;
	int state;
};

struct getopt_option {
	const char *name;
	int flag;
	int required;
};

void getopt_init(struct getopt_parser *parser, const char **argv);

void getopt_reset(struct getopt_parser *parser);

int getopt(struct getopt_parser *parser, const char **value,
           const struct getopt_option *opts, int length);

#ifdef __cplusplus
}
#endif

#endif
