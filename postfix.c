/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);	/*postfix에 넣어질 값을 받는 함수*/
char postfixPop();			/*postfix에서 변환되어 나올 함수*/
void evalPush(int x);		/*eval에 넣어질 값을 받는 함수*/
int evalPop();				/*eval에서 변환되어 나올 함수*/
void getInfix();			/*infix를 입력받는 함수*/
precedence getToken(char symbol);	/*우선순위를 받는 함수*/
precedence getPriority(char x);		/*우선순위를 받는 함수*/
void charCat(char* c);		/*문자를 받아 postfix에 넣는 함수*/
void toPostfix();			/*infix를 postfix로 변환하는 함수*/
void debug();				/*디버그를 위한 함수*/
void reset();				/*초기화를 위한 함수*/
void evaluation();			/*계산을 위한 함수*/

int main()
{
	char command;	/*메뉴 선택을 위한 변수*/

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
			getInfix();		/* infix 수식을 입력받는 함수 호출 */
			break;
		case 'p': case 'P':
			toPostfix();	/* infix 수식을 postfix 수식으로 변경하는 함수 호출 */
			break;
		case 'e': case 'E':
			evaluation();	/* postfix 수식을 계산하는 함수 호출 */
			break;
		case 'd': case 'D':
			debug();		/* 디버깅을 위한 함수 호출 */
			break;
		case 'r': case 'R':
			reset();		/* 초기화를 위한 함수 호출 */
			break;
		case 'q': case 'Q':	/* 종료 */
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');	/* q나 Q가 입력되면 종료 */

	printf("[----- [Lee Tae Jeong] [2023041062] -----]");

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;	/*postfixStack에 값을 넣는다*/
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';							/*postfixStack이 비어있으면 null을 반환*/
    else {
    	x = postfixStack[postfixStackTop--];	/*postfixStack에서 값을 빼낸다*/
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;				/*evalStack에 값을 넣는다*/	
}

int evalPop()
{
    if(evalStackTop == -1)						/*evalStack이 비어있으면 -1을 반환*/
        return -1;
    else
        return evalStack[evalStackTop--];		/*evalStack에서 값을 빼낸다*/
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
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
	return getToken(x);					/*우선순위를 반환*/
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);	/*postfixExp가 비어있으면 문자를 넣는다*/
	else
		strncat(postfixExp, c, 1);	/*postfixExp가 비어있지 않으면 문자를 이어붙인다*/
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
		char *exp = infixExp;
		char x;
	
		while(*exp != '\0')
		{
			if(getToken(*exp) == operand)		/*피연산자일 경우*/
				charCat(exp);
			else if(getToken(*exp) == lparen)	/*왼쪽 괄호를 스택에 넣는 코드 추가*/ 
				postfixPush(*exp);
			else if(getToken(*exp) == rparen)	/*오른쪽 괄호를 만나면 왼쪽 괄호가 나올 때까지 스택에 있는 연산자를 postfixExp에 넣는 코드 추가*/
			{
				while(postfixStackTop != -1 && postfixStack[postfixStackTop] != '(')
					charCat(&postfixStack[postfixStackTop--]);	/*왼쪽 괄호가 나올 때까지 스택에 있는 연산자를 postfixExp에 넣는다*/
				if(postfixStackTop != -1)
					postfixStackTop--; /*왼쪽 괄호를 스택에서 제거*/
			}
			else
			{

				while(postfixStackTop != -1 && getPriority(*exp) <= getPriority(postfixStack[postfixStackTop]))
					charCat(&postfixStack[postfixStackTop--]);	/*우선순위가 높은 연산자를 postfixExp에 넣는다*/
				postfixPush(*exp);	/*연산자를 스택에 넣는다*/
			}
			exp++;	/*다음 문자로 넘어간다*/
		}
	
		while(postfixStackTop != -1)					/*스택에 남은 연산자가 있을 경우*/
			charCat(&postfixStack[postfixStackTop--]);	/*스택에 남은 연산자를 postfixExp에 넣는다*/
}
	

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);	

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';	
         
	postfixStackTop = -1;	
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
    char *exp = postfixExp;	/*postfixExp를 읽기 위한 포인터*/
    int x, y;	/*연산을 위한 변수*/
    while(*exp != '\0')	/*postfixExp가 끝날 때까지*/
    {
        if(getToken(*exp) == operand)	/*피연산자일 경우*/
            evalPush(*exp - '0');		/*evalStack에 넣는다*/
        else	/*연산자일 경우*/
        {
            y = evalPop();	/*evalStack에서 값을 빼낸다*/
            x = evalPop();	/*evalStack에서 값을 빼낸다*/
            switch(*exp)	/*연산자에 따라 계산*/
            {
                case '+':
                    evalPush(x + y);
                    break;
                case '-':
                    evalPush(x - y);
                    break;
                case '*':
                    evalPush(x * y);
                    break;
                case '/':
                    evalPush(x / y);
                    break;
            }
        }
        exp++;	/*다음 문자로 넘어간다*/
    }
    evalResult = evalPop();						/*evalStack에 남은 값을 evalResult에 넣는다*/
    printf("Eval result = %d\n", evalResult);	
}

