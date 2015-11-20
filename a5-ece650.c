#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "SAT.h"
#include <unistd.h>
#define MAXLENGTH 5000000 
struct node{
	int index;  //unique index of the node
	int parent; //parent of the node
	int key;    //a big number
	int sum;//sum of incident edges
	int kill2;
	int kill3;
	struct node *child;//next child
	struct node *next; //next node
};
struct argua4{
	struct node *fir;
	int all;
	int *s;

};

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
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
void* a4(void* argu){	
	struct argua4 *arg=(struct argua4*) argu;
	struct node *first=arg->fir;
	int all=arg->all;
	int p[2], bak;
  	pipe(p);
    bak = dup(STDOUT_FILENO);
   	dup2(p[1], STDOUT_FILENO);
    int j,l,count;
    int result;
    int k;
    int numn=0;
	struct node *se = first;
	int *hash2=(int *)malloc(sizeof(int)*(all+1));
	while(se!=NULL){
		numn++;
		se=se->child;
	}
	int *num=(int *)malloc(sizeof(int)*(numn+1));
	se=first;
	int cc=1;
	while(se!=NULL){
		num[cc++]=se->index;
		se=se->child;
	}
	int i=0;
	for(i=1;i<=numn;i++){
		hash2[num[i]]=i;
	}
    for(i=1;i<=numn;i++){
    	k=i;
    	SAT_Manager mgr = SAT_InitManager();
			SAT_SetNumVariables(mgr, numn*k);
			int *c;
			c=(int *)malloc(sizeof(int)*(i*(numn+1)));
			for(j=1;j<=k;j++){
				count=0;
				for(l=1;l<=numn;l++){
					c[count++]=((((l-1)*k)+j)<<1);
				}
				SAT_AddClause(mgr, c, count);
			}
			int p,q;
			int c2[2];
			for(j=1;j<=numn;j++){	
				//if(hash[j]==1){
					for(q=2;q<=k;q++){
						for(p=1;p<q;p++){
							c2[0]=((((j-1)*k)+p)<<1)+1;
							c2[1]=((((j-1)*k)+q)<<1)+1;
							SAT_AddClause(mgr, c2, 2);
						}
					
					}
			}
			fprintf(stdout,"\n");fflush(stdout);
			for(j=1;j<=k;j++){
				for(q=2;q<=numn;q++){
					for(p=1;p<q;p++){
							c2[0]=(((p-1)*k+j)<<1)+1;
							c2[1]=(((q-1)*k+j)<<1)+1;
							SAT_AddClause(mgr, c2, 2);
					}
					
				}
			}
			struct node *head = first;
		struct node *hnext = first;
		struct node *h = first;
		
		
		int *c3;
		c3=(int *)malloc(sizeof(int)*2*k);
		while(h!=NULL){
			hnext=h->next;
			while(hnext!=NULL){
				count=0;
				for(j=1;j<=k;j++){
					c3[count++]=(((hash2[h->index]-1)*k+j)<<1);
					c3[count++]=(((hash2[hnext->index]-1)*k+j)<<1);	
				}
				SAT_AddClause(mgr, c3, 2*k);
				hnext=hnext->next;
			}
			h=h->child;
		}
		
		result = SAT_Solve(mgr);
	    int ck=1;
	    if(result == SATISFIABLE) {
	    	int *res=(int *)malloc(sizeof(int)*(numn+1));
			int nn = SAT_NumVariables(mgr);
			int pre=-1;
			dup2(bak, STDOUT_FILENO);
			for(i=1;i<=numn;i++){
				int coun=0;
				for(coun=1;coun<=k;coun++){
					int a = SAT_GetVarAsgnment(mgr, (i-1)*k+coun);
					if(a==1){
						res[ck++]=num[i];
						break;
					}else if(a==0){
						continue;
					}else{
						fprintf(stderr,"Error: zchaff error!"); fflush(stdout);
					}
				}
				
			}
			res[0]=ck;
			arg->s=res;
			// for(i=0;i<ck-1;i++){
			// 	fprintf(stdout,"%d ", res[i]); fflush(stdout);
			// }
			// fprintf(stdout, "%d\n",res[ck-1]);fflush(stdout);
			return NULL;
			break;
			free(res);
	    }
	    else {
			continue;
	    }
	    free(c3);
	    free(c);
	    free(hash2);
    }
}
struct node* copy(struct node* first){
	struct node *he=first;
	struct node *head=first;
	struct node *ne=(struct node*)malloc(sizeof(struct node));
	struct node *init=ne;
	struct node *newf=ne;
	struct node *newfc=ne;
	struct node *newfn=ne;
	while(he!=NULL){
		head=he;
		newf->index=he->index;
		newfn=newf;
		int count=0;
		while(head->next!=NULL){
			count++;
			head=head->next;
			newfn->next=(struct node*)malloc(sizeof(struct node));
			newfn=newfn->next;
			newfn->index=head->index;
			if(head->next==NULL){
				newfn->next=NULL;
				break;
			}
		}
		newf->sum=count;
		he=he->child;
		if(he!=NULL){
			newf->child=(struct node*)malloc(sizeof(struct node));
			newf=newf->child;
		}else{
			newf->child=NULL;
			break;
		}
	}
	return init;
}
void* approx1(void * argu){
	struct argua4 *arg=(struct argua4 *)argu;
	struct node *first=arg->fir;
	struct node *he=first;
	struct node *head=first;
	struct node *fhead;
	//struct node new;
	struct node *init;
	init=copy(first);
	struct node *newf= init;
	struct node *newfc=init;
	struct node *newfn=init;
	he=init;
	int max=0;
	int id=0;
	int *res=(int *)malloc(MAXLENGTH*sizeof(int));
	int cres=1;
	//fprintf(stdout,"START \n");
	fflush(stdout);
	while(true){
		he=init;
		max=0;
		id=0;
		while(he!=NULL){
			if(max<he->sum){
				max=he->sum;
				id=he->index;
			}
			he=he->child;
		}
		if(max==0){
			break;
		}
		res[cres++]=id;
		//fprintf(stdout,"s:%d ",id);fflush(stdout);
	   // res=(int *)realloc(res,(cres+1)*sizeof(int));	V 10 E {<0,1>,<1,4>,<2,3>,<3,6>,<6,8>}	
		//first=init;
		if(init->index==id){
			init=init->child;
		}
		first=init;
		he=init;
		while(he!=NULL){
			int csum=0;
			if(he->index==id){
				if(he==init){
					init=init->child;
					he=init;
					first=he;
					// if(he==NULL) {
					// 	res[0]=cres;
					// 	arg->s=res;
					// 	return NULL;
					// }
					// else{
					// 	he=he->child;
					// }
				}else{
					first->child=he->child;
					he=first->child;
				}
			}else{
				head=he->next;
				fhead=he;
				while(head!=NULL){
					if(head->index==id){
						fhead->next=head->next;
						csum++;
						head=fhead->next;
					}else{
						fhead=head;
						head=fhead->next;
					}
				}
				he->sum=he->sum-csum;
				if(he->sum==0){
					if(he==init){
						init=init->child;
						first=init;
						if(init==NULL) {
							res[0]=cres;
							arg->s=res;
							return NULL;
						}
						he=first;///////
						
					}else{
						first->child=he->child;
						he=first->child;
					}
				}else{
					first=he;
					he=first->child;
				}
				
			}//end if
			
		}//end while

	}
	res[0]=cres;
	arg->s=res;
	// int iii=0;
	// for(;iii<cres;iii++){
	// 	fprintf(stdout,"last:%d ",res[iii]);fflush(stdout);
	// }
	//return (void *)res;
	//if(he!=NULL) max=
	//int cres=0;
	//int *res=malloc(sizeof(int));
	// while(true){
	// 	he=first;
	// 	max=0;
	// 	index=-1;
	// 	while(he!=NULL){
	// 		if(he->sum!=0&&max<he->sum) {
	// 			max=he->sum;
	// 			index=he->index;
	// 		}
	// 		he=he->child;
	// 	}
	// 	if(max==0) break;
	// 	res[cres++]=index;
	// 	res=remalloc(res,(cres+1)*sizeof(int));
	// 	struct node *test=first;
	// 	struct node *star=first;
	// 	struct node *fa=first;


	// 	while(test!=NULL){
	// 		head=test;
	// 		he=test;
	// 		star=test;
	// 		struct node* fa=he;
	// 		if(head->kill2>0){
	// 			int su=0;
	// 			while(head!=NULL){
	// 				if(head->kill2>0&&head->index==index){
	// 					su++;
	// 					head->kill2=0;
	// 					if(head!=he){
	// 						if(fa!=NULL){
	// 							fa->next=head->next;
	// 						}
	// 					}
	// 				}
	// 				fa=head;
	// 				head=head->next;
	// 			}
	// 			star->sum=star->sum-su;
	// 			if(star->sum<=0) {
	// 				star->kill2=0;
	// 			}
	// 		}
	// 		fa=head;
	// 		head=head->child;
	// 	}

	// }
	


}
struct node* deletes(struct node* fr,int id1,int id2){
	struct node *head=fr;
	struct node *fhead;
	struct node *he;
	struct node *fhe;
	struct node *init=fr;
	while(head!=NULL){
		fhe=head;
		he=head->next;
		while(he!=NULL){
			if(he->index==id1||he->index==id2){
				fhe->next=he->next;
				he=fhe->next;
			}else{
				fhe=he;
				he=he->next;
			}
		}
		head=head->child;
	}
	head=init;
	fhead=init;
	while(head!=NULL&&head->next==NULL){
		head=head->child;
	}
	init=head;
	if(init ==NULL) return NULL;
	head=init->child;
	fhead=init;
	while(head!=NULL){
		if(head->next==NULL){
			fhead->child=head->child;
		}
		else{
			fhead=head;
		}
		head=fhead->child;
	}
	return init;
}
void* approx2(void * argu){
	struct argua4 *arg=(struct argua4 *)argu;
	struct node *first=arg->fir;
	struct node *he=first;
	struct node *head=first;
	struct node *fhead;
	struct node *init;
	init=copy(first);
	he=init;
	int *res=(int *)malloc(MAXLENGTH*sizeof(int));
	int cres=1;
	while(he!=NULL){
		head=he;
		int id1;
		int id2;
		id1=head->index;
		id2=head->next->index;
		res[cres++]=id1;
		res[cres++]=id2;
			//fprintf(stdout,"d1:%d d2:%d",id1,id2);fflush(stdout);
    	//res=(int *)realloc(res,(cres+3)*sizeof(int));	
		init=init->child;
		fhead=init;
		head=init;
		if(head==NULL) {
			res[0]=cres;
			arg->s=res;
			return 	NULL;
		}
			
		if(head->index==id2){
			init=init->child;
		}else{
			while(head!=NULL){
				if(head->index==id2){
					fhead->child=head->child;
					head=fhead->child;
					break;
				}else{
					fhead=head;
					head=fhead->child;
				}
			}
		}
		init=deletes(init,id1,id2);
		if(init==NULL) 
		{
			res[0]=cres;
			arg->s=res;
			return 	NULL;
		}
		he=init;
	}
	res[0]=cres;
	arg->s=res;
	// int iii=0;
	// for(;iii<cres;iii++){
	// 	fprintf(stdout,"last:%d ",res[iii]);fflush(stdout);
	// }
	//return (void *)res;
}

