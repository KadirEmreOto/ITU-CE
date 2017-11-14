/* @Author
 * Student Name: Kadir Emre Oto
 * Student ID: 150140032
 * Date: 25.12.2016
 */

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>

#define unary 1
#define binary 2
#define ubinary 3
#define polynary 4
#define leftparen 5
#define rightparen 6
#define number 7
#define inf 8
#define ERROR  9

using namespace std;
struct exception;
struct stack;
struct token;

string to_string2(long t){		// implementation my own to_string function
	if (t == 0) return "0";
	
	string a = "";
	bool negative = false;
	
	if (t < 0){
		negative = true;
		t = -t;
	}
	
	while (t){
		a = (char)(t % 10 + 48) + a;
		t /= 10;
	}
	if (negative) a = "-" + a;
	
	return a;
}

struct qnode{		// queue node
	token* data;	// holds token as data
	qnode* next;
	
	qnode(token* t){
		data = t;
		next = NULL;
	}
};

struct queue{
	qnode* head;
	qnode* tail;
	
	queue(){
		head = NULL;
		tail = NULL;
	}
	
	bool is_empty(){
		return (head == NULL);
	}
	
	void push(qnode* q){
		if (head == NULL)
			head = tail = q;
		else{
			tail->next = q;
			tail = q;
		}
	}
	
	qnode* pop(){
		qnode* temp = head;
		head = head->next;
		return temp;
	}
}* answers;					// global queue pointer that holds answers


struct token{
	string data;
	token* prev;			// for stack operations
	int type;				// type of token (ex: number, uniary op, binary op...)
	bool stackable;			// true: we need to push the token to stack
	long value;
	bool is_ans;			// true: the token is actually ans
	string ans_data;		// ans -> "ans", -ans -> "-ans"
	
	token(const char* operation){
		prev = NULL;
		typecast(operation);
	}
	
	void typecast(const char* operation){
		data = operation;
		stackable = true;
		is_ans = false;
		
		if (data == "abs" || data == "sqrt")
			type = unary;
		
		else if (data == "div" || data == "sub")
			type = binary;
		
		else if (data == "exp" || data == "log" || data == "mod")
			type = ubinary;
		
		else if (data == "sum" || data == "product" || data == "min" || data == "max")
			type = polynary;
		
		else if (data == "(")
			type = leftparen;
		
		else if (data == ")"){
			type = rightparen;
			stackable = false;
		}
		
		else if (data == "ans" || data == "-ans"){
			is_ans = true;
			ans_data = data;
			
			if (answers->tail == NULL)		// if token is ans and there is no answer before
				type = ERROR;
			else {
				type = answers->tail->data->type;
				value = answers->tail->data->value;
				data = answers->tail->data->data;
			}
			
			if (*operation == '-'){
				value *= -1;
				if (data.at(0) == '-')
					data.erase(data.begin());
				else
					data = "-" + data;
			}
			stackable = false;
		}
		
		else if (data == "inf" || data == "-inf"){
			type = inf;
			stackable = false;
		}
		
		else{
			bool check = true;
			
			for (int i = (data[0] == '-'); i < data.length(); i++)
				check &= isdigit(data[i]);		// check if all elements of data are digits
			
			if (check && strlen(operation)) type = number;
			if (data == "" || data == "-" || !check) type = ERROR;
			stackable = false;
		}
		calc();			// calculate the value of token
	}
	
	bool is_number(){
		return (type == number || type == inf);
	}
	
	bool calc(){
		if (type == number) value = atol(data.c_str());
		else if (type == inf) value = LONG_MAX;
		else return false;
		
		if (type == inf && data.at(0) == '-') value *= -1;
		return true;
	}
};

struct stack{
	token* head;
	int length;
	
	stack(){
		head = NULL;
		length = 0;
	}
	
	void push(token* t){
		t->prev = head;
		head = t;
		length++;
	}
	
	token* top(){
		return head;
	}
	
	token* pop(){
		token* ret = head;
		head = head->prev;
		length--;
		return ret;
	}
	
