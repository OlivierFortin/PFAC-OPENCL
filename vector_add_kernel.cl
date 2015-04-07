__kernel void vector_add(__global int *A, __global int *B, __global int *C) {

 

 // Get the index of the current element
    int i = get_global_id(0);

if (i>1000) printf("\n\nExecuting Kernel vector_add %d\n", i);


   
    // Do the operation
    C[i] = A[i] + B[i];
 //printf("%d ", C[i]);


}

__kernel void CheckMatch(__global int *h_input, __global int *h_StatesTab, __global int *h_C, __global int* h_OutputStates){

//printf("\nExecuting Kernel CheckMatch %d\n", get_global_id(0));

//*********************************************//
	int threadsPerBlock=1;
	int totalThreadNumber= 16;
	int inputL= 1024 *16;
//*********************************************//


	//h_C[0] = 222;	

	int Alphabet_Size = 71;  //nvar globale
	int PatternNum=50;	// var globale
	int currentState = 0;
	int nextState = 0;



	int i = get_local_id(0);
	int j= get_group_id(0);

	int threadID = j* threadsPerBlock+i;
	h_C[j] = 111;

	if (j == 0 )  h_C[19] =2;
	 
	//printf("Thread id =  %d\n", threadID);

/*

	__local int local_input [128];
	

	 for (int l=0; l<4; l++){

		local_input[i+threadsPerBlock*l] = h_input[j*inputL+i+threadsPerBlock*l];
		if (get_group_id(0)==0) {  printf("local_input[%d] = h_input[%d]=%d, globalId= %d;\n",i+threadsPerBlock*l,j*inputL+i+threadsPerBlock*l, h_input[j*inputL+i+threadsPerBlock*l], get_global_id(0));}
	}

	//printf("get_global_id(0)= %d; j= get_global_id(1)= %d,   get_group_id(0)=%d;   get_group_id(1)=%d; \n",i,j, get_group_id(0),get_group_id(1));
	barrier(CLK_LOCAL_MEM_FENCE);

*/
	
	

	 int k=0;	
	 int iii= j* h_OutputStates[0]+i;
	int StartPosition = iii;
         int finIndexSegment=(j+1)* h_OutputStates[0];
 	h_C[j] = 2101;	
	h_C[25] = h_input[0];
	h_C[26] = h_OutputStates[0];
	if (iii<inputL)  {
	h_C[j] = 2102;
	//printf("Debut scan Thread id =  %d  a la position %d \n", threadID, iii);
		do {
				k= currentState*Alphabet_Size +h_input[iii];
				nextState= h_StatesTab[k];
				
		
				if (nextState==0) {
                                        h_C[j] = 2103;
					StartPosition=StartPosition +threadsPerBlock;
					iii=StartPosition; 

					currentState=0;


				}
				if ((nextState>0)& (h_OutputStates[nextState]==1)) 
					{
					//printf("Patron trouve ID= %d----threadIDX= %d  \n", nextState,i);	
					h_C[16] = iii;	                                       
					//printf("Malicous Pattern Found----  position= %d \n", iii); 		
					currentState= 0 ;
					StartPosition=StartPosition +threadsPerBlock;
					iii=StartPosition;
					}

				if ((nextState>0)& (h_OutputStates[nextState]==0)){
						 currentState= nextState ;
						 iii=iii+1;
				}

		}while ((iii<finIndexSegment )& (nextState>=0));
	}
	h_C[j] = 2104;




}






