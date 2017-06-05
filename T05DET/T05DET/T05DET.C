    /* FILE NAME: T05DET.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: determinater
 */

#include <stdio.h>
#include <time.h>

#define max 42
#define mtrMax 4

int p[max], mtr[mtrMax][mtrMax];
int n = mtrMax, parity = 0, det;

/* function swap */
void swap( int* a, int* b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}/* End of swap */

/* function PrintPerm */
void PrintPerm( void )
{
  int i;
  FILE* F;

  F = fopen("PERM.TXT", "a");

  if (F == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  for (i = 0; i < n; i++)
    fprintf(F, "%i", p[i]);
  fprintf(F, " - %s\n", parity ? "odd" : "even");

  fclose(F);
}/* End of PrintPerm */

/* function Go */
/* ! RECURSIVE ! */
void Go( int pos )
{
  int i, tmp, saveParity;;

  if (pos >= n)
  {
    //PrintPerm();
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
  for (i = pos + 1; i <n; i++)
    p[i - 1] = p[i];
  p[n - 1] = tmp;
  parity = saveParity;

}/* End of Go */

/* function main */
void main( void )
{
  int i, j;
  float t;
  FILE* mF;

  mF = fopen("MTR.TXT", "r");

  if (mF == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  for (i = 0; i < mtrMax; i++)
    for (i = 0; i < mtrMax; i++)
      fscanf(mF, "%i", &mtr[i][j]);
  
  for (i = 0; i < n; i++)
    p[i] = i;



  t = clock();
  Go(0);
  t = clock() - t;
  t = t / CLOCKS_PER_SEC;
  //fprintf("Timer: %f\n\n", t);

}/* End of main */

/* EOF 'T05DET.C' */