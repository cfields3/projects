/** 
    @file vtype.h
    @author CSC 230
    @author Christopher Fields (cwfields)
    Header for the Integer VType superclass. defines the superclass
    used to represent strings and integers in the map. This superclass
    is used by both the Integer and Text subclasses to add key and values
    of different types into a Map.
*/

#ifndef VTYPE_H
#define VTYPE_H

#include <stdbool.h>

/** Abstract type used to represent an arbitrary value. */
typedef struct VTypeStruct {
  /** Pointer to a function that prints this value to the terminal.
      @param v Pointer to the vtype object to print. */
  void (*print)( struct VTypeStruct const *v );

  /** Compare the two given instances, returning true
      if they are equivalent.
      @param a Pointer to the left-hand value to compare (the one
      containing this function)
      @param b Poitner to the right-hand value to compare.
      @return True if the values are equal. */
  bool (*equals)( struct VTypeStruct const *a,
                  struct VTypeStruct const *b );

  /** Compute a hash function for this value.
      @param v Pointer to the inststance this funciton is called for.
      @return Hash value for this instance */
  unsigned int (*hash)( struct VTypeStruct const *b );

  /** Pointer to a function that frees memory for this instance.
      @param v Pointer to the node containing the value to print. */
  void (*destroy)( struct VTypeStruct *v );
} VType;

#endif
