#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

void rotate_left(rbtree* tree, node_t* node);
void rotate_right(rbtree* tree, node_t* node);
void rbtree_insert_fixup(rbtree* tree,node_t* newNode);
void rbtree_transplant(rbtree* tree,node_t* u,node_t* v);
void rbtree_delete_fixup(rbtree *t, node_t *x);
void delete_rbtree_internal(rbtree* tree,node_t* node);
void setArrayByInorder(const rbtree* tree,const node_t* current,key_t *arr,const size_t n,int* index);

rbtree* new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
    node_t *nilNode = (node_t *) calloc(1, sizeof(node_t));
    nilNode->color = RBTREE_BLACK;
    p->nil = nilNode;
    p->root = nilNode;
    return p;
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
    if (t != NULL) {
        delete_rbtree_internal(t,t->root);
        free(t->nil);
        free(t);
    }
}

void delete_rbtree_internal(rbtree* tree,node_t* node){
    if (node != tree->nil) {
        delete_rbtree_internal(tree, node->left);
        delete_rbtree_internal(tree, node->right);
        free(node);
    }
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
    node_t *parentNode = t->root;
    node_t *tmpNode = t->root;
    node_t *newNode = (node_t *) calloc(1, sizeof(node_t));

    newNode->key = key;

    //tmpNode가 leaf에 도달하면 while 탈출
    while (tmpNode != t->nil) {
        parentNode = tmpNode;
        if (newNode->key < tmpNode->key) {
            tmpNode = tmpNode->left;
        }else{
            tmpNode = tmpNode->right;
        }
    }

    //while문을 통해 설정한 parentNode를 newNode의 부모 노드로 설정
    newNode->parent = parentNode;

    //parentNode가 leaf node이면 트리가 비어 있는 상태
    if (parentNode == t->nil) {
        //새로 추가할 노드를 tree의 루트 노드로 설정
        t->root = newNode;
    } else if (newNode->key < parentNode->key) {
        parentNode->left = newNode;
    } else{
        //중복 제한 조건
        if (newNode->key == parentNode->key && parentNode->right->key == newNode->key) {
            return NULL;
        }
        parentNode->right = newNode;
    }
    //새로 추가된 노드의 leaf node를 nil node로 설정
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->color = RBTREE_RED;//항상 삽입되는 노드의 색깔을 RED

    //일반적인 BST의 편향 트리를 조정하기 위한 함수 호출
    rbtree_insert_fixup(t,newNode);
    return newNode;
}


void rbtree_insert_fixup(rbtree* tree,node_t* newNode){
    node_t* uncle;
    //추가할 노드의 부모가 RED => RED -> RED 연속되는 상황 -> 트리 수정 필요
    while (newNode->parent->color == RBTREE_RED) {
        //왼쪽 서브 트리인 경우
        if (newNode->parent == newNode->parent->parent->left) {
            uncle = newNode->parent->parent->right;
            //1. 새로운 노드의 Uncle 노드가 적색인 경우
            if (uncle->color == RBTREE_RED) {
                //부모와 삼촌 노드를 BLACK으로
                newNode->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                newNode->parent->parent->color = RBTREE_RED;
                newNode = newNode->parent->parent;// 할아버지 노드가 RED로 변경되었기에 다시 확인하기 위해 newNode를 grand로 설정
            }
                //새로운 노드의 Uncle 노드가 BLACK
            else{
                //2. 새로운 자식이 오른쪽 자식인 경우 -> 할아버지 노드까지 가는 경로가 꺽인 경우
                //왼쪽 회전으로 일자로 펴주고 회전 한번 더
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    rotate_left(tree, newNode);//꺽인 경로를 일자로 만들기 위해 부모 노드 기준으로 회전
                }
                //색깔 먼저 바꾸고 후에 rotate
                newNode->parent->color = RBTREE_BLACK;
                newNode->parent->parent->color = RBTREE_RED;
                rotate_right(tree, newNode->parent->parent);//할아버지 노드 기준 회전 -> 할아버지가 내려오고 newNode가 새로운 부모 노드가 됨
            }
        }
            //grand의 오른쪽 서브트리인 경우
        else{
            uncle = newNode->parent->parent->left;
            //Uncle이 적색
            if (uncle->color == RBTREE_RED) {
                newNode->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                newNode->parent->parent->color = RBTREE_RED;
                newNode = newNode->parent->parent;// 할아버지 노드가 RED로 변경되었기에 다시 확인하기 위해 newNode를 grand로 설정
            }
                //Uncle이 흑색
            else{
                //경로가 꺽여 있는 경우
                if (newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rotate_right(tree, newNode);//꺽인 경로를 일자로 만들기 위해 부모 노드 기준으로 회전
                }
                //색깔 먼저 바꾸고 후에 rotate
                newNode->parent->color = RBTREE_BLACK;
                newNode->parent->parent->color = RBTREE_RED;
                rotate_left(tree, newNode->parent->parent);//할아버지 노드 기준 회전 -> 할아버지가 내려오고 newNode가 새로운 부모 노드가 됨
            }
        }
    }
    //root 노드는 항상 BLACK
    tree->root->color = RBTREE_BLACK;
}

