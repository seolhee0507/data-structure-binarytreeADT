#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"


/* =========================================
   노드 생성
   ========================================= */

static Node* create_node(char value)
{
    Node *node = (Node*)malloc(sizeof(Node));

    if (node == NULL)
        return NULL;

    node->data = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}


/* =========================================
   경로로 노드 찾기

   예:
   /A/B/C

   root A
     ↓
   B
     ↓
   C
   ========================================= */

static Node* find_node(BTree *tree, const char *path)
{
    Node *current;

    if (tree == NULL || tree->root == NULL)
        return NULL;

    if (path == NULL || path[0] != '/')
        return NULL;

    if (path[1] == '\0')
        return NULL;

    current = tree->root;

    /* 첫 번째 노드는 루트 */
    if (path[1] != current->data)
        return NULL;

    int i = 2;

    while (path[i] != '\0') {

        if (path[i] != '/')
            return NULL;

        i++;

        if (path[i] == '\0')
            return NULL;

        if (path[i] == current->data)
            return NULL;

        if (current->left != NULL &&
            current->left->data == path[i]) {

            current = current->left;
        }
        else if (current->right != NULL &&
                 current->right->data == path[i]) {

            current = current->right;
        }
        else {
            return NULL;
        }

        i++;
    }

    return current;
}


/* =========================================
   부모 찾기
   ========================================= */

static Node* find_parent(BTree *tree, Node *target)
{
    if (tree == NULL ||
        tree->root == NULL ||
        target == NULL ||
        tree->root == target)
        return NULL;

    Node *queue[1000];
    int front = 0;
    int rear = 0;

    queue[rear++] = tree->root;

    while (front < rear) {

        Node *current = queue[front++];

        if (current->left == target ||
            current->right == target) {

            return current;
        }

        if (current->left != NULL)
            queue[rear++] = current->left;

        if (current->right != NULL)
            queue[rear++] = current->right;
    }

    return NULL;
}


/* =========================================
   ADT 생성
   ========================================= */

BTree* create_btree(int size)
{
    if (size <= 0)
        return NULL;

    BTree *tree = (BTree*)malloc(sizeof(BTree));

    if (tree == NULL)
        return NULL;

    tree->root = NULL;
    tree->size = size;
    tree->count = 0;

    return tree;
}


/* =========================================
   루트 삽입
   ========================================= */

int insert_root(BTree *tree, char value)
{
    if (tree == NULL)
        return 0;

    if (tree->root != NULL)
        return 0;

    if (value < 'A' || value > 'Z')
        return 0;

    tree->root = create_node(value);

    if (tree->root == NULL)
        return 0;

    tree->count++;

    return 1;
}


/* =========================================
   자식 삽입
   ========================================= */

int insert_child(BTree *tree,
                 const char *parent,
                 char child,
                 char value)
{
    Node *p;
    Node *new_node;

    if (tree == NULL)
        return 0;

    if (tree->count >= tree->size)
        return 0;

    if (value < 'A' || value > 'Z')
        return 0;

    if (child != LEFT && child != RIGHT)
        return 0;

    p = find_node(tree, parent);

    if (p == NULL)
        return 0;

    /* 부모가 단말 노드인지 확인 */
    if (p->left != NULL || p->right != NULL)
        return 0;

    /* 같은 부모의 다른 자식과 데이터 중복 확인 */
    if (child == LEFT) {

        if (p->right != NULL &&
            p->right->data == value)
            return 0;

        if (p->left != NULL)
            return 0;

    }
    else {

        if (p->left != NULL &&
            p->left->data == value)
            return 0;

        if (p->right != NULL)
            return 0;
    }

    new_node = create_node(value);

    if (new_node == NULL)
        return 0;

    if (child == LEFT)
        p->left = new_node;
    else
        p->right = new_node;

    tree->count++;

    return 1;
}


/* =========================================
   단말 노드 삭제
   ========================================= */

int delete_node(BTree *tree, const char *path)
{
    Node *target;
    Node *parent;

    if (tree == NULL || tree->root == NULL)
        return 0;

    target = find_node(tree, path);

    if (target == NULL)
        return 0;

    /* 단말 노드인지 확인 */
    if (target->left != NULL ||
        target->right != NULL)
        return 0;

    /* 루트인 경우 */
    if (target == tree->root) {

        free(target);

        tree->root = NULL;
        tree->count--;

        return 1;
    }

    parent = find_parent(tree, target);

    if (parent == NULL)
        return 0;

    if (parent->left == target)
        parent->left = NULL;
    else
        parent->right = NULL;

    free(target);

    tree->count--;

    return 1;
}


/* =========================================
   값 수정
   ========================================= */

int update_value(BTree *tree,
                 const char *path,
                 char value)
{
    Node *target;
    Node *parent;

    if (tree == NULL)
        return 0;

    if (value < 'A' || value > 'Z')
        return 0;

    target = find_node(tree, path);

    if (target == NULL)
        return 0;

    parent = find_parent(tree, target);

    /*
       루트는 부모가 없으므로
       중복 검사 필요 없음
    */
    if (parent != NULL) {

        if (parent->left == target &&
            parent->right != NULL &&
            parent->right->data == value) {

            return 0;
        }

        if (parent->right == target &&
            parent->left != NULL &&
            parent->left->data == value) {

            return 0;
        }
    }

    target->data = value;

    return 1;
}


/* =========================================
   자식 정보 출력
   ========================================= */

int read_child(BTree *tree, const char *path)
{
    Node *parent;

    if (tree == NULL)
        return 0;

    parent = find_node(tree, path);

    if (parent == NULL)
        return 0;

    if (parent->left == NULL &&
        parent->right == NULL) {

        printf("자식이 없습니다.\n");
        return 1;
    }

    if (parent->left != NULL)
        printf("%c(L)", parent->left->data);

    if (parent->left != NULL &&
        parent->right != NULL)
        printf(", ");

    if (parent->right != NULL)
        printf("%c(R)", parent->right->data);

    printf("\n");

    return 1;
}


/* =========================================
   트리 출력
   ========================================= */

static void print_tree(Node *node, int depth)
{
    if (node == NULL)
        return;

    /*
       오른쪽을 먼저 출력하면
       트리가 왼쪽으로 눕는다.
    */
    print_tree(node->right, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("+---%c\n", node->data);

    print_tree(node->left, depth + 1);
}


void print_btree(BTree *tree)
{
    if (tree == NULL ||
        tree->root == NULL) {

        printf("트리가 비어 있습니다.\n");
        return;
    }

    print_tree(tree->root, 0);
}


/* =========================================
   트리 삭제
   ========================================= */

static void destroy_node(Node *node)
{
    if (node == NULL)
        return;

    destroy_node(node->left);
    destroy_node(node->right);

    free(node);
}


void destroy_btree(BTree *tree)
{
    if (tree == NULL)
        return;

    destroy_node(tree->root);

    free(tree);
}
