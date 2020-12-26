#include <iostream>
#include <cstring>
#include <string>

using namespace std;

struct token{
	string data;
	token* prev;
	
	token(const char* operation){
		prev = NULL;
		data = operation;
	}
	
	int is_operator(){
		if (data == "abs" || data == "sqrt") return 1;	// unary
		if (data == "div" || data == "sub") return 2;	// binary
		if (data == "exp" || data == "log"||			// unary or binary
			data == "mod") return 3;
		if (data == "sum" || data == "product" ||		// polynary
			data == "min" || data == "max") return 4;
		if (data == "(") return 5;						// paranthesis
		if (data == ")") return 6;
		
		return 0;										// operand (number)
	}
};

struct expression{
    string data;
	token** prefix;
	token** postfix;
	expression** subexp;
	int tokensNumber;
	int expNumber;
	
    expression(const char* command){
        data = command;
		subexp = new expression* [100];
		prefix = new token* [100];
		postfix = new token* [100];
		
		tokensNumber = 0;
		expNumber = 0;
		
		strip();
		split();
		
		find_sub();
		convert();
    }
	
	~expression(){
		delete [] subexp;
		delete [] prefix;
		delete [] postfix;
	}
	
	void evaluate(){
		return;
	}
	
	void convert(){
		// conversion from prefix to postix
		if (tokensNumber == 1) {
			postfix[0] = prefix[0];
			return;
		}
		
		if (expNumber == 1 && prefix[0]->is_operator() == 5){
			postfix[0] = prefix[0];
			for (int i=0; i < subexp[0]->tokensNumber; i++)
				postfix[i+1] = subexp[0]->postfix[i];
			postfix[tokensNumber-1] = prefix[tokensNumber-1];
			return;
		}
		
		int index = 0;
		for (int i=0; i < expNumber; i++){
			for (int j=0; j < subexp[i]->tokensNumber; j++){
				postfix[index++] = subexp[i]->postfix[j];
			}
			postfix[index] = prefix[0];
		}
	}
	
	void find_sub(){
		int index = 0;
		int i = 1, end;
		
		if (data.at(0) == '('){
			expression* e = new expression(data.substr(1, data.length()-2).c_str());
			expNumber = 1;
			subexp[0] = e;
			return;
		}
		
		while (i < tokensNumber){
			end = find_scope(i);
			string subdata;
			for (int j=i; j < end; j++){
				if (j == tokensNumber-1 && prefix[0]->is_operator() == 5 &&
					prefix[tokensNumber-1]->is_operator() == 6) continue;
				subdata += prefix[j]->data + " ";
			}
			expression* e = new expression(subdata.c_str());
			subexp[index++] = e;
			i = find_scope(i);
		}
		expNumber = index;
	}
	
	int find_scope(int begin){
		if (begin >= tokensNumber) return begin;
		
		token* t = prefix[begin];
		if (t->is_operator() == 0) return begin + 1;
		if (t->is_operator() == 1) return find_scope(begin+1);
		if (t->is_operator() == 2) return find_scope(find_scope(begin+1));
		if (t->is_operator() == 3) return find_scope(find_scope(begin+1));
		if (t->is_operator() == 5){
			int count = 1, index = begin+1;
			token* n;
			while (count != 0){
				n = prefix[index];
				if (n->is_operator() == 5) count++;
				if (n->is_operator() == 6) count--;
				index++;
			}
			return index;
		}
		return tokensNumber;
	}
	
	void strip(){
		if (data.length() == 0) return;
		
		while (data.at(0) == ' ')
			data.erase(data.begin());
		
		while (data.at(data.length()-1) == ' ')
			data.erase(data.end()-1);
	}
	
	void split(){
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
	
	token* pop(){
		token* r = head;
		head = head->prev;
		length--;
		return r;
	}
	
	bool is_empty(){
		return !length;
	}
};

struct qnode{
	// queue node
	
	long value;
	qnode* next;
	
	qnode(int v){
		value = v;
		next = NULL;
	}
};

struct queue{
	qnode* head;
	
	queue(){
		head = NULL;
	}
	
	bool is_empty(){
		return (head == NULL);
	}
	
	void push(qnode* q){
		// The "indirect" pointer points to
		// the address of the node we will update
		
		qnode** indirect = &head;
		
		while ((*indirect) != NULL)
			indirect = &(*indirect)->next;
		
		*indirect = q;
	}
	
	qnode* pop(){
		qnode* temp = head;
		head = head->next;
		return temp;
	}
};

int main(int argc, const char * argv[]) {
	expression* e = new expression("sum 1 2 product 3 4 5");
	e->convert();
	for (int i=0; i < e->tokensNumber; i++)
		printf("%s ", e->postfix[i]->data.c_str());
	printf("\n");
	
    return 0;
}
