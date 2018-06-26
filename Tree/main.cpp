#include <iostream>
#include <cassert>
#include "math.h"
#define is_sin *s == 's' && *(s+1) == 'i' && *(s+2) == 'n'
#define is_cos *s == 'c' && *(s+1) == 'o' && *(s+2) == 's'
#define is_tan *s == 't' && *(s+1) == 'a' && *(s+2) == 'n'
#define is_exp *s == 'e' && *(s+1) == 'x' && *(s+2) == 'p'
#define is_ln  *s == 'l' && *(s+1) == 'n'

const int    OP       = 1;
const int    NUM      = 2;
const int    VAR      = 3;
const int    FUN      = 4;
const int    CONST    = 5;
const double ADD      = 21;
const double SUB      = 22;
const double MUL      = 23;
const double DIV      = 24;
const double POW      = 25;
const double SIN      = 31;
const double COS      = 32;
const double TAN      = 33;
const double EXP      = 34;
const double LN       = 35;
const double VAR_X    = 101;
const double VAR_Y    = 102;
const double CONST_E  = 202;
const double CONST_PI = 203;

char *s;

struct node {
    double value;
    int code;
    node *left;
    node *right;
    node *parent;
};

struct tree {
    node * root;
};

node  *node_construct   ();
void   node_destruct    (node *cell);
tree  *tree_construct   ();
void   tree_destruct    (tree *birch);
void   left_insert      (node *cell, node *in_node);
void   right_insert     (node *cell, node *in_node);
node  *GetN             (char *str);
node  *GetT             (char *str);
node  *GetE             (char *str);
node  *GetP             (char *str);
node  *GetExp           (char *str);
node  *GetFun           (char *str);
tree  *GetG0            (char *str);
tree  *tree_diff        (tree *birch);
node  *node_diff        (node *cell);
void   tree_print       (tree *birch);
void   node_print       (node *cell);
void   value_print      (node *cell);
void   tree_polsk_print (tree *birch);
void   node_polsk_print (node *cell);
void   copy_left        (node *cell, node *copy);
void   copy_right       (node *cell, node *copy);
void   simple_tree      (tree *birch);
void   simple_node      (node *cell);

int main () {
    char *str = (char *) malloc(sizeof(char));
    scanf("%s", str);
    tree *birch = tree_construct ();
    birch = GetG0(str);
    tree *palm = tree_construct();
    palm = tree_diff(birch);
    tree_print(palm);
    tree_destruct(birch);
}

node * node_construct () {
    node * cell = (node *) malloc(sizeof(node));
    cell->value = 0;
    cell->code = -1;
    cell->left = NULL;
    cell->right = NULL;
    cell->parent = NULL;
    return cell;
}

tree *tree_construct () {
    tree * birch = (tree *) malloc(sizeof(tree));
    birch->root = node_construct();
    return birch;
}

void node_destruct (node *cell) {
    assert (cell);
    cell->value = 0;
    cell->code = -1;
    if (cell->left) node_destruct(cell->left);
    if (cell->right) node_destruct(cell->right);
    cell->left = NULL;
    cell->right = NULL;
    cell->parent = NULL;
    free (cell);
}

void tree_destruct (tree *birch) {
    assert (birch);
    node_destruct(birch->root);
    free (birch);
}

void left_insert (node *cell, node *in_node) {
    assert(cell);
    assert(in_node);
    cell->left = in_node;
    in_node->parent = cell;
}

void right_insert (node *cell, node *in_node) {
    assert(cell);
    assert(in_node);
    cell->right = in_node;
    in_node->parent = cell;
}

void copy_left (node *cell, node *copy) {
    assert(cell);
    assert(copy);
    node *in_node = node_construct();
    in_node->code = copy->code;
    in_node->value = copy->value;
    if (copy->left) copy_left(in_node, copy->left);
    if (copy->right) copy_right(in_node, copy->right);
    cell->left = in_node;
    in_node->parent = cell;
}

void copy_right (node *cell, node *copy) {
    assert(cell);
    assert(copy);
    node *in_node = node_construct();
    in_node->code = copy->code;
    in_node->value = copy->value;
    if (copy->left) copy_left(in_node, copy->left);
    if (copy->right) copy_right(in_node, copy->right);
    cell->right = in_node;
    in_node->parent = cell;
}

