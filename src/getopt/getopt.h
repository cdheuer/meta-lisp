#ifndef GETOPT_H
#define GETOPT_H

#ifdef __cplusplus
extern "C" {
#endif

struct option {
	char *name;
	int has_arg;
	int *flag;
	int val;
};

int getopt_long(int argc, char **argv, char *optstring, struct option *longopts, int *longindex);

extern int optopt;
extern int opterr;
extern int optreset;

#ifdef __cplusplus
}
#endif


#endif
