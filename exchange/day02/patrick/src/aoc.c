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

#define DAY 02
int part = 2;

struct data_entry {
	int *values;
	size_t value_count;
};

struct data {
	struct data_entry *entries;
	size_t list_size;
	size_t list_max_size;
};

char* solve(char *path) {
	struct data *data = read_data(path);
	uint64_t result = 0;
	for (int i = 0; i < data->list_size; ++i) {
		struct data_entry *e = data->entries + i;
		struct data_entry orig;
		if (part == 2) {
			orig.values = malloc(sizeof(int) * e->value_count);
			orig.value_count = e->value_count;
			memcpy(orig.values, e->values, sizeof(int) * orig.value_count);
		}
		int last_remove = -1;
		printf("[%.2d]:", i);
		for (int ii = 0; ii < e->value_count; ++ii) {
			printf(" %d", e->values[ii]);
		}
		retry: ;
		if (e->value_count <= 1) {
			abort();
		}
		int ii = 2;
		if (e->values[0] == e->values[1]) {
			unsafe: if (part == 2) {
				e->value_count = orig.value_count - 1;
				if (last_remove < 0 || last_remove < orig.value_count) {
					if (last_remove < 0) {
						last_remove = ii -= 2;
					} else {
						ii = ++last_remove;
					}
					memcpy(e->values, orig.values, sizeof(int) * ii);
					memcpy(e->values + ii, orig.values + ii + 1,
							sizeof(int) * (orig.value_count - ii));
					goto retry;
				}
				free(e->values);
				e->values = orig.values;
				e->value_count = orig.value_count;
			}
			puts("    U");
			continue;
		} else if (e->values[0] > e->values[1]) {
			if (e->values[0] - e->values[1] > 3) {
				goto unsafe;
			}
			for (ii = 2; ii < e->value_count; ++ii) {
				if (e->values[ii - 1] <= e->values[ii]) {
					goto unsafe;
				}
				if (e->values[ii - 1] - e->values[ii] > 3) {
					goto unsafe;
				}
			}
		} else {
			if (e->values[1] - e->values[0] > 3) {
				goto unsafe;
			}
			for (ii = 2; ii < e->value_count; ++ii) {
				if (e->values[ii - 1] >= e->values[ii]) {
					goto unsafe;
				}
				if (e->values[ii] - e->values[ii - 1] > 3) {
					goto unsafe;
				}
			}
		}
		if (part == 2 && last_remove >= 0) {
			printf(" -- S (REM=%d)\n", last_remove);
		} else {
			puts(" -- S");
		}
		result++;
		if (part == 2) {
			free(e->values);
			e->values = orig.values;
			e->value_count = orig.value_count;
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
	data->entries[data->list_size - 1].values = malloc(sizeof(int) * 16);
	for (off_t off = 0; 66; ++off) {
		if (off >= entries_size) {
			entries_size <<= 1;
			data->entries[data->list_size - 1].values = reallocarray(
					data->entries[data->list_size - 1].values, entries_size,
					sizeof(int));
		}
		data->entries[data->list_size - 1].values[off] = strtol(ptr, &ptr, 10);
		if (errno) {
			puts("strtol");
			abort();
		}
		while (isspace(*ptr))
			++ptr;
		if (!*ptr) {
			data->entries[data->list_size - 1].value_count = off + 1;
			return data;
		}
	}
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
