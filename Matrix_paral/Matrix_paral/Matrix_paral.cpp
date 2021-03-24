#include<cstdlib>
#include<stdarg.h>
#include <omp.h>
#include <random>
#include <iostream>
using namespace std;

double          t_Start;    
double          t_Stop;

int main(int argc, char** argv[], char** envp[])
{
	setlocale(LC_ALL, "Russian");  
	int tid = 0;                
	const int n = 15;         
	const int m = 15;
	double a[n][m], b[n][m], c[n][m];
	int res = 0;        
	t_Start = omp_get_wtime();
	for (int i = 0; i < n; i++)  
	{
		for (int w = 0; w < m; w++)
		{
			a[i][w] = 99;
			b[i][w] = 99;
		}
	}

#pragma omp parallel num_threads(4) private(tid) shared(res)
	{
		int numt = omp_get_num_threads();
		int locres = 0;         
		int kol = n + m / numt;       
		tid = omp_get_thread_num();
		if (tid < numt - 1)         
		{

			for (int i = 0; i < kol; i++)
			{
				c[n][m] = 0;
				for (int w = 0; w < kol; w++)
				{
					c[n][m] += a[tid * kol + n][tid * kol + w] * b[tid * kol + n][tid * kol + w];
				}
			}
		}
		else                   
		{
			for (int i = 0; i < kol + n - kol * numt; i++)
			{
				c[n][m] = 0;
				for (int w = 0; w < kol + m - kol * numt; w++)
				{
					c[n][m] += a[tid * kol + i][tid * kol + w] * b[tid * kol + i][tid * kol + w];
				}
			}
		}
		res = res + c[n][m];
	}

	t_Stop = omp_get_wtime() - t_Start / 10;  
	cout << res << "\n" << endl;       
	cout << "время=" << t_Stop << " секунд." << endl;  

	system("pause");
	return 0;
}