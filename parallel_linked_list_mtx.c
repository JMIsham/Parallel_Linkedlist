/* Compile:  gcc -g -Wall -o parallel_linked_list_mtx parallel_linked_list_mtx.c -pthread
 *           
 * Run:      ./parallel_linked_list_mtx n m mMember mInsert mDelete 
 * IndexNumber : 140236P  
 */      
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
 
struct list_node_s
{
    int data;
    struct list_node_s* next;
};
 
const int MAX_THREADS = 1024;
 
long thread_count;
pthread_mutex_t mutex;
struct list_node_s* head = NULL;    
 
int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_Member=0;
int count_Insert=0;
int count_Delete=0;
 
int Member( int value, struct  list_node_s* head_p );
int Insert(int value, struct list_node_s** head_pp);
int Delete (int value, struct list_node_s** head_pp);
int PrintList( struct  list_node_s* head_p ); 
void* Action(void* rank);
 
 
/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);
 
/* Main function */
int main(int argc, char* argv[])
{
    int i=0;
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;
    double start, finish, elapsed;
     
    /* read command line arguments */
    Get_args(argc, argv); 
      
    /* initially populating the link list */
    for(;i<n;i++)
    {   
        int r = rand()%65536;
        if(!Insert(r,&head))
        {
            i--;
        }
    }
     
     
    //printf("%f\n",mMember);
    //printf("%f\n",mInsert);
         
     
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));  
     
    start = clock();
    pthread_mutex_init(&mutex, NULL);
     
    for (thread = 0; thread < thread_count; thread++)  
    {
        pthread_create(&thread_handles[thread], NULL,Action , (void*)thread);  
    }
     
    for (thread = 0; thread < thread_count; thread++) 
    {
        pthread_join(thread_handles[thread], NULL); 
    }
     
    pthread_mutex_destroy(&mutex);
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;
     
    printf("Elapsed time = %e seconds\n", elapsed);
     
    //printf("%.10f,\n", elapsed);
    //printf("Member operation count = %d\n",count_Member);
    //printf("Insert operation count = %d\n",count_Insert);
    //printf("Delete operation count = %d\n",count_Delete);
    //PrintList(head);
    return 0;
}/*main*/  
 
/*------------------------------------------------------------------
 * Function:       Action 
 * Purpose:        Compleetea the link list oparations by the thread running this 
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count, mMember, mInsert, mDelete
 * Global in/out:  count_Member, count_Insert, count_Delete 
 */
void* Action(void* rank) 
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i < my_m; i++ )
    {
 
        float prob = (rand()%10000/10000.0);
        //printf("%f\n",prob);
     
     
        int r = rand()%65536;
        if(prob<mMember)
        {
            pthread_mutex_lock(&mutex);
            Member(r,head);
            count_Member++;
            pthread_mutex_unlock(&mutex);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_mutex_lock(&mutex);
            Insert(r,&head);
            count_Insert++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {           
            pthread_mutex_lock(&mutex);
            Delete(r,&head);
            count_Delete++;
            pthread_mutex_unlock(&mutex);
        }   
    }  
 
   return NULL;
}  /* Thread_sum */
 
/*------------------------------------------------------------------
 * Function:       Member
 * Purpose:        Check if the given values is in the link list
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:   
 * Return val: Return 1 if value exist otherwise 0
 */
int Member( int value, struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;
     
    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }
 
    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}/* Member */
 
/*------------------------------------------------------------------
 * Function:       Insert
 * Purpose:        Add new values in to link list
 * In arg:         value, head_p
 * Globals in:  
 * Global in/out:  
 * Return val: Return 1 if value successfully add to the list otherwise 0
 */
int Insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;          
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;
 
    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
     
    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));        
        temp_p->data = value;
        temp_p->next = curr_p;
         
        if(pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;
  
    }
    else
    {
        return 0;
    }
}   /*Insert*/
 
 
/*------------------------------------------------------------------
 * Function:       Delete
 * Purpose:        remove values from the link list 
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:  
 * Return val: Return 1 if value successfully remove from the list otherwise 0
 */
int Delete (int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
     
    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }   
     
    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
         
    }
    else
    {
        return 0;
    }
 
}   /*Delete*/
 
 
/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n, m, mMember, mInsert, mDelete
 */
void Get_args(int argc, char* argv[]) {
    if (argc != 7)
    {
        Usage(argv[0]);
    }
    thread_count = strtol(argv[1], NULL, 10);  
    if (thread_count <= 0 || thread_count > MAX_THREADS)
    {
        Usage(argv[0]);
    }
    
    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);
     
    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);
     
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);
    
}  /* Get_args */
 
/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n> <m> <mMember> <mInsert> <mDelete>\n", prog_name);
   fprintf(stderr,"n is the number of initial unique values in the Link List.\n");
   fprintf(stderr,"m is number of random Member, Insert, and Delete operations on the link list.\n");
   fprintf(stderr,"mMember is the fractions of operations of Member operation.\n");
   fprintf(stderr,"mInsert is the fractions of operations of Insert operation.\n");
   fprintf(stderr,"mDelete is the fractions of operations of Delete operation.\n");
              
   exit(0);
}  /* Usage */
 
/*------------------------------------------------------------------
 * Function:       PrintList
 * Purpose:        Add in the terms computed by the thread running this 
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:   
 * Return val: Estimate of pi using n terms of Maclaurin series
 */
int PrintList( struct  list_node_s* head_p ) 
{
    struct list_node_s* curr_p = head_p;
     
    while(curr_p != NULL)
    {
        printf("%d ",curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}