	bool is_empty(){
		return !length;
	}
};

struct expression{
    string data;			// the original data of expression
	token** prefix;			// token pointer to pointer: holds prefix token pointers
	token** postfix;		// token pointer to pointer: holds postfix token pointers
	
	int tokensNumber;
	token* result;			// holds result: it is actually a token and is able to hold inf data vs.
	bool valid;				// true: the expression is valid
	
    expression(const char* command){	// constructor: takes the expression as a parameter
        data = command;					// set the original data
		prefix = new token* [100];
		postfix = new token* [100];
		
		tokensNumber = 0;
		
		strip();						// removes leading and trailing whitespace characters of data (space and return carriage)
		split();						// splits the data according to space character and puts them to prefix array rankly
		valid = true;					// assume the expression is valid
		convert();						// convert the prefix expression to postfix expression + set valid false if the conversion fails
    }
	
	~expression(){						// deconstructer: some deletion operations
		delete [] prefix;
		delete [] postfix;
	}
	
	bool evaluate(){
		if (!parencheck() || !valid) return false;
		
		stack s;
		
		for (int i=0; i < tokensNumber; i++){
			if (postfix[i]->type == ERROR) return false;
			
			if (postfix[i]->type == rightparen){	// -*- rightparen -*-
				if (s.is_empty())
					return false;
				
				else{
					if (s.is_empty())
						return false;
					
					token* t = new token(s.pop()->data.c_str());
					
					if (t->is_number() && s.top()->type == leftparen){
						s.pop();
						s.push(t);
					}
				}
			}
			
			
			else if (!postfix[i]->stackable || postfix[i]->type == leftparen)
				s.push(postfix[i]);								// -*- number + inf + leftparen -*-
			
			else if (postfix[i]->type == unary){				// -*- unary operators -*-
				if (s.top()->is_number()){
					token* t = new token("");
					*t = *s.pop();
					
					if (postfix[i]->data == "abs"){				// -*- abs: absolute -*-
						if (t->data.at(0) == '-')
							t->data.erase(t->data.begin());
					}
					
					else{										//-*- sqrt: square root -*-
						if (t->data.at(0) == '-') return false;
						if (t->type != inf)
							t->data = to_string2((long)sqrt(t->value));
					}
					s.push(t);
				}
				
				else return false;
			}
			
			else if (postfix[i]->type == binary){				// -*- binary operators -*-
				if (s.is_empty() || !s.top()->is_number()) return false;
				token* first = new token("");
				*first = *s.pop();
				
				if (s.is_empty() || !s.top()->is_number()) return false;
				token* second = new token("");
				*second = *s.pop();
				
				token* t = new token("");
				
				if (!first->calc() || !second->calc()) return false;
				
				if (postfix[i]->data == "sub"){					// -*- sub: substraction -*-
					if (second->type == inf){
						if ( first->type != inf ||
							(second->value / first->value < 0)){
							
							*t = *second;
						}
						else return false;
					}
					
					else if (first->type == inf){
						if (first->data.at(0) -= '-') t = new token("-inf");
						else t = new token("inf");
					}
					
					else t = new token(to_string2(second->value-first->value).c_str());
				}
				
				else{											//-*- div: division -*-
					if (first->value == 0) return false;
					
					if (second->type == inf){
						if (first->type != inf){
							if (second->value / first->value < 0) t = new token("-inf");
							else t = new token("inf");
						}
						else return false;
					}
					
					else if (first->type == inf) t = new token("0");
					else t = new token(to_string2(second->value / first->value).c_str());
				}
				
				s.push(t);
				delete second;
				delete first;
			}
			
			else if (postfix[i]->type == ubinary){				// -*- ubinary operators -*-
				if (s.is_empty() || !s.top()->is_number()) return false;
				token* first = new token("");
				*first = *s.pop();
				
				token* second = new token("");
				if (!s.is_empty() && s.top()->is_number()){
					*second = *s.pop();
				}
				
				token* t = new token("");
				
				if (second->type == ERROR){						// -*- meaning: unary operator -*-
					if (postfix[i]->data == "exp"){				// -*- exp: exponentiation -*-
						if (first->type == inf){
							if (first->value < 0) t = new token("0");
							else t = new token("inf");
						}
						else
							t = new token(to_string2((long)exp(first->value)).c_str());
					}
					
					else if (postfix[i]->data == "log"){		// -*- log: logarithm -*-
						if (first->value <= 0) return false;
						
						if (first->type == inf)
							t = new token("inf");
						else
							t = new token(to_string2((long)log(first->value)).c_str());
					}
					
					else if (postfix[i]->data == "mod"){
						if (first->type == inf)
							t = new token(first->data.c_str());
						else
							t = new token(to_string2(first->value % 10).c_str());
					}
				}
				
				else{									// -*- meaning: binary operator -*-
					if (postfix[i]->data == "exp"){		// -*- exp: exponentiation -*- :: (second = base, first = exponent)
						if (first->type != inf && second->type != inf)
							t = new token(to_string2((long)pow(second->value, first->value)).c_str());
				
						else if (first->type == inf && first->value < 0)	 // a ** -inf
							t = new token("0");
						
						else if (first->type == inf)						 // a ** inf
							t = new token("inf");
						
						else if (second->type == inf){						 // inf ** a
							if (first->value == 0) return false;			 // inf ** 0
							
							if (first->value & 1)	// if exponent is odd	 // ((+-) inf ) ** odd
								t = new token(second->data.c_str());
							else					// even
								t = new token("inf");						 // ((+-) inf ) ** even = inf
						}
					}
					
					else if (postfix[i]->data == "log"){	// -*- log: logarithm -*- :: (second = base, first exponent)
						if (second->value <= 1) return false;
						
						if (first->value == 0)
							t = new token("inf");
						
						else if (first->type == inf)
							t = new token(first->data.c_str());
						
						else if (second->type == inf) return false;
						
						else
							t = new token(to_string2((long)(log(first->value) / log(second->value))).c_str());
					}
					
					else if (postfix[i]->data == "mod"){	// -*- mod: modulo -*- :: (first % second)
						if (second->value == 0) return false;
						if (first->type == inf)
							t = new token("inf");
						else if (second->type == inf)
							return false;
						else
							t = new token(to_string2(first->value % second->value).c_str());
					}
				}
				
				s.push(t);
				delete first;
				delete second;
			}
			
			else if (postfix[i]->type == polynary){						// -*- polynary operators -*-
				
				token* t = new token("0");
				token* walk = new token("");
				
				if (postfix[i]->data == "product") t->typecast("1");
				if (postfix[i]->data == "min") t->typecast("inf");
				if (postfix[i]->data == "max") t->typecast("-inf");
				
				if (s.is_empty() || !s.top()->is_number()) return false;
				
				while (!s.is_empty() && s.top()->is_number()){
					*walk = *s.pop();
					
					if (postfix[i]->data == "sum"){						// -*- sum: summation -*-
						if (walk->type == inf && t->type == inf && walk->value / t->value < 0)
							return false;
						if (t->type != inf && walk->type == inf)
							t->typecast(walk->data.c_str());
						else if (t->type != inf)
							t->typecast(to_string2(t->value + walk->value).c_str());
					}
					
					else if (postfix[i]->data == "product"){			// -*- product -*-
						if (t->value == 0 && walk->type == inf) return false;
						if (t->type == inf && walk->value == 0) return false;
						
						if (t->value != 0){
							if (t->type == inf && t->value / walk->value < 0)
								t->typecast("-inf");
							
							else if (t->type == inf)
								t->typecast("inf");
							
							else if (walk->type == inf){
								if (walk->value / t->value > 0)
									t->typecast("inf");
								else
									t->typecast("-inf");
							}
							else
								t->typecast(to_string2(t->value * walk->value).c_str());
						}
					}
					
					else if (postfix[i]->data == "max"){				// -*- max: maximum -*-
						if (t->value < walk->value)
							*t = *walk;
					}
					
					else if (postfix[i]->data == "min"){				// -*- min: minimum -*-
						if (t->value > walk->value)
							*t = *walk;
					}
				}
				
				s.push(t);
			}
			
			else
				return false;
			
		}
		
		if (s.is_empty() || !s.top()->is_number() || s.length != 1) return false;
		
		result = s.pop();
		result->calc();
		qnode* q = new qnode(result);
		answers->push(q);
		return true;
	}
	
