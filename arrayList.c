#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LIST_INIT_SIZE 2
#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

int listMaxSize;
int * list;
int length;
int flag = 1;
int insertItemAt(int pos, int item);
void shrink();
void deleteLast();
void clear();
void deleteAll();

int getLength()
{
    return length;
}
void initializeList()
{
	listMaxSize = LIST_INIT_SIZE;
	list = (int*)malloc(sizeof(int)*listMaxSize) ;
	length = 0 ;
}

int searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

int insertItem(int newitem)
{
	int * tempList ;
	if(flag ==1)
	{
	    if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};

	list[length] = newitem ; //store new item
	length++ ;
	}
	else if(flag ==0)
	{
	    flag =1;
	    initializeList();
	    insertItem( newitem);
	}
	return SUCCESS_VALUE ;
}


int deleteItemAt(int position) //version 2, do not preserve order of items
{
	if ( position >= length ) return NULL_VALUE;
	list[position] = list[length-1] ;
	length-- ;
	shrink();
	return SUCCESS_VALUE ;
}


int deleteItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return NULL_VALUE;
	deleteItemAt(position) ;
	//shrink();
	return SUCCESS_VALUE ;
}

void printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);

}

int isDuplicate(int item)
{

    int i, count = 0;
    for(i = 0;i<length;i++)
    {
        if(item == list[i])
        {
            count++;

        }

    }

    if(count>1) return 1;
    else return 0;

}

void removeAllMinusOne()
{
    int check;
    check = searchItem(-1);
    if(check == NULL_VALUE)
    {
        printf("no minus one found\n");
    }
    else{

        while(searchItem(-1) != NULL_VALUE)
        {
            deleteItem(-1);
        }
        printf("no minus one remains\n");
        printList();
    }

}
/*
int main(void)
{

    initializeList();
    while(1)
    {

        printf("1. Insert new item. 2. Delete item at. 3. Delete item.\n");
        printf("4. (Add from homework). 5. Print. 6. exit. 7.deleteralloccur\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertItem(item);
        }
        else if(ch==2)
        {
            int pos;
            scanf("%d", &pos);
            deleteItemAt(pos);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            deleteItem(item);
        }
        else if(ch == 4)
        {
            removeAllMinusOne();


            //printf("current length = %d\n",getLength());
            //printf("\n%d\n",getLength());


            //shrink();
            //deleteLast();
            //clear();
            //deleteAll();
            //smain();
        }
        else if(ch==5)
        {
            printList();
        }
        else if(ch==6)
        {
            break;
        }
        else if(ch == 7)
        {
            int itemTodelete;
            printf("enter item to delete " );
            scanf("%d",&itemTodelete);
            deleteAllOccurrance(itemTodelete);
        }
    }

}*/

void deleteAllOccurrance(int item)
{
    int i;
    for(i = 0;i<length;i++)
    {
        if(item == list[i])
        {
            list[i] = -1;

        }

    }

}


int insertItemAt(int pos, int item)
{
   if(flag ==1)
   {
        if(pos>= length) return NULL_VALUE;
    int * tempList ;
	if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};



    list[length] = list[pos];
    list[pos] = item;
    length++;
   }
   else if(flag ==0)
   {
       flag =1;
	    initializeList();
	    insertItemAt( pos, item);
   }
    return SUCCESS_VALUE;
}

void shrink()
{
    if(length == listMaxSize/2)
    {
        if(listMaxSize>LIST_INIT_SIZE)
        {
            int * tempList ;


		listMaxSize = listMaxSize/2 ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ;
        }
        free(list) ;
        list = tempList ;
        }
    }
}

void deleteLast()
{
    length--;
    shrink();
    return;
}

void clear()
{
    free(list);
    length =0;
    listMaxSize =0;
    flag = 0;
}

void deleteAll()
{
    length =0;

            listMaxSize = LIST_INIT_SIZE;
        list = (int*) realloc(list,LIST_INIT_SIZE*sizeof(int));


}
