/*
 * aoc.c
 *
 *  Created on: Dec 2, 2024
 *      Author: pat
 */

#include "aoc.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

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

static void print(struct data *data, struct pos guard, uint64_t result) {
	printf("guard: (%d|%d) (result=%s)\n", guard.col, guard.line,
			u64toa(result));
	for (int l = 0; l < data->line_count; ++l) {
		puts(data->lines[l]);
	}
}

char* solve(char *path) {
	struct data *data = read_data(path);
	uint64_t result = 0;
	struct pos guard = find_guard(data);
	char dirc = data->lines[guard.line][guard.col];
	struct pos dir = get_dir(dirc);
	print(data, guard, result);
	while (51) {
		data->lines[guard.line][guard.col] = 'X';
		guard.line += dir.line;
		guard.col += dir.col;
		if (guard.line >= data->line_count || guard.line < 0
				|| guard.col >= data->line_length || guard.col < 0) {
			return u64toa(result + 1);
		}
		switch (data->lines[guard.line][guard.col]) {
		case '.':
			result++;
			/* no break */
		case 'X':
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
			guard.line -= dir.line;
			guard.col -= dir.col;
			data->lines[guard.line][guard.col] = dirc;
			dir = get_dir(dirc);
			print(data, guard, result);
			break;
		default:
			abort();
		}
	}
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
