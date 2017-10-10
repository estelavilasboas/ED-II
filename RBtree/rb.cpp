#include <stdio.h>
#include <stdlib.h>

#define BLACK true
#define RED false

struct node{
	bool color;
	node * left, *right, *parent;
	int key;
};

struct tree{
	node *root;
	node *nil;
};


tree * start(){
	tree* newTree = (tree *)malloc(sizeof(tree));
	newTree->nil = (node *)malloc(sizeof(node));

	newTree->nil->parent = NULL;
	newTree->nil->left = NULL;
	newTree->nil->right = NULL;

	newTree->root = newTree->nil;
	return newTree;
}


void leftRotate(tree *t, node *n){
	node *a, *b;

	a = n;
	b = n->right;

	b->parent = a->parent;
	a->right = b->left;

	if(a->right != t->nil)
		a->right->parent = a;

	a->parent = b;
	b->left = a;

	if(b->parent == t->nil){
		t->root = b;
	}else{
		if(b->key < b->parent->key)
			b->parent->left = b;
		else
			b->parent->right = b;
	}
}

void rightRotate (tree *t, node *n){
	node *a, *b;

	a = n;
	b = n->left;

	b->parent = a->parent;
	a->left = b->right;

	if(a->left != t->nil)
		a->left->parent = a;

	a->parent = b;
	b->right = a;

	if(b->parent == t->nil){
		t->root = b;
	}else{
		if(b->key < b->parent->key)
			b->parent->left = b;
		else
			b->parent->right = b;
	}
}

void fixUpInsert(tree *t, node *n){
 	if(n->parent == t->nil){
		n->color = BLACK;

	}else{
		if(n->color == RED && n->parent->color == RED){
			if(n->parent->parent->left == n->parent){	//o nodo a esquerda do avô é o pai
				if(n->parent->parent->right->color == RED && n->parent->parent->right != t->nil){		//Caso 1: o tio é vermelho
					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					n->parent->parent->right->color = BLACK;
					fixUpInsert(t, n->parent->parent);
				}else{
					if(n->parent->right == n){	//Caso 2: o tio é preto e o nodo é o filho a direita de seu pai
						leftRotate(t, n->parent);
						n = n->left;
					}

					//Caso 3: o caso 2 cai no caso 3. O nodo e seu pai são vermelhos, o tio é preto.

					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					rightRotate(t, n->parent->parent);
				}

			}else{	//o nodo a direita do avô é o pai
				if(n->parent->parent->left->color == RED && n->parent->parent->left != t->nil){	//Caso 1
					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					n->parent->parent->left->color = BLACK;
					fixUpInsert(t, n->parent->parent);
				}else{
					if(n->parent->left == n){	//Caso 2
						rightRotate(t, n->parent);
						n = n->right;
					}

					//Caso 3
					n->parent->color = BLACK;
					n->parent->parent->color = RED;
					leftRotate(t, n->parent->parent);
				}
			}
		}
	}
}


void insert (tree *t, node *root, int key){
	node *n = (node *)malloc(sizeof(node));

	n->key = key;
	n->left = t->nil;
	n->right = t->nil;
	n->parent = t->nil;

	n->color = RED;

	if(root==t->nil){
		t->root = n;
		n->color = BLACK;
		fixUpInsert(t, n);
	}else{
		node *current = root;

		//encontra lugar do nodo
		while(1){
			if(key < current->key){
				if(current->left == t->nil){
					current->left = n;
					n->parent = current;
					break;
				}
				current = current->left;
			}else{
				if(current->right == t->nil){
					current->right = n;
					n->parent = current;
					break;
				}
				current = current->right;
			}
		}

		fixUpInsert(t, n);
	}

}

void deleteFixUp(tree *t, node *n){
	node *aux;

	while (t->root != n && n->color == BLACK){
		if(n == n->parent->left){	// se for o filho a esquerda
			aux = n->parent->right;

			if(aux->color == RED){	// o irmão do nodo é vermelho CASO 2
				aux->color = BLACK;
				n->parent->color = RED;
				leftRotate(t, n->parent);
				aux = n->parent->right;

			}
			if(aux->right->color == BLACK && aux->left->color == BLACK){
				aux->color = RED;
				n = n->parent;

			}
			if(aux->right->color == BLACK && aux->left->color != BLACK){
				aux->left->color = BLACK;
				aux->color = RED;
				rightRotate(t, aux);
				aux = n->parent->right;
			}

			if(aux->color == BLACK && aux->right->color == RED){
				aux->color = n->parent->color;
				n->parent->color = BLACK;
				aux->right->color = BLACK;
				leftRotate(t, n->parent);
				n = t->root;
			}
		}
		n->color = BLACK;
	}

}

