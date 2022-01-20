#include "points.h"
#include "wielomianbaza4.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


// M/  main do poly -> trzeba to chyba będzie zepnąć w jednego, wspólnego maina 
// + poly trzeba będzie opracować na spl 
// zaczynam kwestionować naszą metode XD

char *usage =
  "Usage: %s -f poly-file [-p points-file]\n"
  "\n"
  "Program simulating least-squares function approximation with quartic plane curve base\n"
  "\n"	      
  "	       if points-file is given then\n"
  "               reads discrete 2D points from points-file\n"
  "               writes approximation to poly-file\n"
  "               - number of points should be >= 4\n"
  "            else (points-file not given)\n"
  "               reads poly from poly-file\n"
  "            endif\n";

int main (int argc, char **argv)
{
  int opt;
  char *inp = NULL;
  char *out = NULL;
  int n = 100;
  char *progname= argv[0];

  points_t pts;
  poly_t poly;

  pts.n = 0;
  poly.n = 0;

  printf("Przed getopt\n");

  /* process options, save user choices */
  while ((opt = getopt (argc, argv, "p:f:")) != -1) {
    switch (opt) {
    case 'p':
      inp = optarg;
      break;
    case 'f':
      out = optarg;
      break;
    default:                   /* '?' */
      fprintf (stderr, usage, progname);
      exit (EXIT_FAILURE);
    }
  }
	if( optind < argc ) 
  {
		fprintf( stderr, "\nBad parameters!\n" );
		for( ; optind < argc; optind++ )
			fprintf( stderr, "\t\"%s\"\n", argv[optind] );
		fprintf( stderr, "\n" );
		fprintf( stderr, usage, progname );
		exit( EXIT_FAILURE );
	}

   printf("Po getopt\n");

  /* if points-file was given, then read points, generate spline, save it to file */
  if (inp != NULL)
  {
    FILE *ouf = NULL; /* we shall open it later, when we shall get points */

    FILE *inf = fopen (inp, "r");
    if (inf == NULL) 
    {
      fprintf (stderr, "%s: can not read points file: %s\n\n", argv[0], inp);
      exit (EXIT_FAILURE);
    }

    printf("czytanie punktów\n");

    if (read_pts_failed (inf, &pts)) //jezeli nie uda nam sie odczytac punktow to wywalamy erro
    {
      fprintf (stderr, "%s: bad contents of points file: %s\n\n", argv[0], inp);
      exit (EXIT_FAILURE);
    }
    else //jezeli sie uda to zamykamy plik
      fclose (inf);

    printf("Przeczytano punkty\nOtwieram poly-file\n");
	
    ouf = fopen (out, "w");
    if (ouf == NULL) {
      fprintf (stderr, "%s: can not write poly-file: %s\n\n", argv[0], out);
      exit (EXIT_FAILURE);
    }
    
    printf("Otworzono poly-file, robię make_poly\n");

    make_poly (&poly, &pts);

    printf("Po make_poly, robię write_poly\n");

    if( poly.n > 0 )
      write_poly(&poly, ouf);
      //wielomian baza 4 rob

    // narazie wyrzucam, zobaczymy czy moze tak byc
    // if( spl.n > 0 )
		// 	write_spl (&spl, ouf);
      printf("Po write_poly\n");

      fclose (ouf);
  } else if (out != NULL) {  /* if point-file was NOT given, try to read splines from a file */
    printf("out != NULL\n");

    FILE *polyf = fopen (out, "r");
    if (polyf == NULL) {
      fprintf (stderr, "%s: can not read poly-file: %s\n\n", argv[0], inp);
      exit (EXIT_FAILURE);
    }
    
    printf("Robię read_poly\n");

    if (read_poly (polyf, &poly)) {
      fprintf (stderr, "%s: bad contents of poly-file: %s\n\n", argv[0],
               inp);
      exit (EXIT_FAILURE);
    }
  } else { // ponts were not given nor spline was given -> it is an error 
    fprintf (stderr, usage, argv[0]);
    exit (EXIT_FAILURE);
  }
    printf("Po read_poly\n");

  if (poly.n < 1) { /* check if there is a valid poly */
    fprintf (stderr, "%s: bad poly: n=%d\n\n", argv[0], poly.n);
    exit (EXIT_FAILURE);
  }

   printf("Koniec, dziękuję\n");
   return 0;
}
