# Parallel_Linkedlist

This is pThread implementation of linked list. this program supports 3 functions,
	1. Member : searching
	2. Insert : adding new nodes
	3. Delete : deleting existing value
The main function is written in shuch way to automate these actions and produce the time taken to cmplete the task

parallel_linked_list_rw.c
-------------------------

pthread_rwlock_t is been used in this implementation.
Compile : gcc -g -Wall -o parallel_linked_list_rw parallel_linked_list_rw.c -pthread
Run     : ./parallel_linked_list_rw <number of threads> <number of random nodes> <number of 		 
	      operations> <fraction of Membering> <fraction of Inserting> <fraction of Deleting          >
Output  : Time taken to complete the actions


parallel_linked_list_mtx.c
-------------------------

pthread_mutex_t is been used in this implementation.
Compile : gcc -g -Wall -o parallel_linked_list_mtx parallel_linked_list_mtx.c -pthread
Run     : ./parallel_linked_list_mtx <number of threads> <number of random nodes> <number of 
	      operations> <fraction of Membering> <fraction of Inserting> <fraction of 		   
	      Deleting>
Output  : Time taken to complete the actions
