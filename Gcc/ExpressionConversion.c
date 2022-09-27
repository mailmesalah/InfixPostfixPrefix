///header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///functions prototypes
void getType(char equ[][10], int size);   ///function to find the type of fix
int isBracketsBalanced(char equ[][10], int size);           ///function to balance the brackets
void infixToPostfix(char equ[][10], int size);        ///infix to postfix
void infixToPrefix(char equ[][10], int size);         ///infix to prefix
void postfixToInfix(char equ[][10], int size);     ///postfix to infix
void postfixToPrefix(char equ[][10], int size);// postfix to prefix
void prefixToPostfix(char equ[][10], int size);        ///prefix to postfix
void prefixToInfix(char equ[][10], int size);// prefix to infix
int getPriorityValue(char* a);                       ///function to check priority
void pop();                             ///function to pop from stack
void push(char* a);                      ///function to push into stack
char* peek();// function to show the string on the top of stack
int isEmpty();// function to check if the stack is empty
char* tokenizeStr(char *str, const char *delim, char **nextp); // function to tokenise string to sub string depending on the delimiter
int isDigit(char *str);//checks if the given string is a digit
void print2DData(char equ[][10]);// prints 2 dimensional character array
int compareStr(char *str, char c);// compare a string value to a character if both are same
int copyStr(char *str, char *c);// copies a string (char array) to another one

void openFile(char fileName[30], char rw[3]); // opens a file with the file name and read write property in rw
void closeFile();// closes currently opened file
void readFromFile();// read a line from the opened file
void appendToFile(char* param);// appends the current data in outBuff with the string send as parameter

void execute(int choice, char fileName[50]);// executes the conversions function depending on the choice made by user
void fillBuffer(char strs[][10]);// 2 dimensional char array is copied to outBuff char array 
void printToFile(char* fileName, char* rw, char output[][10], char* param);// prints outputs with parameters in the file specified by filename
void checkForError(char equ[][10], int size);// checks for any mismatch in operator oerand count also checks for entry of invalid characters
int evaluateExp(char equ[][10], int size, int choice);
void printEValToFile(char* fileName, char* rw);


///Global variables declaration
char equation[50][10];   ///char array of main equation
int size;            ///array size
char postfix[10][10];   ///to store postfix  expression equation
char infix[10][10];      ///to store infix  expression equation
char prefix[10][10]; //to store prefix  expression equation
int fType;          ///type of expression

//file variables
FILE* filePointer; // file pointer used for file manipulation
char inBuf[50] = ""; // input buffer from file to read from file
char outBuf[50] = ""; // output buffer to store data which is written to file
int total = 0;// keeps the total value of evaluation

struct node//singly linked list struct
{
	char data[50];// linked list data, char array to store each operators and operands
	struct node *next; // address of next node is stored here. NULL defines a node end
}*top = NULL;// initially the node variable top is set with NULL to specify its empty

/*
Checks if Stack is empty or not
*/
int isEmpty() {
	if (top == NULL)// if top is null it is empty
		return 1;
	else
		return 0;
}

/*
Returns the top element of Stack
*/
char* peek() {
	return top->data;// returns the data variable of current stack top
}

/*
Push an Element in Stack
*/
void push(char* data) {

	struct node *temp;// creates a tmporary node pointer variable
	temp = (struct node *)malloc(1 * sizeof(struct node)); // memory is allocated to temp variable
	copyStr(temp->data, data);// parameter data is stored in the newly created node data

	//printf("added  Element : %s\n", temp->data);

	if (top == NULL) {// checks if the node has any previous node
		top = temp;// if not then current memory is assigned as top
		top->next = NULL;// next address of current memory is set as null
	}
	else {// if stack is not empty
		temp->next = top;// current top node is pushed to newly created nodes next address
		top = temp;// top node is assigned with the newly created node
	}
}

/*
Pop Operation: Removes Top Element of the Stack
*/
void pop() {
	struct node *temp;// new node pointer variable is created
	if (isEmpty(top)) {// checks if top node is null or empty
		printf("\nStack is Empty\n");// prints the status
		return;
	}
	else {// if the stack is not empty
		temp = top;// temp variable is assigned with the address of top node
		top = top->next;// top node's address is changed with the address in top nodes next member 
		//printf("Removed  Element : %s\n", temp->data);
		free(temp);// frees the memory space old top node
	}
}

