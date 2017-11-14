#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

struct tnode{
	int value;
	tnode* left;
	tnode* right;

	tnode(){
		left = NULL;
		right = NULL;
	}
};

struct qnode{
	tnode* value;
	qnode* next;

	qnode(){
		next = NULL;
	}
};

struct queue{
	qnode* head;

	queue(){
		head = NULL;
	}

	bool is_empty(){
		return head == NULL;
	}

	void add(qnode* n){
		if (head == NULL){
			head = n;
			return;
		}

		qnode* cur;
		for (cur=head; cur->next; cur=cur->next);
		cur->next = n;
	}

	qnode* pop(){
		qnode* c = head;
		head = head->next;
		return c;
	}
};

struct stack{
	tnode* array[1000];
	int cursor;

	stack(){
		cursor = 0;
	}

	void push(tnode* t){
		array[cursor++] = t;
	}

	tnode* pop(){
		return array[--cursor];
	}
};

struct tree{
	tnode* root;

	tree(){
		root = NULL;
	}

	void add(tnode* tn){
		if (root == NULL){
			root = tn;
			return;
		}

		qnode* qn = new qnode;
		qn->value = root;

		queue* que = new queue;
		que->add(qn);

		do{
			qnode* cur = que->pop();
			if (cur->value->left == NULL){
				cur->value->left = tn;
				return;
			}

			if (cur->value->right == NULL){
				cur->value->right = tn;
				return;
			}

			qnode* ln = new qnode;
			qnode* rn = new qnode;

			ln->value = cur->value->left;
			rn->value = cur->value->right;

			que->add(ln);
			que->add(rn);

			delete cur;
		}while (!que->is_empty());
	}

	void createTree(const int size){
		int* ar = new int[size];
		for (int i=0; i < size; i++)
			ar[i] = (rand() % size) + 1;

		printf("Array:     ");
		for (int i=0; i < size; i++){
			tnode* tn = new tnode;
			tn->value = ar[i];
			printf("%d ", ar[i]);

			add(tn);
		}
		delete [] ar;
		printf("\n");
	}

	int BFS(int num){
		qnode* qn = new qnode;
		qn->value = root;

		queue* que = new queue;
		que->add(qn);
		int count = 1;
		do{
			qnode* cur = que->pop();
			count++;

			if (cur->value->value == num) return count ;

			if (cur->value->left){
				qnode* ln = new qnode;
				ln->value = cur->value->left;
				que->add(ln);
				count++;
			}

			if (cur->value->right){
				qnode* rn = new qnode;
				rn->value = cur->value->right;
				que->add(rn);
				count++;
			}
			printf("B: %d ", cur->value->value);

		}while (!que->is_empty());


		return -1;
	}

	int DFS(int num){
		stack* s = new stack;
		s->push(root);

		tnode* cur;
		int count = 1;
		while (s->cursor > 0){
			cur = s->pop();
			count++;

			if (cur->value == num) return count;

			if (cur->right) {
				s->push(cur->right);
				count++;
			}

			if (cur->left){
				s->push(cur->left);
				count++;
			}
			printf("D: %d\n", cur->value);
		}
		return -1;
	}


	int Inorder(int num){
		int count = 0;
		bool found = false;
		printNode(root, num,  &count, &found);
		if (!found) return -1;
		return count;
	}

	void printNode(tnode* tn, int num,  int *c, bool *f){
		if (*f) return;
		(*c)++;

		if (tn->left != NULL)
			printNode(tn->left, num, c, f);

		if (tn->value == num) *f = true;
		printf("%d " , tn->value);

		if (tn->right != NULL)
			printNode(tn->right, num, c, f);

		if (!(*f)) (*c)++;
	}
} *t;

int main(int argc, char const *argv[]) {
	srand((unsigned int)time(NULL));
	t = new tree;

	t->createTree(10); printf("\n");

	int n, resB, resD, resI;
	printf("Number: "); scanf("%d", &n);

	resB = t->BFS(n);
	resD = t->DFS(n);
	resI = t->Inorder(n);

	if (resB != -1){
		printf("BFS: %d \n", resB);
	}
	else
		printf("BsFS: Not found\n");

	if (resD != -1)
		printf("DFS: %d \n", resD);
	else
		printf("DFS: Not found\n");

	if (resI != -1)
		printf("Inorder: %d \n", resI);
	else
		printf("Inorder: Not found\n");


	printf("\n");

	return 0;
}
