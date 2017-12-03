
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 2
#define MAX_KIDS (2*T)
#define MAX_KEY (MAX_KIDS - 1) 
#define MIN_OCUP (T - 1)

  
typedef struct node {
    int *keys;
    struct node **kids;
    int numKey;
    bool isLeaf;
}Node;
  
typedef struct tree{
    struct node *root;
}Tree;


Tree *cria() {
    Tree *arv = (Tree*) malloc(sizeof(Tree));
    arv->root = NULL;
    return arv;
}
  
Node *criaNodo(Tree * arvore, Node *nodo) {
    Node *newParent = (Node*) malloc (sizeof(Node));
    int *keysP = (int*) calloc ((MAX_KEY), sizeof(int));
    Node **kidsP = (Node**) calloc (MAX_KIDS, sizeof(Node));

    kidsP[0] = nodo;

    newParent->keys = keysP;
    newParent->kids = kidsP;
    newParent->numKey = 0;
    newParent->isLeaf = 0;

    arvore->root = newParent;
    return newParent;
}


  
Node *Split(Tree *arvore, Node *nodo, Node *Parent, int chave){
    if(Parent == NULL){
        Parent = criaNodo(arvore, nodo);
    }

    Node *newNode = (Node*) malloc (sizeof(Node));
    int *newKeys = (int*) calloc ((MAX_KEY), sizeof(int));
    Node **newKids = (Node**) calloc (MAX_KIDS, sizeof(Node));

    newNode->isLeaf = nodo->isLeaf;
    newNode->numKey = MIN_OCUP;
    int j;

    for (j = 0 ; j < MIN_OCUP ; j++){
        newKeys[j] = nodo->keys[j+T];
    }
    if (nodo->isLeaf == 0){
        for (j = 0 ; j < T ; j++){
            newKids[j] = nodo->kids[j+T];
        }
    }

    nodo->numKey = MIN_OCUP;
    int i;

    for (i = 0 ; Parent->kids[i] != nodo ; i++);
    for (j = Parent->numKey + 1 ; j > i + 1 ; j--){
        Parent->kids[j + 1] = Parent->kids[j];
    }

    Parent->kids[i + 1] = newNode;

    for (j = Parent->numKey ; j > i ; j--){
        Parent->keys[j + 1] = Parent->keys[j];
    }

    Parent->keys[i] = nodo->keys[MIN_OCUP];
    Parent->numKey += 1;

    newNode->keys = newKeys;
    newNode->kids = newKids;

    return Parent;
}




void insertSplit(Tree *arvore, Node *nodo, Node *Parent, int chave){ //inserir quando a àrvore não està vazia. Ocorre split.
    if (nodo->numKey == MAX_KEY){
        nodo = Split(arvore, nodo, Parent, chave);
    }
    int i, aux, aux2 = chave;
    for (i = 0 ; i < nodo->numKey ; i++){
        if(nodo->keys[i] == chave){                                 //a chave jà foi inserida
            return;
        }
    }
    if(nodo->isLeaf == 1){
        for (i = 0 ; i < nodo->numKey ; i++){
            if(nodo->keys[i] > chave){
                aux = aux2;
                aux2 = nodo->keys[i];
                nodo->keys[i] = aux;
                if(i == nodo->numKey - 1) nodo->keys[i + 1]  = aux2;
            }else if(i == nodo->numKey - 1){
                nodo->keys[i+1] = chave;
                break;
            }
        }
        nodo->numKey += 1;
    } else {
        for (i = 0 ; i < nodo->numKey ; i ++){
            if (nodo->keys[i] > chave){
                insertSplit(arvore, nodo->kids[i], nodo, chave);
            }else if( i == nodo->numKey - 1){
                insertSplit(arvore, nodo->kids[i + 1], nodo, chave);
            }
        }
    }
}

Tree *insert(Tree *arvore, int chave){
    if (arvore->root == NULL){                                  //inserir apenas se a àrvore està vazia
        Node *nodo = (Node*) malloc (sizeof(Node)); 
        int *keys = (int*) calloc ((MAX_KEY), sizeof(int));     //Aloca o vetor
        Node **filhos = (Node**) calloc (MAX_KIDS, sizeof(Node));
        keys[0] = chave;
        nodo->keys = keys;
        nodo->kids = filhos;
        nodo->numKey = 1;
        nodo->isLeaf = true; 
        arvore->root = nodo;
    }else{                                                          //insert para quando a àrvore não està vazia
        insertSplit(arvore, arvore->root, NULL, chave);             //passa a àrvore, sua raiz, NULL (pois é a raiz da àrvore e ela não tem pai), e a chave
    }
    return arvore;
}


void imprime(Node *nodo) {          //imprime a àrvore B
    if(nodo == NULL) {
        printf("Arvore vazia\n");
    }
    else {
        int i;
  
        for (i = 0; i < nodo->numKey; i++) {
            printf("%d ", nodo->keys[i]);
            if(nodo->isLeaf == true) printf("- folha ");
            else printf("- raiz \t");
        }
        if(nodo->isLeaf == false) {
            printf("\n");
            for(int j = 0; nodo->kids[j] != NULL;j++ ) {      
                imprime(nodo->kids[j]);   
                printf("       ");    
            }
        }
    }
}

  
  
int main() {
    Tree *btree = cria();

    btree = insert(btree, 3);     //insere 3
    imprime(btree->root);
    printf("\n\n");

    btree = insert(btree, 2);     //insere 2 e 5
    btree = insert(btree, 5);
    imprime(btree->root);
    printf("\n\n");

    btree = insert(btree, 6);     //ocorre split. Insere 6.
    imprime(btree->root);          //A chave 3 é raiz, 2 à esquerda, 5 e 6 à direita.
    printf("\n\n");

    btree = insert(btree, 7);     //Insere 7. Ocorre split. Insere 8.
    btree = insert(btree, 8);     //As chaves 3 e 6 são raízes, 2 à esquerda do 3,
    imprime(btree->root);         //5 entre o 3 e 6, 7 e 8 à direita da chave 6.
    printf("\n\n");

}
  

