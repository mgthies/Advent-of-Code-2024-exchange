/*
 * aoc.c
 *
 *  Created on: Dec 2, 2024
 *      Author: pat
 */

#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define DAY 03
int part = 2;

struct data_entry {
	char *line;
};

struct data {
	struct data_entry *entries;
	size_t list_size;
	size_t list_max_size;
};

static int get_num(char **c, uint64_t *a, char end) {
	if (!isdigit(**c)) {
		return 31;
	}
	*a = **c - '0';
	if (isdigit(*(++*c))) {
		*a = *a * 10 + **c - '0';
	} else if (**c != end) {
		return 37;
	} else {
		return 0;
	}
	if (isdigit(*(++*c))) {
		*a = *a * 10 + **c - '0';
	} else if (**c != end) {
		return 42;
	} else {
		return 0;
	}
	if (*(++*c) != end) {
		return 45;
	}
	return 0;
}

char* solve(char *path) {
	struct data *data = read_data(path);
	uint64_t result = 0;
	for (int i = 0; i < data->list_size; ++i) {
		struct data_entry *e = data->entries + i;
		uint64_t tres = 0;
		for (char *c = strstr(e->line, "mul("); c; c = strstr(c, "mul(")) {
			c += strlen("mul(");
			uint64_t a;
			if (get_num(&c, &a, ',')) {
				continue;
			}
			c++;
			uint64_t b;
			if (get_num(&c, &b, ')')) {
				continue;
			}
			tres += a * b;
		}
		printf("%-128s ==> %s\n", e->line, u64toa(tres));
		result += tres;
	}
	return u64toa(result);
}

static struct data* parse_line(struct data *data, char *line) {
	for (; *line && isspace(*line); ++line)
		;
	if (!*line) {
		return data;
	}
	if (!data) {
		data = malloc(sizeof(struct data));
		data->entries = malloc(sizeof(struct data_entry) * 16);
		data->list_size = 0;
		data->list_max_size = 16;
	}
	if (++data->list_size > data->list_max_size) {
		data->list_max_size <<= 1;
		data->entries = reallocarray(data->entries, sizeof(struct data_entry),
				data->list_max_size);
	}
	char *ptr = line;
	size_t entries_size = 16;
	char *end;
	for (end = line + strlen(line); isspace(*--end);)
		;
	end[1] = '\0';
	data->entries[data->list_size - 1].line = strdup(line);
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
