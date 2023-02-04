/** 
    @file text.c
    @author Christopher Fields (cwfields)
    Implementation of the Text VType. Defines a subclass
    for storing strings. Differs from VType in providing
    parseText function to make an instance of an Text.
*/

#include "text.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// print method for Text.
static void print( VType const *v )
{
  // Convert the VType pointer to a more specific type.
  Text const *this = (Text const *) v;
  printf( "\"%s\"", this->val );
}

// equals method for Text.
static bool equals( VType const *a, VType const *b )
{
  // Make sure the b object is also a Text object.
  // (i.e., it uses the same print funtion)
  if ( b->print != print )
    return false;

  // Compare the str fields inside a and b.
  Text const *this = (Text const *) a;
  Text const *that = (Text const *) b;

  return strcmp(this->val, that->val) == 0;
}

// hash method for Text.  It hashes to the string it contains,
// with negative values overflowing to positive.
static unsigned int hash( VType const *v )
{
  // Convert the VType pointer to a more specific type.
  Text const *this = (Text const *) v;

  // Get the length of the string contained in the Text object.
  int length = strlen(this->val);

  // Hash using Jenkins 32-bit hash function
  size_t i = 0;
  unsigned int hash = 0;
  while (i != length) {
    hash += this->val[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

// destroy method for Text.
static void destroy( VType *v )
{
  // Convert the VType pointer to a more specific type.
  Text *this = (Text *) v;

  // Free dynamically allocated string within Text.
  free(this->val);

  // Free entire Text object.
  free(this);
}

VType *parseText( char const *init, int *n )
{
  // Make sure the string is in the right format.
  char *str = malloc(strlen(init));
  bool quote = false;
  int idx = 0;
  int len = 0;
  // Iterate through each character in initialization string.
  for (int i = 0; init[i]; i++) {
    if (init[i] == '"') {
      if (quote) { // Second quotation mark reached
        len = i + 1;
        str[idx] = '\0';
        break;
      } else { // First quotation mark reached
        quote = true;
      }
    } else {
      if (quote) { // If the quote was found, read into str
        if (init[i] == '\\') {
          if (init[i + 1] == '"') { // Quotation escape sequence
            str[idx] = '"';
            i++;
            idx++;
            continue;
          } else if (init[i + 1] == 'n') { // Newline escape sequence
            str[idx] = '\n';
            i++;
            idx++;
            continue;
          } else if (init[i + 1] == 't') { // Horizontal tab escape sequence
            str[idx] = '\t';
            i++;
            idx++;
            continue;
          } else if (init[i + 1] == '\\') { // Backslash escape sequence
            str[idx] = '\\';
            i++;
            idx++;
            continue;
          }
        }
        str[idx] = init[i];
        idx++;
      }
    }
  }

  if (!quote) {
    free(str);
    return NULL;
  }

  // Reallocate str to take up exact space
  str = realloc(str, strlen(str) + 1);

  // Fill in the end pointer, if the caller asked for it.
  if ( n )
    *n = len;
  
  // Allocate a Text object on the heap and fill in its fields.
  Text *this = (Text *) malloc( sizeof( Text ) );
  this->val = str;
  this->print = print;
  this->equals = equals;
  this->hash = hash;
  this->destroy = destroy;

  // Return it as a pointer to the superclass.
  return (VType *) this;
}
