/** 
    @file text.h
    @author Christopher Fields (cwfields)
    Header for the Text subclass of VType. Defines
    the functions for Text, including an extra,
    parseText. Also includes a val field in the
    Text typedefed struct.
*/

#ifndef TEXT_H
#define TEXT_H

#include "vtype.h"

/** Subclass of VType for storing text. */
typedef struct {
  /** Inherited from VType */
  void (*print)( struct VTypeStruct const *v );

  /** Inherited from VType */
  bool (*equals)( struct VTypeStruct const *a,
                  struct VTypeStruct const *b );

  /** Inherited from VType */
  unsigned int (*hash)( struct VTypeStruct const *b );

  /** Inherited from VType */
  void (*destroy)( struct VTypeStruct *v );

  /** Value stored by this text. */
  char *val;
} Text;

/** Make an instance of Text holding a value parsed from the init string.
    @param init String containing the initializaiton value as text.
    @param n Optional return for the number of characters used from init.
    @return pointer to the new VType instance.
*/
VType *parseText( char const *init, int *n );

#endif
