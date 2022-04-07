struct tnode* makeLeafNode(int n) {
	struct tnode* temp;
	temp = malloc(sizeof(struct tnode));
	temp->val = n;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}
struct tnode* makeOperatorNode(char c, struct tnode* l, struct tnode* r) {
	struct tnode* temp;
	temp = malloc(sizeof(struct tnode));
	temp->op = malloc(1);
	*(temp->op) = c;
	temp->left = l;
	temp->right = r;
	return temp;
}

int reg_index = -1;
int getReg() {
	if(reg_index >= 19) {
		fprintf(stderr, "Error: too many registers\n");
		exit(1);
	}
	reg_index++;
	return reg_index;
}
void freeReg() {
	if(reg_index <= 0) {
		fprintf(stderr, "Error: no registers to free\n");
		exit(1);
	}
	reg_index--;
}

int codeGen(struct tnode* t) {
	/*
	 * codeGen will return the register number containing the result of
	 * the AST pointed by t
	 */
	if(t == NULL) {
		fprintf(stderr, "Error: null tree\n");
		exit(1);
	}
	else if(t->left == NULL && t->right == NULL) {
		int r0 = getReg();
		fprintf(stdout, "MOV R%d, %d\n", r0, t->val);
		return r0;
	}
	else {
		int r0 = codeGen(t->left);
		int r1 = codeGen(t->right);
		switch(*(t->op)) {
			case '+':
				fprintf(stdout, "ADD R%d, R%d\n", r0, r1);
				break;
			case '-':
				fprintf(stdout, "SUB R%d, R%d\n", r0, r1);
				break;
			case '*':
				fprintf(stdout, "MUL R%d, R%d\n", r0, r1);
				break;
			case '/':
				fprintf(stdout, "DIV R%d, R%d\n", r0, r1);
				break;
		}
		//result will be stored in r0
		//so we will release r1 using freeReg()
		//freeReg() will release the highest register currently in use
		freeReg();
		return r0;
	}
}
void magic() {
	fprintf(stdout, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0); 
}
void putBrk() {
	fprintf(stdout, "BRKP\n");
}
