#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<dirent.h>

#define PI 3.14159265

 // super-simplified BMP read algorithm to pull out RGB data
 // read image for coloring scheme
double RGB2HUE(double,double,double);
int nearestk(double[],int,int,int[]);
double euclidian(int[],int[],int);

int main(){	
double val=180.0/PI;	
double hue_val;
int width,height,size,i,j,k,k_val,byte,tet_num,train_num,test_num,temp;

	printf("Enter the number of test images : ");	scanf("%d",&test_num);
	printf("Enter the number of train images : ");  scanf("%d",&train_num);
	printf("Enter width of image : "); 				scanf("%d",&width);
	printf("Enter height of image : "); 			scanf("%d",&height); 
	
	size=width*height;
	int image[size][3]; // first number is pixels in my image, 3 is for RGB values
 
	printf("Enter the k value for most similar images : "); scanf("%d",&k_val);
 
 typedef struct{
 	double hueperpix[size];
	int neighbour[k_val];
	int histogram[361];
 	char name[20];
 }TEST;
 
  typedef struct{
 	double hueperpix[size];
 	int  histogram[361];
 	char name[20];
 }TRAIN;
 
 TEST *test;
 TRAIN *train;
	test=(TEST*)calloc(sizeof(TEST),100); 
	train=(TRAIN*)calloc(sizeof(TRAIN),150);
	
///////////////////////////////////////////////////////////////////TRAIN DATASI ÝÇÝN OKUMA VE HUE ÇIKARTMA/////////////////////////////////
 DIR *pdir = NULL; 
 struct dirent *pent = NULL;

	pdir = opendir ("/Users/m.yasin/Desktop/ödevyasin/TRAIN"); 
    if (pdir == NULL){
        printf ("\nERROR! pdir could not be initialised correctly");
        exit (0);
    } 
	pent = readdir (pdir);
	pent = readdir (pdir);
	i=0;
    while (pent = readdir (pdir)){
    	
        if (pent == NULL){
            printf ("ERROR! pent could not be initialised correctly");
            exit (0);
        }

    	strcpy(train[i].name,pent->d_name);
    	printf ("%s\n", train[i++].name);
	}
system("PAUSE");
system("CLS");
printf("Working...\n");
//system("CLS");
 FILE *streamIn;
 char path2[]="/Users/m.yasin/Desktop/ödevyasin/TRAIN/";
 char t_path[]="/Users/m.yasin/Desktop/ödevyasin/TRAIN/";
	for(i=0;i<train_num;i++){
		strcat(path2,train[i].name);
		streamIn = fopen(path2,"rb");
		strcpy(path2,t_path);
 		if (streamIn == (FILE *)0){
   			printf("File opening error ocurred. Exiting program.\n");
   			exit(0);
		}
		
		for(j=0;j<54;j++) 
			byte = getc(streamIn);  // strip out BMP header

		for(j=0;j<size;j++){    // foreach pixel
    		image[j][2] = getc(streamIn);  // use BMP 24bit with no alpha channel
    		image[j][1] = getc(streamIn);  // BMP uses BGR but we want RGB, grab byte-by-byte
    		image[j][0] = getc(streamIn);  // reverse-order array indexing fixes RGB issue...
    		train[i].hueperpix[j]=RGB2HUE(image[j][0],image[j][1],image[j][2]);
    		temp=(int)train[i].hueperpix[j];
    		train[i].histogram[temp]++;
    		//printf("picture %s pixel %d : [%d,%d,%d] ---> hue value is : %f \n",train[i].name,j+1,image[j][0],image[j][1],image[j][2],train[i].hueperpix[j]);
   			if(train[i].hueperpix[j]<0)
   				system("PAUSE");
 		}
 		fclose(streamIn);
	}

closedir(pdir);
printf("Success...\n ");
system("PAUSE");
/////////////////////////////////////////////////TEST DATASI ÝÇÝN OKUMA VE HUE ÇIKARTMA
DIR *adir = NULL; 
adir = opendir ("/Users/m.yasin/Desktop/ödevyasin/TEST"); 
    if (adir == NULL){
        printf ("\nERROR! pdir could not be initialised correctly");
        exit (0);
    } 

	pent = readdir (adir);
	pent = readdir (adir);
	i=0;
    while (pent = readdir (adir)){
    	if (pent == NULL){ 
            printf ("ERROR! pent could not be initialised correctly");
            exit (0);
        }
    	strcpy(test[i].name,pent->d_name);
    	printf ("%s\n", test[i++].name);
	}
	
	
system("PAUSE");
system("CLS");
printf("Working...\n");

strcpy(path2,"/Users/m.yasin/Desktop/ödevyasin/TEST/");
strcpy(t_path,"/Users/m.yasin/Desktop/ödevyasin/TEST/");
FILE *testIn;

	for(i=0;i<test_num-1;i++){
		strcat(path2,test[i].name);
		testIn = fopen(path2,"rb");
		strcpy(path2,t_path);
		
 		if (testIn == (FILE *)0){
   			printf("%s File opening error ocurred. Exiting program.\n",test[i].name);
   			exit(0);
		}
		
		for(j=0;j<54;j++)
			byte = getc(testIn);  // strip out BMP header (header size : 54 bytes)

		for(j=0;j<size;j++){    // foreach pixel
    		image[j][2] = getc(testIn);  // use BMP 24bit with no alpha channel
    		image[j][1] = getc(testIn);  // BMP uses BGR but we want RGB, grab byte-by-byte
    		image[j][0] = getc(testIn);  // reverse-order array indexing fixes RGB issue...
    		test[i].hueperpix[j]=RGB2HUE(image[j][0],image[j][1],image[j][2]);
    		temp=(int)test[i].hueperpix[j];
    		test[i].histogram[temp]++;
   			 //printf("picture %s pixel %d : [%d,%d,%d] ---> hue value is : %f \n",test[i].name,j+1,image[j][0],image[j][1],image[j][2],test[i].hueperpix[j]);
   			//system("PAUSE");
 		}
fclose(testIn);
	}

closedir(adir);
printf("Success...\n\n");
system("PAUSE");
system("CLS");
							//calculating euclidian distances for test data and finding k-nearest neighbours
double distance[train_num];
	for(i=0;i<test_num;i++){
		for(j=0;j<train_num;j++){
			distance[j]=euclidian(test[i].histogram,train[j].histogram,361);
		//	printf(" test %d train %d distance : %f \n",i+1,j+1,distance[j]);
		}
		int finish=nearestk(distance,train_num,k_val,test[i].neighbour);	//yakýn bulma olaylarý gelecek
		if(finish==0)
			printf("Found... ");
		printf("\nMost similar images of %s  is : \n",test[i].name);
		for(k=0;k<k_val;k++){
		printf(" %d. %s \n",k+1,train[test[i].neighbour[k]].name);
		//system("PAUSE");
		}
		system("PAUSE");
		printf("\n");
	}

free(test);
free(train);

system ("PAUSE");
return 0;

}
 