void deleteNode(tree *t, node* r, int key){
    if (r != t->nil){
        if(key < r->key){
            deleteNode(t, r->left, key);

        }else if (key > r->key){
            deleteNode(t, r->right, key);

        }else{
	    if(r->right == t->nil){
                node *aux = r->left;

		if(r->parent != t->nil){	//se for igual a t->nil, significa que é a raiz da árvore
		
			if(r == r->parent->right){
				r->parent->right = aux;
				if(aux != t->nil)
					aux->parent = r->parent->right;
			
			}else{
				r->parent->left = aux;
				aux->parent = r->parent->left;
			}

		}else{
			t->root = aux;
		}

                free(r);

            }else{
		node *aux = r;
    	        node *aux2 = r->right;
	
                while(aux2->left != t->nil){
			aux = aux2;
			aux2 = aux2->left;
		}
	
		if(aux!=r){
			aux->left = aux2->right;
			aux2->right = r->right;
		}

        	aux2->left = r->left;
            
		if(r->parent != t->nil){	//se for igual a t->nil, significa que é a raiz da árvore
		
			if(r == r->parent->right){
				r->parent->right = aux;
				aux->parent = r->parent->right;
			
			}else{
				r->parent->left = aux;
				aux->parent = r->parent->left;
			}

		}else{
			t->root = aux;
			aux->parent = t->nil;
		}
		    
	            free(r);

/*

	    //com dois filhos

	    //busca o mais a esquerda da subarvore direita e seu pai
		    node *aux = r->right;

		    while(aux->left != t->nil){
			aux = aux->left;
		    }

		    if(aux->parent!=r){
			aux->parent->left = aux->right;
			aux->right = r->right;
		    }

        	    aux->left = r->left;
		    aux->parent = r->parent;
		    
		    if(r->parent != t->nil){		
			if(r == r->parent->right){
				r->parent->right = aux;
				aux->parent = r->parent->right;
			
			}else{
				r->parent->left = aux;
				aux->parent = r->parent->left;
			}

		    }else{
			t->root = aux;
		    }
			r->right->parent = aux;
			r->left->parent = aux;
		    free(r);*/
	     }
        }
    }
}


void printNode(tree *t, node *n){
	printf("%d\t Parent: ", n->key);
	printf(n->parent == t->nil? "Root\t" : "%d\t", n->parent->key);
	printf("Color: ");
	printf(n->color == true? "Black\n" : "Red\n");
}

void preorder(tree *t, node *r){
    if(r != t->nil){
        printNode(t, r);
        preorder(t, r->left);
        preorder(t, r->right);
    }
}

void postorder(tree *t, node *r){
    if(r != t->nil){
        postorder(t, r->left);
        postorder(t, r->right);
        printNode(t, r);
    }
}

void inorder(tree *t, node *r){
    if(r != t->nil){
        inorder(t, r->left);
        printNode(t, r);
        inorder(t, r->right);
    }
}

void drawTree(tree * t, node * n, int h){
	if(n->right != t->nil)
		drawTree(t, n->right, h+1);

	int i;
	for(i=0; i<h; i++){
		printf("    ");
	}

	if(n->color == RED)
		printf("R-%d\n", n->key);
	else
		printf("B-%d\n", n->key);

	if(n->left != t->nil)
		drawTree(t, n->left, h+1);

}

int main(){
	tree* t = start();

	insert(t, t->root, 3);
	insert(t, t->root, 8);
	insert(t, t->root, 5);
	insert(t, t->root, 1);
	insert(t, t->root, 4);

	drawTree(t, t->root, 0);

	printf("\n\n\n");

	deleteNode(t, t->root, 8);

	drawTree(t, t->root, 0);
	printf("\n\n\n");

	deleteNode(t, t->root, 3);
	drawTree(t, t->root, 0);

}
