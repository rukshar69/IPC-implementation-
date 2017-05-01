//main file to generate IPC
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue3.c"
#include "reqQueueB.c"
#include "arrayList.c"
#define TRUE 1
#define FALSE 0
pthread_t A;
pthread_t C;
pthread_t E;

pthread_t B;
pthread_t D;
pthread_t students[30];

int approvedFromACE[30];
int approvedByB[30];
int password[30];
int requestToD[30];

sem_t empty;
sem_t full;

sem_t emptyReqToB;
sem_t fullReqToB;

sem_t emptyReqToD;
sem_t fullReqToD;
int count = 0;

pthread_mutex_t lockReqToD;
pthread_mutex_t lock;
pthread_mutex_t lockReqToB;

int requestToB = -1;


void init_semaphore()
{
	sem_init(&empty,0,10);
	sem_init(&full,0,0);
	sem_init(&emptyReqToB,0,10);
	sem_init(&fullReqToB,0,0);
	sem_init(&emptyReqToD,0,10);
	sem_init(&fullReqToD,0,0);
	pthread_mutex_init(&lock,0);
    pthread_mutex_init(&lockReqToB,0);
        pthread_mutex_init(&lockReqToD,0);

	int i;
	initializeList();
	for(i  =0;i<30;i++) approvedFromACE[i] = 0; //student id list approved by a,c,e
	for(i  =0;i<30;i++) approvedByB[i] = 0; //approved by B which is checked by D before generating password
	//for(i  =0;i<30;i++) requestToD[i] = 0;
	for(i  =0;i<30;i++) password[i] = -1; //password for the students
}

void *studentThreadFunction(void *arg)
{
    int *id= (int*)arg;
	printf(" std id %d\n",*id);
	int i  = *id;

	//for(i=1;i<=10;i++)
	//{
	while(1){

            sem_wait(&empty);

            pthread_mutex_lock(&lock);

            printf("THREAD  STUDENT %d requesting to ACE\n\n",i);
			approvedFromACE[i] = insert(i);

			printf("student %d produced item %d\n",*id, i);
			printf("display from %d std:\n",i);
			display();



        	sleep(1);
            pthread_mutex_unlock(&lock);

            sem_post(&full);

            //start of B
			//sleep(1);
	    sem_wait(&emptyReqToB);

            pthread_mutex_lock(&lockReqToB);

            printf("THREAD  STUDENT %d requesting to B\n\n",i);
			//insertstudentB(i);
            if(requestToB == -1){
			printf("student %d sends request to B\n",*id);
			//printf("displaystudentB from %d std :\n",i);
			//displaystudentB();
            requestToB = i;
            }else{

                printf("%d can't make request to B\n",i);
                printf("Current request held by %d\n",requestToB);
                sleep(1);


            }

        	sleep(1);
            pthread_mutex_unlock(&lockReqToB);

            sem_post(&fullReqToB);


//req to D---------------

            sem_wait(&emptyReqToD);

            pthread_mutex_lock(&lockReqToD);

            printf("THREAD  STUDENT %d requesting to D\n\n",i);
			insertstudentB(i);

			printf("displaystudentB from %d std :\n",i);
			displaystudentB();


        	sleep(1);
            pthread_mutex_unlock(&lockReqToD);

            sem_post(&fullReqToD);


	}
}

//ACE thread function..............................

int generatePassword()
{
    int pass;
    pass=(rand()%30)*31*13;
    return pass;

}

void *threadFuntionForD(void *arg)
{

	while(1){

        sem_wait(&fullReqToD);

        pthread_mutex_lock(&lockReqToD);
        printf("THREAD D -------------------------\n");
        int item = removeDatastudentB();
            //printf("b4 popping\n");
            //displaystudentB();

            printf("D  popped item %d\n", item);
            //printf("display from D\n");
            //displaystudentB();
            if(approvedByB[item] == 1)
            {
                int pass;
                pass = generatePassword();
                password[item] = pass;
                printf("password to %d is %d\n",item,password[item]);
                approvedByB[item]= 0;
                //printf("now aproved by b for %d is %d\n",item,approvedByB[item]);
            }
            else{
                printf("no password for %d yet\n",item);
            }

        sleep(1);

        pthread_mutex_unlock(&lockReqToD);

        sem_post(&emptyReqToD);
	}
}

void *threadFuntionForB(void *arg)
{

	while(1)
	{

            sem_wait(&fullReqToB);

            pthread_mutex_lock(&lockReqToB);

            /*
            int item = removeDatastudentB();

            printf("B  popped item %d\n", item);
            printf("display from B\n");
            displaystudentB();*/
            printf("THREAD B----------------\n\n");
            if(requestToB == -1)
            {
                printf("no request made to B, value of reqToB %d\n",requestToB);
                sleep(1);
            }
            else
            {
                printf("request to B is %d\n",requestToB);

                int index;
                index = isDuplicate(requestToB);
                if(index == 1)
                {
                    printf("b4 duplicate:\n");
                    printList();
                    printf("%d is duplicate\n",requestToB);
                    deleteAllOccurrance(requestToB);
                    printf("after deleting from B\n");
                    printList();
                    printf("refreshed list without -1 from B\n");
                    removeAllMinusOne();
                }
                else
                {
                    int isInList ;
                    isInList = searchItem(requestToB);
                    if(isInList != NULL_VALUE){
                        int itemToSendToD;
                        itemToSendToD = list[isInList];
                        approvedByB[itemToSendToD] = 1; //will be made 0 when D accessed
                        printf("putting %d in D queue an deleting from list \n\n",itemToSendToD);
                        printf("aproved by B :index %d, value %d\n",itemToSendToD, approvedByB[itemToSendToD]);
                        //generate password



                        deleteItem(requestToB);
                        printf("after deleting from B\n");
                        printList();

                    }
                    else
                    {
                        printf("Not in ACE approved list\n");
                    }

                    requestToB = -1;
                    printf("after processing value of requestTOb %d\n",requestToB);
                }




            }


            sleep(1);

            pthread_mutex_unlock(&lockReqToB);

            sem_post(&emptyReqToB);
	}
}


