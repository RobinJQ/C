#include <stdio.h>

#ifndef MAP_H
#define MAP_H

typedef struct pair_t pair_t;
typedef struct map_t map_t;
typedef long m_key_t;
typedef double m_value_t; 

void remove_pair(pair_t* pair, map_t* map);
pair_t* set_pair(m_key_t key, m_value_t value, map_t* map);
m_value_t* value_for_key(m_key_t key,map_t* map);
size_t hash(m_key_t key, map_t* map);
void free_map(map_t* map);
pair_t* init_pair(m_key_t key, m_value_t value);
map_t* init_map_with_size(size_t size);
size_t total_map_size(map_t* map);
pair_t** map_to_sorted_array(map_t* map, size_t* size);
m_value_t* get_value(pair_t* pair);
m_key_t* get_key(pair_t* pair);

#endif