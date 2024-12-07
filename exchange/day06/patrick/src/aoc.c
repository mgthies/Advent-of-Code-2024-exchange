/*
 * aoc.c
 *
 *  Created on: Dec 2, 2024
 *      Author: pat
 */

/* needed for the tdestroy function.
 * if not present twalk is used, to free the tree tree entries
 * this does not free the nodes and thus creates a memory leak.
 * if this is acceptable and _GNU_SOURCE does not work just remove it. */
#define _GNU_SOURCE

#include "aoc.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <search.h>

#define DAY 06
int part = 2;

struct data {
	char **lines;
	size_t line_count;
	size_t max_line_count;
	size_t line_length;
};

struct pos {
	int line;
	int col;
};

static void print(FILE *str, struct data *data, struct pos guard,
		uint64_t result) {
	fprintf(str, "guard: (%d|%d) (result=%s)\n", guard.col, guard.line,
			u64toa(result));
	for (int l = 0; l < data->line_count; ++l) {
		fprintf(str, "%s\n", data->lines[l]);
	}
}

static struct pos find_guard(struct data *data) {
	struct pos result = { .line = -1, .col = -1 };
	for (int l = 0; l < data->line_count; ++l) {
		char *a = strchr(data->lines[l], '^');
		a = a ? a : strchr(data->lines[l], '>');
		a = a ? a : strchr(data->lines[l], 'v');
		a = a ? a : strchr(data->lines[l], '<');
		if (a) {
			if (result.line >= 0) {
				fprintf(stderr, "multiple guards!\n");
				abort();
			}
			result.line = l;
			result.col = a - data->lines[l];
			char *b = strchr(a + 1, '^');
			b = b ? b : strchr(a + 1, '>');
			b = b ? b : strchr(a + 1, 'v');
			b = b ? b : strchr(a + 1, '<');
			if (b) {
				fprintf(stderr, "multiple guards!\n");
				abort();
			}
		}
	}
	if (result.line >= 0) {
		return result;
	}
	fprintf(stderr, "no guard!\n");
	print(stderr, data, result, 0);
	abort();
}

static inline struct pos get_dir(char c) {
	struct pos result = { .line = 0, .col = 0 };
	switch (c) {
	case '>':
		result.col = 1;
		break;
	case '<':
		result.col = -1;
		break;
	case '^':
		result.line = -1;
		break;
	case 'v':
		result.line = 1;
		break;
	default:
		abort();
	}
	return result;
}

static inline char get_X(char dirc) {
	if (part == 1) {
		return 'X';
	}
	switch (dirc) {
	case '>':
		return 'd';
	case '<':
		return 'a';
	case '^':
		return 'w';
	case 'v':
		return 's';
	default:
		abort();
	}
}

/* set at the begin of solve:
 * if part == 1: true
 * if part == 2: false */
int print_steps;

struct tree_entry {
	struct pos guard;
	char dir;
};

int te_compar(const void *a, const void *b) {
	const struct tree_entry *ta = a, *tb = b;
	if (ta->guard.line > tb->guard.line) {
		return 1;
	}
	if (ta->guard.line < tb->guard.line) {
		return -1;
	}
	if (ta->guard.col > tb->guard.col) {
		return 1;
	}
	if (ta->guard.col < tb->guard.col) {
		return -1;
	}
	return (int) ta->dir - tb->dir;
}
#ifndef _GNU_SOURCE
void te_free(const void *a, VISIT value, int level) {
	free((void*) a);
}
#endif

