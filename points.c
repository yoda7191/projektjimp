#include "points.h"
#include <stdlib.h>

#define MAX_DATA 10000

static int realloc_pts_failed (points_t * pts, int size)
{
  pts->x = realloc(pts->x, size * sizeof * pts->x);
  pts->y = realloc(pts->y, size * sizeof * pts->y);

  return pts->x == NULL || pts->y == NULL;
}

int read_pts_failed (FILE * inf, points_t * pts)
{
  int size;
  double x, y;

  if (pts->n == 0) 
  {
    pts->x = malloc (MAX_DATA * sizeof *pts->x);
    if (pts->x == NULL)
      return 1;
    pts->y = malloc (MAX_DATA * sizeof *pts->y);
    if (pts->y == NULL) 
    {
      free (pts->x);
      return 1;
    }
    size = MAX_DATA;
  }
  else
    size = pts->n;

  while (fscanf (inf, "%lf %lf", &x, &y) == 2) 
  {
    pts->x[pts->n] = x;
    pts->y[pts->n] = y;
    pts->n++;
    if (!feof (inf) && pts->n == size) 
    {
      if (realloc_pts_failed (pts, 2 * size))
        return 1;
      else
        size *= 2;
    }
  }

  if (pts->n != size)
    if (realloc_pts_failed (pts, pts->n))
      return 1;

  return 0;
}

void free_pts( points_t *pts )
{
	free( pts->x );
	free( pts->y );
}

