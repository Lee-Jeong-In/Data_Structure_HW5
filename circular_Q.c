 /* circularQ.c */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();


int main(void)
{
	QueueType *cQ = createQueue();              // 큐 생성
	element data;                               // 입력받을 데이터를 임시로 저장하는 곳

	char command;                               // 사용자가 원하는 기능을 받는 곳

	printf("[----- [이정인] [2019038015] -----]");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);                 // 입력받음

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
            freeQueue(cQ);
            printf("Free Queue...\n");
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()                            // 큐를 만들어 주는 함수
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));    // 큐 시작을 잡아줄 포인터 변수
	cQ->front = 0;                                  // 큐의 시작을 잡아줄 front index
	cQ->rear = 0;                                   // 큐의 끝을 잡아줄 front index
                                                    // 큐 생성시 큐는 비어있으니 front와 rear는 값은 곳을 가르킨다.
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) 
        return 1;
    free(cQ);

    return 1;
}

element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear)                      // Queue가 비워있으면 rear 과 front는 같다.
    {
		printf("Circular Queue is empty!");
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ)
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) 	// Queue가 꽉찼으면 rear다음엔 무조건 front index가 있다.
    {
		printf(" Circular Queue is full!");
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) 
        return;
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;     // 먼저 rear index를 올려놓고 값을 넣어준다.
                                                        // rear index에 값이 들어가고, front index는 기준은 잡아준다.
		cQ->queue[cQ->rear] = item;
	}
}

void deQueue(QueueType *cQ)              				// dequeue 과정 +  element* item 은 필요없으니 삭제
{
	if(isEmpty(cQ)) 
        return;
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;     // dequeue, 출력을 하고 index의 값을 올려야 한다.
		cQ->queue[cQ->front]=NULL;						// 값을 비워준다. 
		return;
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;             // 첫번째 값은 front index 다음에 존재 한다.
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;               // 시작과 끝을 나타내는 front index를 가르킨다.

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG---\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front)                                  // 기준은 잡는 front queue 자리
        {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);           // 나머지 queue에 있는 값을 출력한다.

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 현재 index의 위치를 출력한다.
}