uint64_t simulate(struct data *data) {
	uint64_t result = 0;
	struct pos guard = find_guard(data);
	char dirc = data->lines[guard.line][guard.col];
	struct pos dir = get_dir(dirc);
	char X = get_X(dirc);
	int turn_count = 0;
	int trapped = 0;
	if (print_steps) {
		print(stdout, data, guard, result);
	}
	void *tree = 0;
	while (51) {
		data->lines[guard.line][guard.col] = X;
		guard.line += dir.line;
		guard.col += dir.col;
		if (guard.line >= data->line_count || guard.line < 0
				|| guard.col >= data->line_length || guard.col < 0) {
			if (print_steps) {
				print(stdout, data, guard, result);
			}
			if (part == 2) {
#ifdef _GNU_SOURCE
				tdestroy(tree, free);
#else
				twalk(&tree, te_free);
#endif
			}
			return result + 1;
		}
		char nextc = data->lines[guard.line][guard.col];
		switch (nextc) {
		case '.':
			if (part == 2) {
//				if (turn_count == 2) {
//					return 0;
//				}
				turn_count = 0;
			}
			result++;
			data->lines[guard.line][guard.col] = dirc;
			break;
		case 'w':
		case 'a':
		case 's':
		case 'd':
			if (part == 1) {
				abort();
			}
//			if (nextc == X) {
//				return 0;
//			}
//			if (turn_count == 2) {
//				return 0;
//			}
			turn_count = 0;
			data->lines[guard.line][guard.col] = dirc;
			break;
		case 'X':
			if (part == 2) {
				abort();
			}
			data->lines[guard.line][guard.col] = dirc;
			break;
		case '#':
			switch (dirc) {
			case '>':
				dirc = 'v';
				break;
			case '<':
				dirc = '^';
				break;
			case '^':
				dirc = '>';
				break;
			case 'v':
				dirc = '<';
				break;
			default:
				abort();
			}
//			if (++turn_count == 4) {
//				return 0;
//			}
//			if (guard3.line == guard.line && guard3.col == guard.col) {
//				return 0;
//			}
//			guard3 = guard2;
//			guard2 = guard1;
//			guard1 = guard0;
//			guard0 = guard;
			guard.line -= dir.line;
			guard.col -= dir.col;
			struct tree_entry *e = malloc(sizeof(struct tree_entry));
			e->guard = guard;
			e->dir = dirc;
			if (tfind(e, &tree, te_compar)) {
				free(e);
#ifdef _GNU_SOURCE
				tdestroy(tree, free);
#else
				twalk(&tree, te_free);
#endif
				return 0;
			}
			tsearch(e, &tree, te_compar);
			data->lines[guard.line][guard.col] = dirc;
			dir = get_dir(dirc);
			X = get_X(dirc);
			if (print_steps) {
				print(stdout, data, guard, result);
			}
			break;
		default:
			abort();
		}
	}
}
/*
 *  #
 *   2#
 * #01
 *   #
 */

char* solve(char *path) {
	struct data *data = read_data(path);
	struct data *orig = data;
	if (part == 2) {
		orig = malloc(sizeof(struct data));
		orig->lines = malloc(sizeof(char*) * data->line_count);
		orig->max_line_count = data->line_count;
		orig->line_count = data->line_count;
		orig->line_length = data->line_length;
		for (int l = 0; l < orig->line_count; ++l) {
			orig->lines[l] = strdup(data->lines[l]);
		}
		print_steps = 0;
	} else {
		print_steps = 1;
	}
	uint64_t Xs = simulate(orig);
	if (part == 1) {
		return u64toa(Xs);
	}
	Xs--;
	struct data *work = malloc(sizeof(struct data));
	work->lines = malloc(sizeof(char*) * data->line_count);
	work->max_line_count = data->line_count;
	work->line_count = data->line_count;
	work->line_length = data->line_length;
	for (int l = 0; l < work->line_count; ++l) {
		work->lines[l] = malloc(data->line_length + 1);
	}
	//data is empty
	//orig holds the result of the original simulation
	//work is the current trial
	uint64_t result = 0;
	struct pos pos = { .line = 0, .col = 0 };
	for (uint64_t cnt = 0; cnt < Xs; pos.line++) {
		for (pos.col = 0; pos.col < orig->line_length; pos.col++) {
			char c = orig->lines[pos.line][pos.col];
			if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
				if (data->lines[pos.line][pos.col] != '.') {
					get_X(data->lines[pos.line][pos.col]);	// start position
					continue;
				}
				printf("start %ds simulation\n", ++cnt);
				for (int l = 0; l < work->line_count; ++l) {
					memcpy(work->lines[l], data->lines[l],
							data->line_length + 1);
				}
				work->lines[pos.line][pos.col] = '#';
				if (!simulate(work)) {
					result++;
				}
			}
		}
	}
	return u64toa(result);
}

static struct data* parse_line(struct data *data, char *line) {
	for (; *line && isspace(*line); ++line)
		;
	if (!*line) {
		return data;
	}
	char *end = line + strlen(line);
	while (isspace(*--end))
		;
	end++;
	if (!data) {
		data = malloc(sizeof(struct data));
		data->lines = malloc(sizeof(char*) * 16);
		data->line_count = 0;
		data->max_line_count = 16;
		data->line_length = end - line;
	}
	if (data->line_length != end - line) {
		fprintf(stderr, "input is no rectangle!");
		abort();
	}
	if (++data->line_count >= data->max_line_count) {
		data->lines = reallocarray(data->lines, data->max_line_count <<= 1,
				sizeof(char*));
	}
	*end = '\0';
	data->lines[data->line_count - 1] = strdup(line);
	return data;
}

