#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
// The hard part of initializing these matrices... 
// It is settled that each matrix *should* have its
// own copies of the entries. 
// since I don't want to get bogged down with pointers 
// to variable length arrays... lets work with a zero
// length array. 
int 
matrix_zero_init
(struct matrix **m_dp, int rows, int cols) {
	
	struct matrix *ret = malloc(sizeof(struct matrix));
	ret->rows = rows;
	ret->cols = cols;

	ret->entries = malloc(sizeof(float *) * rows);
	for(int i = 0; i < rows; i++) {
		ret->entries[i] = malloc(sizeof(float) * cols);
	}

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			ret->entries[i][j] = 0;
		}
	}

	*m_dp = ret;
	return 0;
}

// swap two rows, given their indices. 
// IMPORTANT: This uses math notation, not matrix notation. 
// so, to swap the first and second row, use indices 1, 2
// not 0, 1 which is common in arrays. 
int
matrix_swap_rows
(struct matrix *m_p, int i, int j) {
	i--; 
	j--;
	
	float *tmp = m_p->entries[i];
	m_p->entries[i] = m_p->entries[j];
	m_p->entries[j] = tmp;
	return 0;
}

// multiply a row by a certain value. 
// should be non-zero. 
// IMPORTANT: again, indices start at 1, not 0. 
int
matrix_mult_row
(struct matrix *m_p, int i, float k) {
	i--;

	for(int j = 0; j < m_p->cols; j++) {
		m_p->entries[i][j] *= k;
	}
	return 0;
}

// Same as above two row operations, 
// this row operation is: 
// Ri = Ri + (k * Rj)
int 
matrix_add_row
(struct matrix *m_p, int r_1, int r_2, float k) {

	int i_1 = r_1 - 1;
	int i_2 = r_2 - 1;

	for(int j = 0; j < m_p->cols; j++) {
		m_p->entries[i_1][j] += k * m_p->entries[i_2][j];
	}

	return 0;
}
// 

// Need to decide how matrix multiplication is going to work. 
// on every mult, do we create a new matrix? 
// it kind of seems so. 

// General form of working with matrices: 
// An operation may produce a new matrix, or 
// modify the existing one. For things like 
// multiply matrix by constant, divide, transpose maybe
// the header of the fn will show if it produeces a new
// matrix or not

// A nice simple copy
// produces new matrix. 
int
matrix_copy
(struct matrix **m_dp, struct matrix *A) {
	matrix_zero_init(m_dp, A->rows, A->cols);

	struct matrix *ret = *m_dp;
	
	for(int i = 0; i < A->rows; i++) {
		for(int j = 0; j < A->cols; j++) {
			ret->entries[i][j] = A->entries[i][j];
		}
	}
	return 0;
}

// Create a new matrix
// that is the transpose of A. 
int
matrix_transpose
(struct matrix **m_dp, struct matrix *A) {
	matrix_zero_init(m_dp, A->cols, A->rows);
	
	struct matrix *ret = *m_dp;

	for(int j = 0; j < A->cols; j++) {
		for(int i = 0; i < A->rows; i++) {
			ret->entries[j][i] = A->entries[i][j];
		}
	}
	return 0;
}

// create new matrix that is the 
// progut of A and B. 
// TODO Better error handling. Use assert? 
int
matrix_product
(struct matrix **m_dp, struct matrix *A, struct matrix *B) {
	// common enough of a potential error to report 
	// if the dimensions mismatch. 
	if(A->cols != B->rows) return 1;

	matrix_zero_init(m_dp, A->rows, B->cols);
	struct matrix *ret = *m_dp;

	// now the actual multiplication... 
	
	for(int i = 0; i < A->rows; i++) {
		for(int j = 0; j < B->cols; j++) {
			for(int k = 0; k < A->cols; k++) {
				float val = A->entries[i][k] * B->entries[k][j];
				ret->entries[i][j] += val;
			}
		}
	}

	return 0;

}

int 
matrix_scalar_product
(struct matrix *m_p, float k) {
	for(int i = 0; i < m_p->rows; i++) {
		for(int j = 0; j < m_p->cols; j++) {
			m_p->entries[i][j] *= k;
		}
	}
	return 0;
}

// currently only works for 2x2 matrices!! 
struct matrix *
matrix_inverse(struct matrix *A) {
	struct matrix *inv = NULL;
	matrix_zero_init(&inv, 2, 2);
	inv->entries[0][0] = A->entries[1][1];
	inv->entries[1][1] = A->entries[0][0];
	inv->entries[1][0] = -1 * A->entries[0][1];
	inv->entires[0][1] = -1 * A->entries[1][0];

	float ad = A->entries[0][0] * A->entries[1][1];
	float bc = A->entries[0][1] * A->entries[1][0];
	
	matrix_scalar_product(inv, 1 / (ad - bc));
		
	return inv;
}

int
matrix_print(struct matrix *m_p) {
	// print the elements with 2 decimal places.
	for(int i =0; i < m_p->rows;i++) {
		int j = 0;
		while(j < m_p->cols - 1) {
			printf("%.2f\t", m_p->entries[i][j]);
			j++;
		}
		printf("%.2f\n", m_p->entries[i][j]);
	}
	return 0;
}

// testing functions. There *has* to be a better way 
// to initialize a matrix using a pointer to a var len
// 2d array

// This is for a 3 x 4 array. 
int fill_matrix_01(struct matrix *m_p) {

	// might as well write the matrix nicely	
	float data[12] = {
		1, 2, 3, 4, 
		5, 6, 7, 8,
		9, 10, 11, 12
	};

	for(int i = 0; i < m_p->rows; i++) {
		for(int j = 0; j < m_p->cols; j++) {
			m_p->entries[i][j] = data[4 * i + j];
		}
	}
	
	return 0;
}

int fill_row_vec_01(struct matrix *m_p) {

	float data[3] = {2, 3, 4};

	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			m_p->entries[i][j] = data[j];
		}
	}

	return 0;
}

int fill_col_vec_01(struct matrix *m_p) {

	float data[3] = {-3, 2, 9};

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			m_p->entries[i][j] = data[i];
		}
	}

	return 0;
}


int main(void) {

	struct matrix *A;
	matrix_zero_init(&A, 1, 3);

	struct matrix *B;
	matrix_zero_init(&B, 3, 1);

	fill_row_vec_01(A);
	fill_col_vec_01(B);

	struct matrix *C;
	matrix_product(&C, A, B);
	matrix_print(C);

	return 0;
}
