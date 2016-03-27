/**
 * Autor: Martin Maga (xmagam00)
 * PRL Proj1
 * Minimum Extraction sort
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


#define READ_MODE "r"

//const section
const int halt_value = -255;

//this value is used when user provide incorrect number of numbers as input
const int default_dd_value = -1;


int get_proc_number(int procs);

//main code
int main(int argc, char **argv)
{
    MPI_Status status;
    int my_id;
    
    int parent_number = 0; 
    int my_number = default_dd_value; 
    int child_left = 0;
    int child_right = 0; // miesto pre cisla od childov
    int children_stop = 0;
    int halt = 0; 
    
    int number_of_processors;   //pocet procesorov
    int proc_num = atoi(argv[2]);
    int real_num_count = 0;
    int character;
    int proc_count;
    
    
    //init MPI
    MPI_Init(&argc,&argv);
    //get number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processors);
    //get my proccessor of id       
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);       
    
    MPI_Barrier(MPI_COMM_WORLD); 
    double start = MPI_Wtime();
    //Am I Root node ?
    if (0 == my_id)
    { 
        //FILE pointer
        FILE *fr = NULL;
        //read "numbers" file with numbers from dd utility
        fr = fopen(argv[1], READ_MODE); 
        if (NULL == fr) {
            exit(EXIT_FAILURE);
        }
        //get proc numbers
        proc_count = get_proc_number(proc_num);
        
        //real until EOF
        while (1)
        {   
            character = getc(fr);
            if (character == EOF) {
                break;
            } 
            //printf("%d",character); 
            //putchar(' ');
            
            MPI_Send(&character, 1, MPI_INT, proc_count, 0, MPI_COMM_WORLD);
            proc_count += 1;
        }
        
        //close file
        fclose(fr);

        // according definition minimum extraction 
        if (proc_count != proc_num){

            character = default_dd_value; 
            if ((proc_count + 1) != proc_num) {
                //putchar(' ');
            }
            while(proc_count != proc_num){
                MPI_Send(&character, 1, MPI_INT, proc_count, 0, MPI_COMM_WORLD);
                proc_count += 1;
            }

        }
        //put new line
        //putchar('\n');
    }
    
    
    if (get_proc_number(proc_num) <= my_id){
        MPI_Recv(&my_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
    }
    
    while (halt != halt_value){
        if (my_id < get_proc_number(proc_num)){ 
            
            if ((my_id == 0) && (my_number >= 0)){
                //printf("%d\n",my_number);
                my_number = default_dd_value;
            }

            if ((my_number == default_dd_value) && (children_stop != halt_value)){
                
                int left_rank_child = my_id * 2 + 1;
                int right_rank_child = my_id * 2 + 2;
                
                MPI_Recv(&child_right, 1, MPI_INT, right_rank_child, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&child_left, 1, MPI_INT, left_rank_child, 0, MPI_COMM_WORLD, &status);
                
              
                if ((child_left == default_dd_value) && (child_right != default_dd_value)){
                    my_number = child_right;
                    child_right = default_dd_value;
                }
                else if ((child_right == default_dd_value) && (child_left != default_dd_value)){
                    my_number = child_left;
                    child_left = default_dd_value;
                }
                else if ((child_left == default_dd_value) && (child_right == default_dd_value)){
                    child_left = halt_value;
                    child_right = halt_value;
                    children_stop = halt_value;
                    if (my_id == 0) {
                        halt = halt_value;
                    }
                }
                else if (child_left <= child_right){
                    my_number = child_left;
                    child_left = default_dd_value;
                }
                else if (child_left > child_right){
                    my_number = child_right;
                    child_right = default_dd_value;
                }
                //values send back to sons
                MPI_Send(&child_right, 1, MPI_INT, right_rank_child, 0, MPI_COMM_WORLD);
                MPI_Send(&child_left, 1, MPI_INT, left_rank_child, 0, MPI_COMM_WORLD);
                
            }
        } 

        if (my_id != 0){
            int parent_number_rank = 0;
            
            if((my_id % 2) != 0) {
                parent_number_rank = (my_id - 1)/2; // neparny, lavy syn
            } 
            if((my_id % 2) == 0) {
                parent_number_rank = (my_id - 2)/2; // parny, pravy syn
            }

            MPI_Send(&my_number, 1, MPI_INT, parent_number_rank, 0, MPI_COMM_WORLD);
            MPI_Recv(&my_number, 1, MPI_INT, parent_number_rank, 0, MPI_COMM_WORLD, &status);
            if (my_number == halt_value) {
              halt = halt_value;  
            } 
        }
    }
    //its import to finish all proccesses
    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();
    if (my_id == 0) {
        printf("%f\n", (end - start)*1000);
    }
    MPI_Finalize();
    //return message
    return EXIT_SUCCESS;
}

int get_proc_number(int procs) {
    return (((procs + 1) / 2) - 1);
}