int main(){
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
					fflush(stdout);
					fau = 1;
					break;
				}
				else{
					num = num + p*(name[i] - '0');
				}
			}

			// if (num == 0 && fau == 0){
			// 	fprintf(stdout, "Error: wrong V\n");
			// 	fflush(stdout);
			// 	continue;
			// }
			// else{
			// 	all = num;
			// }
			all=num;
			index = 1;
			
		}
		else if (name[0] == 'E'){
			//fprintf(stdout,"1111");
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
				fflush(stdout);
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
			int empty=0;
			for (i = 4; i<strlen(name) - 1; i++){
				if (fault == 1) break;

				if (name[i] == ',' && name[i - 1] != '>'){
					empty++;
					pointer = 1;
					cor = (int **)realloc(cor, (alloc + 1)*sizeof(int*));
					cor[alloc++] = (int*)malloc(2 * sizeof(int));
					cor[nume][0] = getint(x);
					if (cor[nume][0]<0 || cor[nume][0] >= all){
						fprintf(stdout, "Error: Wrong point\n");
						fflush(stdout);
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
						fflush(stdout);
						fault = 1;
						break;
					}
					pointer = 0;
					cor[nume][1] = getint(x);
					if(cor[nume][0]==cor[nume][1]){
						//fprintf(stdout, "Error: Wrong point\n");
						//fflush(stdout);
						//fault = 1;
						nume++;

						continue;
					}
					if (cor[nume][1]<0 || cor[nume][1] >= all){
						fprintf(stdout, "Error: Wrong point\n");
						fflush(stdout);
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
					fprintf(stdout, "Error: Wrong point\n");fflush(stdout);
					fault = 1;
					break;	
				}
				else{
					xp = 0;
				}

			}
			//fprintf(stdout,"22222");fflush(stdout);
			if(empty==0) continue;
			if (fault == 1) continue;
			//fprintf(stdout,"222223");fflush(stdout);
			struct argua4 *ar0=(struct argua4*)malloc(sizeof(struct argua4));
			ar0->fir=first;
			ar0->all=all;
			ar0->s=NULL;
			struct argua4 *ar=(struct argua4*)malloc(sizeof(struct argua4));
			ar->fir=first;
			ar->all=all;
			ar->s=NULL;
			struct argua4 *ar2=(struct argua4*)malloc(sizeof(struct argua4));
			ar2->fir=first;
			ar2->all=all;
			ar2->s=NULL;
			//int *s1;
			//int *s2;
			//fprintf(stdout,"22222\n");fflush(stdout);
			pthread_t th1,th2,th3;
			pthread_create(&th1,NULL,&a4,ar0);
			pthread_create(&th2,NULL,&approx1,ar);
			pthread_create(&th3,NULL,&approx2,ar2);
			pthread_join(th1,NULL);
			pthread_join(th2,NULL);
			pthread_join(th3,NULL);
			int sum0=ar0->s[0];
			int sum1=ar->s[0];
			int sum2=ar2->s[0];
			int ii=0;
			qsort(&ar0->s[1], sum0-1, sizeof(int), cmpfunc);
			qsort(&ar->s[1], sum1-1, sizeof(int), cmpfunc);
			qsort(&ar2->s[1], sum2-1, sizeof(int), cmpfunc);
			if(sum0<2){
				fprintf(stdout,"CNF-SAT-VC:\n");
			}else{
				fprintf(stdout,"CNF-SAT-VC: ");
				for(ii=1;ii<sum0-1;ii++){
					fprintf(stdout,"%d,",ar0->s[ii]);
				}
				fprintf(stdout,"%d\n",ar0->s[sum0-1]);
			}

			if(sum1<2){
				fprintf(stdout,"APPROX-VC-1:\n");
			}else{
				fprintf(stdout,"APPROX-VC-1: ");
				for(ii=1;ii<sum1-1;ii++){
					fprintf(stdout,"%d,",ar->s[ii]);
				}
				fprintf(stdout,"%d\n",ar->s[sum1-1]);
			}
			
			if(sum2<2){
				fprintf(stdout,"APPROX-VC-2:\n");
			}else{
				fprintf(stdout,"APPROX-VC-2: ");
				for(ii=1;ii<sum2-1;ii++){
					fprintf(stdout,"%d,",ar2->s[ii]);
				}
				fprintf(stdout,"%d\n",ar2->s[sum2-1]);
			}



		}
		memset(name,0,maxl * sizeof(char));
	}
}
