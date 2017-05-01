#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXstudentB 5

int studentBArray[MAXstudentB];
int frontstudentB = 0;
int rearstudentB = -1;
int itemCountstudentB = 0;

int peekstudentB() {
   return studentBArray[frontstudentB];
}

bool isEmptystudentB() {
   return itemCountstudentB == 0;
}

bool isFullstudentB() {
   return itemCountstudentB == MAXstudentB;
}

int sizestudentB() {
   return itemCountstudentB;
}

int insertstudentB(int data) {

   if(!isFullstudentB()) {

      if(rearstudentB == MAXstudentB-1) {
         rearstudentB = -1;
      }

      studentBArray[++rearstudentB] = data;
      itemCountstudentB++;
	return 1;
   }
   else
   {
        printf("queue full\n");
	return 0;
   }
}

int removeDatastudentB() {

    if(!isEmptystudentB()){
       int data = studentBArray[frontstudentB++];

       if(frontstudentB == MAXstudentB) {
          frontstudentB = 0;
       }

       itemCountstudentB--;
       return data;
   }
   else
   {
        printf("queue empty\n");
        return -1;
   }
}

void displaystudentB()
{
   int i;

    if (isEmptystudentB())

        printf("Queue is empty \n");

    else

    {

        printf("Queue is : \n");

        if(frontstudentB<=rearstudentB){

            for (i = frontstudentB; i <= rearstudentB; i++)

                printf("%d ", studentBArray[i]);

            printf("\n");
        }
        else
        {
           for (i = frontstudentB; i < MAXstudentB; i++)

                printf("%d ", studentBArray[i]);
            for (i = 0; i <= rearstudentB; i++)

                printf("%d ", studentBArray[i]);
            printf("\n");
        }

    }
}

/*
int main() {

   insertstudentB(3);
   insertstudentB(5);
   insertstudentB(9);
   insertstudentB(1);
   insertstudentB(12);

   // front : 0
   // rear  : 4
   // ------------------
   // index : 0 1 2 3 4
   // ------------------
   // queue : 3 5 9 1 12
   insertstudentB(15);

   // front : 0
   // rear  : 5
   // ---------------------
   // index : 0 1 2 3 4  5
   // ---------------------
   // queue : 3 5 9 1 12 15

   if(isFullstudentB()){
      printf("Queue is full!\n");
   }

   // remove one item
   int num = removeDatastudentB();

   printf("Element removed: %d\n",num);
   // front : 1
   // rear  : 5
   // -------------------
   // index : 1 2 3 4  5
   // -------------------
   // queue : 5 9 1 12 15

   // insert more items
   insertstudentB(16);

   // front : 1
   // rear  : -1
   // ----------------------
   // index : 0  1 2 3 4  5
   // ----------------------
   // queue : 16 5 9 1 12 15

   // As queue is full, elements will not be inserted.
   insertstudentB(17);
   insertstudentB(18);

   // ----------------------
   // index : 0  1 2 3 4  5
   // ----------------------
   // queue : 16 5 9 1 12 15
   printf("Element at front: %d\n",peekstudentB());

   printf("----------------------\n");
   printf("index : 5 4 3 2  1  0\n");
   printf("----------------------\n");
   printf("Queue:  ");


   displaystudentB();
}
*/