///start of program
int main()
{
	int i, counter = 0;   /// i for loop counters
	int status = 0;      /// status to check if parenthesis are balanced
	int type = 0;        /// type of fix
	int convType = 0;   /// user selected options for conversion

	char *tok = NULL; // used in tokenising string to  keep each token at a time
	char *savePtr = NULL;// used to keep input string address
	char str[50] = "";// to store input string of expression
	int totalElement = 0;// to calculate total number of operators and operands along with paranthesis
	int choice = 0;// for using in choice making by user
	char fileName[50] = "";

	///printing interactive menu for user
	printf("***Welcome to fixes and parentheses balance program**\n");
	printf("Please Select your input form (1 for keyboard and 2 for file) :\n");
	scanf("%d", &choice);// selects is user want to use keyboard input of file input
	getchar();// to skip enter key press from skipping next read
	if (choice == 1) {
		printf("Enter your equation :\n");
		printf("When done press 'enter'\n");

		//fgets(str, 50, stdin);
		scanf("%[^\n]s", str);// get space delimited string which is terminated by \n
		savePtr = str;// keeps the address of str in savePtr

		while ((tok = tokenizeStr(savePtr, " ", &savePtr)) != NULL) {// iterates through each space delimited string
			copyStr(equation[totalElement++], tok); // coppies current token to equation string array
		}

		for (i = 0; i < totalElement; i++)// prints the tokens recieved in equation variable
			printf("%s ", equation[i]);

		size = totalElement;// size of the equation array is stored in size.
		execute(1,fileName);// calls the execute function with choice of keyboard with empty file name variable

	}
	else if (choice == 2) {// if the choice is to input from file
		printf("Press 1 for default file, 2 to provide file name :\n");
		scanf("%d", &choice);// lets user input choice for read default or provide files name by user
		getchar();// skips the next line character
		if (choice == 1) {// user selects for deafult file selection for input
			openFile("default.txt", "a+");// default file is opened for reading and appending
			readFromFile();// reads a line from the opened file
			printf("\nread %s", inBuf); // prints the read string to screen
			//Tockenising
			savePtr = inBuf; // savePtr keeps inBuff address for tokening the string
			while ((tok = tokenizeStr(savePtr, " ", &savePtr))) {// tokenises the input string
				copyStr(equation[totalElement++], tok);// tokens are copied to equation
			}

			for (i = 0; i < totalElement; i++)//prints the tokenised values from equation
				printf("%s ", equation[i]);

			size = totalElement;// number of tokens are kept in size
			execute(2, fileName);// execute type conversions by sending the dummy file name and choice to to select default file
		}
		else if (choice == 2) {// user select choice to input file name to select input from that file
			printf("Please provide file name :\n");
			scanf("%s", fileName);// filename is read from keyboard

			openFile(fileName, "a+");// opens the file for appending and reading
			readFromFile();// reads from the file
			printf("\nread %s", inBuf);// prints the read one line string
			//Tockenising
			savePtr = inBuf;

			// buf address is saved in saveptr
			while ((tok = tokenizeStr(savePtr, " ", &savePtr))) {// tokenises the input string
				copyStr(equation[totalElement++], tok);// copies the token to equation
			}

			for (i = 0; i < totalElement; i++)// prints all the tockens in equation
				printf("%s ", equation[i]);

			size = totalElement;// number of tokens are stored in size
			execute(3, fileName);// executes expression conversion function , sets the parameter with the filename
		}
		else {
			printf("Invalid Choice \n");
		}
	}
	else {
		printf("Invalid Choice \n");
	}



	return 0;
}

