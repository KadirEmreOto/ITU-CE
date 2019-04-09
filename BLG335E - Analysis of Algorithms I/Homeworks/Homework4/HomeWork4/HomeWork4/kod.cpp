#include <iostream>
#include <cstdio>

#define RED true
#define BLACK false

using namespace std;


struct node {
    string name;
    char gender;
    unsigned short age;
    unsigned int man;
    unsigned int woman;
    
    node* left;
    node* right;
    node* parent;
    
    bool color;
    
    node(string name, const char gender, const unsigned short age){
        this->name = name;
        this->gender = gender;
        this->age = age;
        
        man = gender == 'M';
        woman = gender == 'F';
    
        left = right = parent = NULL;
        color = RED;
    }
    
    bool operator< (const node& other) const{
        return this->name < other.name;
    }
    
    node* uncle(){
        if (parent == NULL || parent->parent == NULL)
            return NULL;
        
        if (parent->parent->left == parent)
            return parent->parent->right;
        
        return parent->parent->left;
    }
};


class RBTree {
public:
    node* root;
    
    RBTree(){
        root = NULL;
    }
    
    string man(unsigned int x){
        return count(root, x, 'M');
    }
    
    string woman(unsigned int x){
        return count(root, x, 'F');
    }
    
    string count(node* cur, unsigned int x, char gender){
        if ((gender == 'M' ? cur->man : cur->woman) < x)
            return string("not found!");
        
        unsigned int L = 0;
        
        if (cur->left){
            if (gender == 'M')
                L += cur->left->man;
            else
                L += cur->left->woman;
        }
        
        if (L + (cur->gender == gender) == x && cur->gender == gender)
            return cur->name;
        
        if (L >= x)
            return count(cur->left, x, gender);
        
        return count(cur->right, x - L - (cur->gender == gender), gender);
    }
    
    void insert(node* toAdd){
        node** indirect = &root;
        
        while (*indirect != NULL){
            (*indirect)->man += toAdd->man;
            (*indirect)->woman += toAdd->woman;
            
            toAdd->parent = *indirect;
            
            if (*toAdd < **indirect)
                indirect = &((*indirect)->left);
            else
                indirect = &((*indirect)->right);
        }
        
        *indirect = toAdd;
        balance(toAdd);
    }
    
    void balance(node* current){
        if (current == root)
            root->color = BLACK;
        
        else if (current->parent->color == BLACK)
            return;
        
        else if (current->uncle() && current->uncle()->color == RED){
            current->parent->color = BLACK;
            current->uncle()->color = BLACK;
            current->parent->parent->color = RED;
            
            balance(current->parent->parent);
        }
        
        else{
            if (current->parent->left == current &&
                current->parent->parent->left == current->parent){
                
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                rightRotate(current->parent->parent);
            }
            
            else if (current->parent->right == current &&
                     current->parent->parent->left == current->parent){
                
                leftRotate(current->parent);
                balance(current->left);
            }
            
            else if (current->parent->right == current &&
                     current->parent->parent->right == current->parent){
                
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                leftRotate(current->parent->parent);
            }
            
            else if (current->parent->left == current &&
                     current->parent->parent->right == current->parent){
                
                rightRotate(current->parent);
                balance(current->right);
            }
        }
    }
    
    void rightRotate(node* current){  // reference of pointer because the pointer of pointer of node is needed
        unsigned int man = current->man;
        unsigned int woman = current->woman;
        
        current->man -= current->left->man;
        current->woman -= current->left->woman;
        
        current->left->man = man;
        current->left->woman = woman;
        
        node* left = current->left;
        node** subroot;  // Now we do not have to check the node is root or not
        if (current == root)
            subroot = &root;
        else
            subroot = &(current->parent->right);
        
        if (left->right)
            left->right->parent = current;
        current->left = left->right;
        
        left->right = current;
        left->parent = current->parent;
        current->parent = left;
        
        *subroot = left;
    }
    
    void leftRotate(node* current){
        unsigned int man = current->man;
        unsigned int woman = current->woman;
        
        current->man -= current->right->man;
        current->woman -= current->right->woman;
        
        current->right->man = man;
        current->right->woman = woman;
        
        node* right = current->right;
        node** subroot;  // Now we do not have to check the node is root or not
        
        if (current == root)
            subroot = &root;
        else
            subroot = &(current->parent->left);
        
        if (right->left)
            right->left->parent = current;
        current->right = right->left;
        
        right->left = current;
        right->parent = current->parent;
        current->parent = right;
        
        *subroot = right;
    }
    
    void display(){
        display(root);
    }
    
    void display(node* cur, int depth=0, int state=0){
        if (cur->left)
            display(cur->left, depth + 1, 1);
        
        for (int i=0; i < depth; i++)
            printf("     ");
        
        if (state == 1) // left
            printf("┌───");
        else if (state == 2)
            printf("└───");
        
        printf("(%c)%s-%d-%c\n", cur->color==RED?'R':'B', cur->name.c_str(), cur->age, cur->gender);
        
        if (cur->right)
            display(cur->right, depth + 1, 2);
    }
    
};


int main(int argc, const char * argv[]) {
    RBTree tree;
    
    node* n1 = new node("Glen", 'F', 29);
    node* n2 = new node("Ryan", 'F', 17);
    node* n3 = new node("Alex", 'M', 13);
    node* n4 = new node("Dane", 'F', 14);
    node* n5 = new node("Leah", 'F', 23);
    node* n6 = new node("Jude", 'F', 26);
    node* n7 = new node("Evan", 'M', 18);
    node* n8 = new node("Izzy", 'M', 27);
    node* n9 = new node("Fran", 'M', 30);
    
    tree.insert(n1);
    tree.insert(n2);
    tree.insert(n3);
    tree.insert(n4);
    tree.insert(n5);
    tree.insert(n6);
    tree.insert(n7);
    tree.insert(n8);
    tree.insert(n9);
    tree.display();
    
    printf("\n3rd woman: %s\n", tree.woman(3).c_str());
    printf("4th man: %s\n", tree.man(4).c_str());
    
    return 0;
}
