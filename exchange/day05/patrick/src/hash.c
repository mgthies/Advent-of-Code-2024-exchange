// SPDX-License-Identifier: AGPL-3.0-or-later

/*
 * hash.c
 *
 *  Created on: Aug 14, 2024
 *      Author: pat
 */

#include "hash.h"
#include <stdlib.h>

/* WARN: this is not necessarily portable, since it relies on pointers to have
 * a positive value if interpreted as a signed integer */

struct hs_list {
	struct hs_list *next;
	void *value;
};

union hs_entry {
	ptrdiff_t number;
	void *value;
	struct hs_list *list;
};

#if PTRDIFF_MIN == 0
#	error "ptrdiff_t is unsigned"
#endif

void* hs_get(struct hashset *hs, void *val) {
	if (!hs->entry_count) {
		return 0;
	}
	uint64_t index = hs->hash(val) & (hs->data_size - 1);
	union hs_entry e = hs->data[index];
	if (e.number > 0) {
		if (hs->equal(e.value, val)) {
			return e.value;
		}
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = e.list;
		do {
			if (hs->equal(l->value, val)) {
				return l->value;
			}
			l = l->next;
		} while (l);
	}
	return 0;
}

static void free_entries(ssize_t entry_count, union hs_entry *data) {
	ssize_t remain = entry_count;
	for (off_t o = 0; entry_count < 0 ? o < -entry_count : remain; o++) {
		union hs_entry e = data[o];
		if (e.number > 0) {
			remain--;
		} else if (e.number < 0) {
			e.number = -e.number;
			do {
				void *f = e.list;
				e.list = e.list->next;
				free(f);
				remain--;
			} while (e.list);
		}
	}
	free(data);
}

struct ra {
	union hs_entry *data;
	uint64_t mask;
	uint64_t (*hash)(const void*);
};

static int v_rebuild(void *arg, void *element) {
	struct ra *a = arg;
	uint64_t index = a->hash(element) & a->mask;
	union hs_entry *ep = a->data + index;
	union hs_entry e = *ep;
	if (e.number > 0) {
		struct hs_list *l0 = malloc(sizeof(struct hs_list));
		struct hs_list *l1 = malloc(sizeof(struct hs_list));
		if (!l0 || !l1) {
			free(l0);
			free(l1);
			return -1;
		}
		l0->next = l1;
		l0->value = element;
		l1->next = 0;
		l1->value = e.value;
		e.list = l0;
		ep->number = -e.number;
		return 0;
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = malloc(sizeof(struct hs_list));
		if (!l) {
			return -1;
		}
		l->next = e.list;
		l->value = element;
		e.list = l;
		ep->number = -e.number;
		return 0;
	} else {
		ep->value = element;
		return 0;
	}
}

void* hs_remove(struct hashset *hs, void *val) {
	if (!hs->entry_count) {
		return 0;
	}
	if (hs->entry_count < (hs->data_size >> 4) && hs->entry_count > 64) {
		size_t ns = hs->data_size >> 1;
		union hs_entry *new_data = calloc(ns, sizeof(union hs_entry));
		struct ra arg = { new_data, ns - 1, hs->hash };
		if (hs_for_each(hs, &arg, v_rebuild)) {
			free_entries(-ns, new_data);
		} else {
			void *old_data = hs->data;
			hs->data = new_data;
			hs->data_size = ns;
			free_entries(hs->entry_count, old_data);
		}
	}
	uint64_t index = hs->hash(val) & (hs->data_size - 1);
	union hs_entry *ep = hs->data + index;
	union hs_entry e = *ep;
	if (e.number > 0) {
		if (hs->equal(e.value, val)) {
			ep->number = 0;
			hs->entry_count--;
			return e.value;
		}
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = e.list, *prev = 0;
		do {
			if (hs->equal(l->value, val)) {
				if (prev) {
					prev->next = l->next;
				} else if (l->next) {
					if (l->next->next) {
						e.list = l->next;
						ep->number = -e.number;
					} else {
						ep->value = l->next->value;
						free(l->next);
					}
				} else {
					ep->number = 0;
				}
				void *res = l->value;
				free(l);
				hs->entry_count--;
				return res;
			}
			prev = l;
			l = l->next;
		} while (l);
	}
	return 0;
}

