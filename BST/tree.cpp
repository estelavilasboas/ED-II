/*
    ÁRVORE BINÁRIA DE BUSCA - BST
*/

#include <stdio.h>
#include <stdlib.h>

struct node{
	int key;
	node *left, *right;
};

node *searchNode (node *r, int key){
	if(r==NULL|| key == r->key) 
		return r;

	if(key <= r->key) 
		return searchNode(r->left, key);
		
	if(key >= r->key)
    	return searchNode (r->right, key);
}

node *insert (node *r, int key){
	node *n = (node *)malloc(sizeof(node));
	
	n->key = key;
	n->left = NULL;
	n->right = NULL;

	if(r==NULL)
	    return n;

	node *current = r;				

	while(1){
		if(key < current->key){
			if(current->left==NULL){
				current->left=n;
				return r;
			}
			current = current->left;
		}else{
			if(current->right==NULL){
				current->right = n;
				break;
			}
			current = current->right;
		}
	}
	return r;
}

node * menorValor(node* n){
    node *current = n;
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

node * maiorValor(node *n){
    node *current = n;
    while (current->right !=NULL)
        current = current->right;
    
    return current;
}

node* remove(node* r, int key){
    if (r == NULL){
        return r;
        
    }else{
        if(key < r->key){
            r->left = remove(r->left, key);
            
        }else if (key > r->key){
            r->right = remove(r->right, key);
            
        }else{
            //com um ou nenhum filho
            if(r->left == NULL){
                node *aux = r->right;
                free(r);
                return aux;
                
            }else if(r->right == NULL){
                node *aux = r->left;
                free(r);
                return aux;
            }
            
	    //com dois filhos

	    //busca o mais a esquerda da subarvore direita e seu pai
	    node *aux = r;
            node *aux2 = r->right;

	    while(aux2->left != NULL){
		aux = aux2;
		aux2 = aux2->left;
	    }

	    if(aux!=r){
		aux->left = aux2->right;
		aux2->right = r->right;
	    }
            aux2->left = r->left;
            r->right = remove(r->right, aux->key);
            
        }
        return r;
    }
    
}

void preorder(node *r){
    if(r!=NULL){
        printf("%d\n", r->key);
        preorder(r->left);
        preorder(r->right);
    }
}

void postorder(node *r){
    if(r!=NULL){
        postorder(r->left);
        postorder(r->right);
        printf("%d\n", r->key);
    }
}

void inorder(node *r){
    if(r!=NULL){
        inorder(r->left);
        printf("%d\n", r->key);
        inorder(r->right);
    }
}

int main(){
	node *root = NULL;

	root = insert(root, 5);
	root = insert(root, 1);
	root = insert(root, 4);
	root = insert(root, 10);

    	printf("Inorder:\n");
	inorder(root);
	printf("\nPostorder:\n");
	postorder(root);
	printf("\nPreorder:\n");
	preorder(root);
	
	node *n = maiorValor(root);
	printf("\nMaior Valor: %d", n->key);
	n = menorValor(root);
	printf("\nMenor Valor: %d\n", n->key);
	
	root = remove(root, 4);
	printf("\nRemove 4:\n");
	inorder(root);

	root = remove(root, 1);
	printf("\nRemove 1:\n");
	inorder(root);
	
	printf("\nRemove 5:\n");
	root = remove(root, 5);
	inorder(root);

	return 0;
}
