/* -*- C -*- */
/* main.c */
/* erzeugt Freitag, 26. Januar 2024 14:48 (C) 2024 von Leander Jedamus */
/* modifiziert Donnerstag, 07. März 2024 16:09 von Leander Jedamus */
/* modifiziert Sonntag, 03. März 2024 18:08 von Leander Jedamus */
/* modifiziert Sonntag, 03. MÃ¤rz 2024 13:24 von Leander Jedamus */
/* modifiziert Dienstag, 27. Februar 2024 04:34 von Leander Jedamus */
/* modifiziert Montag, 26. Februar 2024 16:47 von Leander Jedamus */
/* modifiziert Samstag, 24. Februar 2024 14:52 von Leander Jedamus */
/* modifiziert Freitag, 23. Februar 2024 16:43 von Leander Jedamus */
/* modifiziert Donnerstag, 22. Februar 2024 17:42 von Leander Jedamus */
/* modifiziert Freitag, 16. Februar 2024 15:10 von Leander Jedamus */
/* modifiziert Montag, 29. Januar 2024 07:05 von Leander Jedamus */
/* modifiziert Sonntag, 28. Januar 2024 06:12 von Leander Jedamus */
/* modifiziert Samstag, 27. Januar 2024 18:12 von Leander Jedamus */
/* modifiziert Freitag, 26. Januar 2024 19:47 von Leander Jedamus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>

#ifdef __unix__
#include <popt.h>
#endif

#if defined __linux__
  #include <linux/limits.h>
#elif defined __APPLE__
  #include <sys/syslimits.h>
#endif

#include "project.h"

#include "types.h"
#include "getlocaledir.h"
#include "gettext.h"

#ifdef AUTOMAKE
#include <config.h>
#else
#define PACKAGE PROJECT 
#include "version.h"
#endif

#ifdef __unix__
void usage(poptContext optCon, int exitcode, char *error, char *addl) {
           poptPrintUsage(optCon, stderr, 0);
  if (error) fprintf(stderr, "%s: %s\n", error, addl);
  exit(exitcode);
}
#endif

void get_names_and_path(char ** in_filename, char ** out_path, char ** in_path, char ** out_filename) {
  char   c;
  char * ptr;

  ptr = strrchr(*in_filename, PATH_DELIMITER);
  if(ptr == NULL) {
    *in_path = ".";
  } else {
    c = *ptr;
    *ptr = '\0';
    *in_path = malloc(PATH_MAX);
    strcpy(*in_path, *in_filename);

    *ptr = c;
    *in_filename = malloc(PATH_MAX);
    ++ptr;
    strcpy(*in_filename, ptr);
  }

  *out_filename = *in_filename;

  if(*out_path == NULL) {
    *out_path = malloc(2);
    strcpy(*out_path, ".");
  }
}

#define BUFSIZE 1024L

int main(int argc, char * argv[]) {
  size_t        size;
  char          infilename[PATH_MAX*2+2+20];
  char          outfilename[PATH_MAX*2+2+20];
  size_t        nr = 1;
  static size_t wanted_size = BUFSIZE;
  int           verbose = 0;

  FILE *        rf;
  FILE *        wf;
  char *        localedir;

#ifdef __unix__
  char          tmpstr[100];
  poptContext   optCon;   /* context for parsing command-line options */
  int           c;
  static char * in_filename = NULL;
  static char * in_path = NULL;
  static char * out_filename = NULL;
  static char * out_path = NULL;

  struct poptOption optionsTable[] = {
    { "size", 's', POPT_ARG_INT, &wanted_size, 0,
      _("wanted size"), "SIZE" },
    { "kb", 'k', 0, 0, 'k',
      _("in kb"), "KB" },
    { "mb", 'm', 0, 0, 'm',
      _("in mb"), "MB" },
    { "gb", 'g', 0, 0, 'g',
      _("in gb"), "GB" },
    { "verbose", 'v', 0, 0, 'v',
      _("verbose"), NULL },
    /* { "infile", 'i', POPT_ARG_STRING, &in_filename, 0,
      _("input file"), NULL }, */
    { "outpath", 'o', POPT_ARG_STRING, &out_path, 0,
      _("output path"), NULL },
    POPT_AUTOALIAS
    POPT_AUTOHELP
    POPT_TABLEEND
    /* { NULL, 0, 0, NULL, 0 } */
  };
#else
  int          i;
  size_t       len;
  /*
  static char  in_filename[PATH_MAX];
  static char  in_path[PATH_MAX];
  static char  out_filename[PATH_MAX];
  static char  out_path[PATH_MAX];
  */
  static char * in_filename = NULL;
  static char * in_path = NULL;
  static char * out_filename = NULL;
  static char * out_path = NULL;
#endif

#ifdef AUTOMAKE
  localedir = LOCALEDIR;
#else
  localedir = getlocaledir(argv[0]);
#endif

#ifdef DEBUG
  printf("%s\n",localedir);
#endif

  setlocale(LC_ALL, "");
  /* setlocale(LC_CTYPE, "");  */
  /* setlocale(LC_MESSAGES, ""); */

  bindtextdomain(PACKAGE, localedir);
  textdomain(PACKAGE);

  printf(_("%s V%s (C) %s by %s <%s>\n"), PACKAGE, VERSION, YEARS, AUTHOR, EMAIL);