void* hs_compute_absent(struct hashset *hs, void *val, void *param,
		void* (*get_val)(void *param, void *val)) {
	if (!hs->data_size) {
		// use data_size instead of entry_size to avoid reinitializing data
		hs->data = calloc(16, sizeof(union hs_entry));
		hs->data_size = 16;
		hs->entry_count = 1;
		uint64_t index = 15 & hs->hash(val);
		hs->data[index].value = val = get_val(param, val);
		return val;
	}
	if (hs->entry_count > (hs->data_size >> 2) && (hs->data_size << 1)) {
		size_t ns = hs->data_size << 1;
		union hs_entry *new_data = calloc(ns, sizeof(union hs_entry));
		struct ra arg = { new_data, ns - 1, hs->hash };
		if (hs_for_each(hs, &arg, v_rebuild)) {
			free_entries(-ns, new_data);
		} else {
			void *old_data = hs->data;
			hs->data = new_data;
			hs->data_size = ns;
			free_entries(hs->entry_count, old_data);
		}
	}
	uint64_t index = hs->hash(val) & (hs->data_size - 1);
	union hs_entry *ep = hs->data + index;
	union hs_entry e = *ep;
	if (e.number > 0) {
		if (hs->equal(e.value, val)) {
			return e.value;
		}
		struct hs_list *l0 = malloc(sizeof(struct hs_list));
		struct hs_list *l1 = malloc(sizeof(struct hs_list));
		if (!l0 || !l1) {
			free(l0);
			free(l1);
			return 0;
		}
		l0->next = l1;
		l0->value = val = get_val(param, val);
		l1->next = 0;
		l1->value = e.value;
		e.list = l0;
		ep->number = -e.number;
		hs->entry_count++;
		return val;
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = e.list;
		do {
			if (hs->equal(l->value, val)) {
				return l->value;
			}
			l = l->next;
		} while (l);
		l = malloc(sizeof(struct hs_list));
		if (!l) {
			return 0;
		}
		l->next = e.list;
		l->value = val = get_val(param, val);
		e.list = l;
		ep->number = -e.number;
		hs->entry_count++;
		return val;
	} else {
		ep->value = val = get_val(param, val);
		hs->entry_count++;
		return val;
	}
}

void* hs_set(struct hashset *hs, void *val) {
	if (!hs->data_size) {
		// use data_size instead of entry_size to avoid reinitializing data
		hs->data = calloc(16, sizeof(union hs_entry));
		hs->data_size = 16;
		hs->entry_count = 1;
		hs->data[15 & hs->hash(val)].value = val;
		return 0;
	}
	if (hs->entry_count < (hs->data_size >> 2) && (hs->data_size << 1)) {
		size_t ns = hs->data_size << 1;
		union hs_entry *new_data = calloc(ns, sizeof(union hs_entry));
		struct ra arg = { new_data, ns - 1, hs->hash };
		if (hs_for_each(hs, &arg, v_rebuild)) {
			free_entries(-ns, new_data);
		} else {
			void *old_data = hs->data;
			hs->data = new_data;
			hs->data_size = ns;
			free_entries(hs->entry_count, old_data);
		}
	}
	uint64_t index = hs->hash(val) & (hs->data_size - 1);
	union hs_entry *ep = hs->data + index;
	union hs_entry e = *ep;
	if (e.number > 0) {
		if (hs->equal(e.value, val)) {
			ep->value = val;
			return e.value;
		}
		struct hs_list *l0 = malloc(sizeof(struct hs_list));
		struct hs_list *l1 = malloc(sizeof(struct hs_list));
		if (!l0 || !l1) {
			free(l0);
			free(l1);
			return (void*) -1;
		}
		l0->next = l1;
		l0->value = val;
		l1->next = 0;
		l1->value = e.value;
		e.list = l0;
		ep->number = -e.number;
		hs->entry_count++;
		return 0;
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = e.list;
		do {
			if (hs->equal(l->value, val)) {
				void *res = l->value;
				l->value = val;
				return res;
			}
			l = l->next;
		} while (l);
		l = malloc(sizeof(struct hs_list));
		if (!l) {
			return (void*) -1;
		}
		l->next = e.list;
		l->value = val;
		e.list = l;
		ep->number = -e.number;
		hs->entry_count++;
		return 0;
	} else {
		ep->value = val;
		hs->entry_count++;
		return 0;
	}
}

