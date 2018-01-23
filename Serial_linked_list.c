/* Compile:  gcc -g -Wall -o Serial_linked_list Serial_linked_list.c -pthread
 *           
 * Run:      ./Serial_linked_list n m mMember mInsert mDelete 
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

struct list_node_s* head = NULL;    
 
int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_Member=0;
int count_Insert=0;
int count_Delete=0;
int cMember ;
int cInsert ;
int cDelete ;

 
int Member( int value, struct  list_node_s* head_p );
int Insert(int value, struct list_node_s** head_pp);
int Delete (int value, struct list_node_s** head_pp);
void* Action(void* rank);
void Usage(char* prog_name);
 
int main(int argc, char* argv[])
{
    int i=0;
    long       thread;  
    pthread_t* thread_handles;
    double start, finish, elapsed;
     
     if (argc != 6)
    {
        Usage(argv[0]);
    }
    
    n = (int) strtol(argv[1], (char **)NULL, 10);
    m = (int) strtol(argv[2], (char **)NULL, 10);
     
    mMember = (float) atof(argv[3]);
    mInsert = (float) atof(argv[4]);
    mDelete = (float) atof(argv[5]);
    cMember = mMember*m;
    cInsert = mInsert*m;
    cDelete = mDelete*m;
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);

      
    for(;i<n;i++)
    {   
        int r = rand()%65536;
        if(!Insert(r,&head))
        {
            i--;
        }
    }
    double factor, my_sum = 0.0;
    long long my_m = m;
    for( i=0; i < my_m; i++ )
    {
 
        float prob = (rand()%10000/10000.0);     
     
        int r = rand()%65536;
        if(prob<mMember && count_Member!=cMember )
        {
            Member(r,head);
            count_Member++;
        }
        else if(prob < mMember + mInsert && count_Insert!=cInsert)
        {
            Insert(r,&head);
            count_Insert++;
        }
        else if(prob <= mMember + mInsert + mDelete && count_Delete!=cDelete)
        {           
            Delete(r,&head);
            count_Delete++;
        }else{
            i--;
        }   

    }  
      
    
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;
     
    printf("Elapsed time = %e seconds\n", elapsed);
     
    return 0;
} /*main*/  
 


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
 
 

 
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n> <m> <mMember> <mInsert> <mDelete>\n", prog_name);
   fprintf(stderr,"n : Number of Unique Values.\n");
   fprintf(stderr,"m : Total Number of Action.\n");
   fprintf(stderr,"mMember : Fraction of Memeber Operations.\n");
   fprintf(stderr,"mInsert : Fraction of Insert Operations.\n");
   fprintf(stderr,"mDelete : Fraction of Delete Operations.\n");
              
   exit(0);
}  /* Usage */
 