#ifdef __unix__
  optCon = poptGetContext(NULL, argc, (const char **) argv, optionsTable, 0);
  poptSetOtherOptionHelp(optCon, _("[OPTIONS]* <filename>"));

  if (argc < 2) {
    poptPrintUsage(optCon, stderr, 0);
    exit(EXIT_FAILURE);
  } else {
    while ((c = poptGetNextOpt(optCon)) >= 0) {
      /* printf("c=%c\n", c); */
      switch (c) {
	case 'k':
	  wanted_size *= 1024;
	  break;
	case 'm':
	  wanted_size *= 1024*1024;
	  break;
	case 'g':
	  wanted_size *= 1024*1024*1024;
	  break;
	case 'v':
	  verbose++;
	  break;
      }
    }
    in_filename = (char *) poptGetArg(optCon);
    if((in_filename == NULL) || !(poptPeekArg(optCon) == NULL)) {
      sprintf(tmpstr,_("e.g. %ctmp%ctest"), PATH_DELIMITER, PATH_DELIMITER);
      usage(optCon, 1, _("Specify a file name"), tmpstr);
      return(EXIT_FAILURE);
    }

    if(wanted_size < BUFSIZE) {
      printf(_("warning: size %ld too small, now is %ld.\n"), wanted_size, BUFSIZE);
      wanted_size = BUFSIZE;
    }

    get_names_and_path(&in_filename, &out_path, &in_path, &out_filename);

    if(verbose > 1) {
      printf("localedir=%s\n", localedir);
      printf("wanted_size=%ld\n", wanted_size);
      printf("in_filename=%s\n", in_filename);
      printf("in_path=%s\n", in_path);
      printf("out_filename=%s\n", out_filename);
      printf("out_path=%s\n", out_path);

      printf(_("filename ='%s', path='%s'\n"), in_filename, in_path);
    }

    /* exit(EXIT_SUCCESS); */
#else
  verbose = 1;

  if (argc < 3) {
    printf(_("usage: %s <filename> <1M> [<output path>]\n"),argv[0]);
  } else {
    in_filename = malloc(PATH_MAX);
    strcpy(in_filename, argv[1]);
    if (argc == 4) {
      out_path = malloc(PATH_MAX);
      strcpy(out_path, argv[3]);
    } else {
      out_path = NULL;
    }
    get_names_and_path(&in_filename, &out_path, &in_path, &out_filename);

    if(verbose > 1) {
      printf("in_filename=%s\n", in_filename);
      printf("in_path=%s\n", in_path);
      printf("out_filename=%s\n", out_filename);
      printf("out_path=%s\n", out_path);
    }

    len = strlen(argv[2]);
    wanted_size = 0;
    for(i = 0; i < len; i++) {
      char d = argv[2][i];

      if(verbose > 2) 
        printf("d=%c, ", d);

      if((d >= '0') && (d <= '9')) {

	if(verbose > 2) 
	  printf("=%d", d - '0');

	wanted_size = wanted_size * 10 + d - '0';
      } else {
	if(wanted_size == 0) wanted_size = 1;
	d = tolower(d);
	if(d == 'k') { wanted_size *= 1024; break; }
	if(d == 'm') { wanted_size *= 1024*1024; break; }
	if(d == 'g') { wanted_size *= 1024*1024*1024; break; }
	break;
      };/* else */
    };/* for i */

    if(verbose > 1)
      printf("wanted_size=%ld\n", wanted_size);

    if(wanted_size < BUFSIZE) {
      printf(_("warning: size %ld too small, now is %ld.\n"), wanted_size, BUFSIZE);
      wanted_size = BUFSIZE;
    }

#endif

    nr   = 1;
    size = 0;

    sprintf(infilename,"%s%c%s", in_path, PATH_DELIMITER, in_filename);
    if(verbose > 0) {
      printf(_("Reading from \"%s\".\n"), infilename);
    }
    rf   = fopen(infilename, "rb");
    if(rf == NULL) {
      fprintf(stderr, _("error: fopen '%s': %s\n"), infilename, _(strerror(errno)));
      return EXIT_FAILURE;
    }
    sprintf(outfilename,"%s%c%s.%ld", out_path, PATH_DELIMITER, out_filename, nr);
    if(verbose > 0) {
      printf(_("Writing to \"%s\".\n"), outfilename);
    }
    wf = fopen(outfilename, "wb");
    if(wf == NULL) {
      fprintf(stderr, _("error: fopen '%s': %s\n"), outfilename, _(strerror(errno)));
      return EXIT_FAILURE;
    }
    for(;;) {
      unsigned char buffer[BUFSIZE];
      size_t        n = fread(buffer, 1, BUFSIZE, rf);

      if(n < BUFSIZE) {
	if(ferror(rf)) {
          fprintf(stderr, _("error: fread '%s': %s\n"), infilename, _(strerror(errno)));
          return EXIT_FAILURE;
	}
      }

      if(n > 0) {
	if(fwrite(buffer, 1, n, wf) != n)
	  if(ferror(wf)) {
            fprintf(stderr, _("error: fwrite '%s': %s\n"), outfilename, _(strerror(errno)));
            return EXIT_FAILURE;
	  }

	size += n;
	if(size >= wanted_size) {
	  fclose(wf);
	  nr++;
	  size = 0;
	  sprintf(outfilename,"%s%c%s.%ld", out_path, PATH_DELIMITER, out_filename, nr);
          if(verbose > 0) {
            printf(_("Writing to \"%s\".\n"), outfilename);
          }
	  wf = fopen(outfilename, "wb");
          if(wf == NULL) {
            fprintf(stderr, _("error: fopen '%s': %s\n"), outfilename, _(strerror(errno)));
            return EXIT_FAILURE;
	  }
	};/* if size >= wanted_size */
      };/* if n > 0 */
      if(n < BUFSIZE) break;
    }

    fclose(wf);
    fclose(rf);
  }

  return EXIT_SUCCESS;
}

/* vim:set cindent ai sw=2: */