double RGB2HUE(double r,double g,double b){
	
 double val=180.0/PI;
 double sum=0.0;
 	if(r==b && b==g)
	 return 0.0;	
	else{
		sum= ((r-g)+(r-b)) / (2*sqrt(pow(r-g,2)+(r-b)*(g-b)));
		if(b<=g)
			return acos(sum)*val;
		if(b>g)
			return 360.0-(acos(sum)*val);
	}
}

double euclidian(int sample[],int center[],int x){
	
	
	int i;
	double sum=0.0;
		for(i=0;i<x;i++){
			if(sample[i]==0.0 && center[i]==0.0)
				i++;
			sum+=pow(sample[i]-center[i],2);	
		}
		return (double)sqrt(sum);
		sum=0.0;
}


int nearestk(double distance[],int y,int k_val,int neighbour[]){
	int i,j;
	double max_val,min_val;	
		max_val=distance[0];
		for(j=1;j<y;j++){
			if(distance[j]>max_val)
				max_val=distance[j];			
		}
		
		for(i=0;i<k_val;i++){
			min_val=distance[0];
			for(j=0;j<y;j++){
				if(distance[j]<min_val){
					min_val=distance[j];
					neighbour[i]=j;
				}
			}
			distance[neighbour[i]]=max_val+i+1;
		}
	return 0;
}

