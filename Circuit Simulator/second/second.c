#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
struct action{
  int actn;
  char a[100];
  char b[100];
  char c[100];
  int inputs;
  char **muxstr;
  char **dcdstr;
};

int and(int a,int b){
	if(a+b==2)
	return 1;
	return 0;
}

int or(int a, int b){
	if(a+b>0)
	return 1;
	return 0;
}
int not(int a){
	if(a==1)
	return 0;
	return 1;
}
int nand(int a,int b){
	return not(and(a,b));
}
int nor(int a,int b){
	return not(or(a,b));
}
int xor(int a,int b){
	if(a+b==1)
	return 1;
	return 0;
}
int **grey(int i,int x,int ** code){
	int y;
	int z;
	for(y=0;y<x;y++){
		for(z=(1<<x);z<(1<<(x+1));z++){
			code[i-y-1][z] = code[i-y-1][(2<<x)-z-1];
		}
	}

	for(y=0;y<(2<<x);y++){
		if(y<(1<<x)){
		code[i-x-1][y]=0;
		}else{
		code[i-x-1][y]=1;
		}
	}
		
	if(x!=i-1){
		x++;
		code = grey(i,x,code);
	}
	return code;
}

int main(int argc, char *argv[]){

	FILE *f = fopen(argv[1],"r");
	int inputs=0;
	int outputs=0;
	int x;
	int k;
	fscanf(f,"INPUTVAR %d",&inputs);
	int **IN = (int **)malloc(inputs * sizeof(int*));
		for(x=0;x<inputs;x++)
			IN[x] = (int *)malloc((1<<inputs) * sizeof(int));
	char **Inames = (char **)malloc(inputs * sizeof(char*));
		for(x=0;x<inputs;x++)
			Inames[x]=(char *)malloc(100*sizeof(char));

	for(x=0;x<inputs;x++){
		fscanf(f,"%s",Inames[x]);
	}

	IN[inputs-1][0]=0;
	IN[inputs-1][1]=1;
	IN = grey(inputs,1,IN);
	
	fscanf(f,"\n");
	fscanf(f,"OUTPUTVAR %d",&outputs);

	int **OUT = (int **)malloc(outputs * sizeof(int*));
		for(x=0;x<outputs;x++)
			OUT[x] = (int *)malloc((1<<inputs) * sizeof(int));

	char **Onames = (char **)malloc(outputs * sizeof(char*));
		for(x=0;x<outputs;x++)
			Onames[x]=(char *)malloc(100*sizeof(char));

	for(x=0;x<outputs;x++){
		fscanf(f,"%s",Onames[x]);
	}

	struct action *acts = (struct action *)malloc(100*sizeof(struct action));

	fscanf(f,"\n");
	x=0;
	while(x>-1){
		char comm[20];
		if(feof(f))break;
		fscanf(f,"%s ",comm);
		if(comm[0]=='A'){
			acts[x].actn=2;
			fscanf(f," %s ",acts[x].a);
			fscanf(f," %s ",acts[x].b);
			fscanf(f," %s\n",acts[x].c);
		}else if(comm[0]=='O'){
			acts[x].actn=3;
			fscanf(f," %s ",acts[x].a);
			fscanf(f," %s ",acts[x].b);
			fscanf(f," %s\n",acts[x].c);
		}else if(comm[0]=='D'){
			int factor;
			acts[x].actn=7;
			fscanf(f,"%d",&factor);

			char **input = (char **)malloc(factor*sizeof(char *));
				for(k=0;k<factor;k++)
					input[k] = (char *)malloc(100*sizeof(char));

			char **output = (char **)malloc((1<<factor)*sizeof(char *));
				for(k=0;k<(1<<factor);k++)
					output[k] = (char *)malloc(100*sizeof(char));

			for(k=0;k<factor;k++){
				fscanf(f,"%s",input[k]);
			}
			for(k=0;k<(1<<factor)-1;k++){
				fscanf(f,"%s",output[k]);
			}
			fscanf(f,"%s\n",output[(1<<factor)-1]);
			acts[x].inputs = factor;
			acts[x].muxstr = input;
			acts[x].dcdstr = output;
		}else if(comm[0]=='M'){
			int factor;
			acts[x].actn=8;
			fscanf(f,"%d",&factor);
			int inputs = log(factor)/log(2);
			char ** input = (char **)malloc((factor+inputs)*sizeof(char *));
				for(k=0;k<factor+inputs;k++)
					input[k] = (char *)malloc(100*sizeof(char));

			for(k=0;k<factor+inputs;k++){
				fscanf(f,"%s",input[k]);
			}
			acts[x].muxstr = input;
			fscanf(f,"%s\n",acts[x].c);
			acts[x].inputs=factor;
		}else if(comm[0]=='N'){
			if(comm[1]=='A'){
				acts[x].actn=4;
				fscanf(f," %s ",acts[x].a);
				fscanf(f," %s ",acts[x].b);
				fscanf(f," %s\n",acts[x].c);
			}else if(comm[2]=='R'){
				acts[x].actn=5;
				fscanf(f," %s ",acts[x].a);
				fscanf(f," %s ",acts[x].b);
				fscanf(f," %s\n",acts[x].c);
			}else if(comm[2]=='T'){
				acts[x].actn=1;
				fscanf(f," %s ",acts[x].a);
				fscanf(f," %s\n",acts[x].c);

			}
		}else if(comm[0]=='X'){
			acts[x].actn=6;
			fscanf(f," %s ",acts[x].a);
			fscanf(f," %s ",acts[x].b);
			fscanf(f," %s\n",acts[x].c);
		}
		x++;
	}

	char **Tnames = (char **)malloc(100 * sizeof(char*));
		for(k=0;k<100;k++)
			Tnames[k]=(char *)malloc(100*sizeof(char));
	int temps=2;
	Tnames[0]="0";
	Tnames[1]="1";
	int j;
	for(k=0;k<x;k++){
		if(acts[k].actn<2){
			int pre=0;
			for(j=0;j<inputs;j++){
				if(strcmp(acts[k].a,Inames[j])==0)pre=1;
			}
			for(j=0;j<temps;j++){
				if(strcmp(acts[k].a,Tnames[j])==0)pre=1;
			}
			if(pre==0){
				acts[x] = acts[k];
				for(j=k+1;j<=x;j++){
					acts[j-1]=acts[j];
				}
				k--;
			}
			if(pre==1){
				for(j=0;j<outputs;j++){
					if(strcmp(acts[k].c,Onames[j])==0){
						pre=0;
					}
				}
				if(pre==1){
					Tnames[temps]=acts[k].c;
					temps++;
				}

			}

		}else if(acts[k].actn<7){
			int preA = 0;
			int preB = 0;
			for(j=0;j<inputs;j++){
				if(strcmp(acts[k].a,Inames[j])==0)preA=1;
			}

			for(j=0;j<temps;j++){
				if(strcmp(acts[k].a,Tnames[j])==0)preA=1;
			}

			for(j=0;j<inputs;j++){
				if(strcmp(acts[k].b,Inames[j])==0)preB=1;
			}

			for(j=0;j<temps;j++){
				if(strcmp(acts[k].b,Tnames[j])==0)preB=1;
			}

			if(preA==0||preB==0){
				acts[x] = acts[k];
				for(j=k+1;j<=x;j++){
					acts[j-1]=acts[j];
				}
				k--;
				
			}
			if(preA==1&&preB==1){
				for(j=0;j<outputs;j++){
					if(strcmp(acts[k].c,Onames[j])==0){
						preA=0;
					}
				}
				if(preA==1){
					Tnames[temps]=acts[k].c;
					temps++;
				}
			}
		}else{
			int value = acts[k].inputs;
			if(acts[k].actn==8)value+=log2(acts[k].inputs);
			int clear = 0;
			for(j=0;j<value;j++){
				int g;
				int pre=0;
				for(g=0;g<inputs;g++){
					if(strcmp(acts[k].muxstr[j],Inames[g])==0)pre=1;
				}
				for(g=0;g<temps;g++){
					if(strcmp(acts[k].muxstr[j],Tnames[g])==0)pre=1;
				}
				if(pre==0){
					acts[x] = acts[k];
					for(g=k+1;g<=x;g++){
						acts[g-1]=acts[g];
					}
					clear++;
					k--;
					break;
				}
			}
			if(clear==0){

				int outs=1;
				int pre=0;
				int g;
				if(acts[k].actn==7){
					outs=(1<<acts[k].inputs);
					for(j=0;j<outs;j++){
						pre=0;
						for(g=0;g<outputs;g++){
							if(strcmp(acts[k].dcdstr[j],Onames[g])==0){
								pre=1;
							}
						}
						if(pre==0){
							Tnames[temps]=acts[k].dcdstr[j];
							temps++;
						}
					}
				}else{
					for(j=0;j<outs;j++){
						if(strcmp(acts[k].c,Onames[j])==0){
							pre=1;
						}
					}
					if(pre==0){
						Tnames[temps]=acts[k].c;
						temps++;
					}
				}
			}
		}
	}

	int *Tval = (int *)malloc(temps * sizeof(int));
	Tval[0]=0;
	Tval[1]=1;

	for(k=0;k<(1<<inputs);k++){
		int g;
		for(g=0;g<x;g++){
			int a=0;
			int b=0;
			int c=0;
			int q;
			int temp=0;
			int out=0;
			if(acts[g].actn<2){
				for(q=0;q<inputs;q++)
					if(strcmp(acts[g].a,Inames[q])==0)a=q;
				for(q=0;q<temps;q++){
					if(strcmp(acts[g].a,Tnames[q])==0){
						a=q;
						temp=1;
					}
					if(strcmp(acts[g].c,Tnames[q])==0){
						c=q;
					}
				}
				for(q=0;q<outputs;q++){
					if(strcmp(acts[g].c,Onames[q])==0){
						c=q;
						out=1;
					}
				}
				if(temp==0&&out==0){
					Tval[c] = not(IN[a][k]);
				}
				if(temp==1&&out==0){
					Tval[c] = not(Tval[a]);
				}
				if(temp==0&&out==1){
					OUT[c][k] = not(IN[a][k]);
				}
				if(temp==1&&out==1){
					OUT[c][k] = not(Tval[a]);
				}
			}else if(acts[g].actn<7){
				int tempa=0;
				int tempb=0;
				for(q=0;q<inputs;q++){
					if(strcmp(acts[g].a,Inames[q])==0)a=q;
					if(strcmp(acts[g].b,Inames[q])==0)b=q;
				}
				for(q=0;q<temps;q++){
					if(strcmp(acts[g].a,Tnames[q])==0){
						a=q;
						tempa=1;
					}
					if(strcmp(acts[g].b,Tnames[q])==0){
						b=q;
						tempb=1;
					}

					if(strcmp(acts[g].c,Tnames[q])==0){
						c=q;
					}
				}
				for(q=0;q<outputs;q++){
					if(strcmp(acts[g].c,Onames[q])==0){
						c=q;
						out=1;
					}
				}
				if(acts[g].actn==2){
					if(tempa==0&&tempb==0&&out==0)
						Tval[c] = and(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==0)
						Tval[c] = and(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==0)
						Tval[c] = and(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==0)
						Tval[c] = and(Tval[a],Tval[b]);
					if(tempa==0&&tempb==0&&out==1)
						OUT[c][k] = and(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==1)
						OUT[c][k] = and(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==1)
						OUT[c][k] = and(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==1)
						OUT[c][k] = and(Tval[a],Tval[b]);
				}
				if(acts[g].actn==3){
					if(tempa==0&&tempb==0&&out==0)
						Tval[c] = or(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==0)
						Tval[c] = or(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==0)
						Tval[c] = or(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==0)
						Tval[c] = or(Tval[a],Tval[b]);
					if(tempa==0&&tempb==0&&out==1)
						OUT[c][k] = or(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==1)
						OUT[c][k] = or(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==1)
						OUT[c][k] = or(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==1)
						OUT[c][k] = or(Tval[a],Tval[b]);
				}
				if(acts[g].actn==4){
					if(tempa==0&&tempb==0&&out==0)
						Tval[c] = nand(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==0)
						Tval[c] = nand(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==0)
						Tval[c] = nand(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==0)
						Tval[c] = nand(Tval[a],Tval[b]);
					if(tempa==0&&tempb==0&&out==1)
						OUT[c][k] = nand(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==1)
						OUT[c][k] = nand(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==1)
						OUT[c][k] = nand(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==1)
						OUT[c][k] = nand(Tval[a],Tval[b]);
				}
				if(acts[g].actn==5){
					if(tempa==0&&tempb==0&&out==0)
						Tval[c] = nor(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==0)
						Tval[c] = nor(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==0)
						Tval[c] = nor(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==0)
						Tval[c] = nor(Tval[a],Tval[b]);
					if(tempa==0&&tempb==0&&out==1)
						OUT[c][k] = nor(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==1)
						OUT[c][k] = nor(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==1)
						OUT[c][k] = nor(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==1)
						OUT[c][k] = nor(Tval[a],Tval[b]);
				}
				if(acts[g].actn==6){
					if(tempa==0&&tempb==0&&out==0)
						Tval[c] = xor(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==0)
						Tval[c] = xor(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==0)
						Tval[c] = xor(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==0)
						Tval[c] = xor(Tval[a],Tval[b]);
					if(tempa==0&&tempb==0&&out==1)
						OUT[c][k] = xor(IN[a][k],IN[b][k]);
					if(tempa==1&&tempb==0&&out==1)
						OUT[c][k] = xor(Tval[a],IN[b][k]);
					if(tempa==0&&tempb==1&&out==1)
						OUT[c][k] = xor(IN[a][k],Tval[b]);
					if(tempa==1&&tempb==1&&out==1)
						OUT[c][k] = xor(Tval[a],Tval[b]);
				}

			}else if(acts[g].actn==7){
				int greys[acts[g].inputs];
				int size=acts[g].inputs;
				int l;
				for(l=0;l<acts[g].inputs;l++){
					for(q=0;q<inputs;q++){
						if(strcmp(acts[g].muxstr[l],Inames[q])==0){
							greys[l]=IN[q][k];
						}
					}
					for(q=0;q<temps;q++){
						if(strcmp(acts[g].muxstr[l],Tnames[q])==0){
							greys[l]=Tval[q];
						}
					}
				}
				for(l=1;l<size;l++){
					greys[l]=xor(greys[l-1],greys[l]);
					
				}
				int state=0;
				for(l=0;l<size;l++){
					state+=greys[l]<<(size-l-1);
				}
				for(l=0;l<1<<size;l++){
					for(q=0;q<temps;q++){
						if(strcmp(acts[g].dcdstr[l],Tnames[q])==0){
							if(l==state){
								Tval[q]=1;
							}else{
								Tval[q]=0;
							}
						}
					}
					for(q=0;q<outputs;q++){
						if(strcmp(acts[g].dcdstr[l],Onames[q])==0){
							if(l==state){
								OUT[q][k]=1;
							}else{
								OUT[q][k]=0;
							}
						}
					}


				}
			}else if(acts[g].actn==8){
				int outm[acts[g].inputs];
				int size=acts[g].inputs;
				int l;
				for(l=0;l<size;l++){
					for(q=0;q<inputs;q++){
						if(strcmp(acts[g].muxstr[l],Inames[q])==0){
							outm[l]=IN[q][k];
						}
					}
					for(q=0;q<temps;q++){
						if(strcmp(acts[g].muxstr[l],Tnames[q])==0){
							outm[l]=Tval[q];
						}
					}
				}
				int select[(int)log2(size)];
				int sel = log2(size);
				for(l=0;l<sel;l++){
					for(q=0;q<inputs;q++){
						if(strcmp(acts[g].muxstr[l+size],Inames[q])==0){
							select[l]=IN[q][k];
						}
					}
					for(q=0;q<temps;q++){
						if(strcmp(acts[g].muxstr[l+size],Tnames[q])==0){
							select[l]=Tval[q];
						}
					}
				}
				for(l=1;l<sel;l++){
					select[l]=xor(select[l-1],select[l]);
					
				}
				int state=0;
				for(l=0;l<sel;l++){
					state+=select[l]<<(sel-l-1);
				}
				for(q=0;q<temps;q++){
					if(strcmp(acts[g].c,Tnames[q])==0){
							Tval[q]=outm[state];
					}
				}
				for(q=0;q<outputs;q++){
					if(strcmp(acts[g].c,Onames[q])==0){
							OUT[q][k]=outm[state];
					}
				}
				
			}
		}
	}

	int y;
	for(y=0;y<1<<inputs;y++){
		for(k=0;k<inputs;k++){
			printf("%d ",IN[k][y]);
		}
		for(k=0;k<outputs;k++){
			printf("%d ",OUT[k][y]);
		}
		printf("\n");
	}

	return 0;
}


