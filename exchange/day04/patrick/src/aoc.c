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

#define DAY 04
int part = 2;

struct data_entry {
	char *line;
};

struct data {
	struct data_entry *entries;
	size_t list_size;
	size_t list_max_size;
};

static int print_invalid = 0;
static int print_color = 0;

static uint64_t count_xmas(struct data *data, int c, int l, char *line,
		uint64_t line_len, const char *xmas, char **debug) {
	uint64_t result = 0;
	size_t xmas_len = strlen(xmas);
	if (!memcmp(line + c, xmas, xmas_len)) {
		for (int i = 0; i < xmas_len; ++i) {
			debug[l][c + i]++;
		}
		result++;
	}
	const int down = 0x1, diagonal_down = 0x2, diagonal_up = 0x4;
	int directions = 0x7;
	if (data->list_size - l < xmas_len) {
		directions &= ~(down | diagonal_down);
	}
	if (l < xmas_len - 1) {
		directions &= ~(diagonal_up);
	}
	if (line_len - c < xmas_len) {
		directions &= ~(diagonal_down | diagonal_up);
	}
	for (int i = 0; i < xmas_len; ++i) {
		if ((directions & down) && data->entries[l + i].line[c] != xmas[i]) {
			directions &= ~down;
		}
		if ((directions & diagonal_down)
				&& data->entries[l + i].line[c + i] != xmas[i]) {
			directions &= ~diagonal_down;
		}
		if ((directions & diagonal_up)
				&& data->entries[l - i].line[c + i] != xmas[i]) {
			directions &= ~diagonal_up;
		}
	}
	if (directions & down) {
		for (int i = 0; i < xmas_len; ++i) {
			debug[l + i][c]++;
		}
		result++;
	}
	if (directions & diagonal_down) {
		for (int i = 0; i < xmas_len; ++i) {
			debug[l + i][c + i]++;
		}
		result++;
	}
	if (directions & diagonal_up) {
		for (int i = 0; i < xmas_len; ++i) {
			debug[l - i][c + i]++;
		}
		result++;
	}
	return result;
}

static uint64_t count_x_mas(struct data *data, int c, int l, char *line,
		uint64_t line_len, char **debug) {
	if (c && l && data->list_size - c > 1 && line_len - l > 1) {
		char ul = data->entries[l - 1].line[c - 1];
		char ur = data->entries[l - 1].line[c + 1];
		char dl = data->entries[l + 1].line[c - 1];
		char dr = data->entries[l + 1].line[c + 1];
#define moa(c) (c == 'M' || c == 'S')
		if (moa(ul) && moa(ur) && moa(dl) && moa(dr)) {
			if (ul != dr && ur != dl) {
				debug[l][c] += 0x40;
				debug[l + 1][c + 1]++;
				debug[l + 1][c - 1]++;
				debug[l - 1][c + 1]++;
				debug[l - 1][c - 1]++;
				return 1;
			}
		}
	}
	return 0;
}

char* solve(char *path) {
	struct data *data = read_data(path);
	uint64_t result = 0, line_len = strlen(data->entries[0].line);
	char **buf = malloc(sizeof(void*) * data->list_size);
	for (int l = 0; l < data->list_size; ++l) {
		buf[l] = malloc(line_len);
		memset(buf[l], 0, line_len);
	}
	for (int l = 0; l < data->list_size; ++l) {
		char *line = data->entries[l].line;
		for (int c = 0; c < line_len; ++c) {
			if (part == 1) {
				if (line[c] == 'X') {
					result += count_xmas(data, c, l, line, line_len, "XMAS",
							buf);
				} else if (line[c] == 'S') {
					result += count_xmas(data, c, l, line, line_len, "SAMX",
							buf);
				}
			} else if (part == 2 && line[c] == 'A') {
				result += count_x_mas(data, c, l, line, line_len, buf);
			}
		}
	}
	for (int l = 0; l < data->list_size; ++l) {
		char *line = data->entries[l].line;
		char *lbuf = buf[l];
		for (int c = 0; c < line_len; ++c) {
			if (lbuf[c]) {
//				if (part == 2 && lbuf[c] > 1 && c && l && c + 1 < line_len
//						&& l + 1 < data->list_size) {
//					if (buf[l + 1][c + 1] && buf[l + 1][c - 1]
//							&& buf[l - 1][c + 1] && buf[l - 1][c - 1]) {
//						result++;
//					}
//				}
				if (print_color) {
					if (lbuf[c] < 0x40 && (!c || lbuf[c - 1] != 1)) {
						fputs("\033[32m", stdout);
					} else if (lbuf[c] >= 0x40 && (!c || lbuf[c - 1] <= 1)) {
						fputs("\033[31m", stdout);
					}
				}
				fputc(line[c], stdout);
			} else {
				if (print_color && (!c || lbuf[c - 1])) {
					fputs("\033[38;2;127;127;127m", stdout);
				}
				if (print_invalid) {
					fputc(line[c], stdout);
				} else {
					fputc('.', stdout);
				}
			}
		}
		fputc('\n', stdout);
	}
	if (print_color) {
		fputs("\033[0m", stdout);
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
	if (data->list_size > 1
			&& strlen(data->entries[0].line)
					!= strlen(data->entries[data->list_size - 1].line)) {
		fprintf(stderr, "input is no rectangle!\n");
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