node *GetN (char *str) {
    assert(str);
    double res = 0;
    int frac = 0;
    double exp = 1;;
    while (('0' <= *s && *s <= '9') || *s == '.') {
        if (!frac && *s != '.') res = res * 10 + *s - '0';
        if (frac && *s != '.') res = res + double(*s - '0') / pow(10, exp++);
        if (*s == '.' ) frac = 1;
        s++;
    }
    node *cell = node_construct();
    if (*s == 'x' || *s == 'X') {
        cell->code = VAR;
        cell->value = VAR_X;
        s++;
        return cell;
    }
    if (*s == 'y' || *s == 'Y') {
        cell->code = VAR;
        cell->value = VAR_Y;
        s++;
        return cell;
    }
    if (*s == 'e' || *s == 'E') {
        cell->code = CONST;
        cell->value = CONST_E;
        s++;
        return cell;
    }
    if ((*s == 'p' || *s == 'P') && (*(s+1) == 'i' || *(s+1) == 'I')) {
        cell->code = CONST;
        cell->value = CONST_PI;
        s += 2;
        return cell;
    }
    cell->code = NUM;
    cell->value = res;
    return cell;
}

node *GetT (char *str) {
    assert(str);
    node *cell = node_construct();
    cell = GetExp(str);
    while (*s == '*' || *s == '/') {
        char *op = s;
        s++;
        node *res1 = node_construct();
        res1->value = cell->value;
        res1->code = cell->code;
        res1->right = cell->right;
        res1->left = cell->left;
        cell->code = OP;
        cell->left = res1;
        res1->parent = cell;
        node *res2 = node_construct();
        res2 = GetExp(str);
        cell->right = res2;
        res2->parent = cell;
        if (*op == '*') cell->value = MUL;
        if (*op == '/') cell->value = DIV;
    }
    return cell;
}

node *GetE (char *str) {
    assert(str);
    node *cell = node_construct();
    cell = GetT (str);
    while (*s == '+' || *s == '-') {
        char *op = s;
        s++;
        node *res1 = node_construct();
        res1->value = cell->value;
        res1->code = cell->code;
        res1->right = cell->right;
        res1->left = cell->left;
        cell->code = OP;
        cell->left = res1;
        res1->parent = cell;
        node *res2 = node_construct();
        res2 = GetT(str);
        cell->right = res2;
        res2->parent = cell;
        if (*op == '+') cell->value = ADD;
        if (*op == '-') cell->value = SUB;
    }
    return cell;
}

node *GetP (char *str) {
    assert(str);
    if (*s == '(') {
        s++;
        node * cell = node_construct();
        cell = GetE (str);
        if (*s != ')') exit(0);
        s++;
        return cell;
    } else return GetN(str);
}

tree *GetG0 (char *str) {
    assert(str);
    s = str;
    tree *birch = tree_construct();
    birch->root = GetE(str);
    return birch;
}

void tree_print (tree *birch) {
    assert(birch);
    node_print(birch->root);
    printf("\n");
}

void node_print (node *cell) {
    assert(cell);
    if (cell->code == FUN) {
        value_print(cell);
        printf("(");
        if (cell->left) node_print(cell->left);
        printf(")");
        return;
    }
    if (cell->code == OP && (cell->value == ADD || cell->value == SUB) && cell->parent) {
        if ((cell->parent->value == DIV  || cell->parent->value == MUL || cell->parent->value == POW) && cell->parent->code == OP) printf("(");
    }
    if (cell->code == OP && (cell->value == MUL || cell->value == DIV) && cell->parent ) {
        if (cell->parent->value == POW && cell->parent->code == OP) printf("(");
    }
    if (cell->left) node_print(cell->left);
    value_print(cell);
    if (cell->right) node_print(cell->right);
    if (cell->code == OP && (cell->value == ADD || cell->value == SUB) && cell->parent) {
        if ((cell->parent->value == DIV  || cell->parent->value == MUL || cell->parent->value == POW) && cell->parent->code == OP) printf(")");
    }
    if (cell->code == OP && (cell->value == MUL || cell->value == DIV) && cell->parent ) {
        if (cell->parent->value == POW && cell->parent->code == OP) printf(")");
    }
}

void tree_polsk_print (tree *birch) {
    assert(birch);
    node_polsk_print(birch->root);
    printf("\n");
}

void node_polsk_print (node *cell) {
    assert(cell);
    if (cell->left) node_polsk_print(cell->left);
    if (cell->right) node_polsk_print(cell->right);
    value_print(cell);
}


