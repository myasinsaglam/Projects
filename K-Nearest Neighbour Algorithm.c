#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int main(){
	
FILE *fileptr;
int count_lines = 1;
int count_features=0;
char filename[40], chr,sepsign;

//dosya okuma, toplam sat�r ve �zellik say�s�n�n tespit edilmesi(memory allocation i�in) i�lemleri
	printf("1-Please add labels as a last feature(end of every line) in your data\n2-Please clear empty lines at the end of data");
    printf("\nEnter file name: ");
    gets(filename);
 	printf("\nEnter the symbol that seperates for given data features : ");
    scanf("%c",&sepsign);

    fileptr = fopen(filename, "r");
    int k=0;
	chr = fgetc(fileptr);
	while(chr!='\n'){
		k++;
		if(chr==sepsign)
			count_features++;
        chr = fgetc(fileptr);	
	}
	
	rewind(fileptr);
    int all_data=0;
    
    chr = fgetc(fileptr);
    while (chr != EOF){
    	
		if(chr==sepsign)
    		all_data++;
    	
        if(chr == '\n')
            count_lines++;
        
        chr = fgetc(fileptr);
    }
 		if(all_data!=count_features*count_lines)
 		printf("There is a missing data feature or seperating error\n\n");
 	
    printf("\nThere are %d lines %d features in %s  in a file all data number is %d \n", count_lines, count_features, filename, all_data);
system("PAUSE");

//struct olu�umu,datalar�n par�alanmas� ve  par�alanan datalar�n de�i�kenlere atanmas� i�lemleri
	typedef	struct	{
		float feature[count_features];
		char label[20];
	}SAMPLE;

	SAMPLE *sample;
	sample=(SAMPLE*)calloc(sizeof(SAMPLE),count_lines);	
	rewind(fileptr);
	char line[k+10];
	char *part;
	int i,j;
			
		for(i=0;i<count_lines;i++){
			fgets(line,k+10,fileptr);	
			part=strtok(line,", \n");
				for(j=0;j<count_features;j++){
					sample[i].feature[j]=atof(part);	
					part = strtok (NULL, ", \n");		 			
				}		
			strcpy(sample[i].label,part);		
		}
fclose(fileptr);

//	kontrol ama�l� ekrana yazd�rma
for(i=0;i<count_lines;i++){
	printf("data %d-->		",i+1);
		for(j=0;j<count_features;j++){
			printf("%1.1f ",sample[i].feature[j]);
		}	
		printf("%s",sample[i].label);
	printf("\n");
	}	
system("PAUSE");
system("CLS");

//KNN algoritmas�n�n uygulanmas� ve sonu�larla beraber confusion matrisinin ekrana yazd�r�lmas�...
//veriler randomize olarak belirli bir y�zdesi e�itim kalan y�zdesi ise test ama�l� kullan�lacakt�r...

//randomize i�lemlerinin fazla i�lem y�k� i�ermemesine dikkat edilmi�tir.
srand(time(NULL));
int random[count_lines];
int x;
for(i=0;i<count_lines;i++)
	random[i]=i;
	for(i=0;i<count_lines;i++){
		j=rand()%(count_lines);
		k=rand()%(count_lines);
		x=random[j];
		random[j]=random[k];
		random[k]=x;	
	}   
  /* for(i=0;i<count_lines;i++)
	printf("%d----	%d \n",i+1,random[i]); */
	
	
// k de�eri ile datam�z�n train ve test olmak �zere ayr�lma y�zdesinin kullan�c�dan al�nmas�.
	int k_value;
	float train_percent;
	printf("Please enter the percentage of data for training (0-1): ");	scanf("%f",&train_percent);
	int train=(int)(train_percent*count_lines);
	int test=count_lines-train;
	printf(" test : %d	 train : %d",test,train);
	printf("\nk value : "); scanf("%d",&k_value);
//	printf("nearest neighbours are : ");
	
