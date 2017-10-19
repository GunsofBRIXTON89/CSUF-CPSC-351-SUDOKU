// ============================================================================
// file: project_one.c
// ============================================================================
// Programmers: Jose Ordaz (CWID:805325115) & Faraj Haddad (CWID:)
// Date: 10/19/2017
// Class: CPSC-351 "Operating System Concepts"
// Time: T/TH 2:30 P.M.
// Instructor: Dr. McCarthy, W.
// Project: Sudoku Solution Validator
//
// Description: As stated in the 9th Edition Silbershatz text, Ch04, Pg 197,
// this program consists of designing a multithreaded application that
// determines whether a soduku solution is valid. The approch we took was to
// create three seperate threads to cover the nine Rows, Columns, and Grids.
//
// HOW to Compile: gcc -Wall -std=c99 project_one.c -pthread -o project_one.exe
// ============================================================================

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW_LEN 9
#define MAX_COL_LEN 9
#define NUM_THREADS 3

struct 	runner_struct {
	int case_sum; // counts number of valid rows/grids/columns
	};
 

//The sum computed by the background threads
int g_sudoku_set[MAX_ROW_LEN][MAX_COL_LEN] = { {6, 2, 4, 5, 3, 9, 1, 8, 7},
		      		               {5, 1, 9, 7, 2, 8, 6, 3, 4},
		      			       {8, 3, 7, 6, 1, 4, 2, 9, 5},
	      				       {1, 4, 3, 8, 6, 5, 7, 2, 9},
	      				       {9, 5, 8, 2, 4, 7, 3, 6, 1}, 
	     				       {7, 6, 2, 3, 9, 1, 4, 5, 8}, 
  	      			      	       {3, 7, 1, 9, 5, 6, 8, 4, 2}, 
	      				       {4, 9, 6, 1, 8, 2, 5, 7, 3}, 
	     				       {2, 8, 5, 4, 7, 3, 9, 1, 6} };

// === Traverse_One_Grid =====================================================================
//
// This funciton traverses a section of a two-dimensional global array and adds up the sum of 
// elements within its bounds.
// Input: 
// 	il[IN] -- lower row bound; iu[IN] -- upper row bound;
//      jl[IN] -- lower column bound; ju[IN] -- uppper column bound;
//
// Output: Returns interger 1 if the sum of the elements traversed is 35, 0 otherwise.
// 
// ===========================================================================================


int Traverse_One_Grid(int il,int iu,int jl,int ju){
	int sum = 0;
	for(;il <= iu; ++il){
		for(; jl <= ju; ++jl){
			assert((g_sudoku_set[il][jl] > 0) &&(g_sudoku_set[il][jl] <= 9));
			sum += g_sudoku_set[il][jl];
		}
		jl = jl - 3;
	}
	if(sum == 45){return 1;}

	return 0;

} // end of Traverse_One_Grid




// === CheckRows_runner ======================================================================
// Runner thread that checks each nine rows for validation.
// Inputs: param[IN/OUT] -- param is a void pointer that is casted to a struct object 
// 	                    containing an int member.
// ===========================================================================================
void* CheckRows_runner(void* param){
	int sum;
	struct runner_struct *arg_struct = (struct runner_struct*)param;
	for(int i=0; i <= MAX_ROW_LEN -1 ; ++i){
		sum = 0;
		for(int j=0;j <= MAX_COL_LEN - 1; ++j){
			assert((g_sudoku_set[i][j] > 0) && (g_sudoku_set[i][j] <= 9));
			sum += g_sudoku_set[i][j];
			if( /*(j == MAX_COL_LEN -1) &&*/ (sum == 45) ){
				(arg_struct -> case_sum)++;
				printf("arg_struct -> case_sum in ROWS = %d\n",
				                         arg_struct -> case_sum);
			}
		}
	}

	pthread_exit(0);

 } // end of CheckRows_runner



// === CheckColumns_runner ===================================================================
// Runner thread that checks each nine Columns for validation.
// Inputs: param[IN/OUT] -- param is a void pointer that is casted to a struct object 
// 	                    containing an int member.
// ===========================================================================================
void* CheckColumns_runner(void* param){
	int sum;
	struct runner_struct *arg_struct = (struct runner_struct*)param;
	for(int j=0; j <= MAX_COL_LEN - 1; ++j){
		sum = 0;
		for(int i=0;i <= MAX_ROW_LEN - 1; ++i){
			assert((g_sudoku_set[i][j] > 0) && (g_sudoku_set[i][j] <= 9));
			sum += g_sudoku_set[i][j];
			if( /*(i == MAX_ROW_LEN - 1) &&*/ (sum == 45)){
			(arg_struct -> case_sum)++;
			}
		}
	}

	pthread_exit(0);

} // end of CheckColumns_runner




// === CheckGrid_runner ======================================================================
// Runner thread that checks each of the nine grids for Validation. This runner calls 
// the Traverse_One_Grid function 9 times.
// Inputs: param[IN/OUT] -- param is a void pointer that is casted to a struct object 
// 	                    containing an int member.
// ===========================================================================================
void* CheckGrid_runner(void* param){
	int sum = 0;
	struct runner_struct*  arg_struct = (struct runner_struct*) param;
	// checking grid 1 through 9
	sum += Traverse_One_Grid(0,2,0,2); // Grid 1
	sum += Traverse_One_Grid(0,2,3,5); // Grid 2
	sum += Traverse_One_Grid(0,2,6,8); // Grid 3
	sum += Traverse_One_Grid(3,5,0,2); // Grid 4
	sum += Traverse_One_Grid(3,5,3,5); // Grid 5
	sum += Traverse_One_Grid(3,5,6,8); // Grid 6
	sum += Traverse_One_Grid(6,8,0,2); // Grid 7
	sum += Traverse_One_Grid(6,8,3,5); // Grid 8
	sum += Traverse_One_Grid(6,8,6,8); // Grid 9
	(arg_struct -> case_sum) = sum;
	pthread_exit(0);

} // end of CheckGrid_runner



// === main =================================================================================
// 
// ==========================================================================================
int	main(){

	void*   function_pointers[NUM_THREADS] = {CheckRows_runner,CheckColumns_runner,
						  CheckGrid_runner};
	pthread_t   t_ids[NUM_THREADS];
	// Launch Threads
	struct runner_struct arg[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; ++i){
		arg[i].case_sum = 0;
		pthread_attr_t attr;
		pthread_attr_init(&attr);   // get the default attributes
		pthread_create(&t_ids[i],&attr,function_pointers[i],&arg[i]); // create the thread

	}

	int sum = 0; // sum of all arg->case_sums

	// Wait untill each thread is done working
	for(int i = 0; i < NUM_THREADS; ++i){
		pthread_join(t_ids[i],NULL);    // wait for the thread to exit
		sum += arg[i].case_sum;
	}

	if( sum == 27){printf("The Sudoku Set is a valid set\n");}
	 else{ printf("The Sudoku Set is Not a valid set\n"); }

	return 0;


}// end of main
