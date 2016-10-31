
/*
Kullan�lacak fonksiyonlar 
-�klit mesafesi hesaplayan fonksiyon(hem yak�n olan merkezi bulurken hem de de�i�en merkezler aras�ndaki fark�n epsilondan<= oldu�unu kontrol ederken laz�m)
-en yak�n oldu�u s�n�f� bulan fonksiyon
 2 adet struct olsun birisi k�me merkezlerinin de�erlerini ve yak�n olanlar�n dahil olaca�� s�n�f adlar�n� tutsun
di�er struct ise gelen datan�n hangi s�n�fa dahil oldu�unu ve de�erlerini tutsun.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int nearest(float[],int);
float euclidian(float[],float[],int);
int main(){

int count_lines = 1;
int count_features=0;
char filename[40], chr,sepsign;

//dosya okuma, toplam sat�r ve �zellik say�s�n�n tespit edilmesi(memory allocation i�in) i�lemleri
	printf("1-Please add labels as a last feature(end of every line) in your data\n2-Please clear empty lines at the end of data");
    printf("\nEnter file name: ");
    gets(filename);
 	printf("\nEnter the symbol that seperates for given data features : ");
    scanf("%c",&sepsign);
	
	FILE *fileptr;
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
		int id;
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
		}
fclose(fileptr);

//	kontrol ama�l� ekrana yazd�rma
for(i=0;i<count_lines;i++){
	printf("data %d-->		",i+1);
		for(j=0;j<count_features;j++){
			printf("%1.1f ",sample[i].feature[j]);
		}	
	printf("\n");
	}	
system("PAUSE");
system("CLS");

int k_value;
float error;
printf("\nPlease enter the k value for k-means algorithm : ");	scanf("%d",&k_value);
printf("\nPlease enter the error value : ");	scanf("%f",&error);

typedef struct{
	float feature[count_features];
}CENTERS;

CENTERS *center;
CENTERS *temp_center;
center=(CENTERS*)calloc(sizeof(CENTERS),k_value);
temp_center=(CENTERS*)calloc(sizeof(CENTERS),k_value);
srand(time(NULL));
//ba�lang�� k�me merkezlerinin belirlendi�i k�s�m 
	int rnd=rand()%count_lines;
	for(i=0;i<k_value;i++){
		printf("Center %d : ",i+1);
		for(j=0;j<count_features;j++){
			center[i].feature[j]=sample[rnd].feature[j];
			printf(" %1.1f ",center[i].feature[j]);
		}
		printf("\n");
			//	burada de�i�iklik yapaca��z... deneme ama�l� 
		rnd=rand()%count_lines;
	}
	system("PAUSE");
		
	int count=0;
	float *distance;
	distance=(float*)calloc(sizeof(float),k_value);
	int *number;
	number=(int*)calloc(sizeof(int),k_value);
	int iter=0;
	float pass2,pass;

	while(count!=k_value){
count=0;
iter++;
	//her bir �rne�in en yak�n k�me merkezi bulunuyor ve atan�yor	
		for(i=0;i<count_lines;i++){
		//	printf("Data %d distance from centers : ",i);
			for(j=0;j<k_value;j++){
				distance[j]=euclidian(sample[i].feature,center[j].feature,count_features);
		//		printf(" %f ",distance[j]);
			}	
			sample[i].id=nearest(distance,k_value);
		//	printf("\nNearest center number is %d for data %d ",sample[i].id,i);
			number[sample[i].id]++;
		//	system("PAUSE");
			
				for(k=0;k<count_features;k++){
					pass=sample[i].feature[k];
					temp_center[sample[i].id].feature[k]+=pass;
				}
		}
		for(i=0;i<k_value;i++){
			if(number[i]==0)
				number[i]=1;
		//	printf("numbers of cluster %d is %d \n",i,number[i]);
		}
		
// yeni k�me merkezleri olu�turuluyor;
		for(k=0;k<k_value;k++){
			for(j=0;j<count_features;j++)
				temp_center[k].feature[j]=(temp_center[k].feature[j]/(float)number[k]);
		}		
// yeni k�me merkezleri ile eskileri aras�ndaki mesafe hesaplan�yor. 
//her bir merkez i�in eski yeni aras�nadaki mesafenin epsilon de�erinden k���kl��� kontrol ediliyor
	
	for(k=0;k<k_value;k++){
		pass2=euclidian(center[k].feature,temp_center[k].feature,count_features);
		if (pass2<=error)
			count++;
		//	printf("\ndifferences of centers %d %f",count,pass2);
		}
//	system("PAUSE");
//k�me merkezleri atan�yor ve yeni merkezler i�in haz�rl�k yap�l�yor
		for(i=0;i<k_value;i++){
			number[i]=0;
			distance[i]=0.000;
				for(j=0;j<count_features;j++){
					center[i].feature[j]=temp_center[i].feature[j];
					temp_center[i].feature[j]=0.0000;
				}
		}		
				
	}
	
	printf("\nIteration number : %d\n",iter);
	
	for(i=0;i<count_lines;i++){
		printf("data %d-->		",i+1);
		for(j=0;j<count_features;j++)
			printf("%1.1f ",sample[i].feature[j]);
	printf(" id %d \n",sample[i].id);
	}	
	
fflush;
system("PAUSE");
return 0;
}

float euclidian(float sample[],float center[],int x){
	int i;
	float sum=0.0;
		for(i=0;i<x;i++){
			sum+=pow(sample[i]-center[i],2);	
		}
		return (float)sqrt(sum);
		sum=0.0;
}


int nearest(float distance[],int y){
	int i,min;
	float temp=distance[0];	
		min=0;
		for(i=1;i<y;i++){
			if(distance[i]<temp)
				min=i;			
		}
	return min;
}

