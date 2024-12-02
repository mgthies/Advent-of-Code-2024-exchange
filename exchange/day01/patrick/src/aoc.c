/*
 * aoc.c
 *
 *  Created on: Dec 1, 2024
 *      Author: pat
 */

#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define DAY 01
int part = 2;

struct data_entry {
	int value;
	int index;
};

struct data {
	struct data_entry *left_list;
	struct data_entry *right_list;
	int list_size;
	int list_max_size;
};

int data_entry_compare(const void *a0, const void *b0);

void mergesort(void *base, size_t nmemb, size_t size,
		int (*compar)(const void*, const void*));

char* solve(char *path) {
	struct data *data = read_data(path);
	qsort(data->left_list, data->list_size, sizeof(struct data_entry),
			data_entry_compare);
	qsort(data->right_list, data->list_size, sizeof(struct data_entry),
			data_entry_compare);
	uint64_t result = 0;
	if (part == 1) {
		for (int i = 0; i < data->list_size; ++i) {
			int lv = data->left_list[i].value;
			int rv = data->right_list[i].value;
			int li = data->left_list[i].index;
			int ri = data->right_list[i].index;
			int diffv = lv > rv ? lv - rv : rv - lv;
			int diffi = li > ri ? li - ri : ri - li;
			printf("[%d]: [%.3d]=%.6d : [%.3d]=%.6d ==> [%.3d]:%.6d\n", i, li,
					lv, ri, rv, diffi, diffv);
			result += diffv;
		}
	} else {
		uint64_t cnt = 0;
		for (int li = 0, ri = 0, last = -1; li < data->list_size; ++li) {
			int lv = data->left_list[li].value;
			if (lv != last) {
				cnt = 0;
				for (; ri < data->list_size; ++ri) {
					if (data->right_list[ri].value > lv) {
						break;
					}
					if (data->right_list[ri].value == lv) {
						cnt++;
					}
				}
				last = lv;
			}
			result += lv * cnt;
		}
	}
	return u64toa(result);
}

int data_entry_compare(const void *a0, const void *b0) {
	const struct data_entry *a = a0;
	const struct data_entry *b = b0;
	if (a->value < b->value) {
		return -1;
	}
	if (a->value > b->value) {
		return 1;
	}
	return 0;
}

void mergesort(void *base, size_t nmemb, size_t size,
		int (*compar)(const void*, const void*)) {
	if (nmemb <= 1) {
		return;
	}
	size_t half_nmemb = nmemb >> 1;
	char *work = malloc(size * nmemb);
	memcpy(work, base, size * nmemb);
	mergesort(work, half_nmemb, size, compar);
	mergesort(work + (half_nmemb * size), nmemb - half_nmemb, size, compar);
	off_t ai = 0, bi = half_nmemb, i = 0;
	char *b = base;
	while (ai < half_nmemb && bi < nmemb) {
		off_t start_ai = ai;
		for (; ai < half_nmemb; ++ai) {
			if (compar(work + (ai * size), work + (bi * size)) > 0) {
				break;
			}
		}
		size_t cpy = ai - start_ai;
		memcpy(b + (i * size), work + (start_ai * size), cpy * size);
		i += cpy;
		off_t start_bi = bi;
		for (; bi < nmemb; ++bi) {
			if (compar(work + (ai * size), work + (bi * size)) <= 0) {
				break;
			}
		}
		cpy = bi - start_bi;
		memcpy(b + (i * size), work + (start_bi * size), cpy * size);
		i += cpy;
	}
	size_t cpy = half_nmemb - ai;
	memcpy(b + (i * size), work + (ai * size), cpy * size);
	i += cpy;
	memcpy(b + (i * size), work + (bi * size), (nmemb - bi) * size);
}

static struct data* parse_line(struct data *data, char *line) {
	for (; *line && isspace(*line); ++line)
		;
	if (!*line) {
		return data;
	}
	if (!data) {
		data = malloc(sizeof(struct data));
		data->left_list = malloc(sizeof(struct data_entry) * 16);
		data->right_list = malloc(sizeof(struct data_entry) * 16);
		data->list_size = 0;
		data->list_max_size = 16;
	}
	if (++data->list_size > data->list_max_size) {
		data->list_max_size <<= 1;
		data->left_list = reallocarray(data->left_list,
				sizeof(struct data_entry), data->list_max_size);
		data->right_list = reallocarray(data->right_list,
				sizeof(struct data_entry), data->list_max_size);
	}
	char *ptr;
	data->left_list[data->list_size - 1].value = strtol(line, &ptr, 10);
	data->right_list[data->list_size - 1].value = strtol(ptr, &ptr, 10);
	data->left_list[data->list_size - 1].index = data->list_size - 1;
	data->right_list[data->list_size - 1].index = data->list_size - 1;
	while (isspace(*ptr))
		++ptr;
	if (*ptr) {
		fprintf(stderr, "invalid line: %s", line);
		abort();
	}
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
