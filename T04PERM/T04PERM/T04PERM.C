/* FILE NAME: T04PERM.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: permetations
 */

#include <stdio.h>

#define max 42

int p[max];
int n, parity = 0;

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
  int i;

  if (pos >= n)
  {
    PrintPerm();
    return;
  }

  for (i = pos; i <n; i++)
  {
    swap(&p[pos], &p[i]);
    if (i != pos)
      parity = !parity;
    Go(pos + 1);
    swap(&p[pos], &p[i]);
    if (i != pos)
      parity = !parity;
  }
}/* End of Go */

/* function main */
void main( void )
{
  int i;
  
  scanf("%i", &n);

  for (i = 0; i < n; i++)
    p[i] = i + 1;

  Go(0);


}/* End of main */

/* EOF 'T04PERM.C' */