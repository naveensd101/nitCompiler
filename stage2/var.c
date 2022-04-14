struct tnode* makeTree(int val, char* varname, int nodetype, tnode* left, tnode* right) {
	struct tnode* temp;
	temp = malloc(sizeof(struct tnode));
	temp->nodetype = nodetype;
	temp->left = left;
	temp->right = right;
	switch(nodetype) {
		case variable_node:
			temp->varname = malloc(1);
			temp->varname[0] = varname[0];
			break;
		case number_node:
			temp->val = val;
			break;
		case math_node:
			temp->varname = malloc(1);
			temp->varname[0] = varname[0];
			break;
	}
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
	if(reg_index < 0) {
		fprintf(stderr, "Error: no registers to free\n");
		exit(1);
	}
	reg_index--;
}

int codeGen(struct tnode* t) {
	if(t == NULL) {
		;
	}
	else if(t->nodetype == connector_node) {
		int _temp;
		_temp = codeGen(t->left);
		_temp = codeGen(t->right);
	}
	else if(t->nodetype == math_node) {
		int r0, r1;
		r0 = codeGen(t->left);
		r1 = codeGen(t->right);
		switch(t->varname[0]) {
			case '+':
				fprintf(stdout, "ADD R%d, R%d\n", r0, r1);
				freeReg();//to make r1 free
				return r0;
			case '-':
				fprintf(stdout, "SUB R%d, R%d\n", r0, r1);
				freeReg();//to make r1 free
				return r0;
			case '*':
				fprintf(stdout, "MUL R%d, R%d\n", r0, r1);
				freeReg();//to make r1 free
				return r0;
		}
	}
	else if(t->nodetype == assign_node) {
		int r0 = getReg();
		int r1 = codeGen(t->right);
		int varpos = (int)(t->left->varname[0] - 'a') + 4096;
		fprintf(stdout, "MOV R%d, %d\n", r0, varpos);
		fprintf(stdout, "MOV [R%d], R%d\n", r0, r1);
		freeReg();
		freeReg();
		return -1;
	}
	else if(t->nodetype == variable_node) {
		int r0 = getReg();
		int varpos = (int)(t->varname[0] - 'a') + 4096;
		fprintf(stdout, "MOV R%d, [%d]\n", r0, varpos);
		return r0;
	}
	else if(t->nodetype == number_node) {
		int r0 = getReg();
		int val = t->val;
		fprintf(stdout, "MOV R%d, %d\n", r0, val);
		return r0;
	}
	else if(t->nodetype == read_node) {
		int r0 = getReg();
		int varpos = (int)(t->left->varname[0] - 'a') + 4096;
		fprintf(stdout, "MOV R%d, \"Read\"\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //function code
		fprintf(stdout, "MOV R%d, -1\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //arg1
		fprintf(stdout, "MOV R%d, %d\n", r0, varpos);
		fprintf(stdout, "PUSH R%d\n", r0); //arg2
		fprintf(stdout, "PUSH R%d\n", r0); //arg3
		fprintf(stdout, "PUSH R%d\n", r0); //ret pos
		fprintf(stdout, "CALL 0\n");
		freeReg();
		return -1;
	}
	else if(t->nodetype == write_node) {
		int r0 = getReg();
		int r1 = codeGen(t->left);
		fprintf(stdout, "MOV R%d, \"Write\"\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //function code
		fprintf(stdout, "MOV R%d, -2\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //arg1
		fprintf(stdout, "MOV R%d, R%d\n", r0, r1);
		fprintf(stdout, "PUSH R%d\n", r0); //arg2
		fprintf(stdout, "PUSH R%d\n", r0); //arg3
		fprintf(stdout, "PUSH R%d\n", r0); //ret pos
		fprintf(stdout, "CALL 0\n");
		freeReg();
		freeReg();
		return -1;
	}
	return -1;
}
void magic() {
	fprintf(stdout, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0); 
	fprintf(stdout, "MOV SP, %d\n", 4096+(int)('z'-'a'));
}
void putBrk() {
	fprintf(stdout, "BRKP\n");
}
void sexyExit() {
	fprintf(stdout, "MOV R0, \"Exit\"\n");
	fprintf(stdout, "PUSH R0\n"); //fun code
	fprintf(stdout, "PUSH R0\n"); //arg1
	fprintf(stdout, "PUSH R0\n"); //arg2
	fprintf(stdout, "PUSH R0\n"); //arg3
	fprintf(stdout, "PUSH R0\n"); //ret val
	fprintf(stdout, "CALL 0\n"); //ret val
}
