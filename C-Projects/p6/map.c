/** 
    @file map.c
    @author Christopher Fields (cwfields)
    Hash table implementation of a map. Provides an
    implementation of the map as a hash table. Allows
    the client to make a Map, get the size of the Map,
    add to the Map, remove from the Map, get an element
    from the Map, and free the memory contained by the Map.
*/

#include "map.h"
#include <stdlib.h>

#include "vtype.h"

/** Mutlpilier to change capacity by if size reaches capacity of table */
#define CAP_MULTIPLIER 2

/** Node containing a key / value pair. */
typedef struct NodeStruct {
  /** Pointer to the key part of the key / value pair. */
  VType *key;
  
  /** Pointer to the value part of the key / value pair. */
  VType *val;
  
  /** Pointer to the next node at the same element of this table. */
  struct NodeStruct *next;
} Node;

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  Node **table;

  /** Current length of the table. */
  int tlen;
  
  /** Current size of the map (number of different keys). */
  int size;
};

Map *makeMap( int len )
{
  Map *m = (Map *) malloc( sizeof( Map ) );
  m->size = 0;

  m->tlen = len;
  m->table = malloc(m->tlen * sizeof(Node *));

  for (int i = 0; i < m->tlen; i++) {
    m->table[i] = NULL;
  }
  
  return m;
}

int mapSize( Map *m )
{
  return m->size;
}

/**
   Helper method to expand the capacity of the Map if
   the size becomes equal to capacity. Moves all data
   from table into new table, freeing the old table of
   smaller capacity.
 
   @param m the Map to expand the table capacity of
 */
static void expandMap(Map *m)
{
  // Create a newLen and allocate a new table
  int newLen = CAP_MULTIPLIER * m->tlen;
  Node **newTable = malloc(newLen * sizeof(Node *));

  // Initialize new table values to NULL
  for (int i = 0; i < newLen; i++) {
    newTable[i] = NULL;
  }

  // Iterate through each element of the Map table, adding it to the newTable
  for (int i = 0; i < m->tlen; i++) {
    if (m->table[i]) {
      Node *current = m->table[i];
      while (current) { // Iterate through each element at an index, adding it to newTable
        int idx = current->key->hash(current->key) % newLen; // Index of element in newTable
        Node *node = malloc(sizeof(Node));
        node->key = current->key;
        node->val = current->val;
        node->next = NULL;
        if (newTable[idx] == NULL) {
          newTable[idx] = node;
        } else {
          node->next = newTable[idx];
          newTable[idx] = node;
        }
        // Free the old node in original Map
        Node *oldNode = current;
        current = current->next;
        free(oldNode);
      }
    }
  }

  // Make change the reference of the original map to be the new table
  Node **oldTable = m->table;
  m->table = newTable;
  m->tlen = newLen;

  // Free the memory associated with the old table
  free(oldTable);
}

void mapSet(Map *m, VType *key, VType *val) {
  if (m->size >= m->tlen) {
    expandMap(m);
  }
  int idx = key->hash(key) % m->tlen;
  Node *current = m->table[idx];
  while (current) { // Check if item is in list and replace it
    if (current->key->equals(current->key, key)) {
      current->val->destroy(current->val);
      current->val = val;
      key->destroy(key);
      return;
    }
    current = current->next;
  }
  Node *node = malloc(sizeof(Node)); // Allocate a new node to add
  node->key = key;
  node->val = val;
  node->next = NULL;
  if (m->table[idx] == NULL) { // If the hashed index is empty
    m->table[idx] = node;
  } else { // Hashed index is not empty, add to the beginning of a linked list
    node->next = m->table[idx];
    m->table[idx] = node;
  }
  m->size++;
}

VType *mapGet( Map *m, VType *key )
{
  int idx = key->hash(key) % m->tlen; // Hashed index to find key at
  Node *current = m->table[idx];

  while (current) { // Iterate through values in linked list, searching for key
    if (current->key->equals(current->key, key)) {
      return current->val;
    }
    current = current->next;
  }
  return NULL;
}

bool mapRemove(Map *m, VType *key) {
  Node **target = &( m->table[key->hash(key) % m->tlen] ); // Use pointer to pointer to remove

  while (*target && !(*target)->key->equals((*target)->key, key)) { // Until you reach key (or end of list)
    target = &(*target)->next;
  }

  if (*target) { // If you found the key (value of target is not NULL), return it
    Node *n = *target;
    *target = (*target)->next;
    n->key->destroy(n->key);
    n->val->destroy(n->val);
    free(n);
    m->size--;
    return true;
  }

  return false;
}

void freeMap( Map *m )
{
  // Free each entry in the table and each key/value pair in the Nodes
  for (int i = 0; i < m->tlen; i++) {
    Node *current = m->table[i];
    while (current) {
      // Use destroy to free any allocated memory within the key/val
      current->key->destroy(current->key);
      current->val->destroy(current->val);
      // Free the node itself and move to the next node
      Node *node = current;
      current = current->next;
      free(node);
    }
  }

  // Free the table and map itself
  free(m->table);
  free( m );
}