//temp가 node 보다 상위 노드로 바뀌어야함
void rotate_left(rbtree* tree, node_t* node){
    node_t * temp;
    temp = node->right;
    node->right = temp->left;
    //tmp의 left가 NIL이 아니면 해당 노드의 부모를 node로 설정
    if (temp->left != tree->nil) {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    //node가 root인 경우 -> temp가 루트 노드가 되어야함
    if (node->parent == tree->nil) {
        tree->root = temp;
    }
        //node가 왼쪽 자식이면
    else if (node == node->parent->left) {
        node->parent->left = temp;
    }
        //node가 오른쪽 자식이면
    else{
        node->parent->right = temp;
    }
    temp->left = node;
    node->parent = temp;
}

void rotate_right(rbtree* tree, node_t* node){
    node_t * temp;
    //target의 left를 temp로 지정
    temp = node->left;
    //temp의 오른쪽 서브트리를 target의 왼쪽 서브트리로 변경
    node->left = temp->right;
    //tmp의 오른쪽 서브트리가 NIL이 아니면 해당 노드의 부모를 node로 설정
    if (temp->right != tree->nil) {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    //node가 root인 경우 -> temp가 루트 노드가 되어야함
    if (node->parent == tree->nil) {
        tree->root = temp;
    }
        //node가 왼쪽 자식이면
    else if (node == node->parent->left) {
        node->parent->left = temp;
    }
        //node가 오른쪽 자식이면
    else{
        node->parent->right = temp;
    }
    temp->right = node;
    node->parent = temp;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
    node_t *returnNode = t->root;
    while (returnNode != t->nil) {
        if (returnNode->key == key) {
            return returnNode;
        }
        if (returnNode->key < key) {
            returnNode = returnNode->right;
        }else{
            returnNode = returnNode->left;
        }
    }
    return NULL;
}

int rbtree_erase(rbtree *tree, node_t *targetNode) {
    // TODO: implement erase
    node_t *y;
    node_t *x;
    color_t yOriginalColor;

    y = targetNode;
    yOriginalColor = y->color;
    //지울 노드의 왼쪽이 NIL이면
    if (targetNode->left == tree->nil) {
        x = targetNode->right;
        rbtree_transplant(tree,targetNode,targetNode->right);
    } else if (targetNode->right == tree->nil) {
        x = targetNode->left;
        rbtree_transplant(tree, targetNode, targetNode->left);
    }else{
        //Successor 설정
        y = targetNode->right;
        while (y->left != tree->nil) {
            y = y->left;
        }
        yOriginalColor = y->color;//설정된 직후 노드의 색깔로 닫시 설정
        x = y->right;//x를 직후노드의 오른쪽 자식으로 설정,직후 노드는 left child를 가질 수 없기 때문에
        //y의 부모가 삭제할 노드인 경우,즉 depth가 1만큼 차이가 나는 경우
        if (y->parent == targetNode) {
            x->parent = y;
        }
        else{
            rbtree_transplant(tree, y, y->right);//y의 오른쪽 노드와 y의 위치를 변경
            y->right = targetNode->right;//
            y->right->parent = y;
        }
        rbtree_transplant(tree, targetNode, y);
        y->left = targetNode->left;
        y->left->parent = y;
        y->color = targetNode->color;
    }

    if (yOriginalColor == RBTREE_BLACK) {
        rbtree_delete_fixup(tree, x);
    }
    free(targetNode);
    return 0;
}

void rbtree_transplant(rbtree* tree,node_t* firstSubtreeRoot,node_t* secondSubtreeRoot){
    //firstSubtreeRoot가 루트 노드이면
    if (firstSubtreeRoot->parent == tree->nil) {
        //트리의 root node를 secondSubtreeRoot로 변경
        tree->root = secondSubtreeRoot;
    }
        //firstSubtreeRoot가 왼쪽 서브트리이면
    else if (firstSubtreeRoot == firstSubtreeRoot->parent->left) {
        firstSubtreeRoot->parent->left = secondSubtreeRoot;
    }
        //firstSubtreeRoot가 오른쪽 서브트리이면
    else{
        firstSubtreeRoot->parent->right = secondSubtreeRoot;
    }
    //두번째 서브트리의 루트노드를 첫번째 서브트리의 루트노드로 변경
    secondSubtreeRoot->parent = firstSubtreeRoot->parent;
}

void rbtree_delete_fixup(rbtree *tree, node_t *x){
    while (x != tree->root && x->color == RBTREE_BLACK) {
        //CASE 1 ~ 4 : LEFT CASE, doubled black이 왼쪽 노드인 경우
        if (x == x->parent->left) {
            node_t *w = x->parent->right;//w는 x의 형제 노드
            //형제 노드가 RED인 경우
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;//Tree 구조가 변경 되었기에 w 형제노드를 새롭게 연결
            }
            //x의 형제 w는 흑색이고 w의 자식들이 모두 BLACK
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;//부모 노드가 red인지 black인지 알 수 없는 상황이라서 x에 부모 노드 주소 대입
            }

            else{
                //x의 형제 w는 흑색,w의 왼쪽 자식은 적색, w의 오른쪽 자식은 흑색인 경우
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_right(tree, w);
                    w = x->parent->right;//rotate하고 난 뒤, 형제 노드 w를 새롭게 연결
                }
                //x의 오른쪽 형제가 black이고 그 형제의 오른쪽 자녀가 red인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        }
            //CASE 5 ~ 8 : Right CASE, doubled black이 오른쪽 노드인 경우
        else{
            node_t *w = x->parent->left;//w는 x의 형제 노드
            //CASE5 : x의 형제 노드가 RED
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            //CASE 6 : x의 형제 노드 w는 흑색이고 w의 두 자식이 모두 흑색인 경우
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else{
                //CASE 7 : x의 형제는 black, w의 왼쪽 자식은 적색, w의 오른쪽 자식은 흑색
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                //CASE 8 : x의 형제는 흑색이고 오른쪽 자식이 red인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
    if (t->root == t->nil) {
        return NULL;
    }
    node_t *minNode = t->root;
    while (minNode->left != t->nil) {
        minNode = minNode->left;
    }
    return minNode;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
    if (t->root == t->nil) {
        return NULL;
    }
    node_t *maxNode = t->root;
    while (maxNode->right != t->nil) {
        maxNode = maxNode->right;
    }
    return maxNode;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array, 중위 순회 사용 => 오름차순 정렬
    if (t->root == t->nil) {
        return 0;
    }
    int index = 0;
    setArrayByInorder(t,t->root, arr, n, &index);
    return 0;
}

void setArrayByInorder(const rbtree* tree,const node_t* current,key_t *arr,const size_t n,int* index){
    if (current == tree->nil) {
        return;
    }
    setArrayByInorder(tree,current->left, arr, n, index);
    if (*index < n) {
        arr[*index] = current->key;
        (*index)++;
    }
    setArrayByInorder(tree,current->right, arr, n, index);
}