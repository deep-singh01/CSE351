/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Deepesh Singh  
 * NetID(s): dsingh01
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
   
   // iterators: a = row; b = column; c = block in row; d = block in column
   int a, b, c, d;
   // if c == d
   int equal_index, equal_val;
   // variables for storage & access for 64 x 64 matrix
   int v64_1, v64_2, v64_3, v64_4, v64_5, v64_6;
   if (M == 32) {
      // block size: 8
      for (a = 0; a < M; a += 8) {
         for (b = 0; b < N; b += 8) {
            for (c = a; c < a + 8; c++) {
               for (d = b; d < b + 8; d++) {
                  
                  if (c == d) {
                     equal_val = A[c][c];
                     equal_index = c;

                  } else {
                     B[d][c] = A[c][d];
                  }
               }
               
               if (a == b) {
                  B[equal_index][equal_index] = equal_val;
               }
            }
         }
      }
   } else if (M == 64) {
      // block size: 4
      for (a = 0; a < M; a += 4) {
         for (b = 0; b < N; b += 4) {
            
	         v64_1 = A[a][b];
            v64_2 = A[a+1][b];
            v64_3 = A[a+1][b+1];
            v64_4 = A[a+2][b];
            v64_5 = A[a+2][b+1];
            v64_6 = A[a+2][b+2];

            // B[b][]
            B[b][a] = v64_1;
            B[b][a+1] = v64_2;
            B[b][a+2] = v64_4;
            B[b][a+3] = A[a+3][b];
	         v64_1 = A[a+3][b+3];
	         v64_2 = A[a+3][b+1];
	         v64_4 = A[a+3][b+2];

            // B[b+1][]
	         B[b+1][a] = A[a][b+1];
	         B[b+1][a+1] = v64_3;
	         B[b+1][a+2] = v64_5;
	         B[b+1][a+3] = v64_2;
	    
            // B[b+2][]
	         B[b+2][a] = A[a][b+2];
	         B[b+2][a+1] = A[a+1][b+2];
	         B[b+2][a+2] = v64_6; 
	         B[b+2][a+3] = v64_4;

            // B[b+3][]
	         B[b+3][a] = A[a][b+3];
	         B[b+3][a+1] = A[a+1][b+3];
	         B[b+3][a+2] = A[a+2][b+3];
	         B[b+3][a+3] = v64_1;
	      }
      }
   }
}

// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
