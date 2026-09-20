#ifndef BTREE_H
#define BTREE_H

#define LEFT  'L'
#define RIGHT 'R'

typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    int size;
    int count;
} BTree;


/* ADT */
BTree* create_btree(int size);

int insert_root(BTree *tree, char value);

int insert_child(BTree *tree, const char *parent,
                 char child, char value);

int delete_node(BTree *tree, const char *leaf);

int update_value(BTree *tree, const char *path,
                 char value);

int read_child(BTree *tree, const char *parent);

void print_btree(BTree *tree);

void destroy_btree(BTree *tree);

#endif
