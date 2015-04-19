__kernel void CheckMatch(__global int *h_input, __global int *h_StatesTab, __global int *h_C, __global int* h_OutputStates,
  int totalThreadNumber,
  int threadsPerBlock){

//*********************************************//
	//int threadsPerBlock=1;
	//int totalThreadNumber= 16;
	int inputL= 1024 * totalThreadNumber;
//*********************************************//

	h_C[0] = 222;

	int Alphabet_Size = 71;  //nvar globale
	int PatternNum=50;	// var globale
	int currentState = 0;
	int nextState = 0;



	int i = get_local_id(0);
	int j= get_group_id(0);

	int threadID = j * threadsPerBlock+i;

	if (j == 0 )  h_C[19] =2;

	int k=0;
	int iii= j* h_OutputStates[0]+i;
	int StartPosition = iii;
  int finIndexSegment=(j+1)* h_OutputStates[0];
 	h_C[j] = 2101;
	h_C[25] = h_input[0];
	h_C[26] = h_OutputStates[0];
	if (iii<inputL)  {
	   h_C[j] = 2102;
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
					h_C[16] = iii;
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
