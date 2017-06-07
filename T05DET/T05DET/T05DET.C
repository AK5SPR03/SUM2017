    /* FILE NAME: T05DET.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: determinater
 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#define max 37
#define mtrMax 37

#pragma warning(disable: 4244)

int p[max];
int n, parity = 0, sign = 1;
double m[mtrMax][mtrMax], determ;

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
double GaussMethod( void )
{
  int i, j, k, max_i, max_j;
  double det = 1;

  for (i = 0; i < n; i++)
  {             //start for 
    
    /* search maximum */
    max_i = i;
    max_j = i;
    for (j = i; j < n; j++)
      for (k = i; k < n; k++)
        if (fabs(m[max_i][max_j]) < fabs(m[j][k]))
        {
          max_i = j; 
          max_j = k;
        }

    /* swapping */
    if (max_i != i)
    {
      sign *= -1;
      for (j = i; j < n; j++)
        fswap(&m[max_i][j], &m[i][j]);
    }
    if (max_j != i)
    {
      sign *= -1;
      for (j = i; j < n; j++)
        fswap(&m[j][max_j], &m[j][i]);
    }

    for (k = i + 1; k < n; k++)
    {
      double coef = m[k][i] / m[i][i];

      for (j = i; j < n; j++)
        m[k][j] -= m[i][j] * coef;
    }
  }              //end for

  for (i = 0; i < n; i++)
    det *= m[i][i];
  det *= sign;
  return det;
  
}
/* End of GaussMethod */

/* function Go */
/* !RECURSIVE! */
void Go( int pos )
{
  int i, saveParity;
  double tmp = 1;


  if (pos >= n)
  {
    for (i = 0; i < n; i++)
      tmp *= m[i][p[i]];
    determ+= !parity ? tmp : -tmp;
    return;
  }


  Go(pos + 1);
  saveParity = parity;
  for (i = pos + 1; i <n; i++)
  {
    swap(&p[pos], &p[i]);
    parity = !parity;
    Go(pos + 1);
    
  }

  tmp = p[pos];
  for (i = pos + 1; i < n; i++)
    p[i - 1] = p[i];
  p[n - 1] = tmp;
  parity = saveParity;

}/* End of Go */

/* DefDeterm function */
double DefDeterm( void )
{
  int i;

  determ =  0;

  for (i = 0; i < n; i++)
    p[i] = i;
  Go(0);
  return determ;
}
/* End of DefDeterm */

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
    for (j = 0; j < n; j++)
      fscanf(F, "%lf", &m[i][j]);

  fclose(F);
}
/* End of MatrixLoad */

/* function main */
void main( void )
{
  int t;
  FILE* F;

  MatrixLoad("Matrix.txt");

  F = fopen("Matrix.txt", "a");
  if (F == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  t = clock();
  //fprintf(F ,"DeterminatorDEF  : %f\n", DefDeterm());
  t = clock() - t;
  fprintf(F ,"Def time:   %.15f\n", t / CLOCKS_PER_SEC);

   t = clock();
  fprintf(F ,"DeterminatorGAUSS: %f\n", GaussMethod());
  t = clock() - t;
  fprintf(F ,"Gauss time: %.15f\n", t / CLOCKS_PER_SEC);

  fclose(F);

}/* End of main */

/* EOF 'T05DET.C' */