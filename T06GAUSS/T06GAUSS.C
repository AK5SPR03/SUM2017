    /* FILE NAME: T06GAUSS.C
 * PROGRAMMER: AK5
 * DATE: 07.06.17
 * PURPOSE: SLAU
 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#define max 6
#define mtrMax 6

#pragma warning(disable: 4244)

int p[max];
int n, parity = 0, sign = 1;
double M[mtrMax][mtrMax], determ, B[max], X[max];
/*
double m[2][2] = {{1, -1}, {2, -1}};
double determ, X[max], B[max] = {5, -1};
*/

/* function swap */
void swap( int* a, int* b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}/* End of swap */

/* function fswap */
void fswap( double* a, double* b )
{
  double tmp = *a;
  *a = *b;
  *b = tmp;
}/* End of fswap */

/* function GaussMethod */
void GaussMethod( void )
{
  int i, j, k, max_j;
  double det = 1;

  for (i = 0; i < n; i++)
  {             /* start for */ 
    
    /* search maximum */
    max_j = i;
    for (j = i; j < n; j++)
      if (fabs(M[max_j][i]) < fabs(M[j][i]))
      {
        max_j = j;
      }

    /* swapping */
    if (max_j != i)
    {
      sign *= -1;
      for (j = 0; j < n; j++)
        fswap(&M[max_j][j], &M[i][j]);
      fswap(&B[max_j], &B[i]);
    }

    /* triangulation matrix */
    for (k = i + 1; k < n; k++)
    {
      double coef = M[k][i] / M[i][i];

      for (j = 0; j < n; j++)
        M[k][j] -= M[i][j] * coef;
      B[k] -= B[i] * coef;
    }
  }              /* end for */

  /* counting X */
  for (i = n - 1; i >= 0; i--)
  {
    double tmp = 0;

    for (j = n - 1; j > i; j--)
      tmp += X[j] * M[i][j];
    X[i] = (B[i] - tmp) / M[i][i];
  }
}
/* End of GaussMethod */



/* function MatrixLoad */
void MatrixLoad( char* FileName )
{
  int i, j;
  FILE* F;

  F = fopen(FileName, "r");

  if (F == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  fscanf(F, "%d", &n);

  if (n > mtrMax)
    n = mtrMax;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
      fscanf(F, "%lf", &M[i][j]);
    fscanf(F, "%lf", &B[i]);
  }

  fclose(F);
}
/* End of MatrixLoad */

/* function main */
void main( void )
{
  int i;
  FILE* F;

  MatrixLoad("Matrix.txt");

  F = fopen("Output.txt", "a");
  if (F == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  GaussMethod();

  /* output X */
  for (i = 0; i < n; i++)
    fprintf(F, "%lf \n", X[i]);

  

  fclose(F);

}/* End of main */

/* EOF 'T06GAUSS.C' */

