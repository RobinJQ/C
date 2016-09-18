#ifndef MEMEORY_TESTING_H
	void read_as_float(int a);
	void restrict_arrays();
	void sum_of_restrict_arrays(size_t n,
									 double* restrict a,
									 double* restrict b,
									 double* restrict c);
	void sum_of_arrays(size_t n,
						 double* a,
						 double* b,
						 double* c);
	void overlapping_arrays();
	void non_overlapping_arrays();
	void completely_overlapping_arrays();
	void print_arrays(double* a,
					  double* b, 
					  double* c,
					  int i);
	void restrict_assignment();
#endif 