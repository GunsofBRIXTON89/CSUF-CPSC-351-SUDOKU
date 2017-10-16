/* File: project_one.c
*/

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
//#include <iomanip>

//#include <iostream>
//using namespace std;

#define MAX_ROW_LEN 9
#define MAX_COL_LEN 9
#define NUM_THREADS 3

struct 	runner_struct {
	int case_sum; // counts number of valid rows/grids/columns
	};
 

//The sudoku set read by background threads
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
// 	ilBound[IN] -- lower row bound; iuBound[IN] -- upper row bound;
//      jlBound[IN] -- lower column bound; juBound[IN] -- uppper column bound;
//
// Output: Returns interger 1 if the sum of the elements traversed is 35, 0 otherwise.
// 
// ===========================================================================================


int Traverse_One_Grid(int ilBound,int iuBound,int jlBound,int juBound){
	int sum = 0;
	for(;ilBound <= iuBound; ++ilBound){
		for(; jlBound <= juBound; ++jlBound){
			assert((g_sudoku_set[ilBound][jlBound] > 0) &&
						 (g_sudoku_set[ilBound][jlBound] <= 9));
			sum += g_sudoku_set[ilBound][jlBound];
		}
	}

	if(sum == 35){return 1;}
	
	return 0;

} // end of Traverse_One_Grid




// === CheckRows_runner ======================================================================
// 
// Inputs: param[IN/OUT] --
//
// ===========================================================================================
void* CheckRows_runner(void* param){
	int sum;
	struct runner_struct *arg_struct = (struct runner_struct*)param;
	for(int i=0; i < MAX_ROW_LEN-1; ++i){
		sum = 0;
		for(int j=0;j < MAX_COL_LEN-1; ++j){
			assert((g_sudoku_set[i][j] > 0) && (g_sudoku_set[i][j] <= 9));
			sum += g_sudoku_set[i][j];
			if((j == MAX_COL_LEN -1) && (sum == 35)){
				(arg_struct->case_sum)++;
			}
		} 
	} 
	
	pthread_exit(0);

 } // end of CheckRows_runner



// === CheckColumns_runner ===================================================================
// 
// Inputs: param[IN/OUT] --
//
// ===========================================================================================
void* CheckColumns_runner(void* param){
	int sum;
	struct runner_struct *arg_struct = (struct runner_struct*)param;
	for(int j=0; j < MAX_COL_LEN-1; ++j){
		sum = 0;
		for(int i=0;i < MAX_ROW_LEN-1; ++i){
			assert((g_sudoku_set[i][j] > 0) && (g_sudoku_set[i][j] <= 9));
			sum += g_sudoku_set[i][j];
			if((j == MAX_COL_LEN -1) && (sum == 35)){
			(arg_struct -> case_sum)++;
			}
		}
	} 
	
	pthread_exit(0);

} // end of CheckColumns_runner




// === CheckGrid_runner ======================================================================
// 
// Inputs: param[IN/OUT] --
//
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
	
	arg_struct -> case_sum = sum;
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
	
	for(int i = 0; i < NUM_THREADS - 1; ++i){
		pthread_attr_t attr;
		pthread_attr_init(&attr);   // get the default attributes
		pthread_create(&t_ids[i],&attr,function_pointers[i],&arg);   // create the thread

	}

	int sum = 0; // sum of all arg->case_sums

	// Wait untill each thread is done working
	for(int i = 0; i < NUM_THREADS - 1; ++i){
		pthread_join(t_ids[i],NULL);    // wait for the thread to exit
		printf("Case_sum of thread# %d is %d \n",i+1, arg[i].case_sum);
		sum += arg[i].case_sum;
	}

	if( sum == 27){printf("The Sudoku Set is a valid set\n");}
	 else{ printf("The Sudoku Set is Not a valid set\n"); }
	
	return 0;

}// end of main
