#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

node_t* getGrandParent(node_t* node);
node_t* getUncle(node_t* node);
void rotate_left(node_t* node);
void rotate_right(node_t* node);
void rbtree_insert_fixup(rbtree* tree,node_t* newNode);

rbtree* new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
    node_t *nilNode = (node_t *) calloc(1, sizeof(node_t));
    nilNode->color = RBTREE_BLACK;
    p.nil = nilNode;
    p->root = nilNode;
    return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t* getGrandParent(node_t* node){
    if ((node != NULL) && (node->parent != NULL)) {
        return node->parent->parent;
    } else {
        return NULL;
    }
}

node_t* getUncle(node_t* node){
    node_t* grand = getGrandParent(node);
    if (grand == NULL) {
        return NULL;
    }
    if (node->parent == grand->left) {
        return grand->right;
    } else{
        return grand->left;
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
        if (newNode.key < tmpNode->key) {
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

    rbtree_insert_fixup(t,newNode);

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
