#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

# include <string>

using namespace std;

int     i4_choose_poly (int n,int k);
int     i4_fall (int x,int n);
int     i4_max_poly (int i1, int i2 );
int     i4_min_poly (int i1, int i2 );
void    i4vec_concatenate ( int n1, int a[], int n2, int b[], int c[] );
void    i4vec_permute ( int n, int p[], int base, int a[] );
int*    i4vec_sort_heap_index_a ( int n, int a[] );
int     i4vec_sum_poly ( int n, int a[] );
void    mono_next_grlex ( int d, int x[] );
int     mono_rank_grlex ( int d, int x[] );
void    mono_total_next_grlex ( int d, int n, int x[] );
int*    mono_unrank_grlex ( int d, int rank );
int     mono_upto_enum ( int d, int n );
double* mono_value ( int d, int n, int f[], double x[] );
bool    perm_check ( int n, int p[], int base );
void    polynomial_add ( int o1, double c1[], int e1[], int o2, double c2[], int e2[], int &o, double c[], int e[] );
void    polynomial_axpy ( double s, int o1, double c1[], int e1[], int o2, double c2[], int e2[], int &o, double c[], int e[] );
void    polynomial_compress ( int o1, double c1[], int e1[], int &o2, double c2[], int e2[] );
void    polynomial_dif ( int d, int o1, double c1[], int e1[], int dif[], int &o2, double c2[], int e2[] );
void    polynomial_mul ( int d, int o1, double c1[], int e1[], int o2, double c2[], int e2[], int &o, double c[], int e[] );
void    polynomial_print ( int d, int o, double c[], int e[], string title );
void    polynomial_scale ( double s, int d, int o1, double c1[], int e1[] );
void    polynomial_sort ( int o, double c[], int e[] );
double* polynomial_value ( int d, int o, double c[], int e[], int nx, double x[] );
void    r8vec_concatenate ( int n1, double a[], int n2, double b[], double c[] );
void    r8vec_permute ( int n, int p[], int base, double a[] );

#endif // POLYNOMIAL_H