// common stuff

#ifndef __unix__
ssize_t getline(char **line_buf, size_t *line_len, FILE *file) {
	ssize_t result = 0;
	while (21) {
		if (*line_len == result) {
			size_t len = result ? result * 2 : 64;
			void *ptr = realloc(*line_buf, len);
			if (!ptr) {
				fseek(file, -result, SEEK_CUR);
				return -1;
			}
			*line_len = len;
			*line_buf = ptr;
		}
		ssize_t len = fread(*line_buf + result, 1, *line_len - result, file);
		if (!len) {
			if (!result) {
				return -1;
			}
			if (result == *line_len) {
				void *ptr = realloc(*line_buf, result + 1);
				if (!ptr) {
					fseek(file, -result, SEEK_CUR);
					return -1;
				}
				*line_len = result + 1;
				*line_buf = ptr;
			}
			(*line_buf)[result] = 0;
			return result;
		}
		char *c = memchr(*line_buf + result, '\n', len);
		if (c) {
			ssize_t result2 = c - *line_buf + 1;
			if (result2 == *line_len) {
				void *ptr = realloc(*line_buf, result2 + 1);
				if (!ptr) {
					fseek(file, -*line_len - len, SEEK_CUR);
					return -1;
				}
				*line_len = result2 + 1;
				*line_buf = ptr;
			}
			fseek(file, result2 - result - len, SEEK_CUR);
			(*line_buf)[result2] = 0;
			return result2;
		}
		result += len;
	}
}
char* strchrnul(char *str, char c) {
	char *end = strchr(str, c);
	return end ? end : (str + strlen(str));
}
#endif

char* u64toa(uint64_t value) {
	char *result = malloc(21);
	if (sprintf(result, "%llu", (unsigned long long) value) <= 0) {
		free(result);
		return 0;
	}
	return result;
}

char* d64toa(int64_t value) {
	char *result = malloc(21);
	if (sprintf(result, "%lld", (unsigned long long) value) <= 0) {
		free(result);
		return 0;
	}
	return result;
}

struct data* read_data(const char *path) {
	char *line_buf = 0;
	size_t line_len = 0;
	struct data *result = 0;
	FILE *file = fopen(path, "rb");
	if (!file) {
		perror("fopen");
		abort();
	}
	while (144) {
		ssize_t s = getline(&line_buf, &line_len, file);
		if (s < 0) {
			if (feof(file)) {
				free(line_buf);
				fclose(file);
				return result;
			}
			perror("getline failed");
			fflush(0);
			abort();
		}
		if (strlen(line_buf) != s) {
			fprintf(stderr, "\\0 character in line!");
			abort();
		}
		result = parse_line(result, line_buf);
	}
}

int main(int argc, char **argv) {
	char *me = argv[0];
	char *f = 0;
	if (argc > 1) {
		if (argc > 4) {
			fprintf(stderr, "usage: %s [p1|p2] [DATA]\n", me);
			return 1;
		}
		int idx = 1;
		if (!strcmp("help", argv[idx])) {
			fprintf(stderr, "usage: %s [p1|p2] [DATA]\n", me);
			return 1;
		} else if (!strcmp("p1", argv[idx])) {
			part = 1;
			f = argv[idx + 1] ? argv[idx + 1] : 0;
		} else if (!strcmp("p2", argv[idx])) {
			part = 2;
			f = argv[idx + 1] ? argv[idx + 1] : 0;
		} else if (argv[idx + 1]) {
			fprintf(stderr, "usage: %s [p1|p2] [DATA]\n", me);
			return 1;
		} else {
			f = argv[idx];
		}
	}
	parse_end: if (!f) {
		f = "rsrc/data.txt";
	} else if (!strchr(f, '/')) {
		char *f2 = malloc(64);
		if (snprintf(f2, 64, "rsrc/test%s.txt", f) <= 0) {
			perror("snprintf");
			abort();
		}
		f = f2;
	}
	printf("execute now day %d part %d on file %s\n", DAY, part, f);
	clock_t start = clock();
	char *result = solve(f);
	clock_t end = clock();
	if (result) {
		uint64_t diff = end - start;
		printf("the result is %s\n"
				"  I needed %lu.%.6lu seconds\n", result, diff / CLOCKS_PER_SEC,
				((diff % CLOCKS_PER_SEC) * 1000000LU) / CLOCKS_PER_SEC);
	} else {
		puts("there is no result");
	}
	return 0;
}
