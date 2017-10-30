#include "hash.h"

#define MIN_BASE_SIZE 53

#define PRIME_NUM1

#define PRIME_NUM2

static ht_item_t DELETED_ITEM = {NULL, NULL, NULL};
ht_item_t *
create_ht_item(const char *k, const void *v)
{
	ht_item_t *i = malloc(sizeof(ht_item_t));
	if (!i)
	{
		return NULL;
	} // end if
	
	i->k = strdup(k);
	i->v = v;
	return i;
} // end create_ht_item()

int
destroy_ht_item(ht_item_t *i)
{
	if (!i || !i->container->dtor)
	{
		return -1;
	} // end if
	
	int res = i->container_dtor(i->v);
	if (res == -1)
	{
		return -1;
	} // end if
	
	sfree(i);
	return 0;
} // end destroy_ht_item()

ht_t *
_create_ht(data_dtor_func_t dtor, hash_func_t hf, const int bs)
{
	if (!dtor)
	{
		return NULL;
	} // end if
	
	ht_t *ht = malloc(sizeof(ht_t));
	if (!ht)
	{
		return NULL;
	} // end if
	
	if (bs < MIN_BASE_SIZE)
	{
		ht->base_size = MIN_BASE_SIZE;
	} // end if
	else
	{
		ht->size = get_next_prime(ht->base_size);
	} // end else
	
	ht->count = 0;
	
	ht->items = calloc(ht->size, sizeof(ht_item_t *));
	
	ht->data_dtor = dtor;
	
	if (hf)
	{
		ht->hash_func = hf;
	} // end if
	else
	{
		ht->hash_func = get_hf_index;
	} // end else
	
	return ht;
	
} // end _create_ht()

ht_t *
create_ht(data_dtor_func_t dtor)
{
	if (!dtor)
	{
		return NULL;
	} // end if
	
	return _create_ht(dtor, get_hf_index, MIN_BASE_SIZE);
} // end create_ht()

int
destroy_ht(ht_t *ht)
{
	if (!ht || !ht->items)
	{
		return -1;
	} // end if
	
	int i = 0;
	for (i; i < ht->size; i++)
	{
		ht_item_t *item = ht->items[i];
		if (item != NULL || item != DELETED_ITEM)
		{
			int res = destroy_ht_item(item);
			if (res == -1)
			{
				return -1;
			} // end if
		} // end if
	} // end for
	
	sfree(ht->items);
	sfree(ht);
	return 0;
} // end destroy_ht()

hash_func_data_t *
create_hfd(const char *str, const size_t ht_size, const int i)
{
	if (!str)
	{
		return NULL;
	} // end if
	
	hash_func_data_t *hfd = malloc(sizeof(hash_func_data_t));
	if (!hfd)
	{
		return NULL;
	} // end if
	
	hfd->str = strdup(str);
	hfd->ht_size = ht_size;
	hfd->i = i;
	return hfd;
} // end create_hfd()

int
destroy_hfd(hash_func_data_t *hfd)
{
	if (!hfd)
	{
		return -1;
	} // end if
	
	sfree(hdf->str);
	sfree(hfd);
	return 0;
} // end destroy_hfd()

size_t
get_hash_code(const char *str, const unsigned long pn, const size_t ht_size)
{
	if (!str)
	{
		return -1;
	} // end if
	
	long hash_code = 0;
	const int str_len = strlen(str);
	
	int i = 0;
	for (i; i< str_len; i++)
	{
		hash_code += (long)(pow(pn, str_len - (i + 1)) * str[i]);
		hash_code %= ht_size;
	} // end for
	
	return (size_t)hash_code;
} // end get_hash_code()

size_t /////////////////////////////return type cant be negative problem!!!!!!!
get_ht_index(void *_hfd)
{
	hash_func_data_t * hfd = (hash_func_data_t *)_hfd;
	if (!hfd || !hfd->str)
	{
		return -1;
	} // end if
	
	const size_t hash_code1 = get_hash_code(hfd->str, PRIME_NUM1, hfd->ht_size);
	if (hash_code1 == -1)
	{
		return -1;
	} // end if
	const size_t hash_code2 = get_hash_code(hfd->str, PRIME_NUM2, hfd->ht_size);
	if (hash_code2 == -1)
	{
		return -1;
	} // end if
	
	return ((hash_code1 + (hfd->i * (hash_code2 + 1))) % hfd->ht_size);
} // end get_ht_index()

int
add_to_ht(ht_t **ht, ht_item_t *item, int flags)
{
	if (!ht || !item)
	{
		return -1;
	} // end if
	
	
	// resize?????????????????????????????????????????
	
	// look at prototype
	
	
	hash_func_data_t *hfd = create_hfd(item->k, *ht->size, 0);
	if (!hfd)
	{
		return -1;
	} // end if
	
	int index = get_ht_index((void *)hfd);
	if (index == -1)
	(
		return -1;
	) // end if
	
	ht_item_t *curr_item = ht->items[index];
	int i = 1;
	while (curr_item != NULL && curr_item != &DELETED_ITEM)
	{
		if (strcmp(curr_item->k, item->k) == 0)
		{
			if (!flags)
			{
				return -1; // SPECIAL VALUE????????
			} // end if
			else
			{
				destroy_ht_item(curr_item);
				ht->items[index] = curr_item;
				return 0;
			} // end else
		} // end if
		
		hfd->i = i;
		index = get_ht_index((void *)hfd);
		if (index == -1)
		{
			return -1;
		} // end if
		
		curr_item = ht->item[index];
		i++;	
	} // end while
	
	ht->items[index] = curr_item;
	(ht->count)++
	return 0;
} // end add_to_ht()


