#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

void rotate_left(node_t* node);
void rotate_right(node_t* node);
void rbtree_insert_fixup(rbtree* tree,node_t* newNode);
void printTree(rbtree* tree,node_t* node,int level);

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
  free(t);
}


void printTree(rbtree* tree,node_t * node,int level){
    if (node != tree->nil) {
        printf("노드의 레벨 : %d\n", level);
        printf("노드의 값 : %d\n", node->key);
        printf("노드의 색깔 : %d\n", node->color);
        printTree(tree,node->left,level+1);
        printTree(tree,node->right,level+1);
    }else{
        printf("노드의 레벨 : %d은", level);
        printf("NIL노드입니다.\n");
    }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
    node_t *parentNode = t->nil;
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
        parentNode->right = newNode;
    }
    //새로 추가된 노드의 leaf node를 nil node로 설정
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->color = RBTREE_RED;//항상 삽입되는 노드의 색깔을 RED

    //일반적인 BST의 편향 트리를 조정하기 위한 함수 호출
    rbtree_insert_fixup(t,newNode);
    printTree(t,t->root,1);
    return newNode;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
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