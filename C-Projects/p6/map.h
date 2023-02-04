/** 
    @file map.h
    @author Christopher Fields (cwfields)
    Header for the map component, a hash map. Provides
    the functions to define a Map, including makeMap,
    mapSize, mapSet, mapGet, mapRemove, and freeMap for
    performing specied operations on the Map.
*/

#ifndef MAP_H
#define MAP_H

#include "vtype.h"
#include <stdbool.h>

/** Incomplete type for the Map representation. */
typedef struct MapStruct Map;

/** Make an empty map.
    @param len Initial length of the hash table.
    @return pointer to a new map.
*/
Map *makeMap( int len );

/** Get the size of the given map.
    @param m Pointer to the map.
    @return Number of key/value pairs in the map. */
int mapSize( Map *m );

/**
   Adds the given key/value pair to the given map. If
   the key is already in the map, it replaces its value
   with the given value.
   @param m Pointer to the map to add to.
   @param key Key of the value to add to Map.
   @param val Value to add to the Map.
 */
void mapSet(Map *m, VType *key, VType *val);

/** Return the value associated with the given key. The returned VType
    is still owned by the map.
    @param m Map to query.
    @param k Key to look for in the map.
    @return Value associated with the given key, or NULL if the key
    isn't in the map.
*/
VType *mapGet( Map *m, VType *key );

/**
   Removes the key/value pair associated with the given key. If the key
   isn't in the map, returns NULL.
   @param m 
   @param key 
   @return true 
 */
bool mapRemove(Map *m, VType *key);

/** Free all the memory used to store a map, including all the
    memory in its key/value pairs.
    @param m The map to free.
*/
void freeMap( Map *m );
  
#endif
