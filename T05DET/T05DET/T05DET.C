    /* FILE NAME: T05DET.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: determinater
 */

#include <stdio.h>
#include <time.h>

#define max 42
#define mtrMax 42

#pragma warning(disable: 4244)

int p[max];
int n, parity = 0;
double m[mtrMax][mtrMax], det = 0;

/* function swap */
void swap( int* a, int* b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}/* End of swap */

/* function Go */
/* ! RECURSIVE ! */
void Go( int pos )
{
  int i, saveParity;
  double tmp = 1;

  if (pos >= n)
  {
    for (i = 0; i < n; i++)
      tmp *= m[i][p[i]];
    det+= !parity ? tmp : -tmp;
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

/* function MatrixLoad */
void MatrixLoad(char* FileName)
{
  int i, j;
  FILE* mF;

  mF = fopen(FileName, "r");

  if (mF == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }

  fscanf(mF, "%d", &n);

  if (n > mtrMax)
    n = mtrMax;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      fscanf(mF, "%lf", &m[i][j]);

  fclose(mF);
}
/* End of MatrixLoad */

/* function main */
void main( void )
{
  int i;
  float t;
  FILE* mF;

  MatrixLoad("Matrix.txt");

  mF = fopen("Matrix.txt", "a");
  if (mF == NULL)
  {
    printf("ERROR cannot open file\n");
    return;
  }
  
  for (i = 0; i < n; i++)
    p[i] = i;

  t = clock();
  Go(0);
  t = clock() - t;
  t = t / CLOCKS_PER_SEC;


  fprintf(mF ,"Determinator: %lf\n", det);

  fclose(mF);

}/* End of main */

/* EOF 'T05DET.C' */