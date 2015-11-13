#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SAT.h"
#include <unistd.h>
#define MAXLENGTH 5000000 
struct node{
	int index;  //unique index of the node
	int parent; //parent of the node
	int key;    //a big number
	struct node *child;//next child
	struct node *next; //next node
};

int getint(char *s){
	int i = 0;
	int p = 1;
	int len2 = strlen(s);
	int num = 0;
	for (i = 0; i<len2; i++){
		p = 1;
		int j = 0;
		for (j = 0; j<len2 - i - 1; j++){
			p = p * 10;
		}
		if (!(s[i] - '0' >= 0 && s[i] - '0' <= 9)){
			return -1;
		}
		else{
			num = num + p*(s[i] - '0');
		}
	}
	return num;
}
struct node* findindex(struct node *f, int index){
	while (f != NULL){
		if (f->index == index) return f;
		f = f->next;
	}
	return NULL;
}

struct node* findchild(struct node *f, int index){
	while (f != NULL){
		if (f->index == index) return f;
		f = f->child;
	}
	return NULL;
}

void fr(int** ch,int i){
	if(ch[i+1]!=NULL) fr(ch,i+1);
	if(ch[i]!=NULL){
		free(ch[i]);
	}
}
void freenextnode(struct node *f){
	if(f->next!=NULL) freenextnode(f->next);
	if(f!=NULL) free(f);
}
void freechildnode(struct node *f){
	if(f->child!=NULL) freechildnode(f->child);
	if(f!=NULL){
		if(f->next!=NULL) freenextnode(f->next);
		if(f!=NULL) free(f);
	}
}
int main(){
	fprintf(stdout, "fuck1\n");
	fprintf(stdout, "fuck2\n");
	char *name=NULL;
	unsigned long maxl=MAXLENGTH;
	name = (char *)malloc(maxl * sizeof(char));
	int **cor;
	cor=(int**)malloc(1 * sizeof(int*));
	int nume = 0;
	int all = 0;
	int start;
	int end;
	int index = 0;
	int alloc = 0;
	struct node *first=NULL;
	while (getline(&name,&maxl,stdin)!=EOF){
		name[strlen(name)-1]='\0';
		if (name[0] == 'V'){
			index = 0;
			int len = strlen(name);
			int num = 0;
			int i = 2;
			int j = 0;
			int p = 1;
			int fau = 0;
			for (i = 2; i<strlen(name); i++){
				p = 1;
				for (j=1;j<=len-i-1;j++){
					p = p * 10;
				}
				if (!(name[i] - '0' >= 0 && name[i] - '0' <= 9)){
					fprintf(stdout, "Error: wrong V\n");
					fau = 1;
					break;
				}
				else{
					num = num + p*(name[i] - '0');
				}
			}

			if (num == 0 && fau == 0){
				fprintf(stdout, "Error: wrong V\n");
				continue;
			}
			else{
				all = num;
			}
			index = 1;
			
		}
		else if (name[0] == 'E'){
			alloc = 0;
			if(sizeof(int*)!=sizeof(cor)){
				fr(cor,0);
				cor=NULL;
			}
			if (cor != NULL){	
				free(cor);
			}
			cor = (int **)malloc(1 * sizeof(int*));
			if(first!=NULL&&first->child!=NULL){
				freechildnode(first->child);
			}
			if(first!=NULL){
				free(first);
				first=NULL;
			}
			if (index != 1){
				fprintf(stdout, "Error: Wrong input.\n");
				continue;
			}
			nume = 0;
			int i = 4;
			char x[100];
			int pointer = 0;
			int xp = 0;
			memset(x, 0, 100 * sizeof(char));
			int fault = 0;
			int outof = 0;
			for (i = 4; i<strlen(name) - 1; i++){
				if (fault == 1) break;

				if (name[i] == ',' && name[i - 1] != '>'){
					pointer = 1;
					cor = (int **)realloc(cor, (alloc + 1)*sizeof(int*));
					cor[alloc++] = (int*)malloc(2 * sizeof(int));
					cor[nume][0] = getint(x);
					if (cor[nume][0]<0 || cor[nume][0] >= all){
						fprintf(stdout, "Error: Wrong point\n");
						fault = 1;
						break;
					}
					memset(x, 0, 100 * sizeof(char));
					xp = 0;
				}
				else if (name[i] == '>' && pointer == 1){
					long long mul = (long)all*(long)all;
					if (nume >= mul){
						fprintf(stdout, "Error: Wrong input of E\n");
						fault = 1;
						break;
					}
					pointer = 0;
					cor[nume][1] = getint(x);
					if (cor[nume][1]<0 || cor[nume][1] >= all){
						fprintf(stdout, "Error: Wrong point\n");
						fault = 1;
						break;
					}
					struct node *anode = (struct node *)malloc(sizeof(struct node));
					anode->index = cor[nume][0];
					anode->key = 999999;
					anode->next = NULL;
					anode->child = NULL;
					struct node *tf = first;
					int same = 0;
					if (tf == NULL){
						first = anode;
						struct node *sc = (struct node *)malloc(sizeof(struct node));
						sc->next = NULL;
						sc->child = NULL;
						sc->index = cor[nume][1];
						sc->key = 999999;
						first->next =sc;
						same = 1;
					}
					else{
						while (tf != NULL){
							if (tf->index == anode->index){
								struct node *s2 = findindex(tf->next, cor[nume][1]);
								if (s2 == NULL){
									struct node *ss2 = (struct node *)malloc(sizeof(struct node));
									ss2->next = NULL;
									ss2->child = NULL;
									ss2->key = 999999;
									ss2->index = cor[nume][1];
									ss2->next = tf->next;
									tf->next = ss2;
								}
								same = 1;
								break;
							}
							else{
								tf = tf->child;
							}
						}
					}
					if (same == 0){
						anode->child = first;
						first = anode;
						struct node *s3 = (struct node*)malloc(sizeof(struct node));
						s3->next = NULL;
						s3->child = NULL;
						s3->key = 999999;
						s3->index = cor[nume][1];
						first->next = s3;
					}

					struct node *anode2=(struct node*)malloc(sizeof(struct node));
					anode2->index = cor[nume][1];
					anode2->key = 999999;
					anode2->child = NULL;
					anode2->next = NULL;
					struct node *tf2 = first;
					int same2 = 0;
					while (tf2 != NULL){
						if (tf2->index == anode2->index){
							struct node *s4 = findindex(tf2->next, cor[nume][0]);
							if (s4 == NULL){
								struct node *ss4 = (struct node*)malloc(sizeof(struct node));
								ss4->next = NULL;
								ss4->child = NULL;
								ss4->key = 999999;
								ss4->index = cor[nume][0];
								ss4->next = tf2->next;
								tf2->next = ss4;
							}
								same2 = 1;
								break;
							}
						else{
							tf2 = tf2->child;
						}
					}
					if (same2 == 0){
						anode2->child = first;
						first = anode2;
						struct node *s5 = (struct node*)malloc(sizeof(struct node));
						s5->child = NULL;
						s5->next= NULL;
						s5->key = 999999;
						s5->index = cor[nume][0];
						first->next = s5;
					}
					nume++;
					memset(x, 0, 100 * sizeof(char));
					xp = 0;
				}
				else if (name[i] >= '0' && name[i] <= '9'){
					x[xp++] = name[i];
				}
				else if(name[i]=='-'){
					fprintf(stdout, "Error: Wrong point\n");
					fault = 1;
					break;	
				}
				else{
					xp = 0;
				}

			}
			if (fault == 1) continue;
			index = 2;



			// int p[2], bak;
		 //   pipe(p);
		 //    bak = dup(STDOUT_FILENO);
		 //   dup2(p[1], STDOUT_FILENO);
		    int j,l,count;
		    int result;
		    //all=0  all=1??
		    int k;
		    //fprintf(stdout,"all:%d\n",all);
		    //fflush(stdout);
		    fprintf(stdout,"hehe1\n");
		    for(i=1;i<=all;i++){
		    	//fprintf(stdout,"i:%d\n",i);
		    	//fflush(stdout);
		    	k=i;
		    	SAT_Manager mgr = SAT_InitManager();
   				SAT_SetNumVariables(mgr, all*k);
   				int *c;
   				fprintf(stdout,"hehe2\n");
   				c=(int *)malloc(sizeof(int)*(i*(all+1)));
   				for(j=1;j<=k;j++){
   					count=0;
   					for(l=1;l<=all;l++){
   						c[count++]=((((l-1)*k)+j)<<1);
   						//fprintf(stdout,"%d^", c[count-1]);
   						//fflush(stdout);
   					}
   					SAT_AddClause(mgr, c, count);
   					//fprintf(stdout,"\n");
   					//fflush(stdout);
   				}
   				fprintf(stdout,"hehe3\n");
   				int p,q;
   				int c2[2];
   				for(j=1;j<=all;j++){	
   					
   					for(q=2;q<=k;q++){
   						for(p=1;p<q;p++){
   							c2[0]=((((j-1)*k)+p)<<1)+1;
   							c2[1]=((((j-1)*k)+q)<<1)+1;
   							//fprintf(stdout,"%d^%d", c[0],c[1]);
   							SAT_AddClause(mgr, c2, 2);
   							//fprintf(stdout,"\n");
   							//fflush(stdout);
   						}
   						
   					}
   				}
   				fprintf(stdout,"hehe4\n");
   				for(j=1;j<=k;j++){
   					for(q=2;q<=all;q++){
   						for(p=1;p<q;p++){
   							c2[0]=(((p-1)*k+j)<<1)+1;
   							c2[1]=(((q-1)*k+j)<<1)+1;
   							//fprintf(stdout,"%d^%d", c2[0],c2[1]);
   							SAT_AddClause(mgr, c2, 2);
   							//fprintf(stdout,"\n");
   							//fflush(stdout);
   						}
   						
   					}
   				}
   				fprintf(stdout,"hehe5\n");
   				struct node *head = first;
				struct node *hnext = first;
				struct node *h = first;
				int *c3;
				c3=(int *)malloc(sizeof(int)*2*k);
				while(h!=NULL){
					hnext=h->next;
					while(hnext!=NULL){
						//hnext=hnext->next;
						count=0;
						for(j=1;j<=k;j++){
							//fprintf(stdout,"%d----%d:", h->index,hnext->index);
							c3[count++]=(((h->index)*k+j)<<1);
							c3[count++]=(((hnext->index)*k+j)<<1);	
							//fprintf(stdout,"%d^%d\n", c3[count-2],c3[count-1]);	
							//fflush(stdout);
						}
						//printf("\n");
						//fflush(stdout);
						SAT_AddClause(mgr, c3, 2*k);
						hnext=hnext->next;
					}
					h=h->child;
				}
				fprintf(stdout,"hehe6\n");
				//dup2(bak, STDOUT_FILENO);
				
				result = SAT_Solve(mgr);
			    
			    fprintf(stdout,"hehe9\n");
			    if(result == SATISFIABLE) {
					int nn = SAT_NumVariables(mgr);
					int pre=-1;
					fprintf(stdout,"hehe7\n");
					for(i = 1; i <= nn; i++) {
					    int a = SAT_GetVarAsgnment(mgr, i);
					    if(a == 1) {
					    	if(pre!=-1){
					    		fprintf(stdout,"%d ", (pre-1)/k); fflush(stdout);
					    	}
						 	pre=i;
					    }
					   	else if(a == 0) {
							continue;
				    	}
				   		else {
							fprintf(stderr,"Error: zchaff error!"); fflush(stdout);
				    	}
					}
					fprintf(stdout,"%d\n", (pre-1)/k);
					fflush(stdout);
					break;
			    }
			    else {
					continue;
			    }
			    free(c3);
			    free(c);

		    }


		}
		memset(name,0,maxl * sizeof(char));
	}
}