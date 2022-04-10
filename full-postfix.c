/* postfix.c */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( : 왼쪽 괄호 */
	rparen = 9,  /* ) : 오른쪽 괄호*/
	times = 7,   /* * : 곱셈 */
	divide = 6,  /* / : 나눗셈 */
	plus = 5,    /* + : 덧셈 */
	minus = 4,   /* - : 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];			// 바꿀 수식을 전역변수로 저장
char postfixExp[MAX_EXPRESSION_SIZE];		// 바뀐 수식을 전역변수에 저장
char postfixStack[MAX_STACK_SIZE];			// 바뀐 수식을 Stack에 저장되는 방식
int evalStack[MAX_STACK_SIZE];

int postfixStackTop = -1;					// postfixExp의 맨 윗부분을 표시
int evalStackTop = -1;						// evalStackTop의 맨 윗부분을 표시

int evalResult = 0;							// 연산결과값 초기화

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

		printf("[----- [이정인] [2019038015] -----]");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;

}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()								// 꺼내올때
{
	char x;
    if(postfixStackTop == -1)					// 더 꺼내올께 없다면
        return '\0';
    else 
    	x = postfixStack[postfixStackTop--];	// 배열의 앞순서로 1칸 이동
		
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()								// Stack에서 맨위에 들어간 데이터를 꺼낸다.
{
    if(evalStackTop == -1)					// 꺼낼것이 없을 때
        return -1;
    else									// 까낼것이 있을 때
        return evalStack[evalStackTop--];	
}


void getInfix()								// 사용자에게 수식을 입력받아 배열에 저장한다.
{
    printf("Type the expression >>> ");		
    scanf("%s",infixExp);
}

precedence getToken(char symbol)			// 사용자가 입력한 문자가 어떤건지 의미부여
											// typedef enum를 활용하여 우선순위도 같이 정해준다.
{
	switch(symbol) 							// 연산자와 피연산자를 typedef를 사용하여 구별해준다.
	{
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/*문자하나를 전달받아, postfixExp에 추가 */

void charCat(char* c)				// 받아온 문자를 비교한다
{
	if (postfixExp == NULL)			// postfix의 끝
		strncpy(postfixExp, c, 1);	// 끝을 복사해준다.
	else
		strncat(postfixExp, c, 1);	// 받아온 문자를 postfixExp뒤에 붙인다.
}

/*
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */

void toPostfix()
{
	char *exp = infixExp;						// 사용자에게 방은 수식을 저장해 놓은 배열의 시작을 exp에 넣는다.
	char x; 

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')							// 배열의 마지막은 NULL이기 때문에 끝까지 읽는다.
	{
		if(getPriority(*exp) == operand)
		{
			x = *exp;
        	charCat(&x);
		}
        else if(getPriority(*exp) == lparen) 	// '('를 만났을 때
		{
        	postfixPush(*exp);
        }
        else if(getPriority(*exp) == rparen)	// ')'를 만났을 때
        {
        	while((x = postfixPop()) != '(') 
			{
        		charCat(&x);					// 혹시 '('이 또 들어왔을 때
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
            	x = postfixPop();
            	charCat(&x);
            }
            postfixPush(*exp);
        }
        exp++;									// 다음 인덱스 가르킴(즉, 반복한다)
	}

    while(postfixStackTop != -1)				// 끝까지 안오면 계속 붙인다.	
    {
    	x = postfixPop();
    	charCat(&x);
    }
	printf("Complete to Postfix!\n");
}

void debug()
{
	printf("\n---DEBUG---\n");
	printf("infixExp =  %s\n", infixExp);		// 제대로 작동하는지 확인한다.
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");					// postfix에 쌓인 순서도 보여준다.
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()									// 배열과 변수들을 초기의 상태로 만들어 준다.
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;

	printf("Complete to RESET!\n");
}

void evaluation()
{		
	int opr1, opr2, i;										// 임시로 저장할 변수	
	char symbol;
	int length = strlen(postfixExp);						// postfix가 들어있는 배열의 길이
	evalStackTop = -1;										// evalStack의 가장 윗부분을 표시한다.

	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) 					// postfixExp에서 가저온 기호(symbaol)이 피연산자 일때
		{
			evalPush(symbol - '0');							// evalStack에 쌓아넣는다.
		}
		else 												// 연산자 일때,
		{
			opr2 = evalPop();								// 가장 최근에 들어갔던 피연산자 2개를 Stack에서 가져온다.
			opr1 = evalPop();
			switch(getToken(symbol)) 						// 연산자가 어떤 연산자 인지 구별하고 각각의 기호에 맞는 연산을 해 준다.
			{
			case plus: evalPush(opr1 + opr2); break;		
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}

	printf("Complete Evaluation!.\n");
	evalResult = evalPop();									// 위의 과정이 다끝나고 연산된 마지막 데이터를 가져온다.
}