/*her bir test eleman� i�in en yak�n k komu�usunun dizisi ve tahmini s�n�f�n�n etiketi bulunan kimlik niteli�indeki struct olu�turuluyor.	
Mesafeleri ve indisleri tutacak olan dizilerle kullan�lacak de�i�kenler tan�mlan�yor*/
typedef struct{
	int neighbour[k_value];
	int id;
	char predicted_label[20];
}PROCESS;
	PROCESS *data;
	data=(PROCESS*)malloc(sizeof(PROCESS)*count_lines);
	float distance[train];
	int index[train];
	float sum=0.0;
	float temp,min;
	k=0;
	int z;
/*her bir test datas� i�in train datalar�na olan �klit mesafeleri hesaplanarak tek bir distance dizisinde tutuluyor ve 
bu diziden en yak�n k kom�u indisi bulunarak olu�turdu�umuz structtaki kom�uluk dizisine atan�yor.
*/	
//her bir test datas� i�in �klit mesafesinin hesapland��� b�l�m
	for(i=0;i<test;i++){		
		for(j=test;j<count_lines;j++){			
			for(k=0;k<count_features;k++){				
				sum=sum+pow(sample[random[i]].feature[k]-sample[random[j]].feature[k],2);
			}			
			data[i].id=random[i];
			//printf("%d-) test %d---> ",i,data[i].id);
			//printf("train %d ",random[j]);
			distance[j-test]=(float)sqrt(sum);
		//	printf(" distance = %1.1f\n",distance[j-test]);
			sum=0.0;
		}
//en yak�n k kom�u bulunup structtaki kom�uluk matrisine atan�yor.
		for(z=0;z<k_value;z++){
			min=distance[z];
			index[z]=z;
			for(x=z+1;x<train;x++){
				if(distance[x]<min){
					min=distance[x];
					index[z]=x;
				}
			}
			temp=distance[z];
			distance[z]=distance[index[z]];
			distance[index[z]]=temp;
			data[i].neighbour[z]=random[index[z]+test];
		}		
	}
//kontrol ama�l� yazd�rma	
	
/*	printf("\nfor data %d : ",data[59].id);
	for(i=0;i<k_value;i++)		        			
	printf(" data %d, ",data[59].neighbour[i]); 
 
 */
 
 //tahmini s�n�f�n bulunmas� ve ger�ekle k�yaslanarak algoritman�n isabetlilik y�zdesinin hesaplanmas�
int *predict;
predict=(int*)calloc(sizeof(int),3);
int max,big;
char all_labels[3][20]={"Iris-setosa","Iris-versicolor","Iris-virginica"};
int accuracy=0;

	for(i=0;i<test;i++){
		for(j=0;j<k_value;j++){
			if(strcmp(all_labels[0],sample[data[i].neighbour[j]].label)==0)	
			predict[0]++;
		//	printf("%d ",predict[0]);
			else if(strcmp(all_labels[1],sample[data[i].neighbour[j]].label)==0)
			predict[1]++;
		//	printf("%d ",predict[1]);
			else if(strcmp(all_labels[2],sample[data[i].neighbour[j]].label)==0)
			predict[2]++;	
		//	printf("%d ",predict[2]);
		//	system("PAUSE");	
		}
		max=predict[0];
		for(k=0;k<k_value;k++){
			if(predict[k]>=max){
			max=predict[k];
			big=k;
			}	
		}
	//,	printf("\nbig one is %d \n",big);
		for(k=0;k<k_value;k++)
		predict[k]=0;
		strcpy(data[i].predicted_label,all_labels[big]);
		if(strcmp(data[i].predicted_label,sample[data[i].id].label)==0)
		accuracy++;

	}  
  
  printf("\ntotal accuracy is %d/%d",accuracy,test);
  float percent;
  percent=(float)accuracy/(float)test;
  printf("\nAccuracy percentage is :  %f ",percent*100);

fflush;
system("PAUSE");
return 0;
}
