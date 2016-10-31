#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int main(){
	
FILE *fileptr;
int count_lines = 1;
int count_features=0;
char filename[40], chr,sepsign;

//dosya okuma, toplam satýr ve özellik sayýsýnýn tespit edilmesi(memory allocation için) iþlemleri
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

//struct oluþumu,datalarýn parçalanmasý ve  parçalanan datalarýn deðiþkenlere atanmasý iþlemleri
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

//	kontrol amaçlý ekrana yazdýrma
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

//KNN algoritmasýnýn uygulanmasý ve sonuçlarla beraber confusion matrisinin ekrana yazdýrýlmasý...
//veriler randomize olarak belirli bir yüzdesi eðitim kalan yüzdesi ise test amaçlý kullanýlacaktýr...

//randomize iþlemlerinin fazla iþlem yükü içermemesine dikkat edilmiþtir.
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
	
	
// k deðeri ile datamýzýn train ve test olmak üzere ayrýlma yüzdesinin kullanýcýdan alýnmasý.
	int k_value;
	float train_percent;
	printf("Please enter the percentage of data for training (0-1): ");	scanf("%f",&train_percent);
	int train=(int)(train_percent*count_lines);
	int test=count_lines-train;
	printf(" test : %d	 train : %d",test,train);
	printf("\nk value : "); scanf("%d",&k_value);
//	printf("nearest neighbours are : ");
	
/*her bir test elemaný için en yakýn k komuþusunun dizisi ve tahmini sýnýfýnýn etiketi bulunan kimlik niteliðindeki struct oluþturuluyor.	
Mesafeleri ve indisleri tutacak olan dizilerle kullanýlacak deðiþkenler tanýmlanýyor*/
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
/*her bir test datasý için train datalarýna olan öklit mesafeleri hesaplanarak tek bir distance dizisinde tutuluyor ve 
bu diziden en yakýn k komþu indisi bulunarak oluþturduðumuz structtaki komþuluk dizisine atanýyor.
*/	
//her bir test datasý için öklit mesafesinin hesaplandýðý bölüm
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
//en yakýn k komþu bulunup structtaki komþuluk matrisine atanýyor.
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
//kontrol amaçlý yazdýrma	
	
/*	printf("\nfor data %d : ",data[59].id);
	for(i=0;i<k_value;i++)		        			
	printf(" data %d, ",data[59].neighbour[i]); 
 
 */
 
 //tahmini sýnýfýn bulunmasý ve gerçekle kýyaslanarak algoritmanýn isabetlilik yüzdesinin hesaplanmasý
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