void *threadFuntionForA(void *arg)
{

	int i;
	//for(i=1;i<=10;i++)
	//{
	while(1){
        //if(!allApproved()){
            sem_wait(&full);

            pthread_mutex_lock(&lock);

            printf("THREAD A----------------\n\n");
            int item = removeData();

            printf("A  popped item %d\n", item);
            printf("display from A\n");
            display();

            printf("Request to B list\n");
            insertItem(item);
            printList();


            sleep(1);

            pthread_mutex_unlock(&lock);

            sem_post(&empty);
		//}
		/*else {
            printf("all approved A\n");
            sleep(1);
            //pthread_exit(NULL);
		}*/
	}
}

void *threadFuntionForC(void *arg)
{
    int i;
	//for(i=1;i<=10;i++)
	//{
	while(1){
        //if(!allApproved()){
            sem_wait(&full);

            pthread_mutex_lock(&lock);

            printf("THREAD C----------------\n\n");
            int item = removeData();

            printf("C  popped item %d\n", item);
            printf("display from C\n");
            display();

            printf("Request to B list\n");
            insertItem(item);
            printList();
            sleep(1);

            pthread_mutex_unlock(&lock);

            sem_post(&empty);
		//}
		/*else {
            printf("all approved C\n");
            //pthread_exit(NULL);
            sleep(1);
		}*/
	}
}

void *threadFuntionForE(void *arg)
{
    int i;
	//for(i=1;i<=10;i++)
	//{
	while(1){
        //if(!allApproved()){
            sem_wait(&full);

            pthread_mutex_lock(&lock);

            printf("THREAD E----------------\n\n");
            int item = removeData();

            printf("E  popped item %d\n", item);
            printf("display from E\n");
            display();

            printf("Request to B list\n");
            insertItem(item);
            printList();
            sleep(1);
            pthread_mutex_unlock(&lock);

            sem_post(&empty);
		//}
		/*else {
            printf("all approved E\n");
            //pthread_exit(NULL);
            sleep(1);
		}*/
	}
}
//////////////////////////////////////////////////////


//////////////start threads///////////////////////////////////////////

void startStudentThreads()
{

    int i,check;

    for(i=0;i<30;i++){
    	int *id = (int*)malloc(sizeof(int));
    	*id=i;
    	check=pthread_create(&students[i],NULL,studentThreadFunction,(void *)id);
    	if(check){
    		printf("Error! from creating students threads %d\n",check);
    		exit(1);
    	}
    }

}


void startACEthreads()
{
    int check;void *status;

    check=pthread_create(&A,NULL,threadFuntionForA,NULL);
    if(check){
    	printf("Error! return code from pthread_create() is %d\n",check);
    	exit(1);
    }

    check=pthread_create(&C,NULL,threadFuntionForC,NULL);
    if(check){
    	printf("Error! return code from pthread_create() is %d\n",check);
    	exit(1);
    }

    check=pthread_create(&E,NULL,threadFuntionForE,NULL);
    if(check){
    	printf("Error! return code from pthread_create() is %d\n",check);
    	exit(1);
    }



}

void threadBDstart()
{
    int check;
    check=pthread_create(&B,NULL,threadFuntionForB,NULL);
    if(check){
    	printf("Error! return code from pthread_create() is %d\n",check);
    	exit(1);
    }

    check=pthread_create(&D,NULL,threadFuntionForD,NULL);
    if(check){
    	printf("Error! return code from pthread_create() is %d\n",check);
    	exit(1);
    }

}
//////////////////////////////////////////////////

///////////////////////join threads/////////////////////////

void studentThreadsJoin()
{

    int i,check;
    void *status;
    for(i=0;i<30;i++){
    	check=pthread_join(students[i],&status);
    	if(check){
    		printf("Error! from join %d\n",check);
    		exit(1);
    	}
    }

}


void joinACEthreads()
{

    void *status;
    pthread_join(A,&status);
    pthread_join(C,&status);
    pthread_join(E,&status);

}


void joinBDthreads()
{

    void *status;
    pthread_join(B,&status);
    pthread_join(D,&status);
    //pthread_join(E,&status);

}

////////////////////////////////////////////

int main()
{

    //pthread_join(A,status);
    int i;

	init_semaphore();


    startACEthreads();

    threadBDstart();

    startStudentThreads();


    studentThreadsJoin();

    //pthread_join(A,status);

    //while(1);
    joinACEthreads();

    joinBDthreads();





    return 0;
}
