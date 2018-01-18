
struct matrix {
	
	int rows;
	int cols;

	float **entries;
	
};

int 
matrix_zero_init
(struct matrix **m_dp, int rows, int cols);

int
matrix_copy
(struct matrix **m_dp, struct matrix *A);


int
matrix_swap_rows
(struct matrix *m_p, int i, int j);

int
matrix_mult_row
(struct matrix *m_p, int i, float k);

int 
matrix_add_row
(struct matrix *m_p, int r_1, int r_2, float k);

int
matrix_transpose
(struct matrix **m_dp, struct matrix *A);

int
matrix_product
(struct matrix **m_dp, struct matrix *A, struct matrix *B);

int 
matrix_scalar_product
(struct matrix *m_p, float k);

int
matrix_print(struct matrix *m_p);