void value_print(node *cell) {
    assert(cell);
    switch (cell->code) {
        case OP:
            if (cell->value == ADD) printf("+");
            if (cell->value == SUB) printf("-");
            if (cell->value == MUL) printf("*");
            if (cell->value == DIV) printf("/");
            if (cell->value == POW) printf("^");
            break;
        case NUM:
            if (cell->value < 0) printf("(");
            printf("%.2lf", cell->value);
            if (cell->value < 0) printf(")");
            break;
        case VAR:
            if (cell->value == VAR_X) printf("X");
            if (cell->value == VAR_Y) printf("Y");
            break;
        case FUN:
            if (cell->value == SIN) printf("sin");
            if (cell->value == COS) printf("cos");
            if (cell->value == TAN) printf("tan");
            if (cell->value == EXP) printf("exp");
            if (cell->value == LN ) printf("ln");
            break;
        case CONST:
            if (cell->value == CONST_E) printf("e");
            if (cell->value == CONST_PI) printf("pi");
            break;
        default:
            exit(!(std::cerr << "wrong code\n"));
            break;
    }
}

node *GetExp(char *str) {
    assert(str);
    node * cell = node_construct();
    cell = GetFun(str);
    if (*s == '^') {
        s++;
        node *res1 = node_construct();
        res1->value = cell->value;
        res1->code = cell->code;
        res1->right = cell->right;
        res1->left = cell->left;
        res1->parent = cell;
        cell->left = res1;
        node *res2 = node_construct();
        res2 = GetFun(str);
        cell->right = res2;
        res2->parent = cell;
        cell->value = POW;
        cell->code = OP;
    }
    return cell;
}

node *GetFun (char *str) {
    node *cell = node_construct();
    if (is_sin) {
        s += 3;
        cell->code = FUN;
        cell->value = SIN;
        if (*s != '(') exit(1);
        else s++;
        node *res = node_construct();
        res = GetE(str);
        if (*s != ')') exit(1);
        else s++;
        cell->left = res;
        cell->right = NULL;
        return cell;
    }
    if (is_cos) {
        s += 3;
        cell->code = FUN;
        cell->value = COS;
        if (*s != '(') exit(1);
        else s++;
        node *res = node_construct();
        res = GetE(str);
        if (*s != ')') exit(1);
        else s++;
        cell->left = res;
        cell->right = NULL;
        return cell;
    }
    if (is_tan) {
        s += 3;
        cell->code = FUN;
        cell->value = TAN;
        if (*s != '(') exit(1);
        else s++;
        node *res = node_construct();
        res = GetE(str);
        if (*s != ')') exit(1);
        else s++;
        cell->left = res;
        cell->right = NULL;
        return cell;
    }
    if (is_exp) {
        s += 3;
        cell->code = FUN;
        cell->value = EXP;
        if (*s != '(') exit(1);
        else s++;
        node *res = node_construct();
        res = GetE(str);
        if (*s != ')') exit(1);
        else s++;
        cell->left = res;
        cell->right = NULL;
        return cell;
    }
    if (is_ln) {
        s += 2;
        cell->code = FUN;
        cell->value = LN;
        if (*s != '(') exit(1);
        else s++;
        node *res = node_construct();
        res = GetE(str);
        if (*s != ')') exit(1);
        else s++;
        cell->left = res;
        cell->right = NULL;
        return cell;
    }
    return GetP(str);
}

tree *tree_diff (tree *birch) {
    assert(birch);
    tree *palm = tree_construct();
    palm->root = node_diff(birch->root);
    simple_tree(palm);
    return palm;
}

