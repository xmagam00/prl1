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

const int processor_root_id  = 0;

const char *file_name = "numbers";

//function which count correct number numbers according to processors
int get_proc_number(int procs);

//main code
int main(int argc, char **argv)
{
    MPI_Status status;
    int proc_num = atoi(argv[1]);
    int my_num = default_dd_value; 
    int my_id = -99;
    int parent_num = 0; 
   
    int child_left = 0;
    int child_right = 0;
    int child_halt = 0;
    int halt = 0; 
    
    int number_of_processors = 0;  
   
    int real_num_count = 0;
    int character = 0;
    int proc_count = 0;
    
    //init MPI
    MPI_Init(&argc,&argv);
    //get number of processors
    MPI_Comm_size(MPI_COMM_WORLD,&number_of_processors);
    //get my proccessor of id       
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);       
    
    //this code is used only for computation measurement
    //MPI_Barrier(MPI_COMM_WORLD); 
    //double start = MPI_Wtime();
    

    //Am I Root node ?
    if (processor_root_id == my_id)
    { 
        //FILE pointer
        FILE *fr = NULL;
        //read "numbers" file with numbers from dd utility
        fr = fopen(file_name,READ_MODE); 
        if (NULL == fr) {
            exit(EXIT_FAILURE);
        }
        //get proc numbers
        proc_count = get_proc_number(proc_num);
        
        //real until EOF
        while (1)
        {   
            character = getc(fr);
            if (character == EOF) 
            {
                break;
            } 
            printf("%d",character); 
            putchar(' ');
            
            MPI_Send(&character,1,MPI_INT,proc_count,0,MPI_COMM_WORLD);
            proc_count += 1;
        }
        
        //close file
        fclose(fr);

        // according definition minimum extraction 
        if (proc_count != proc_num)
        {
            character = default_dd_value; 
            
            if ((proc_count + 1) != proc_num) 
            {
                putchar(' ');
            }
            //send numbers to leaf processors
            while(proc_count != proc_num)
            {
                MPI_Send(&character,1,MPI_INT,proc_count,0,MPI_COMM_WORLD);
                proc_count += 1;
            }
        }
        //put new line
        putchar('\n');
    }
    
    //I am leaf processor
    if (get_proc_number(proc_num) <= my_id)
    {
        //as leaf proccessor receive number from ROOT for sorting
        MPI_Recv(&my_num,1,MPI_INT,0,0,MPI_COMM_WORLD,&status); 
    }
    
    //endless loop
    while (halt != halt_value){
        if (get_proc_number(proc_num) > my_id)
        { 
            
            //test if i am root
            if ((my_id == processor_root_id))
            {
                //root can also receive -1
                if ( 0 <= my_num) {
                    printf("%d",my_num);
                    putchar('\n');
                    //set my number as -1 its means that I am IDLE
                    my_num = default_dd_value;
                }
            }


            if ((child_halt != halt_value)&& (my_num == default_dd_value))
            {
                int left_rank_child = (my_id * 2) + 1;
                int right_rank_child = (my_id * 2) + 2;
                
                MPI_Recv(&child_right,1,MPI_INT,right_rank_child,0,MPI_COMM_WORLD,&status);
                MPI_Recv(&child_left,1,MPI_INT,left_rank_child,0,MPI_COMM_WORLD,&status);
                
              
                if (default_dd_value == child_left) 
                {
                    if (default_dd_value != child_right) 
                    {
                        my_num = child_right;
                        child_right = default_dd_value;
                    } else {
                        child_left = halt_value;
                        child_right = halt_value;
                        child_halt = halt_value;
                        //if i am root thent halt
                        if (my_id ==processor_root_id) 
                        {
                            halt = halt_value;
                        }
                    }
                }
                else if (default_dd_value ==child_right )
                {
                    if (default_dd_value !=  child_left) 
                    {
                        my_num = child_left;
                        child_left = default_dd_value;
                    }
                }
                else if (child_right >= child_left )
                {
                    my_num = child_left;
                    child_left = default_dd_value;
                }
                else if (child_right <child_left)
                {
                    my_num = child_right;
                    child_right = default_dd_value;
                }
                
                //send value to right son
                MPI_Send(&child_right, 1, MPI_INT, right_rank_child, 0, MPI_COMM_WORLD);
                
                //send value to left son
                MPI_Send(&child_left, 1, MPI_INT, left_rank_child, 0, MPI_COMM_WORLD);
                
            }
        } 

        //if i am not root processor
        if (my_id != processor_root_id)
        {
            int parent_num_rank = 0;
            int my_id_mod = my_id % 2;
            
            if(my_id_mod != 0) 
            {
                parent_num_rank = ((my_id - 1) / 2); 
            } 
            
            if(my_id_mod  == 0) 
            {
                parent_num_rank = ((my_id - 2) / 2); 
            }

            //send value to parent
            MPI_Send(&my_num,1,MPI_INT,parent_num_rank,0,MPI_COMM_WORLD);
            //receive number from parent
            MPI_Recv(&my_num,1,MPI_INT,parent_num_rank,0,MPI_COMM_WORLD,&status);
            
            //if i as leaf get number for stop then i leave while loop
            if (my_num == halt_value) 
            {
              halt = halt_value;  
            } 
        }
    }
    
    
    //this code is used only for computation measurement
    
    /*MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();
    if (my_id == 0) {
        //printf("%f\n", (end - start)*1000);
    }*/
    
    //its import to finish all proccesses
    MPI_Finalize();
    //return message
    return EXIT_SUCCESS;
}

//function for getting correct number of number for specified number of processor
int get_proc_number(int procs) 
{
    return (((procs + 1) / 2) - 1);
}