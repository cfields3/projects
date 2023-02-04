// Elapsed Real Time for input-5.txt: 0.726 seconds
// Type of GPU: nVidia RTX 2080 Super

/**
 * @file maxsum.cu
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 5, Problem 3. Calculates
 * the maximum sum for a contiguous sequence of numbers
 * over an inputted list using a certain number of workers
 * as threads on a GPU serving as a co-processor.
 */

#include <stdio.h>
#include <stdbool.h>
#include <cuda_runtime.h>
#include <unistd.h>

// Input sequence of values.
int *vList;

// Number of values on the list.
int vCount = 0;

// Capacity of the list of values.
int vCap = 0;

// General function to report a failure and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// Print out a usage message, then exit.
static void usage() {
  printf( "usage: maxsum [report]\n" );
  exit( 1 );
}

// Read the list of values.
__host__ void readList() {
  // Set up initial list and capacity.
  vCap = 5;
  vList = (int *) malloc( vCap * sizeof( int ) );

  // Keep reading as many values as we can.
  int v;
  while ( scanf( "%d", &v ) == 1 ) {
    // Grow the list if needed.
    if ( vCount >= vCap ) {
      vCap *= 2;
      vList = (int *) realloc( vList, vCap * sizeof( int ) );
    }

    // Store the latest value in the next array slot.
    vList[ vCount++ ] = v;
  }
}

__global__ void checkSum( int vCount, bool report, int *devList, int *devSums ) {
  int idx = blockDim.x * blockIdx.x + threadIdx.x;

  if ( idx < vCount ) {
    // Initialize maxSum smallest value possible
    int maxSum = INT_MIN;

    // Calculate the maximum sum for specified worker
    int sum = 0;
    for( int i = idx; i < vCount; i++ ) {
      sum += devList[ i ];
      if ( sum > maxSum )
        maxSum = sum;
    }

    // Add the maximum sum found to the results array
    devSums[idx] = maxSum;

    // Print out a report for the work, if requested
    if ( report )
      printf( "I'm thread %d. The maximum sum I found is %d.\n", idx, maxSum );
  }
}

int main( int argc, char *argv[] ) {
  if ( argc < 1 || argc > 2 )
    usage();

  // If there's an argument, it better be "report"
  bool report = false;
  if ( argc == 2 ) {
    if ( strcmp( argv[ 1 ], "report" ) != 0 )
      usage();
    report = true;
  }

  readList();
  
  // Allocate memory on the device and copy over the list.
  int *devList = NULL;
  cudaMalloc( (void **) &devList, vCount * sizeof(int) );

  // Copy the list over to the device.
  cudaMemcpy( devList, vList, vCount * sizeof(int), cudaMemcpyHostToDevice );

  // Allocate space on the device to hold the results.
  int *devSums = NULL;
  cudaMalloc( (void **) &devSums, vCount * sizeof(int) );

  // Block and grid dimensions.
  int threadsPerBlock = 100;
  // Round up for the number of blocks we need.
  int blocksPerGrid = ( vCount + threadsPerBlock - 1 ) / threadsPerBlock;

  // Run our kernel on these block/grid dimensions
  checkSum<<<blocksPerGrid, threadsPerBlock>>>( vCount, report, devList, devSums );
  if ( cudaGetLastError() != cudaSuccess )
    fail( "Failure in CUDA kernel execution." );

  // Copy results back to the host
  int *sums = (int *) malloc( vCount * sizeof(int) );
  cudaMemcpy( sums, devSums, vCount * sizeof(int), cudaMemcpyDeviceToHost );

  // Compare the local largest products
  int maxSum = INT_MIN;
  for ( int i = 0; i < vCount; i++ ) {
    if ( sums[ i ] > maxSum )
      maxSum = sums[ i ];
  }

  // Report the final largest product
  printf( "Maximum Sum: %d\n", maxSum );

  // Free memory on the device and the host.
  free( vList );
  free( sums );
  cudaFree( devList );
  cudaFree( devSums );

  cudaDeviceReset();

  return 0;
}
