/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;	
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();		/*큐 생성*/
int freeQueue(QueueType *cQ);	/*큐 해제*/
int isEmpty(QueueType *cQ);		/*큐가 비어있는지 확인*/
int isFull(QueueType *cQ);		/*큐가 가득 차있는지 확인*/
void enQueue(QueueType *cQ, element item);	/*큐에 데이터 삽입*/
void deQueue(QueueType *cQ, element* item);	/*큐에서 데이터 삭제*/
void printQ(QueueType *cQ);		/*큐 출력*/
void debugQ(QueueType *cQ);		/*디버깅*/
element getElement();			/*데이터 입력*/	

int main(void)
{
	QueueType *cQ = createQueue();
	element data;
	char command;

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();	/*데이터 입력*/
			enQueue(cQ, data);		
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');	/* q나 Q가 입력되면 종료 */

	printf("[----- [Lee Tae Jeong] [2023041062] -----]");

	return 1;


}

QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));	/*메모리 할당*/
	cQ->front = 0;									/*front 초기화*/
	cQ->rear = 0;									/*rear 초기화*/
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;		/*큐가 비어있으면 1을 반환*/
    free(cQ);						/*메모리 해제*/	
    return 1;	
}

element getElement()
{
	element item;	
	printf("Input element = ");
	scanf(" %c", &item);
	return item;					/*입력받은 데이터 반환*/
}


/* complete the function */
int isEmpty(QueueType *cQ)
{
    if (cQ->front == cQ->rear) {	/*front와 rear가 같으면 큐가 비어있음*/ 
        return 1; // Queue is empty
    } else {
        return 0; // Queue is not empty
    }

    return 0;
}

/* complete the function */
int isFull(QueueType *cQ)
{
    if ((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front) {	/*rear의 다음 값이 front와 같으면 큐가 가득 차있음*/
        return 1; // Queue is full
    } else {
        return 0; // Queue is not full
    }

   return 0;
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{
    if (isFull(cQ)) {	/*큐가 가득 차있으면*/
        printf("Queue is full. Cannot enqueue.\n");
    } else {			/*큐에 데이터 삽입*/
        cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;	/*rear의 다음 위치로 이동*/
        cQ->queue[cQ->rear] = item;					/*데이터 삽입*/
    }

}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
    if (isEmpty(cQ)) {	/*큐가 비어있으면*/
        printf("Queue is empty. Cannot dequeue.\n");
    } else {			/*큐에서 데이터 삭제*/
        cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;	/*front의 다음 위치로 이동*/
        *item = cQ->queue[cQ->front];					/*삭제된 데이터 반환*/
    }
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;		/*front의 다음 위치*/
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;		/*rear의 다음 위치*/

	printf("Circular Queue : [");

	i = first;
	while(i != last){	/*front부터 rear까지 출력*/
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {	/*front일 경우*/
			printf("  [%d] = front\n", i);	
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);	/*front가 아닐 경우*/

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	
}