	void convert(){			// conversion prefix to postfix
		stack s;
		int k = 0;
		for (int i=0; i < tokensNumber; i++){
			if (prefix[i]->type == ERROR){				// token: ERROR (invalid)
				valid = false;
				return;
			}
			if (prefix[i]->stackable){					// token: operators + leftparen
				s.push(prefix[i]);
				if (prefix[i]->type == leftparen)
					postfix[k++] = prefix[i];
			}
			
			else if (prefix[i]->is_number())			// token: number + inf
				postfix[k++] = prefix[i];
			
														// token: rightparen
			else if (prefix[i]->type == rightparen){
				while (!s.is_empty() && s.top()->type != leftparen)
					postfix[k++] = s.pop();
				if (!s.is_empty()) s.pop();
				
				postfix[k++] = prefix[i];
			}
		}
		
		while (!s.is_empty())
			postfix[k++] = s.pop();
	}
	
	void strip(){			// remove leading and trailing whitespace charatecters of expression (space + return carriage)
		if (data.length() == 0) return;
		
		while (data.at(0) == ' ' || data.at(0) == '\r')		// strip from beginning
			data.erase(data.begin());
															// strip from end
		while (data.at(data.length()-1) == ' ' || data.at(data.length()-1) == '\r')
			data.erase(data.end()-1);
	}
	
