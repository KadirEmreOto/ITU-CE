#include <cstdio>
#include <cstdlib>
#include <ctime>
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

	void removeTree(){
		removeNode(root);
	}

	void removeNode(tnode* tn){
		if (tn->left != NULL) removeNode(tn->left);
		if (tn->right != NULL) removeNode(tn->right);

		delete tn;
		tn = NULL;
	}

	void printPreorder(){
		printf("Preorder:  ");
		printNode(root, 2);
		printf("\n");
	}

	void printInorder(){
		printf("Inorder:   ");
		printNode(root, 1);
		printf("\n");
	}

	void printPostorder(){
		printf("Postorder: ");
		printNode(root, 3);
		printf("\n");
	}

	void printNode(tnode* tn, const int order){
		if (order == 2)// pre order
			printf("%d ", tn->value);

		if (tn->left != NULL)
			printNode(tn->left, order);

		if (order == 1)// in order
			printf("%d ", tn->value);

		if (tn->right != NULL)
			printNode(tn->right, order);

		if (order == 3)// post order
			printf("%d ", tn->value);
	}

	int findMax(){
		return findminmax(root, 2);
	}

	int findMin(){
		return findminmax(root, 1);
	}

	int findminmax(tnode* tn, int what){
		if (tn->left == NULL && tn->right == NULL)
			return tn->value;

		int answer = tn->value;
		if (tn->left != NULL){
			if (what == 1) // min
				answer = min(answer, findminmax(tn->left, what));
			else // max
				answer = max(answer, findminmax(tn->left, what));
		}

		if (tn->right != NULL){
			if (what == 1) // min
				answer = min(answer, findminmax(tn->right, what));
			else // max
				answer = max(answer, findminmax(tn->right, what));
		}

		return answer;
	}

	int findNumNode(){
		return findNumberNode(root);
	}

	int findNumberNode(tnode* tn){
		int answer = 1;
		if (tn->left != NULL) answer += findNumberNode(tn->left);
		if (tn->right != NULL) answer += findNumberNode(tn->right);
		return answer;
	}

	int findNumLeaf(){
		return findNumberLeaf(root);
	}

	int findNumberLeaf(tnode* tn){
		if (tn->left == NULL && tn->right == NULL){
			return 1;
		}

		int answer = 0;
		if (tn->left != NULL) answer += findNumberLeaf(tn->left);
		if (tn->right != NULL) answer += findNumberLeaf(tn->right);
		return answer;
	}

	int calculateDepth(){
		return findDepth(root);
	}

	int findDepth(tnode* tn){
		if (tn->left != NULL) return findDepth(tn->left) + 1;
		return 1;
	}

	int calculateSum(){
		return findSumNode(root);
	}

	int findSumNode(tnode* tn){
		int answer = tn->value;
		if (tn->left != NULL) answer += findSumNode(tn->left);
		if (tn->right != NULL) answer += findSumNode(tn->right);
		return answer;
	}

	double calculateAverage(){
		double sum = (double)calculateSum();
		int number = findNumNode();
		return sum / number;
	}
} *t;

int main(int argc, char const *argv[]) {
	srand((unsigned int)time(NULL));
	t = new tree;

	t->createTree(15); printf("\n");

	t->printPreorder();
	t->printInorder();
	t->printPostorder();printf("\n");

	printf("Min: %d \n", t->findMin());
	printf("Max: %d \n", t->findMax());
	printf("Depth: %d\n\n", t->calculateDepth());

	printf("Number of leaf: %d\n", t->findNumLeaf());
	printf("Number of node: %d\n\n", t->findNumNode());

	printf("Sum: %d\n", t->calculateSum());
	printf("Average: %f\n", t->calculateAverage());

	return 0;
}
