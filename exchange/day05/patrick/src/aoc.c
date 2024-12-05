/*
 * aoc.c
 *
 *  Created on: Dec 2, 2024
 *      Author: pat
 */

#include "aoc.h"
#include "hash.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#define DAY 05
int part = 2;

typedef unsigned page;

struct list {
	page *pages;
	size_t page_count;
};
typedef struct list list;

struct rule {
	page before_page;
	list after_pages;
	size_t max_entry_count;
};
typedef struct rule rule;

struct data {
	struct hashset rules;
	list *lists;
	size_t list_count;
	size_t list_max_count;
};

static uint64_t r_hs(const void *a) {
	return ((rule*) a)->before_page;
}
static int r_eq(const void *a, const void *b) {
	return ((rule*) a)->before_page == ((rule*) b)->before_page;
}

static uint64_t p_hs(const void *a) {
	return *((page*) a);
}
static int p_eq(const void *a, const void *b) {
	return *((page*) a) == *((page*) b);
}
static void* new_page(void *param, void *element) {
	page *result = malloc(sizeof(page));
	*result = *(page*) element;
	return result;
}

static int p_cmp(const void *a, const void *b) {
	const page p0 = *(const page*) a, p1 = *(const page*) b;
	if (p0 < p1) {
		return 1;
	}
	if (p0 > p1) {
		return -1;
	}
	return 0;
}

char* solve(char *path) {
	struct data *data = read_data(path);
	uint64_t result = 0;
	for (list *l = data->lists; l < data->lists + data->list_count; ++l) {
		int valid_list = 1;
		printf("list: ");
		for (page *p = l->pages; p < l->pages + l->page_count; p++) {
			if (p == l->pages) {
				printf("%d", *p);
			} else {
				printf(",%d", *p);
			}
		}
		retry: ;
		struct hashset visited = { .equal = p_eq, .hash = p_hs };
		if (!(l->page_count & 1)) {
			fprintf(stderr, "page count is not odd!\n");
			abort();
		}
		for (page *p = l->pages; p < l->pages + l->page_count; p++) {
			rule *r = hs_get(&data->rules, p);
			if (r) {
				for (int i = 0; i < r->after_pages.page_count; ++i) {
					if (hs_get(&visited, r->after_pages.pages + i)) {
						printf(" -- invalid (rule %d|%d)%s", r->before_page,
								r->after_pages.pages[i], part == 1 ? "\n" : "");
						if (part == 2) {
							valid_list = 0;
							hs_clear(&visited);
							for (page *p2 = l->pages; 105; p2++) {
								if (*p2 == r->after_pages.pages[i]) {
									*p = r->after_pages.pages[i];
									*p2 = r->before_page;
									goto retry;
								}
							}
						}
						goto invalid_page;
					}
				}
			}
			if (hs_add(&visited, p)) {
				fprintf(stderr, "duplicate page\n");
				abort();
			}
		}
		if (part == 1) {
			result += l->pages[l->page_count / 2];
			puts(" -- valid");
		} else if (!valid_list) {
			result += l->pages[l->page_count / 2];
			puts("");
		} else {
			puts(" -- valid");
		}
		invalid_page: hs_clear(&visited);
		continue;
	}
	hs_clear(&data->rules);
	return u64toa(result);
}

static void* new_rules(void *param, void *element) {
	rule *result = malloc(sizeof(rule));
	result->before_page = *(page*) element;
	result->after_pages.pages = malloc(sizeof(page) * 16);
	result->after_pages.page_count = 0;
	result->max_entry_count = 16;
	return result;
}

static struct data* parse_line(struct data *data, char *line) {
	for (; *line && isspace(*line); ++line)
		;
	if (!*line) {
		return data;
	}
	if (!data) {
		data = malloc(sizeof(struct data));
		data->rules = (struct hashset ) { .equal = r_eq, .hash = r_hs };
		data->lists = malloc(sizeof(list) * 16);
		data->list_count = 0;
		data->list_max_count = 16;
	}
	char *ptr;
	page p = strtol(line, &ptr, 10);
	if (errno) {
		perror("strtol");
		abort();
	} else if (*ptr == '|') {
		page p2 = strtol(ptr + 1, &ptr, 10);
		rule *r = hs_compute_absent(&data->rules, &p, 0, new_rules);
		if (++r->after_pages.page_count >= r->max_entry_count) {
			r->after_pages.pages = reallocarray(r->after_pages.pages,
					r->max_entry_count <<= 1, sizeof(page));
		}
		r->after_pages.pages[r->after_pages.page_count - 1] = p2;
	} else if (*ptr == ',') {
		if (++data->list_count >= data->list_max_count) {
			data->lists = reallocarray(data->lists, data->list_max_count <<= 1,
					sizeof(list));
		}
		list *l = data->lists + data->list_count - 1;
		l->pages = malloc(sizeof(page) * 16);
		l->page_count = 1;
		l->pages[0] = p;
		size_t max_page_count = 16;
		do {
			p = strtol(ptr + 1, &ptr, 10);
			if (errno) {
				perror("strtol");
				abort();
			}
			if (++l->page_count >= max_page_count) {
				l->pages = reallocarray(l->pages, max_page_count <<= 1,
						sizeof(page));
			}
			l->pages[l->page_count - 1] = p;
		} while (*ptr == ',');
	} else {
		fprintf(stderr, "invalid line: %s", line);
		abort();
	}
	for (; *ptr && isspace(*ptr); ++ptr)
		;
	if (!*ptr) {
		return data;
	}
	fprintf(stderr, "invalid line: %s", line);
	abort();
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
