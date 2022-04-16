%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	int bool = 0;
	FILE *fp, *ogFile;
	int count = 2056;
	int label[1000];
%}

%%

([L][0-9]+)([\n])      {
							if(bool) {
								yytext[0] = '0';
								int label_idx = atoi(yytext);
								printf("%d\n", label[label_idx]);
							}
							else {
								count += 2;
							}
					   }
([L][0-9]+)([:])([\n]) { 
							if(!bool) {
								char number[10];
								int i = 0;
								for(int j = 1; yytext[j] != ':'; ++j) {
									number[i++] = yytext[j];
								}
								number[i] = '\0';
								int label_idx = atoi(number);
								label[label_idx] = count;
							}
					   }
[\n]                   {
							if(!bool) {
								count = count + 2; 
							}
							else {
								printf("\n");
							}
					   }
(.)                    {
							if(bool) {
								printf("%s", yytext);
							}
					   }

%%

int yywrap() {
	if(bool == 0) {
		bool = 1;
		fclose(ogFile);
		fp = fopen("example.xsm","r");
		yyin = fp;
		return 0;
	}
	fclose(fp);
	return 1;
}

int main() {
	for(int i = 0; i < 1000; ++i) label[i] = 0;
	ogFile = fopen("example.xsm", "r");
	//skip 8 initial lines
	char *str;
	str = malloc(100);
	for(int i = 0; i < 8; ++i) {
		fgets(str, 100, ogFile);
		if(bool) printf("%s", str);
	}
	yyin = ogFile;
	yylex();
	return 0;
}
