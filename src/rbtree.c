#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

node_t* getGrandParent(node_t* node);
node_t* getUncle(node_t* node);
void rotate_left(node_t* node);
void rotate_right(node_t* node);

rbtree* new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
    p->root = NULL;
    p->nil = NULL;
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
    node_t *parentNode = NULL;
    node_t *newNode = NULL;
    if (t == NULL){
        return NULL;
    }
    parentNode = t->root;
    while (parentNode != NULL) {
        if (key < parentNode->key) {
            if (parentNode->left == NULL) {
                break;
            }else {
                parentNode = parentNode->left;

            }
        }else{
            if (parentNode->right == NULL) {
                break;
            } else {
                parentNode = parentNode->right;
            }
        }
    }
    newNode = calloc(1, sizeof(node_t));
    if (newNode != NULL) {
        newNode->key = key;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->color = RBTREE_RED;
        if (parentNode == NULL) {
            printf("Root node add here\n");
            printf("Root node's Color %d\n",newNode->color);
            t->root = newNode;
        }else{
            if (newNode->key < parentNode->key) {
                parentNode->left = newNode;
            }else{
                parentNode->right = newNode;
            }
        }
    }
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