//like set but does not overwrite existing values
void* hs_add(struct hashset *hs, void *val) {
	if (!hs->data_size) {
		// use data_size instead of entry_size to avoid reinitializing data
		hs->data = calloc(16, sizeof(union hs_entry));
		hs->data_size = 16;
		hs->entry_count = 1;
		hs->data[15 & hs->hash(val)].value = val;
		return 0;
	}
	if (hs->entry_count < (hs->data_size >> 2) && (hs->data_size << 1)) {
		size_t ns = hs->data_size << 1;
		union hs_entry *new_data = calloc(ns, sizeof(union hs_entry));
		struct ra arg = { new_data, ns - 1, hs->hash };
		if (hs_for_each(hs, &arg, v_rebuild)) {
			free_entries(-ns, new_data);
		} else {
			void *old_data = hs->data;
			hs->data = new_data;
			hs->data_size = ns;
			free_entries(hs->entry_count, old_data);
		}
	}
	uint64_t index = hs->hash(val) & (hs->data_size - 1);
	union hs_entry *ep = hs->data + index;
	union hs_entry e = *ep;
	if (e.number > 0) {
		if (hs->equal(e.value, val)) {
			return e.value;
		}
		struct hs_list *l0 = malloc(sizeof(struct hs_list));
		struct hs_list *l1 = malloc(sizeof(struct hs_list));
		if (!l0 || !l1) {
			free(l0);
			free(l1);
			return (void*) -1;
		}
		l0->next = l1;
		l0->value = val;
		l1->next = 0;
		l1->value = e.value;
		e.list = l0;
		ep->number = -e.number;
		hs->entry_count++;
		return 0;
	} else if (e.number) {
		e.number = -e.number;
		struct hs_list *l = e.list;
		do {
			if (hs->equal(l->value, val)) {
				return l->value;
			}
			l = l->next;
		} while (l);
		l = malloc(sizeof(struct hs_list));
		if (!l) {
			return (void*) -1;
		}
		l->next = e.list;
		l->value = val;
		e.list = l;
		ep->number = -e.number;
		hs->entry_count++;
		return 0;
	} else {
		ep->value = val;
		hs->entry_count++;
		return 0;
	}
}

void hs_clear(struct hashset *hs) {
	union hs_entry *old_data = hs->data;
	size_t entry_count = hs->entry_count;
	hs->entry_count = 0;
	hs->data_size = 0;
	hs->data = 0;
	free_entries(entry_count, old_data);
}

int hs_for_each(struct hashset *hs, void *param,
		int (*visitor)(void *param, void *element)) {
	size_t remain = hs->entry_count;
	for (off_t o = 0; remain; o++) {
		union hs_entry e = hs->data[o];
		if (e.number > 0) {
			int v = visitor(param, e.value);
			if (v) {
				return v;
			}
			remain--;
		} else if (e.number < 0) {
			e.number = -e.number;
			do {
				int v = visitor(param, e.list->value);
				if (v) {
					return v;
				}
				e.list = e.list->next;
				remain--;
			} while (e.list);
		}
	}
	return 0;
}

void hs_filter(struct hashset *hs, void *param,
		int (*retain_element)(void *param, void *element)) {
	size_t remain = hs->entry_count;
	for (off_t o = 0; remain; o++) {
		union hs_entry *ep = hs->data + o;
		union hs_entry e = *ep;
		if (e.number > 0) {
			if (!retain_element(param, e.value)) {
				ep->number = 0;
				hs->entry_count--;
			}
			remain--;
		} else if (e.number < 0) {
			e.number = -e.number;
			struct hs_list *l = e.list, *prev = 0;
			do {
				void *nl = l->next;
				if (!retain_element(param, l->value)) {
					if (prev) {
						prev->next = l->next;
					} else if (l->next) {
						if (l->next->next) {
							e.list = l->next;
							ep->number = -e.number;
						} else {
							e.value = l->next->value;
							if (!retain_element(param, e.value)) {
								ep->number = 0;
								hs->entry_count--;
							} else {
								ep->value = e.value;
							}
							free(l->next);
							nl = 0;
							remain--;
						}
					} else {
						ep->number = 0;
					}
					free(l);
					hs->entry_count--;
				} else {
					prev = l;
				}
				l = nl;
				remain--;
			} while (l);
		}
	}
}

