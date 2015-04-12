#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)


void FillMatrixAtLine (char* line, int LineNum , int * H_StatesTab, int AlphaSize ){
  const char* tok;
  int j =0 ;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
	H_StatesTab [LineNum * AlphaSize + j ]= atoi(tok);
        j= j+1;
    }
}


const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num){

            return tok;
	}
    }
    return NULL;
}

int main(void) {
printf("start\r\n");
	const int Alphabet_Size = 71;
	const int States_Number = 134;

	const int inputL = 1024 *16 ; // input Lenght

printf("Before malloc\r\n");
    const int LIST_SIZE = Alphabet_Size* States_Number;
    int *h_StatesTab = (int*)malloc(sizeof(int)*LIST_SIZE);
    int *h_input = (int*)malloc(sizeof(int)*inputL);
    int *h_outputStates = (int*)malloc(sizeof(int)*States_Number);
    int *C = (int*)malloc(sizeof(int)*inputL);
printf("After malloc\r\n");
 FILE* stream2 = fopen("data/StatesTab.csv", "r");

    char line2[1024];
    int index2=0;

    while (fgets(line2, 1024, stream2))
    {

        char* tmp2 = strdup(line2);

     	FillMatrixAtLine (tmp2, index2, h_StatesTab, Alphabet_Size );
        //printf("Filling matrix at line  %d  %s\n" , index2, tmp2);
        index2= index2+1;
        // NOTE strtok clobbers tmp
        free(tmp2);
    }
 fclose (stream2);
printf("After states tab\r\n");

FILE* stream3 = fopen("data/OutputStates.csv", "r");
printf("%d %d %d %d",CL_INVALID_PLATFORM ,CL_INVALID_VALUE,CL_INVALID_DEVICE,CL_DEVICE_NOT_AVAILABLE);
    char line3[10];
	int index3=0;
    while (fgets(line3, 10, stream3))
    {
        char* tmp3 = strdup(line3);

        h_outputStates[index3]=atoi( getfield(tmp3, 1));
     //   printf("OutputState [%d] =  %s\n" ,index3, getfield(tmp3, 1));
        // NOTE strtok clobbers tmp
        free(tmp3);
	index3= index3+1;
    }
printf("After output states\r\n");
fclose(stream3);


  FILE* stream = fopen("data/inputFile.csv", "r");
  char line[1024];
  int index=0;
  while (fgets(line, 1024, stream))
  {
    char* tmp = strdup(line);

    h_input[index]=atoi( getfield(tmp, 1));
    // NOTE strtok clobbers tmp
    index=index+1;
    free(tmp);
  }

  fclose(stream);

printf("after input file\r\n");
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("src/cl/vector_add_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
printf("after vector_add_kernel\r\n");
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=//

	printf("Before");
	 // Get platform and device information
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;

   //---------------------------------------------------------
   //Discover and initialize the platform
   //---------------------------------------------------------
   cl_uint nplatforms;
   cl_platform_id* platforms;
   cl_platform_id platform;
   char buffer[256];

   clGetPlatformIDs( 0,0,&nplatforms);
   platforms = (cl_platform_id*)malloc(nplatforms*sizeof(cl_platform_id));
   clGetPlatformIDs( nplatforms, platforms, 0);
   int i= 0;
   for(i=0; i<nplatforms; i++) {
      platform = platforms[i];
      clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,256,buffer,0);
      printf(buffer);
	    if (!strcmp(buffer,"coprthr")) break;
   }
   platform_id = platforms[0];

    cl_int ret = clGetPlatformIDs(0, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1,
            &device_id, &ret_num_devices);
    printf("after get device id err : %d \r\n",ret);
    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
printf("after context , error : %d  \r\n",ret);
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
printf("after clCreateCommandQueue");
    // Create memory buffers on the device for each vector
    cl_mem d_StatesTab = clCreateBuffer(context,  CL_MEM_COPY_HOST_PTR,
            LIST_SIZE * sizeof(int), &h_StatesTab[0], &ret);
    cl_mem d_input = clCreateBuffer(context,  CL_MEM_COPY_HOST_PTR,
            inputL * sizeof(int), &h_input[0], &ret);
    cl_mem d_OutputStates = clCreateBuffer(context,  CL_MEM_COPY_HOST_PTR,
            States_Number * sizeof(int), &h_outputStates[0], &ret);

    cl_mem d_C = clCreateBuffer(context,  CL_MEM_COPY_HOST_PTR, inputL * sizeof(int), &C[0], &ret);
  printf("after create buffer\r\n");
//************************************************//

h_outputStates[0]= 1024;


//**************************************************//
    // Copy the lists d_statesTab and d_input to their respective memory buffers
     // ret = clEnqueueWriteBuffer(command_queue, d_StatesTab, CL_TRUE, 0, LIST_SIZE * sizeof(int), h_StatesTab, 0, NULL, NULL);
   // ret = clEnqueueWriteBuffer(command_queue, d_input, CL_TRUE, 0, inputL * sizeof(int), h_input, 0, NULL, NULL);
    //  ret = clEnqueueWriteBuffer(command_queue, d_OutputStates, CL_TRUE, 0, States_Number * sizeof(int), h_outputStates, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1,  (const char **)&source_str, (const size_t *)&source_size, &ret);




    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "CheckMatch", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_input);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_StatesTab);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_C);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&d_OutputStates);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = 16  ; // Process the entire lists
    size_t local_item_size = 1; // Process in groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
            &global_item_size, &local_item_size, 0, NULL, NULL);

    ret = clEnqueueReadBuffer(command_queue, d_C, CL_TRUE, 0, inputL * sizeof(int), C, 0, NULL, NULL);
    int endI = 0;
	for (endI = 0; endI != 30; ++endI) {
	printf("%d \n",C[endI]);
    }


    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(d_input);
    ret = clReleaseMemObject(d_StatesTab);
    ret = clReleaseMemObject(d_C);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(h_StatesTab);
    free(h_input);
    free(C);
    free (h_outputStates);

    return 0;

}
