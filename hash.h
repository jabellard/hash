#ifndef _HASH_H
#define _HASH_H

typedef struct _ht_item_t
{
	char *k;
	void *v;
	struct _ht_t *container;
}ht_item_t;

typedef struct _hash_func_data_t
{
	char *str;
	ht_size_t ht_size;
	int i;
}hash_func_data_t;

typedef ht_size_t (*hash_func_t)(hash_func_data_t *);

typedef int (*data_dtor_func_t)(void *);

typedef ht_size_t (*find_finder_func_t)(ht_size_t);

typedef long ht_size_t;

typedef struct _ht_t
{
	ht_size_t base_size;
	ht_size_t size;
	ht_size_t count;
	ht_item_t **items;
	data_dtor_func_t data_dtor;
	prime_finder_func_t prime_finder;
	hash_func_t hash_func;
}ht_t;

ht_item_t *
create_ht_item(const char *k, const void *v);

int
destroy_ht_item(ht_item_t *i);

ht_t *
_create_ht(int bs, data_dtor_func_t dtor, prime_finder_func_t pf, hash_func_t hf);

ht_t *
create_ht(data_dtor_func_t dtor);

int
destroy_ht(ht_t *ht);

hash_func_data_t *
create_hfd(const char *str, const ht_size_t ht_size, const int i);

int
destroy_hfd(hash_func_data_t *hfd);

ht_size_t
get_hash_code(const char *str, const unsigned long pn, const ht_size_t ht_size);

ht_size_t
get_ht_index(hash_func_data_t *hfd);

int
add_to_ht(ht_t *ht, ht_item_t *item, int flags);

void *
search_ht(ht_t *ht, const char *k);

int
delete_from_ht(ht_t *ht, const char *k);

ht_size_t
is_prime(ht_size_t x);

ht_size_t
get_next_prime(ht_size_t x);

int 
resize_ht(ht_t *ht, ht_size_t new_bs);

int
resize_ht_up(ht_t *ht);

int
resize_ht_down(ht_t *ht);

#endif // _HASH_H