	void split(){			// split the expression
		int index = 0;
		unsigned long left, right;
		unsigned long length = data.length();
		
		left = right = data.find(" ");
		token* tk = new token(data.substr(0, right).c_str());
		prefix[index++] = tk;
		
		while (left < length){
			right = data.find(" ", left+1);
			if (right-left-1 > 0){
				token* tk = new token(data.substr(left+1, right-left-1).c_str());
				prefix[index++] = tk;
			} left = right;
		}
		tokensNumber = index;
	}
	
	bool parencheck(){	// check if the parentheses is valid
		int count = 0;
		for (int i=0; i < tokensNumber; i++){
			count += (prefix[i]->type == leftparen);
			count -= (prefix[i]->type == rightparen);
			
			if (count < 0) return false;
		}
		
		return (count == 0);
	}
};

int main(int argc, const char * argv[]) {
	answers = new queue;
	char* command = new char[100];
	
	if (argc < 2){
		printf("\nInitializing the interpreter...\n\n");
		while (1){
			printf(">> "); scanf("%[^\n]s", command); getchar();
		
			if (!strcmp(command, "exit"))
				break;
	
			expression* e = new expression(command);
			if (e->evaluate()){
				printf("\nPostfix: ");
				for (int i=0; i < e->tokensNumber; i++)
					printf("%s ", e->postfix[i]->is_ans?e->postfix[i]->ans_data.c_str(): e->postfix[i]->data.c_str());
				printf("\n");
			
				printf("%s\n\n", e->result->data.c_str());
			}
			else
				printf("\nSyntax Error!\n\n");
		
			delete e;
		}
		printf("\nTerminating the interpreter...\nAnswer queue: ");
	}
		
	else{
		FILE* buffer = fopen(argv[1], "r");
		
		while (!feof(buffer)){
			fscanf(buffer, "%[^\n]s", command); fgetc(buffer);
			
			expression* e = new expression(command);
			e->evaluate();
			
			delete e;
		}
		printf("\nAnswer queue: ");
	}
	
	
	if (answers->is_empty())
		printf("No answer exists!");
	
	qnode* walk;
	while (!answers->is_empty()) {
		walk = answers->pop();
		printf("%s %s ", walk->data->data.c_str(), answers->is_empty()?"":"->");
	}	printf("\n\n");
	
	
	delete [] command;
	delete [] answers;
    return 0;
}