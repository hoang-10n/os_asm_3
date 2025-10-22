# Background
The classic and famous merge sort algorithm is an illustration of the divide-and-conquer idea. Parallel merge sort takes this idea to the next level. The basic idea of parallel merge sort is demonstrated below in this picture, using an example of an array whose size is 10,000. Although in this assignment we do not store any tree in our program, the idea itself sounds like we are dealing with a binary tree. We keep dividing the array and create new threads to handle the divided smaller arrays, which we will call the subarrays. In binary tree terminology, there is a term called level. The node at the very top is at level 0. Its two children are located at level 1, and its four grandchildren are located at level 2, and so on.

![example.png](example.png)

The idea of parallel merge sort is, starting from level 0, we create two new threads, each of which handles half of the array (of the current node). We repeat such a process so as to extend the binary tree to the next level. As such, we will soon get to level 1, level 2, level 3, level 4, etc. In this assignment, we allow users to specify how many levels they want us to go, and the starter code therefore defines a global variable called cutoff, whose value is passed to the program by the user from the command line. The above picture shows an example of when the cutoff is specified as 2. Continuing with the same example, the following picture shows what your program flow should look like.

![flow.png](flow.png)

Several functions are used in the above picture. Some of them are API functions you can call, others are the functions you need to implement. Read the next few sections of this assignment to understand more details about these functions.

# Starter Code
The starter code looks like this:

```bash
[comp2002-mergesort]$ ls
Makefile  mergesort.c`  mergesort.h  README.template  test-mergesort.c
```

You will be modifying the `mergesort.c` file. You should not modify the mergesort.h file.  You will also need to create a README file, which can be based on the README.template file. 

A testing program is provided in the starter code, called `test-mergesort.c`. To run the test program, you just type make and run:

```bash
[comp2002-mergesort]$ ./test-mergesort 
Usage: ./test-mergesort <input size> <cutoff level> <seed> 
[comp2002-mergesort]$ ./test-mergesort 100 1 1234
Sorting 100 elements took 0.00 seconds.
```

As you can see, this testing program takes 3 parameters. The first parameter is the size of the array, if you type 100, then the testing program will create an array with 100 randomly-generated elements. The second parameter is the cutoff value. The third parameter is a random seed, you can provide any number, with a different seed number, the testing program will create a different array. In the testing program, the size of the array is denoted as n, and the testing program will call your functions like this:

```c
    struct argument *arg=buildArgs(0, n-1, 0);
    parallel_mergesort(arg);