node *node_diff (node *cell) {
    assert(cell);
    node *res = node_construct();
    switch (cell->code) {
        case OP:
            if (cell->value == ADD) {
                res->code = OP;
                res->value = ADD;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                copy_left(res, node_diff(cell->left));
                copy_right(res, node_diff(cell->right));
            }
            if (cell->value == SUB) {
                res->code = OP;
                res->value = SUB;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                copy_left(res, node_diff(cell->left));
                copy_right(res, node_diff(cell->right));
            }
            if (cell->value == MUL) {
                res->code = OP;
                res->value = ADD;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                node *res_l = node_construct();
                node *res_r = node_construct();
                res_l->code = OP;
                res_l->value = MUL;
                res_l->left = node_construct();
                res_l->right = node_construct();
                res_l->left->parent = res_l;
                res_l->right->parent = res_l;
                res_l->left = node_diff(cell->left);
                copy_right(res_l, cell->right);
                res_r->code = OP;
                res_r->value = MUL;
                res_r->left = node_construct();
                res_r->right = node_construct();
                res_r->left->parent = res_r;
                res_r->right->parent = res_r;
                res_r->left = node_diff(cell->right);
                copy_right(res_r, cell->left);
                left_insert(res, res_l);
                right_insert(res, res_r);
            }
            if (cell->value == DIV) {
                res->code = OP;
                res->value = DIV;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                node *res_l = node_construct();
                node *res_r = node_construct();
                res_l->code = OP;
                res_l->value = SUB;
                res_l->left = node_construct();
                res_l->right = node_construct();
                res_l->left->parent = res_l;
                res_l->right->parent = res_l;
                res_l->left->code = OP;
                res_l->left->value = MUL;
                res_l->left->left = node_construct();
                res_l->left->right = node_construct();
                res_l->left->left->parent = res_l->left;
                res_l->left->right->parent = res_l->left;
                copy_left(res_l->left, node_diff(cell->left));
                copy_right(res_l->left, cell->right);
                res_l->right->code = OP;
                res_l->right->value = MUL;
                res_l->right->left = node_construct();
                res_l->right->right = node_construct();
                res_l->right->left->parent = res_l->right;
                res_l->right->right->parent = res_l->right;
                copy_left(res_l->right, node_diff(cell->right));
                copy_right(res_l->right, cell->left);
                res_r->code = OP;
                res_r->value = POW;
                res_r->left = node_construct();
                res_r->right = node_construct();
                res_r->left->parent = res_r;
                res_r->right->parent = res_r;
                copy_left(res_r, cell->right);
                res_r->right->code = NUM;
                res_r->right->value = 2;
                left_insert(res, res_l);
                right_insert(res, res_r);
            }
            if (cell->value == POW) {
                res->code = OP;
                res->value = MUL;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                res->left->code = OP;
                res->left->value = POW;
                res->left->left = node_construct();
                res->left->right = node_construct();
                res->left->left->parent = res->left;
                res->left->right->parent = res->left;
                copy_left(res->left, cell->left);
                copy_right(res->left, cell->right);
                res->right->code = OP;
                res->right->value = ADD;
                res->right->left = node_construct();
                res->right->right = node_construct();
                res->right->left->parent = res->right;
                res->right->right->parent = res->right;
                node *res1 = node_construct();
                node *res2 = node_construct();
                res1->code = OP;
                res1->value = MUL;
                res1->left = node_construct();
                res1->right = node_construct();
                res1->left->parent = res1;
                res1->right->parent = res1;
                left_insert(res1, node_diff(cell->right));
                res1->right->code = FUN;
                res1->right->value = LN;
                res1->right->left = node_construct();
                res1->right->left->parent = res1->right;
                copy_left(res1->right, cell->left);
                res2->code = OP;
                res2->value = DIV;
                res2->left = node_construct();
                res2->right = node_construct();
                res2->left->parent = res2;
                res2->right->parent = res2;
                res2->left->code = OP;
                res2->left->value = MUL;
                res2->left->left = node_construct();
                res2->left->right = node_construct();
                res2->left->left->parent = res2->left;
                res2->left->right->parent = res2->left;
                left_insert(res2->left, node_diff(cell->left));
                copy_right(res2->left, cell->right);
                copy_right(res2, cell->left);
                left_insert(res->right, res1);
                right_insert(res->right, res2);
            }
            break;
        case NUM:
            res->code = NUM;
            res->value = 0;
            break;
        case CONST:
            res->code = NUM;
            res->value = 0;
            break;
        case VAR:
            res->code = NUM;
            res->value = 1;
            break;
        case FUN:
            if (cell->value == SIN) {
                res->code = OP;
                res->value = MUL;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                node *res_l = node_construct();
                res_l->code = FUN;
                res_l->value = COS;
                res_l->left = node_construct();
                res_l->left->parent = res_l;
                copy_left(res_l, cell->left);
                left_insert(res, res_l);
                res->right = node_diff(cell->left);
            }
            if (cell->value == COS) {
                res->code = OP;
                res->value = MUL;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                node *res_l = node_construct();
                res_l->code = OP;
                res_l->value = MUL;
                res_l->left = node_construct();
                res_l->right = node_construct();
                res_l->left->parent = res_l;
                res_l->right->parent = res_l;
                res_l->left->code = NUM;
                res_l->left->value = -1;
                res_l->right->code = FUN;
                res_l->right->value = SIN;
                res_l->right->left = node_construct();
                res_l->right->left->parent = res_l->right;
                copy_left(res_l->right, cell->left);
                left_insert(res, res_l);
                res->right = node_diff(cell->left);
            }
            if (cell->value == TAN) {
                res->code = OP;
                res->value = DIV;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                res->left = node_diff(cell->left);
                res->right->code = OP;
                res->right->value = POW;
                res->right->left = node_construct();
                res->right->right = node_construct();
                res->right->left->parent = res->right;
                res->right->right->parent = res->right;
                res->right->left->code = FUN;
                res->right->left->value = SIN;
                res->right->left->left = node_construct();
                res->right->left->left->parent = res->right->left;
                copy_left(res->right->left, cell->left);
                res->right->right->code = NUM;
                res->right->right->value = 2;
            }
            if (cell->value == EXP) {
                res->code = OP;
                res->value = POW;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                res->left->code = CONST;
                res->left->value = CONST_E;
                copy_right(res, cell->left);
                return node_diff(res);
            }
            if (cell->value == LN) {
                res->code = OP;
                res->value = DIV;
                res->left = node_construct();
                res->right = node_construct();
                res->left->parent = res;
                res->right->parent = res;
                copy_right(res, cell->left);
                left_insert(res, node_diff(cell->left));
            }
            break;
        default:
            assert(!(std::cerr << "wrong node\n"));
            break;
    }
    return res;
}

