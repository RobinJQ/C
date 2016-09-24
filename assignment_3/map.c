#include "map.h"
#include <stdio.h>
#include <stdlib.h>

struct pair_t{
	m_key_t key;
	m_value_t value;
	map_t* equal_hash_map;
};

struct map_t{
	pair_t** values;
	size_t size;
};

pair_t** map_to_sorted_array(map_t* map, size_t* size)
{
	if (map != NULL)
	{
		*size = total_map_size(map);
		if (*size == 0)
		{
			return NULL;
		}
		pair_t** sorted_array = (pair_t**)calloc(sizeof(pair_t*), *size);
		if (sorted_array == NULL)
		{
			fprintf(stderr, "Error: Out of memory\n");
			exit(1);
		}
		size_t current = 0;
		for (size_t i = 0; i < map->size; ++i)
		{
			pair_t* pair = map->values[i];
			if (pair != NULL)
			{
				sorted_array[current++] = pair;
				pair_t* linked_pair = (pair->equal_hash_map)->values[0];
				while (linked_pair != NULL)
				{
					sorted_array[current++] = linked_pair;
					linked_pair = (linked_pair->equal_hash_map)->values[0];
				}
			}
		}
		for (size_t i = 0; i < current-1; ++i)
		{
			for (size_t j = i+1; j < current; ++j)
			{
				if (sorted_array[i]->key < sorted_array[j]->key)
				{
					pair_t* temp = sorted_array[i];
					sorted_array[i] = sorted_array[j];
					sorted_array[j] = temp;
				}
			}
		}
		return sorted_array;
	}
	return NULL;
}

size_t total_map_size(map_t* map)
{
	size_t count = 0;
	if (map != NULL)
	{
		for (size_t i = 0; i < map->size; ++i)
		{
			pair_t* pair = map->values[i];
			if (pair != NULL)
			{
				++count;
				while ((pair->equal_hash_map)->values[0] != NULL)
				{
					++count;
					pair = (pair->equal_hash_map)->values[0];
				}
			}
		}
	}
	return count;
}

map_t* init_map_with_size(size_t size)
{
	map_t* map = calloc(sizeof(map_t), 1);
	pair_t** values = (pair_t**)calloc(sizeof(pair_t*), size);
	if (values == NULL || map == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}
	map->values = values;
	map->size = size;
	return map;
}

pair_t* init_pair(m_key_t key, m_value_t value)
{
	pair_t* pair = calloc(sizeof(pair_t), 1);
    if (pair == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}
	pair->key = key;
	pair-> value = value;
	pair->equal_hash_map = init_map_with_size(1);
	return pair;
}

void free_map(map_t* map)
{
	if (map != NULL)
	{
		for (size_t i = 0; i < map->size; ++i)
		{
			pair_t* pair = map->values[i];
			if (pair != NULL)
			{
				free_map(pair->equal_hash_map);
			}
			free(pair);
		}
		free(map);
	}
}

size_t hash(m_key_t key, map_t* map)
{
	return key % (map->size);
}

m_value_t* value_for_key(m_key_t key,map_t* map)
{
	if (map == NULL)
	{
		return NULL;
	}
	pair_t* pair = map->values[hash(key, map)];
	if (pair->key == key)
	{
		return &pair->value;
	}
	return value_for_key(key, pair->equal_hash_map);
}

pair_t* set_pair(m_key_t key, m_value_t value, map_t* map)
{
	pair_t* current_pair = map->values[hash(key, map)];
	if (current_pair == NULL)
	{
		map->values[hash(key, map)] = init_pair(key, value);
		return map->values[hash(key, map)];
	}
	if (key == current_pair->key)
	{
		return NULL; /*Cannot set equal keys*/
	}
	return set_pair(key, value, current_pair->equal_hash_map);
}

void remove_pair(pair_t* pair, map_t* map)
{
	pair_t* result = map->values[hash(pair->key, map)];
	if (pair != NULL && result != NULL)
	{
		if (result->key == pair->key)
		{
			result = NULL;
		}
		else
		{
			remove_pair(pair, result->equal_hash_map);
		}
 	}
}

m_value_t* get_value(pair_t* pair)
{
	return &pair->value;
}
m_key_t* get_key(pair_t* pair)
{
	return &pair->key;
}