```

In the next section, you will find out what these two functions should do.

# Specification
The testing program will show timing results for the single-thread merge sort (also known as serial merge sort or sequential merge sort) when the number of levels is 0, and show timing results for the parallel merge sort when the number of levels is larger than 0. Compared to serial merge sort, your parallel merge sort must get a speedup of at least 2 on Gradescope. Use n = 100,000,000 elements for your testing.

You are required to implement the following 4 functions:

```c
void mergesort(int left, int right);
```
This function does the merge sort, with one single thread. You should write this `mergesort()` function as a recursive function.

```c
void merge(int leftstart, int leftend, int rightstart, int rightend);
```
In a typical merge sort algorithm, the `mergesort()` function will eventually call `merge()`, which attempts to merge two already-sorted sub-arrays.

```c
void * parallel_mergesort(void *arg);
```
This `parallel_mergesort()` function calls `mergesort()` as its base case. You are highly recommended to write this `parallel_mergesort()` recursively, using the pthread library functions. Note: the term recursion in the context of pthread is slightly different from the term recursion you usually see. In the above `mergesort()` function, you call `mergesort()`, which is the typical recursion scenario you see in other recursive programs. In your `parallel_mergesort()`, you do not call `parallel_mergesort()` directly, instead, you call `pthread_create()`, which will then call `parallel_mergesort()`. In other words, in `mergesort()`, you call `mergesort()` directly, in `parallel_mergesort()`, you call `parallel_mergesort()` indirectly.

`pthread_create()` will be explained in the next section.

```c
struct argument * buildArgs(int left, int right, int level);
```
The only reason we need this function is that `pthread_create()` specifies that its `start_routine()` only allows one void * type pointer, which means `parallel_mergesort()` will only accept one void * type pointer, yet the information we want to pass is more than just one variable. Thus, we can work around this by creating a struct variable and passing its address into `parallel_mergesort()`. That is the purpose of this buildArgs function, which basically prepares the argument for `parallel_mergesort()`.

The definition of struct argument, as well as the meaning of each of the parameters to `buildArgs()`, will be described later in this README.

# APIs
I used the following APIs.
```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
```
According to its man page, the `pthread_create()` function creates "a new thread in the calling process. The new thread starts execution by invoking `start_routine()`; arg is passed as the sole argument of `start_routine()`". You can create a struct pthread_t instance and pass its address into `pthread_create()`, as its first parameter, which represents the thread you are going to create. The second parameter attr describes some attributes of this thread, and for this assignment, you can just use NULL, which indicates the thread is created with default attributes. The third parameter, `start_routine()`, is the function you want the newly created thread to run. This function must take one and only one parameter, which must be a void * type pointer. This function returns a void * type pointer. The fourth parameter is the argument, which is also known as the parameter of this function - the one represented by the third parameter. For obvious reasons, arg is a void * type pointer.
```c
int pthread_join(pthread_t thread, void **retval);
```
According to its man page, the `pthread_join()` function "waits for the thread specified by thread to terminate. If that thread has already terminated, then `pthread_join()` returns immediately". After calling `pthread_create()` to create threads, you may want to wait for these threads to complete. And that is why you call `pthread_join()`. The first parameter of `pthread_join()`, which is thread, is also the first parameter of `pthread_create()`, however, `pthread_create()` takes its address as the parameter, whereas `pthread_join()` takes the variable itself as the first parameter. The second parameter, retval, indicates the exit status of the thread (you are waiting for). In this assignment, you can set this retval to NULL, because in this assignment, we do not really care about the exit status of each thread.
```c
void *memcpy(void *dest, const void *src, size_t n);
```
You may need to call `memcpy()` in your `merge()` function, so as to copy data from one array to another array.
```c
void *malloc(size_t size);
void free(void *ptr);
```
You may need to call `malloc()` to allocate memory in your `buildArgs()` function for the argument you are going to build. You can then free such memory chunks in your `parallel_mergesort()`.

# Global Variables and Pre-defined Data Structures
The starter code defines the following global variables in mergesort.h. Once again, do not modify mergesort.h.

```c
int *A;
```
A represents the array you are going to sort. Each time you run the test program, A will point to a randomly-generated array. How do you know the size of this array? The size of this array is specified by the user who runs the test program. The user provides a command-line argument to specify the size, which is called n in the test program, and then n-1 will be passed to your `buildArgs()` function.

```c
int *B;
```
A typical merge sort algorithm requires some extra storage, which is an array, whose size is the same as the original array - the array you want to sort. Watch this video to understand why such an extra array is needed:

<video width="1920" height="1080" controls>
  <source src="https://www.youtube.com/watch?v=KF2j-9iSf4Q&t=372s" type="video/mp4">
</video>

We define both array A and array B as a global array, so that you do not need to pass them as a parameter to your functions. Instead, you can just use them in any of your functions.

```c
int cutoff;
```
This global variable is described in the Background section of this assignment description. The word cutoff means that when the number of levels reaches this cutoff value, we cut off new thread generation.
```c
struct argument {
    int left;
    int right;
    int level;
};
```
This is the argument we plan to pass to `parallel_mergesort()`. We call `buildArgs()` to build an instance of struct argument. In this struct, the field left represents the leftmost index of the array, and the right field represents the rightmost index of the array. The field level represents the current level. To explain this better, let us once again use the following two lines as an example:
```c
    struct argument *arg=buildArgs(0, n-1, 0);
    parallel_mergesort(arg);
```
These two lines will be called by the testing program. The testing program's `main()` function represents the main thread, which, as the above picture shows, is corresponding to the node at level 0 of the binary tree. If we compare the prototype of `buildArgs()`, which is shown below:
```c
struct argument * buildArgs(int left, int right, int level);
```
Because at level 0, the main thread handles the entire array, whose size is n, therefore the leftmost index is 0, and the rightmost index is n-1. Therefore, we pass (0,n-1,0) to the function `buildArgs()`, which will use them to construct a struct argument variable and return its address, which will then be passed to `parallel_mergesort()`.

Expected Results
If you run test-mergesort before implementing anything, you will get:
```c
[comp2002-mergesort]$ ./test-mergesort 100 1 1234
./test-mergesort: sorting failed!!!!
[comp2002-mergesort]$ ./test-mergesort 100000000 1 1234
./test-mergesort: sorting failed!!!!
```
The sorting fails because the starter code does not include any sorting code. Once your implementation is complete and you run test-mergesort, you are expected to see such a pattern:
```bash
[comp2002-mergesort]$ ./test-mergesort 100000000 0 1234
Sorting 100000000 elements took 28.87 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 1 1234
Sorting 100000000 elements took 14.95 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 2 1234
Sorting 100000000 elements took 7.90 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 3 1234
Sorting 100000000 elements took 4.59 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 4 1234
Sorting 100000000 elements took 3.25 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 5 1234
Sorting 100000000 elements took 2.40 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 6 1234
Sorting 100000000 elements took 2.25 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 7 1234
Sorting 100000000 elements took 2.27 seconds.
[comp2002-mergesort]$ ./test-mergesort 100000000 8 1234
Sorting 100000000 elements took 2.33 seconds.
```
As you can see, as we increase the number of levels, we can see a clear performance gain. Once again, the requirement is: compared to serial merge sort (which is when the number of levels is 0), your parallel merge sort (which is when the number of levels is larger than 0) must get a speedup of at least 2 on GadeScope. Use n = 100,000,000 elements for your testing.

You can also see that once the number of levels reaches 5, you will not get more performance gain. At that point, the number of threads is no longer the bottleneck of this program, and therefore increasing the number of levels, which will accordingly increase the number of threads, will not help much.