void simple_tree (tree *birch) {
    assert(birch);
    simple_node(birch->root);
}

void simple_node (node *cell) {
    assert(cell);
    if (cell->left) simple_node(cell->left);
    if (cell->right) simple_node(cell->right);
    if (cell->code == OP && cell->value == ADD) {
        if (cell->left->code == NUM && cell->right->code == NUM) {
            cell->code = NUM;
            cell->value = cell->left->value + cell->right->value;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->left->code == NUM && cell->left->value == 0) {
            cell->code = cell->right->code;
            cell->value = cell->right->value;
            cell->left = cell->right->left;
            cell->right = cell->right->right;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == 0) {
            cell->code = cell->left->code;
            cell->value = cell->left->value;
            cell->right = cell->left->right;
            cell->left = cell->left->left;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
    }
    if (cell->code == OP && cell->value == SUB) {
        if (cell->left->code == NUM && cell->right->code == NUM) {
            cell->code = NUM;
            cell->value = cell->left->value - cell->right->value;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->left->code == NUM && cell->left->value == 0) {
            cell->code = OP;
            cell->value = MUL;
            cell->left->value = -1;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == 0) {
            cell->code = cell->left->code;
            cell->value = cell->left->value;
            cell->right = cell->left->right;
            cell->left = cell->left->left;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
    }
    if (cell->code == OP && (cell->value == MUL || cell->value == DIV)) {
        if (cell->left->code == NUM && cell->left->value == 1 && cell->value == MUL) {
            cell->code = cell->right->code;
            cell->value = cell->right->value;
            cell->left = cell->right->left;
            cell->right = cell->right->right;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == 1) {
            cell->code = cell->left->code;
            cell->value = cell->left->value;
            cell->right = cell->left->right;
            cell->left = cell->left->left;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
        if (cell->left->code == NUM && cell->left->value == -1 && cell->right->code == NUM && cell->value == MUL) {
            cell->code = NUM;
            cell->value = - cell->right->value;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == -1 && cell->left->code == NUM && cell->value == MUL) {
            cell->code = NUM;
            cell->value = - cell->left->value;
            cell->right = NULL;
            cell->left = NULL;
            return;
        }
        if ((cell->right->code == NUM && cell->right->value == 0 && cell->value == MUL) || (cell->left->code == NUM && cell->left->value == 0)) {
            cell->left = NULL;
            cell->right= NULL;
            cell->code = NUM;
            cell->value = 0;
            return;
        }
    }
    if (cell->code == FUN) {
        if (cell->value == SIN && cell->left->code == NUM && cell->left->value == 0) {
            cell->code = NUM;
            cell->value = 0;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->value == COS && cell->left->code == NUM && cell->left->value == 0) {
            cell->code = NUM;
            cell->value = 1;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->value == TAN && cell->left->code == NUM && cell->left->value == 0) {
            cell->code = NUM;
            cell->value = 0;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->value == LN && cell->left->code == NUM && cell->left->value == 1) {
            cell->code = NUM;
            cell->value = 0;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->value == LN && cell->left->code == CONST && cell->left->value == CONST_E) {
            cell->code = NUM;
            cell->value = 1;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }

    }
    if (cell->code == OP && cell->value == POW) {
        if (cell->left->code == NUM && cell->left->value == 0) {
            cell->code = NUM;
            cell->value = 0;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->left->code == NUM && cell->left->value == 1) {
            cell->code = NUM;
            cell->value = 1;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == 1) {
            cell->code = cell->left->code;
            cell->value = cell->left->value;
            cell->right = cell->left->right;
            cell->left = cell->left->left;
            if (cell->left) cell->left->parent = cell;
            if (cell->right) cell->right->parent = cell;
            return;
        }
        if (cell->right->code == NUM && cell->right->value == 0) {
            cell->code = NUM;
            cell->value = 1;
            cell->left = NULL;
            cell->right = NULL;
            return;
        }
    }
}
