/* -*- C -*- */
/* types.h */
/* erzeugt Montag, 26. Februar 2024 11:51 (C) 2024 von Leander Jedamus */
/* modifiziert Sonntag, 03. März 2024 10:53 von Leander Jedamus */
/* modifiziert Montag, 26. Februar 2024 16:00 von Leander Jedamus */

#ifndef TYPES_H
#define TYPES_H 1

/* #if defined __STDC_VERSION__ && __STDC_VERSION__ > 201112L
#include <stdbool.h>
#else
typedef enum { false, true} bool;
#endif
*/
#include <stdbool.h>

#if defined __unix__ || defined __APPLE__
  #define PATH_SEPARATOR ':'
  #define PATH_DELIMITER '/'
#else
  #define PATH_SEPARATOR ';'
  #define PATH_DELIMITER '\\'
#endif

#endif /* TYPES_H */

/* vim:set cindent ai sw=2 */

