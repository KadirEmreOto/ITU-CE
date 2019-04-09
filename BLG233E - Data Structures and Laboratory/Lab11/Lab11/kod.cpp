#include <cstdio>
#include <cstring>
#include <cstdlib>

struct number{
    char* data;
    number* next;

    number(const char* d){
        next = NULL;
        data = new char[strlen(d)];
        strcpy(data, d);
    }

    ~number(){
        delete [] data;
		if (next) delete next;
		next = NULL;
    }
};

struct contact{
    char* name;
    number* no;
    contact* left;
    contact* right;

    contact(const char* n){
        name = new char[strlen(n)];
        strcpy(name, n);

        no = NULL;
        left = NULL;
        right = NULL;
    }

    ~contact(){
        delete [] name;
		if (no) delete no;
        if (left) delete left;
        if (right) delete right;

		no = NULL;
        left = NULL;
        right = NULL;
    }

    void add(contact* c){
        int cmp = strcmp(c->name, name);

        if (cmp < 0){
            if (left == NULL) left = c;
            else left->add(c);
        }

        else if (cmp > 0){
            if (right == NULL) right = c;
            else right->add(c);
        }
    }

    void add_number(const char* n){
        number** indirect = &no;

        while ((*indirect))
            indirect = &((*indirect)->next);

        number* node = new number(n);
        *indirect = node;
    }

    void update_name(const char* n){
        delete [] name;
        name = new char [strlen(n)];
        strcpy(name, n);
    }

    void print(const int all){
        if (left && all) left->print(all);

        printf("Name: %s\n", name);
        number* walk = no;
        while (walk){
            printf("\t> %s\n", walk->data);
            walk = walk->next;
        }

        if (right && all) right->print(all);
    }
};

struct tree{
    contact* root;

    tree(){
        root = NULL;
    }

    ~tree(){
        if (root) delete root;
        root = NULL;
    }

    void add_contact(const char* name){
        contact* node = new contact(name);

        if (root == NULL) root = node;
        else root->add(node);
    }

    contact* search(const char* name){
        contact* walk = root;

        int cmp = 1;
        while (walk != NULL && cmp != 0){
            cmp = strcmp(name, walk->name);
            if (cmp < 0) walk = walk->left;
            if (cmp > 0) walk = walk->right;
        }

        return walk;
    }

    void remove_contact(const char* name){
        contact* walk = root;
        if (!walk) return;

        int cmp = 1;
        while (walk && cmp){
            cmp = strcmp(name, walk->name);
            if (cmp < 0){
                if (strcmp(name, walk->left->name) == 0)
                    break;
                walk = walk->left;
            }
            else if (cmp > 0){
                if (strcmp(name, walk->right->name) == 0)
                    break;
                walk = walk->right;
            }
        }

        if (cmp < 0){
            contact* l = walk->left->left;
            contact* r = walk->left->right;

            walk->left = NULL;
            if (l) root->add(l);
            if (r) root->add(r);
        }

        else if (cmp > 0){
            contact* l = walk->right->left;
            contact* r = walk->right->right;

            walk->right = NULL;
            if (l) root->add(l);
            if (r) root->add(r);
        }

        else {
            contact* l = root->left;
            contact* r = root->right;

            root = l;
            if (!root && r) root = r;
            if (root != r) root->add(r);
        }
    }

    void update(const char* name, const char* new_name){
        contact* c = search(name);
        remove_contact(name);

        c->update_name(new_name);
        c->left = NULL;
        c->right = NULL;
        if (root) root->add(c);
        else root = c;
    }

    void print(){
        if (root) root->print(1);
        else printf("No record found");
    }
};

void display(){
    system("clear");

    printf("A: Add Contact\n");
    printf("N: Add Number\n");
    printf("U: Update Contact\n");
    printf("S: Search Contact\n");
	printf("R: Remove Contact\n");
    printf("P: Print All Contacts\n");
    printf("E: Exit\n\n");

    printf("[choice]: ");
}

int main(int argc, const char * argv[]) {
    tree* t = new tree;
    char choice = 'x';

    while (choice != 'e' && choice != 'E'){
        display(); choice = getchar(); getchar();

        if (choice == 'a' || choice == 'A'){
            char name[100]; printf("\nName: ");
            scanf("%[^\n]s", name); getchar();
			contact* c = t->search(name);

			if (c) printf("Contact already exist!\n");
            else {
				t->add_contact(name);
				c = t->search(name);
			}

			char phone[100] = {'\0'}; printf("Number: ");
			scanf("%[^\n]s", phone); getchar();
			if (*phone == '\0') continue;
			c->add_number(phone);
        }

        else if (choice == 'n' || choice == 'N'){
            char name[100]; printf("\nName: ");
            scanf("%[^\n]s", name); getchar();
            contact* c = t->search(name);

            if (c){
                char phone[100]; printf("Number: ");
                scanf("%[^\n]s", phone); getchar();
                c->add_number(phone);
            }
            else{
                printf("Record not found!! "); getchar();
            }
        }

        else if (choice == 'u' || choice == 'U'){
            char name[100]; printf("\nName: ");
            scanf("%[^\n]s", name); getchar();
            contact* c = t->search(name);

            if (c){
                char new_name[100]; printf("New name: ");
                scanf("%[^\n]s", new_name); getchar();
                t->update(name, new_name);
            }
            else{
                printf("Record not found!! "); getchar();
            }

        }

		else if (choice == 'r' || choice == 'R'){
            char name[100]; printf("\nName: ");
            scanf("%[^\n]s", name); getchar();
			contact* c = t->search(name);

			if (c) t->remove_contact(name);
			else { printf("Record not found!! "); getchar(); }
		}

        else if (choice == 's' || choice == 'S'){
            char name[100]; printf("\nName: ");
            scanf("%[^\n]s", name); getchar();
            contact* c = t->search(name);

            if (c) { c->print(0); getchar(); }
            else { printf("Record not found!! "); getchar(); }
        }

        else if (choice == 'p' || choice == 'P'){
            t->print(); getchar();
        }
    }

    delete t;
    return 0;
}
