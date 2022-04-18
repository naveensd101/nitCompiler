struct tnode* makeTree(int val, char* varname, int nodetype, tnode* left, tnode* right) {
	struct tnode* temp;
	temp = malloc(sizeof(struct tnode));
	temp->nodetype = nodetype;
	temp->left = left;
	temp->right = right;
	switch(nodetype) {
		case variable_node:
			temp->varname = strdup(varname);
			break;
		case number_node:
			temp->val = val;
			break;
		case math_node:
			temp->varname = malloc(1);
			temp->varname[0] = varname[0];
			break;
		case bool_node:
			temp->varname = malloc(1);
			temp->varname[0] = varname[0];
			break;
		case if_node:
			break;
		case if_else_node:
			break;
		case return_node:
			break;
		case fun_call_node:
			break;
	}
	return temp;
}
struct tnode* makeIfElseTree(tnode *condition, tnode *if_true, tnode *if_false) {
	struct tnode* temp;
	temp = malloc(sizeof(struct tnode));
	temp->nodetype = if_else_node;
	temp->left = condition;
	struct tnode *connector;
	connector = malloc(sizeof(struct tnode));
	connector->val = 0;
	connector->varname = NULL;
	connector->nodetype = connector_node;
	connector->left = if_true;
	connector->right = if_false;
	temp->right = connector;
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
int label_index = 1;
int getLabel() {
	return label_index++;
}

int codeGen(struct tnode* t, struct Lsymbol* head) {
	if(t == NULL) {
		;
	}
	else if(t->nodetype == connector_node) {
		codeGen(t->left, head);
		codeGen(t->right, head);
		return -1;
	}
	else if(t->nodetype == math_node) {
		int r0, r1;
		r0 = codeGen(t->left, head);
		r1 = codeGen(t->right, head);
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
		return -1;
	}
	else if(t->nodetype == assign_node) {
		Lsymbol *ptr = head;
		Lsymbol *prev = NULL;
		int varpos = 0;
		int flag = 0;
		int count = 0;
		while(ptr != NULL) {
			count++;
			if(strcmp(ptr->name, t->left->varname) == 0) {
				varpos = ptr->binding;
				flag = 1;
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
		if(flag == 0) {
			//insert into symbol table this variable
			Lsymbol *_t;
			_t = malloc(sizeof(Lsymbol));
			_t->name = strdup(t->left->varname);
			//find out the binding address of the var
			_t->binding = count;
			fprintf(stdout, "PUSH SP\n");
			prev->next = _t;
			varpos = _t->binding;
		}

		int r0 = getReg();
		if(varpos < 0) {
			//var is argc
			fprintf(stdout, "MOV R%d, BP\n", r0);
			int temp_varpos = -1*varpos;
			fprintf(stdout, "SUB R%d, %d\n", r0, temp_varpos);
		}
		else {
			//var is local variable
			fprintf(stdout, "MOV R%d, BP\n", r0);
			fprintf(stdout, "ADD R%d, %d\n", r0, varpos);
		}
		//fprintf(stdout, "MOV R%d, [R%d]\n", r0, r0);
		int r1 = codeGen(t->right, head);
		fprintf(stdout, "MOV [R%d], R%d\n", r0, r1);
		freeReg();
		freeReg();
		return -1;
	}
	else if(t->nodetype == variable_node) {
		//search in the symbol table for 
		//varname == t->varnam
		Lsymbol *ptr = head;
		Lsymbol *prev = NULL;
		int varpos = 0;
		int flag = 0;
		int count = 0;
		while(ptr != NULL) {
			count++;
			if(strcmp(ptr->name, t->varname) == 0) {
				varpos = ptr->binding;
				flag = 1;
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
		if(flag == 0) {
			//insert into symbol table this variable
			Lsymbol *_t;
			_t = malloc(sizeof(Lsymbol));
			_t->name = strdup(t->varname);
			//find out the binding address of the var
			_t->binding = count;
			fprintf(stdout, "PUSH SP\n");
			prev->next = _t;
			varpos = _t->binding;
		}

		int r0 = getReg();
		if(varpos < 0) {
			//var is argc
			fprintf(stdout, "MOV R%d, BP\n", r0);
			int temp_varpos = -1*varpos;
			fprintf(stdout, "SUB R%d, %d\n", r0, temp_varpos);
		}
		else {
			//var is local variable
			fprintf(stdout, "MOV R%d, BP\n", r0);
			fprintf(stdout, "ADD R%d, %d\n", r0, varpos);
		}
		fprintf(stdout, "MOV R%d, [R%d]\n", r0, r0);
		return r0;
	}
	else if(t->nodetype == number_node) {
		int r0 = getReg();
		int val = t->val;
		fprintf(stdout, "MOV R%d, %d\n", r0, val);
		return r0;
	}
	else if(t->nodetype == read_node) {
		//int r0 = codeGen(t->left, head);
		int r0 = getReg();
		Lsymbol *ptr;
		Lsymbol *prev = NULL;
		int flag = 0;
		int count = 0;
		int varpos = 0;
		for(ptr = head; ptr != NULL; ptr = ptr->next) {
			count++;
			if(strcmp(ptr->name, t->left->varname) == 0) {
				varpos = ptr->binding;
				flag = 1;
				break;
			}
			prev = ptr;
		}
		if(flag == 0) {
			//we have to create a new variable
			//insert into symbol table this variable
			Lsymbol *_t;
			_t = malloc(sizeof(Lsymbol));
			_t->name = strdup(t->left->varname);
			//find out the binding address of the var
			_t->binding = count;
			fprintf(stdout, "PUSH SP\n");
			prev->next = _t;
			varpos = _t->binding;
		}
		if(varpos < 0) {
			//var is argc
			fprintf(stdout, "MOV R%d, BP\n", r0);
			int temp_varpos = -1*varpos;
			fprintf(stdout, "SUB R%d, %d\n", r0, temp_varpos);
		}
		else {
			//var is local variable
			fprintf(stdout, "MOV R%d, BP\n", r0);
			fprintf(stdout, "ADD R%d, %d\n", r0, varpos);
		}
		int r1 = getReg();
		fprintf(stdout, "MOV R%d, \"Read\"\n", r1);
		fprintf(stdout, "PUSH R%d\n", r1); //function code
		fprintf(stdout, "MOV R%d, -1\n", r1);
		fprintf(stdout, "PUSH R%d\n", r1); //arg1
		fprintf(stdout, "PUSH R%d\n", r0); //arg2
		fprintf(stdout, "PUSH R%d\n", r0); //arg3
		fprintf(stdout, "PUSH R%d\n", r0); //ret pos
		fprintf(stdout, "CALL 0\n");
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		freeReg();
		freeReg();
		return -1;
	}
	else if(t->nodetype == write_node) {
		int r0 = getReg();
		int r1 = codeGen(t->left, head);
		fprintf(stdout, "MOV R%d, \"Write\"\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //function code
		fprintf(stdout, "MOV R%d, -2\n", r0);
		fprintf(stdout, "PUSH R%d\n", r0); //arg1
		fprintf(stdout, "PUSH R%d\n", r1); //arg2
		fprintf(stdout, "PUSH R%d\n", r0); //arg3
		fprintf(stdout, "PUSH R%d\n", r0); //ret pos
		fprintf(stdout, "CALL 0\n");
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		fprintf(stdout, "POP R%d\n", r0);
		freeReg();
		freeReg();
		return -1;
	}
	else if(t->nodetype == bool_node) {
		int r0 = codeGen(t->left, head);
		int r1 = codeGen(t->right, head);
		switch(t->varname[0]) {
			case '>':
				fprintf(stdout, "GT R%d, R%d\n", r0, r1);
				break;
			case '<':
				fprintf(stdout, "LT R%d, R%d\n", r0, r1);
				break;
			case 'l':
				fprintf(stdout, "LE R%d, R%d\n", r0, r1);
				break;
			case 'g':
				fprintf(stdout, "GE R%d, R%d\n", r0, r1);
				break;
			case 'e':
				fprintf(stdout, "EQ R%d, R%d\n", r0, r1);
				break;
			case 'n':
				fprintf(stdout, "NE R%d, R%d\n", r0, r1);
				break;
		}
		freeReg();
		return r0;
	}
	else if(t->nodetype == if_node) {
		int r0 = codeGen(t->left, head);
		int label = getLabel();
		fprintf(stdout, "JZ R%d, L%d\n", r0, label);
		codeGen(t->right, head);
		fprintf(stdout, "L%d:\n", label);
		freeReg();
		return -1;
	}
	else if(t->nodetype == if_else_node) {
		int r0 = codeGen(t->left, head);
		int label1 = getLabel();
		int label2 = getLabel();
		fprintf(stdout, "JZ R%d, L%d\n", r0, label1);
		codeGen(t->right->left, head);
		fprintf(stdout, "JMP L%d\n", label2);
		fprintf(stdout, "L%d:\n", label1);
		codeGen(t->right->right, head);
		fprintf(stdout, "L%d:\n", label2);
		freeReg();
		return -1;
	}
	else if(t->nodetype == return_node) {
		int r0 = codeGen(t->left, head);
		int r1 = getReg();
		//pop out variables from stack
		//we have to do a linear search in symbol table
		//we should not pop argc
		//argc is not a local variable
		//argc is in the symbol table only for easiness
		Lsymbol *ptr, *prev;
		prev = NULL;
		for(ptr = head; ptr->next != NULL; ptr = ptr->next) {
			fprintf(stdout, "POP R%d\n", r1);
			free(prev);
			prev = ptr;
		}
		fprintf(stdout, "MOV R%d, BP\n", r1);
		fprintf(stdout, "SUB R%d, 2\n", r1);
		fprintf(stdout, "MOV [R%d], R%d\n", r1, r0);
		fprintf(stdout, "POP BP\n");
		fprintf(stdout, "RET\n");
		freeReg();
		freeReg();
		return -1;
	}
	else if(t->nodetype == fun_def_node) {
		fprintf(stdout, "PUSH BP\n");
		fprintf(stdout, "MOV BP, SP\n");
		//make local symbol table
		//add an entry for argc
		//i.e, symbol table head should point to BP-2
		struct Lsymbol *_t;
		_t = malloc(sizeof(struct Lsymbol));
		_t->name = strdup("argc");
		_t->binding = -2;
		_t->next = NULL;
		t->head = _t;
		setupSymbolTable(t->left, t->head);
		codeGen(t->left, t->head);
		return -1;
	}
	else if(t->nodetype == fun_call_node) {
		int r0 = codeGen(t->left, head);
		int temp = reg_index;
		for(int i = 0; i <= reg_index; ++i) {
			//push all the used registers
			fprintf(stdout, "PUSH R%d\n", i);
		}
		fprintf(stdout, "PUSH R%d\n", r0); //push argument
		fprintf(stdout, "PUSH R%d\n", r0); //push empty for ret
		reg_index = -1;
		fprintf(stdout, "CALL L0\n");
		fprintf(stdout, "POP R0\n");
		fprintf(stdout, "POP R1\n");
		fprintf(stdout, "MOV R%d, R0\n", temp+1);
		for(int i = temp; i >= 0; i--) {
			fprintf(stdout, "POP R%d\n", i);
		}
		fprintf(stdout, "MOV R%d, R%d\n", r0, temp+1);
		reg_index = temp;
		return r0;
	}
	return -1;
}
void magic() {
	fprintf(stdout, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0); 
}
void putBrk() {
	fprintf(stdout, "BRKP\n");
}

tnode* initTree(tnode *stmts) {
	int r0 = getReg();
	//int varpos = (int)(t->left->varname[0] - 'a') + 4096;
	int varpos = 4096; //global variable
	fprintf(stdout, "MOV SP, 4096\n");
	fprintf(stdout, "MOV R%d, \"Read\"\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //function code
	fprintf(stdout, "MOV R%d, -1\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //arg1
	fprintf(stdout, "MOV R%d, %d\n", r0, varpos);
	fprintf(stdout, "PUSH R%d\n", r0); //arg2
	fprintf(stdout, "PUSH R%d\n", r0); //arg3
	fprintf(stdout, "PUSH R%d\n", r0); //ret pos
	fprintf(stdout, "CALL 0\n");
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	freeReg();
	r0 = getReg();
	fprintf(stdout, "MOV R%d, [4096]\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //arg1
	fprintf(stdout, "PUSH R%d\n", r0); //ret empty
	freeReg();
	fprintf(stdout, "CALL L0\n");
	r0 = getReg();
	fprintf(stdout, "POP R%d\n", r0); //ret val
	fprintf(stdout, "MOV [4096], R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0); //arg1
	freeReg();

	r0 = getReg();
	fprintf(stdout, "MOV R%d, \"Write\"\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //function code
	fprintf(stdout, "MOV R%d, -2\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //arg1
	fprintf(stdout, "MOV R%d, [4096]\n", r0);
	fprintf(stdout, "PUSH R%d\n", r0); //arg2
	fprintf(stdout, "PUSH R%d\n", r0); //arg3
	fprintf(stdout, "PUSH R%d\n", r0); //ret pos
	fprintf(stdout, "CALL 0\n");
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	fprintf(stdout, "POP R%d\n", r0);
	freeReg();

	fprintf(stdout, "MOV R0, \"Exit\"\n");
	fprintf(stdout, "PUSH R0\n"); //fun code
	fprintf(stdout, "PUSH R0\n"); //arg1
	fprintf(stdout, "PUSH R0\n"); //arg2
	fprintf(stdout, "PUSH R0\n"); //arg3
	fprintf(stdout, "PUSH R0\n"); //ret val
	fprintf(stdout, "CALL 0\n"); //ret val
	fprintf(stdout, "L0:\n");

	//-------------------------------------
	tnode *root = makeTree(-1, NULL, fun_def_node, stmts, NULL);
	//fprintf(stdout, "PUSH BP\n");
	//fprintf(stdout, "MOV BP, SP\n");
	//create(fun_def_node);
	//big_tree = combine fun_def_node with root
	return root;
}
void setupSymbolTable(tnode* root, Lsymbol* head) {
	if(root == NULL) return;
	else if(root->nodetype == variable_node) {
		Lsymbol *ptr = head;
		Lsymbol *prev = NULL;
		int varpos = 0;
		int count = 0;
		int flag = 0;
		while(ptr != NULL) {
			count++;
			if(strcmp(ptr->name, root->varname) == 0) {
				varpos = ptr->binding;
				flag = 1;
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
		if(flag == 0) {
			//insert into symbol table this variable
			Lsymbol *_t;
			_t = malloc(sizeof(Lsymbol));
			_t->name = strdup(root->varname);
			//find out the binding address of the var
			_t->binding = count;
			fprintf(stdout, "PUSH SP\n");
			prev->next = _t;
			varpos = _t->binding;
			fprintf(stderr, "varname = %s\nbinding = %d\n", _t->name, _t->binding);
		}

		//int r0 = getReg();
		//if(varpos < 0) {
		//	//var is argc
		//	fprintf(stdout, "MOV R%d, BP\n", r0);
		//	int temp_varpos = -1*varpos;
		//	fprintf(stdout, "SUB R%d, %d\n", r0, temp_varpos);
		//}
		//else {
		//	//var is local variable
		//	fprintf(stdout, "MOV R%d, BP\n", r0);
		//	fprintf(stdout, "ADD R%d, %d\n", r0, varpos);
		//}
	}
	else {
		setupSymbolTable(root->left, head);
		setupSymbolTable(root->right, head);
	}
}