/*
Executes the conversion of expressions depending on the selections made by user
*/
void execute(int choice, char fileName[50])
{
	int i, counter = 0;   /// i for loop counters
	int status = 0;      /// status to check if parenthesis are balanced
	int type = 0;        /// type of fix
	int convType = 0;   /// user selected options for conversion
	char tempEqu[50][10];// to keep equation for temporary	

	printf("size %d", size);// prints the size

	status = isBracketsBalanced(equation, size);///to see if parenthesis are balanced
	getType(equation, size);///checks the type of fix user inputed
	checkForError(equation, size);// checks for any operand operator mismatch also for invalid characters

	printf("\nstatus %d type %d\n", status, fType);

	if (status == 1)// checks if parenthesis are balanced
	{
		if (fType == 1)                ///if type of input is an INFIX
		{
			printf("Select:\n1: Convert to Pre-fix\n2: Convert to Post-fix\n3: convert to both Pre and Post fix \n");
			scanf("%d", &convType);                ///ask user which type they would like to convert
		}
		if (fType == 1 && convType == 1)// if the type is infix and want to convert to prefix
		{
			printf("The prefix expression is :\n");
			infixToPrefix(equation, size);// infix to prefix conversion function
			print2DData(prefix);// prints the output

			total = evaluateExp(postfix, size, fType); // evaluates
			printf("\nEvaluation Output : %d", total );// prints the value

			//print to file
			if (choice == 1)// prinnts to files input and output , inputs and outputs
			{
				// prints to file
				printToFile("Input.txt", "w", equation, "Infix Expression :");
				printToFile("Output.txt", "w", prefix, "Prefix Expression :");

				printEValToFile("Output.txt", "a");//Prints evaluated output to file				
				
			}
			else if (choice == 2) // default file is selected for reading and writing
			{
				printToFile("Default.txt", "a", prefix, "Prefix Expression :");
				
				printEValToFile("Default.txt", "a");//Prints evaluated output to file
			}
			else if (choice == 3) // file name from keyboard input is used to read from and print to file
			{
				printToFile(fileName, "a", prefix, "Prefix Expression :");

				printEValToFile(fileName, "a");//Prints evaluated output to file
			}

		}

		if (fType == 1 && convType == 2) // infix to postfix conversion
		{
			printf("The postfix expression is :\n");
			infixToPostfix(equation, size);                 ///function call to function which converts INFIX TO POSTFIX			
			print2DData(postfix);// prints the output to screen

			total = evaluateExp(postfix, size, fType); // evaluates
			printf("\nEvaluation Output : %d", total);// prints the value

			//print to file
			if (choice == 1)// prints to input and output files
			{
				printToFile("Input.txt", "w", equation, "Infix Expression :");
				printToFile("Output.txt", "w", postfix, "Postfix Expression :");

				printEValToFile("Output.txt", "a");//Prints evaluated output to file
			}
			else if (choice == 2)// prints from to default file
			{
				printToFile("Default.txt", "a", postfix, "Postfix Expression :");

				printEValToFile("Default.txt", "a");//Prints evaluated output to file
			}
			else if (choice == 3)// prints from to the file specified in variable fileName
			{
				printToFile(fileName, "a", postfix, "Postfix Expression :");

				printEValToFile(fileName, "a");//Prints evaluated output to file
			}

		}
		if (fType == 1 && convType == 3)// infix to both prefix and postfix conversions
		{
			//copying to keep data
			for (i = 0; i < size; i++) // copies equation in temporary variable to keep the data
			{
				copyStr(tempEqu[i], equation[i]);

			}

			printf("The postfix expression is :\n");
			infixToPostfix(equation, size);                 ///function call which converts INFIX TO POSTFIX
			print2DData(postfix);// prints the output

			
			//print to file
			if (choice == 1)// prints to input and output files
			{
				printToFile("Input.txt", "w", equation, "Infix Expression :");
				printToFile("Output.txt", "a", postfix, "Postfix Expression :");
			}
			else if (choice == 2)// prints to default text file
			{
				printToFile("Default.txt", "a", postfix, "Postfix Expression :");
			}
			else if (choice == 3)// writes to the file specified
			{
				printToFile(fileName, "a", postfix, "Postfix Expression :");
			}

			printf("\n");
			printf("The prefix expression is :\n");
			infixToPrefix(equation, size);// infix to prefix conversion function
			print2DData(prefix);// prints output

			total = evaluateExp(postfix, size, fType); // evaluates
			printf("\nEvaluation Output : %d", total);// prints the value

			//print to file
			if (choice == 1)// prints to input and output files
			{
				printToFile("Output.txt", "a", prefix, "Prefix Expression :");
				printEValToFile("Output.txt", "a");//Prints evaluated output to file
			}
			else if (choice == 2)// prints to default file
			{
				printToFile("Default.txt", "a", prefix, "Prefix Expression :");

				printEValToFile("Default.txt", "a");//Prints evaluated output to file
			}
			else if (choice == 3)// prints to the file specified
			{
				printToFile(fileName, "a", prefix, "Prefix Expression :");

				printEValToFile(fileName, "a");//Prints evaluated output to file
			}

		}

		if (fType == 2)                               ///if type of input is a PREFIX
		{
			printf("Select:\n1: Convert to In-fix\n2: Convert to Post-fix\n3: convert to both In and Post fix \n");
			scanf("%d", &convType);                ///ask user which type they would like to convert

			if (convType == 1) {// converts to infix
				printf("The Infix expression is :\n");
				prefixToInfix(equation, size);// function call too prefix to infix
				print2DData(infix);// prints the output 

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1)// prints to input and output
				{
					printToFile("Input.txt", "w", equation, "Prefix Expression :");
					printToFile("Output.txt", "w", infix, "Infix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2)// prints to default
				{
					printToFile("Default.txt", "a", infix, "Infix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// prints to the files specified
				{
					printToFile(fileName, "a", infix, "Infix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}

			}
			else if (convType == 2) {// converts to postfix
				printf("The postfix expression is :\n");
				prefixToPostfix(equation, size);// prefix to postfix function call
				print2DData( postfix); // prints output

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1) // prints to input and output files
				{
					printToFile("Input.txt", "w", equation, "Prefix Expression :");
					printToFile("Output.txt", "w", postfix, "Postfix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2) // prints to default file
				{
					printToFile("Default.txt", "a", postfix, "Postfix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// prints to the file specified
				{
					printToFile(fileName, "a", postfix, "Postfix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}

			}
			else if (convType == 3) { // converts to both infix and post fix
				//copying to keep data
				for (i = 0; i < size; i++) // keeps equation in temp variable
				{
					copyStr(tempEqu[i], equation[i]);

				}

				printf("The infix expression is :\n");
				prefixToInfix(equation, size);// function call prefix to infix
				print2DData ( infix);// prints infix

				//print to file
				if (choice == 1)// prints to input and output file
				{
					printToFile("Input.txt", "w", equation, "Prefix Expression :");
					printToFile("Output.txt", "a", infix, "Infix Expression :");
				}
				else if (choice == 2)// prints to default file
				{
					printToFile("Default.txt", "a", infix, "Infix Expression :");
				}
				else if (choice == 3)// prints to the file specified
				{
					printToFile(fileName, "a", infix, "Infix Expression :");
				}

				printf("\n");
				printf("The postfix expression is :\n");
				prefixToPostfix(equation, size);// function call to convert prefix to postfix
				print2DData( postfix);// prints postfix

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1)// prints to output file
				{
					printToFile("Output.txt", "a", postfix, "Postfix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2)// print to default
				{
					printToFile("Default.txt", "a", postfix, "Postfix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// print to the file specified
				{
					printToFile(fileName, "a", postfix, "Postfix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}

			}
		}

		if (fType == 3)                               ///if type of input is a POSTFIX
		{
			printf("Select:\n1: Convert to In-fix\n2: Convert to Pre-fix\n3: convert to both In and Pre-fix \n");
			scanf("%d", &convType);                ///ask user which type they would like to convert

			if (convType == 1) {// convert to infix
				printf("The Infix expression is :\n");
				postfixToInfix(equation, size);// function call to convert postfix to infix
				print2DData( infix);// prints the infix

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1)// prints to input and output
				{
					printToFile("Input.txt", "w", equation, "Postfix Expression :");
					printToFile("Output.txt", "w", infix, "Infix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2)// prints to default file
				{
					printToFile("Default.txt", "a", infix, "Infix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// prints to the file specified
				{
					printToFile(fileName, "a", infix, "Infix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}

			}
			else if (convType == 2) {// convert to prefix
				printf("The prefix expression is :\n");
				postfixToPrefix(equation, size);// post fix to prefix conversion call
				print2DData( prefix);

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1)// prints to input and output
				{
					printToFile("Input.txt", "w", equation, "Postfix Expression :");
					printToFile("Output.txt", "w", prefix, "Prefix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2)// prints to default file
				{
					printToFile("Default.txt", "a", prefix, "Prefix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// prints to the file specified
				{
					printToFile(fileName, "a", prefix, "Prefix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}
			}
			else if (convType == 3) { // convert to both infix and prefix
				//copying to keep data
				for (i = 0; i < size; i++)// keeps the equation in temporary file
				{
					copyStr(tempEqu[i], equation[i]);

				}

				printf("The infix expression is :\n");
				postfixToInfix(equation, size);// function to convert from post fix to infix is called
				print2DData( infix);// infix output is printed

				//print to file
				if (choice == 1)// print to input and output files
				{
					printToFile("Input.txt", "w", equation, "Postfix Expression :");
					printToFile("Output.txt", "a", infix, "Infix Expression :");
				}
				else if (choice == 2)// print to default file
				{
					printToFile("Default.txt", "a", infix, "Infix Expression :");
				}
				else if (choice == 3)// print to the file specified
				{
					printToFile(fileName, "a", infix, "Infix Expression :");
				}

				printf("\n");
				printf("The prefix expression is :\n");
				postfixToPrefix(equation, size);// function to post fix to prefix is called
				print2DData( prefix); // prefix output is printed

				total = evaluateExp(equation, size, fType); // evaluates
				printf("\nEvaluation Output : %d", total);// prints the value

				//print to file
				if (choice == 1)// print to output file
				{
					printToFile("Output.txt", "a", prefix, "Prefix Expression :");

					printEValToFile("Output.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 2)// print to default file
				{
					printToFile("Default.txt", "a", prefix, "Prefix Expression :");

					printEValToFile("Default.txt", "a");//Prints evaluated output to file
				}
				else if (choice == 3)// print to the file specified
				{
					printToFile(fileName, "a", prefix, "Prefix Expression :");

					printEValToFile(fileName, "a");//Prints evaluated output to file
				}
			}
		}
	
		
	}
	else
	{
		printf("\nInvalid Expression Type");
	}

	
}

/*
gets the type of the expression
*/
void getType(char equ[][10], int size)                   ///function to decide the type of fix
{
	int i = 0, j = 0, found = 1;
	
	//Infix checking
	for (i = 0; i < size; i++)
	{
		if (equ[i][0] != '(')// checks if the string is '('
		{
			//check first operand to be a number
			while (equ[i][j] != '\0')
			{
				if (!isdigit(equ[i][j]))// checks for the string for not a digit
				{
					found = 0;// if not then break the loops and set the digit found status to 0
					break;
				}

				j++;
			}
			
			if (isdigit(equ[(i + 1)][0]))//check the next one is operator
			{
				found = 0;
				break;
			}

			break;

		}
	}

	if (found) // if found is still 1 then its infix
	{
		fType = 1;
		return;
	}


	//prefix checking
	if (!isdigit(equ[0][0]))// check for the character not a digit
	{
		fType = 2;// set as prefix
		return;
	}


	//postfix checking
	if (isdigit(equ[0][0]))// if first character is a digit
	{
		fType = 3;// return postfix
		return;
	}

	fType = -1;// if none matches then return -1
	return;
}

/*
check if the brackets are balanced
*/
int isBracketsBalanced(char equ[][10], int size)            ///function to check parenthesis balance
{
	int i = 0, j = 0;
	int a = 0, b = 0, c = 0;
	int status;                             ///status if parenthesis is balanced
	for (i = 0; i <= size; i++)                       ///loop to check each element of equation
	{
		for (j = 0; equ[i][j] != '\0'; j++)                       ///loop to check each element of equation
		{
			if (equ[i][j] == '(')                 ///if its an open parenthesis
				a = a + 1;                    ///increment a by 1
			if (equ[i][j] == ')')                 ///if its a close parenthesis
				a = a - 1;                    ///decrement a by 1
		}
	}
	if (a == 0)                            ///When a == 0, parenthesis are balanced
	{
		printf("This is a balanced expression\n");
		status = 1;                     ///status is set to 1
	}
	else                                ///if a != 0
	{
		status = -1;                    ///status is set to 0
		printf("This is an unbalanced expression\n");
		printf("Check your small brackets '()' and try again\n");
		///user is asked to check their parenthesis
	}
	return status;                      ///this function returns status of balancing
}

/*

priority of operator is decided and returned

*/
int getPriorityValue(char* a)                        ///function to check operator precedence with an argument
{
	int pres = 0;
	switch (a[0])
	{
	case '^':
		pres = 3;                           ///highest precedence
		break;
	case '*':
	case '/':
		pres = 2;
		break;
	case '+':                          ///lowest precedence
	case '-':
		pres = 1;
		break;
	default:
		pres = 0;
		break;
	}

	return pres;                    ///returns a number according to precedence

}


void infixToPostfix(char equ[][10], int size)    ///function to convert infix to postfix with two arguments
								   ///infix equation and size of the equation as arguments
{
	int i, j;                           ///i for stack and j for postfix equation
	int pre = 0;                   ///to keep precedence result of two operators
	for (i = 0, j = 0; i < size; i++)          ///to loop through equation and simultaneously postfix equation
	{
		if (isDigit(equ[i]) == 1)        ///if current character is an operand
		{
			copyStr(postfix[j++], equ[i]);     ///put the character into final answer equation, post_fix
		}
		else if (compareStr(equ[i], '(') == 0)          ///else if current character is an open parenthesis
		{
			push(equ[i]);               ///push it onto stack
		}
		///else if current character is an operator
		else if (compareStr(equ[i], '+') == 0 || compareStr(equ[i], '-') == 0 || compareStr(equ[i], '*') == 0 || compareStr(equ[i], '/') == 0 || compareStr(equ[i], '^') == 0)
		{
			if (isEmpty())               ///if stack is empty
			{
				push(equ[i]);           ///push current character onto stack

			}
			else {          ///else if stack is not empty
				if (compareStr(peek(), '(') == 0)  ///current stack top holds an open parenthesis
				{
					push(equ[i]);       ///push current character to stack
				}
				else
				{                       ///else if stack top does not contain an open parenthesis then,it must be an operator
										///check for precedence of current and stack top operator
					pre = getPriorityValue(equ[i]) - getPriorityValue(peek());
					if (pre <= 0)            ///if precedence of current character is lower than stack top operator
						while (!isEmpty() && getPriorityValue(peek()) - getPriorityValue(equ[i]) >= getPriorityValue(equ[i]))
						{                       ///continue to pop from stack till stack operators precedence is higher than current operator
							copyStr(postfix[j++], peek()); ///put the operator into final answer equation, post_fix
							pop();              ///pop will decrement top
						}
					push(equ[i]);           ///push the current operator
				}
			}
		}
		if (compareStr(equ[i], ')') == 0)      ///if current character is a close parenthesis
		{
			while (isEmpty()) ///till stack is empty pop from stack
			{
				if (isEmpty)           ///if stack is empty
				{
					break;              ///break out from loop
				}
				if (compareStr(peek(), '(') == 0)   ///if stack top is an open parenthesis
				{
					pop();              ///pop the open parenthesis to ignore it
				}
				else                    ///else
				{                       ///put the operators onto postfix equation
					copyStr(postfix[j++], peek());
					pop();              ///to decrement top
				}   ///closing for else
			}       ///while loop closing
		}           ///closing for if


	}
	while (!isEmpty())// if stack still has data pop them all to postfix output variable
	{
		if (compareStr(peek(), '(') == 0)
			pop;
		else
			copyStr(postfix[j++], peek());
		pop();
	}
	return;
}

void infixToPrefix(char equ[][10], int size)  ///to convert infix to prefix
{
	int i, j;
	char temp[50][10]; // keeps equ data for temporary

	//reverse the string

	for (i = 0, j = size - 1; i < size; i++, j--)         ///scan through expression from right to left
	{
		copyStr(temp[i], equ[j]);// copies string in reverse
	}

	//copy back
	for (i = 0; i < size; i++)         ///scan through expression
	{
		copyStr(equ[i], temp[i]);// copies back to equ
	}

	for (i = 0; i < size; i++)         ///scan through expression
	{
		if (compareStr(equ[i], '(') == 0)        ///if character is an open bracket
		{
			*equ[i] = ')';///change it to a close bracket			
		}
		else if (compareStr(equ[i], ')') == 0)        ///but its a close bracket
		{
			*equ[i] = '(';            ///change it to an open bracket
		}
	}

	infixToPostfix(equ, size);             ///take postfix of this string

	for (i = 0, j = size - 1; i < size; i++, j--)         ///scan through expression
	{
		copyStr(prefix[i], postfix[j]); // copies to prefix for output in reverse
	}

	return;
}

void postfixToInfix(char equ[][10], int size) ///postfix to prefix
{
	int i, j, len = 0;//len to keep the length of expression pushed to stack
	char temp[50]; // temporary equ data holder


	for (i = 0, j = 0; i < size; i++)
	{
		//initialise temp
		temp[0] = '\0';

		if (isDigit(equ[i]) == 1)// a number
		{
			push(equ[i]);
		}
		else// its operator
		{
			if (isEmpty())//checks if empty
			{
				printf("\nError");
				exit(1);
				//it error
				//set failure global variable
				break;

			}

			//expression creation to push to stack
			len = 1;// length is counted from 1
			temp[0] = '(';// left bracket
			temp[1] = '\0';// sets null terminator
			len += strlen(peek());// gets the length of stack top data
			strcat(temp, peek()); // concatinates temp data with stack data
			temp[len] = '\0';// null terminates for further concatinations
			pop();// pop top of stack
			len++;// increases length
			strcat(temp, equ[i]); //concatinates the operator
			temp[len] = '\0'; // null terminator
			len += strlen(peek());// gets the length of top of stack added to len
			strcat(temp, peek());// concatinates the next operand
			pop(); // pops the second operand
			temp[len] = ')'; // close the bracket
			temp[++len] = '\0';// null terminator

			//push back to stack
			push(temp);
		}
	}

	//pop all to infix variable for printing
	j = 0;
	while (!isEmpty())// pop back to infix data 
	{
		copyStr(infix[j++], peek());
		pop();
	}
}

void postfixToPrefix(char equ[][10], int size) ///postfix to prefix
{
	int i, j, len = 0;
	char temp[50];


	for (i = 0, j = 0; i < size; i++)
	{
		//initialise temp
		temp[0] = '\0';

		if (isDigit(equ[i]) == 1)// a number
		{
			push(equ[i]);// push to stack
		}
		else// its operator
		{
			if (isEmpty())
			{
				printf("\nError");
				exit(1);
				//it error
				//set failure global variable
				break;

			}
			// creation of expression to push to stack
			len = 0;

			// pop out last two expressions from stack
			len += strlen(peek());// get length of stack top data
			strcat(temp, peek()); // concatinate temp with stack
			temp[len] = '\0'; // limits the string
			pop(); // pops the first operator
			len += strlen(peek()); // length of next operator is added to len
			strcat(temp, peek()); // concatinates temp with stack top data
			pop(); // pos second operand
			temp[len] = '\0'; // limits by '\0'

			len++;
			strcat(temp, equ[i]); // concatinates the operator from equ
			temp[len] = '\0';

			//push back to stack
			push(temp); // the expression made pushed back to stack
		}
	}

	//pop all to infix variable for printing
	j = 0;
	while (!isEmpty())// iterate until stack is empty
	{
		copyStr(prefix[j++], strrev(peek()));// copy stack data to prefix
		pop();
	}

}

void prefixToPostfix(char equ[][10], int size)

{
	int i, j, len = 0;// len to keep length for expression
	char temp[50];// to store expression temporarily


	for (i = size - 1; i >= 0; i--)
	{
		//initialise temp
		temp[0] = '\0';

		if (isDigit(equ[i]) == 1)// a number
		{
			push(equ[i]);// the number is pushed to stack
		}
		else// its operator
		{
			if (isEmpty())// check for empty stack
			{
				printf("\nError");
				exit(1);
				//it error
				//set failure global variable
				break;

			}


			// creating the expression
			len = 0;

			len += strlen(peek());// length of stack data is stored
			strcat(temp, peek());// concatinates the first operand
			temp[len] = '\0';

			pop();// pops first operand
			len += strlen(peek()); // second operand length is added to length

			strcat(temp, peek());// concatinates temp with second operand

			pop();// pops the second operand
			temp[len] = '\0';

			len++;
			strcat(temp, equ[i]); // operator is concatinated to the temp
			temp[len] = '\0';

			//push back to stack
			push(temp);
		}
	}

	//pop all to infix variable for printing
	j = 0;
	while (!isEmpty())// iterate still stack is empty
	{
		copyStr(postfix[j++], peek());// copy stack data to postfix
		pop();
	}
}

void prefixToInfix(char equ[][10], int size) // converts prefix to infix

{
	int i, j, len = 0;// to keep expression length
	char temp[50];// to keep expression

	for (i = size - 1; i >= 0; i--)// iterate from right to left
	{
		//initialise temp
		temp[0] = '\0';


		if (isDigit(equ[i]) == 1)// a number
		{
			push(equ[i]);// push digit to stack

		}
		else// its operator
		{
			if (isEmpty())// checks if stack is empty
			{
				printf("\nError");
				exit(1);
				//it error
				//set failure global variable
				break;

			}

			//expression creation
			// len keep tracks of the length of the expression as operands and operator is added to it
			len = 1;
			temp[0] = '(';// left bracket
			temp[1] = '\0';
			len += strlen(peek()); // length of stack data is stored
			strcat(temp, peek()); // first operand is added to temp
			temp[len] = '\0';

			pop();// pops first operand
			len++;
			strcat(temp, equ[i]); // add the operator to the temp
			temp[len] = '\0';
			len += strlen(peek());

			strcat(temp, peek());// adds the second operand to the temp

			pop();// pops the second operand
			temp[len] = ')';
			temp[++len] = '\0';

			//push back to stack
			push(temp);// pushes back the expression created
		}
	}

	//pop all to infix variable for printing
	j = 0;
	while (!isEmpty())// iterate till the stack is empty
	{
		copyStr(infix[j++], peek());// copies the stack data to infix
		pop();
	}

}

int evaluateExp(char equ[][10], int size, int choice)// evaluates the expressions
{
	// since we are using the postfix of infix input to evaluate evaluate function should only be called after conversion
	int i, j;
	int stackedOperand[50],counter=0,k,pow;//to hold the evaluated data as stack we use stackOperand array, counter too keep track of stack size, pow used to calculate power(^)

	switch (choice)
	{
	case 1:// input is infix
		//we take the postfix of infix and evaluate conveting to infix
		for (i = 0, j = 0; i < size; i++)//iterates throught the loop
		{
			if (isDigit(equ[i]) == 1)// a number
			{
				stackedOperand[counter++] = atoi(equ[i]); // pushes the number to stack
			}
			else// its operator
			{
				if (counter == 0)//checks if empty
				{
					printf("\nError");
					exit(1);
					//it error
					//set failure global variable
					break;

				}

				//expression creation to push to stack
				if (equ[i][0] == '+')// add operation
				{
					stackedOperand[counter - 2] += stackedOperand[counter - 1];// last two operand are added
					--counter;// decrement the stack top
				}
				else if (equ[i][0] == '-')// subtract operation
				{
					stackedOperand[counter - 2] =  stackedOperand[counter - 1]- stackedOperand[counter - 2];// last two operands on stack are subtracted
					--counter;// decrements counter
				}
				else if (equ[i][0] == '*')// multiplication
				{
					stackedOperand[counter - 2] *= stackedOperand[counter - 1];// last two operands in stack are multiplied
				}
				if (equ[i][0] == '/')// division
				{
					if (stackedOperand[counter - 2] == 0) { // checks if the second operator is zero
						printf("\nDivision by Zero Error");
						exit(1);
						//it error
						//set failure global variable
						break;
					}
					stackedOperand[counter - 2] =  stackedOperand[counter - 1]/ stackedOperand[counter - 2];// last two operands are divided
					--counter;//decrements
				}
				if (equ[i][0] == '^')// if the operator is a power op
				{
					k = 0;
					pow = 1;// initialises the power variable to keep power data
					for (k = 0; k < stackedOperand[counter - 2]; k++)// loops to calculate power
					{
						pow *= stackedOperand[counter - 1];// first operator is multiplied with the pow data
					}
					stackedOperand[counter - 2] = pow;// pow data is copied to stack top 
					--counter;//decrements
				}

			}
		}

		break;
	case 2://input is prefix
		
		//call this function only after conversion
		for (i = size - 1; i >= 0; i--)// iterate from right to left
		{
			if (isDigit(equ[i]) == 1)// a number
			{
				stackedOperand[counter++] = atoi(equ[i]);//added to stack
			}
			else// its operator
			{
				if (counter == 0)//checks if empty
				{
					printf("\nError");
					exit(1);
					//it error
					//set failure global variable
					break;

				}

				//expression creation to push to stack
				if (equ[i][0] == '+')//+ operator
				{
					stackedOperand[counter - 2] = stackedOperand[counter-1] + stackedOperand[counter - 2];// adds last two operands
					--counter;
				}
				else if (equ[i][0] == '-')// - operator
				{
					stackedOperand[counter - 2] = stackedOperand[counter-1] - stackedOperand[counter - 2];// subtracts last two operands
					--counter;
				}
				else if (equ[i][0] == '*')// * operator
				{
					stackedOperand[counter - 2] = stackedOperand[counter-1]* stackedOperand[counter - 2];// multiplies last two operands
				}
				if (equ[i][0] == '/')// / operator
				{
					if (stackedOperand[counter - 2] == 0) {// check if the second parameter is zero
						printf("\nDivision by Zero Error");
						exit(1);
						//it error
						//set failure global variable
						break;
					}
					stackedOperand[counter - 2] = stackedOperand[counter-1] / stackedOperand[counter - 2];//last two operands are divided			
					--counter;
				}
				if (equ[i][0] == '^')// power operator
				{
					k = 0;
					pow = 1;
					for (k = 0; k < stackedOperand[counter - 2]; k++)// iterates through loop
					{
						pow *= stackedOperand[counter-1];// powers the current data						
					}
					stackedOperand[counter - 2] = pow;// keeps the power data to the top of stack
					--counter;
				}				
			}

		}

		break;
	case 3://input is postfix

		  
		for (i = 0, j = 0; i < size; i++)// iterates through loop
		{
			if (isDigit(equ[i]) == 1)// a number
			{
				stackedOperand[counter++] = atoi(equ[i]);// adds the number to the stack
			}
			else// its operator
			{
				if (counter == 0)//checks if empty
				{
					printf("\nError");
					exit(1);
					//it error
					//set failure global variable
					break;

				}

				//expression creation to push to stack
				if (equ[i][0] == '+')// + operator
				{
					stackedOperand[counter - 2] += stackedOperand[counter - 1];// adds last operands in stack
					--counter;
				}
				else if (equ[i][0] == '-')// minus operator
				{
					stackedOperand[counter - 2] = stackedOperand[counter - 2]- stackedOperand[counter - 1];// substracts last two operands
					--counter;
				}
				else if (equ[i][0] == '*')// multiplication
				{
					stackedOperand[counter - 2] *= stackedOperand[counter - 1];// last two operands are multiplied
				}
				if (equ[i][0] == '/')// division
				{
					if (stackedOperand[counter - 1] == 0) {// check for a division by zero error 
						printf("\nDivision by Zero Error");
						exit(1);
						//it error
						//set failure global variable
						break;
					}
					stackedOperand[counter - 2] = stackedOperand[counter - 2]/stackedOperand[counter - 1];// divides last two operands
					--counter;
				}
				if (equ[i][0] == '^')// power operator
				{
					k = 0;
					pow = 1;
					for (k = 0; k < stackedOperand[counter - 1]; k++)// continues multiplication of the first operand with the same operand to get power until the count reaches second operator
					{
						pow *= stackedOperand[counter - 2];// pow data is multiplied with first stack operand
					}
					stackedOperand[counter - 2] = pow;// power data is assigned back to stack top
					--counter;
				}

			}
		}

		break;
	default :
		break;
	}

	return stackedOperand[0];// returns the total
}

/*
Used to Tokenize first string that is delimited by the delimiter variable, in the string provided.
*/
char* tokenizeStr(char *str, const char *delim, char **nextp)
{
	char *ret;// return string variable (char pointer)

	if (str == NULL)// if the string is null
	{
		str = *nextp; // first address in the nextp is assigned
	}

	str += strspn(str, delim);//returns the number of characters in the initial segment of str which consist only of characters from delimit

	if (*str == '\0')// check if the str content is empty
	{
		return NULL;
	}

	ret = str;// return the address of str to ret to return back

	str += strcspn(str, delim); 

	if (*str)// if str content is not null 
	{
		*str++ = '\0';// increment address in str and set it as null
	}

	*nextp = str; // new address is assigned to nextp content

	return ret;
}

/*
Checks if a character array is number
*/
int isDigit(char *str)
{
	int i = 0;
	int dig = 1;// digit status variable
	while (str[i] != '\0')// iterates till the last character
	{
		if (isdigit(str[i]) == 0) {// checks if the character is digit, if not return 0
			dig = 0;
		}
		++i;
	}
	return dig;
}

/*
Prints two dimentional array
*/
void print2DData(char equ[][10])
{
	int i;
	printf("\n");
	for (i = 0; i < size; i++)//iterates through the array
	{
		printf("%s ", equ[i]);// prints each string in it
	}
	printf("\n");
}

/*
Checks if the first character of a string is same as the character
*/
int compareStr(char *str, char c)
{
	if (*str != c)// checks first character of the string with charcter in c
	{
		return 1; // return 1 if not equals
	}
	return 0;// return 0 if equals
}

/*
copy two strings
*/
int copyStr(char *str, char *c)
{
	while (*c != '\0')// iterate till the last character
	{
		*str = *c;// copy character in c to character in str
		str++;// increment the str to then next address
		c++;// increment c to the next address
	}
	*str = '\0'; // delimit with null character
}

/*
Opens a file with the filename and parameter right rw
*/
void openFile(char fileName[30], char rw[3])
{
	filePointer = fopen(fileName, rw);// opens the file and a pointer to the file is returned
	if (!filePointer) { /* open operation failed. */
		perror("\nFailed opening file");
		exit(1);
	}
}

/*
closes the opened file
*/
void closeFile()
{
	if (fclose(filePointer) != 0) {//closes the file with the pointer filePointer
		perror("\nFailed closing file");
	}
}

/*
Reads a line from already opened file
*/
void readFromFile()
{
	fscanf(filePointer, "%[^\n]s", inBuf); // reads a line of string from file specified by filePointer to inBuf
	printf("\nInput from File is %s", inBuf);//prints the input from file
}

/*
Adds data to the already opened file
*/
void appendToFile(char* param)
{
	fputs(param, filePointer);//prints the param string value to file
	fputs("\n", filePointer);//prints next line character in file
	fputs(outBuf, filePointer);// prints the data in outBuf to file
	fputs("\n", filePointer);
	/* write out any buffered writes to the given file stream.          */
	fflush(filePointer);//force the file to update with new data.
}

void printEValToFile(char* fileName, char* rw)
{
	//Print to file
	openFile(fileName, rw);// opens the file with the read write permission
	fputs("\nThe Evaluated output is :", filePointer);//prints the param string value to file
	fprintf(filePointer, " %d\n", total); //prints the total
										/* write out any buffered writes to the given file stream.          */
	fflush(filePointer);//force the file to update with new data.
	closeFile();// closes the file
}

/*
the two dimentional data is saved to one dimensional outBuf
*/
void fillBuffer(char strs[][10])
{
	int i;
	outBuf[0] = '\0';// delimits the buffer with null character
	for (i = 0; i < size; i++)//iterates through the 2D character array str
	{
		strcat(outBuf, strs[i]); // each string is added to outBuff
		strcat(outBuf, " ");// the string is speperated by a space
	}
}

/*
Prints datas to file
*/
void printToFile(char* fileName, char* rw, char output[][10], char* param)
{
	//Print to file
	openFile(fileName, rw);// opens the file with the read write permission
	fillBuffer(output);// fills the buffer with the right data
	appendToFile(param);// writes the buffer to the file
	closeFile();// closes the file
}


/*
Checks if the number of operand to operator is correct. Also checks if the expression has any invalid characters
*/
void checkForError(char equ[][10], int size)                   ///function to decide the type of fix
{
	int i, numCount = 0, operCount=0;// counts number and operator

	for (i = 0; i < size; i++)// iterate through the expression
	{
		//checks if it is an operator
		if (compareStr(equ[i], '*')==0 || compareStr(equ[i], '/')==0 || compareStr(equ[i], '+')==0 || compareStr(equ[i], '-')==0 || compareStr(equ[i], '^')==0)
		{
			++operCount;//operator count is increased
		}
		else if (isDigit(equ[i]) == 1)// checks if it is number
		{
			++numCount;//increase number count
		}
		else if (!(compareStr(equ[i], '(') == 0 || compareStr(equ[i], ')') == 0))// checks for other characters other than paranthesis
		{
			printf("\nWrong Character found : '%s'", equ[i]);// print error and exit
			exit(1);
		}
		
	}

	if (numCount - 1 != operCount)// check if the operator and operand is in correct numbers
	{
		printf("\nInvalid number of Operator to Operand");
		exit(1);
	}

}