void *
search_ht(ht_t *ht, const char *k)
{
	if (!ht || ht->count == 0 || !k)
	{
		return NULL;
	} // end if

	hash_func_data_t *hfd = create_hfd(k, ht->size, 0);
	if (!hfd)
	{
		return NULL;
	} // end if
	
	int index = get_ht_index((void *)hfd);
	if (index == -1)
	(
		return NULL;
	) // end if
	
	ht_item_t *curr_item = ht->items[index];
	int i = 1;
	while (curr_item != NULL && curr_item != &DELETED_ITEM)
	{
		if (strcmp(curr_item->k, k) == 0)
		{
			return curr_item->v;
		} // end if
		
		hfd->i = i;
		index = get_ht_index((void *)hfd);
		if (index == -1)
		{
			return -1;
		} // end if
		
		curr_item = ht->item[index];
		i++;	
	} // end while
	
	return NULL;
} // end search_ht()

int
delete_from_ht(ht_t *ht, const char *k)
{
	if (!ht || ht->size == 0 || !k)
	{
		return -1;
	} // end if
	
	hash_func_data_t *hfd = create_hfd(item->k, *ht->size, 0);
	if (!hfd)
	{
		return -1;
	} // end if
	
	int index = get_ht_index((void *)hfd);
	if (index == -1)
	(
		return -1;
	) // end if
	
	ht_item_t *curr_item = ht->items[index];
	int i = 1;
	while (curr_item != NULL && curr_item != &DELETED_ITEM)
	{
		if (strcmp(curr_item->k, k) == 0)
		{
			int res = destroy_ht_item(curr_item);
			if (res == -1)
			{
				return -1;
			} // end if
			
			ht->items[index] = &DELETED_ITEM;
			ht->count--;
			return 0;
		} // end if
		
		hfd->i = i;
		index = get_ht_index((void *)hfd);
		if (index == -1)
		{
			return -1;
		} // end if
		
		curr_item = ht->item[index];
		i++;	
	} // end while
	
	return -1;
} // end delete_from_ht()

int
is_prime(const int x)
{
	if (x < 2)
	{
		return 0;
	} // end if
	
	if (x < 4)
	{
		return 1;
	} // end if
	
	if (x % 2)
	{
		return 0;
	} // end if
	
	int i = 3;
	for (i; i <= floor(sqrt((double)x)); i += 2)
	{
		if (x % i == 0)
		{
			return 0;
		} // end if
	} // end for
	
	return 1;
} // end is_prime()

int
get_next_prime(int x)
{
	while (!is_prime(x))
	{
		x++;
	} // end while
} // end get_next_prime()

int 
resize_ht(ht_t *ht, size_t new_bs)
{
	if (!ht || new_bs < MIN_BASE_SIZE)
	{
		return -1;
	} // end if
	
	ht_t *new_ht = _create_ht(new_bs); // wrong
	if (!new_ht)
	{
		return -1;
	} // end if
	
	int i = 0;
	for (i; i < ht->size; i++)
	{
		ht_t *curr_item = ht->items[i];
		if (curr_item != NULL && curr_item != &DELETED_ITEM)
		{
			int res = add_to_ht(new_ht, curr_item);
			if (res == -1)
			{
				return -1;
			} // end if
		} // end if
	} // end for
	
	size_t tmp_bs = ht->base_size;
	ht->base_size = new_ht->base_size;
	new_ht->base_size = temp_bs;
	
	size_t tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = temp_size;
	
	size_t tmp_count = ht->count;
	ht->count = new_ht->count;
	new_ht->count = temp_count;
	
	ht_item_t **tmp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;
	
	int res = destroy_ht(new_ht);
	if (res == -1)
	{
		return -1;
	} // end if
	else
	{
		return 0;
	}
} // end resize_ht()

int
resize_ht_up(ht_t *ht)
{
	if (!ht || ht->base_size == 0)
	{
		return -1;
	} // end if
	
	size_t new_bs = ht->base_size * 2;
	int res = resize(ht, new_bs);
	if (res == -1)
	{
		return -1;
	} // end if
	else
	{
		return 0;
	} // end else
} // end resize_ht_up()

int
resize_ht_down(ht_t *ht)
{
	if (!ht || ht->base_size == 0)
	{
		return -1;
	} // end if
	
	size_t new_bs = ht->base_size / 2;
	int res = resize(ht, new_bs);
	if (res == -1)
	{
		return -1;
	} // end if
	else
	{
		return 0;
	} // end else
} // end resize_ht_down()

