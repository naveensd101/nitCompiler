typedef struct tnode {
	int val; // value of a number for num nodes
	char *varname; // name of variable
	int nodetype; // type of node (read, write, connector, "=", etc)
	struct tnode *left, *right; // left and right branches
}tnode;

tnode* makeTree(int val, char *varname, int nodetype, tnode *left, tnode *right);
tnode* makeIfElseTree(tnode *condition, tnode *if_true, tnode *if_false);

int getReg();
void freeReg();

int getLabel();

int codeGen(struct tnode* t); //returns the register number with the result
void magic();
void putBrk();

//types of nodes
#define variable_node 1
#define number_node 2
#define connector_node 3
#define read_node 4
#define write_node 5
#define math_node 6
#define assign_node 7
#define bool_node 8
#define if_node 9
#define if_else_node 10
