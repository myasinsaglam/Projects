#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
int main(){
	int tahmin_say,i,j,k,hak,kontrol,boy,dogru,skala,ipucu,tekrar;
	char tahmin[4];
	char esas[4];
	bool cevap=false;
	char renk[6]={'R','G','B','Y','P','O'};
	srand(time(NULL));
	
	printf("Renkler: R=Red G=Green B=Blue Y=Yellow P=Purple O=Orange \n");
	tekrar=1;
	do{
	cevap=false;
	printf("Kac tahmin hakki istersiniz : ");
	scanf("%d",&tahmin_say);
		
		for(i=0;i<4;i++)
		esas[i]=renk[rand()%6];
		
		printf("Kontrol icin tutulan esas diziyi gormek icin 1 devam etmek icin 0 giriniz ");	
		scanf("%d",&kontrol);
		if (kontrol==1){
			for(i=0;i<4;i++)
			printf("%c",esas[i]);
		}
		
		dogru=0;	hak=0;		j=0;	i=0; skala=0;	ipucu=0;
	 
			while(hak<tahmin_say && cevap==false){
				printf("\n\nTahmin %d : ",j+1);
				scanf("%s",&tahmin);
		
				boy=strlen(tahmin);		
				if(boy!=4)
				printf("\nLutfen 4 elemanli bir tahmin giriniz\n");
										
					for(i=0;i<boy;i++){
						if(tahmin[i]==esas[i])
						dogru++;
						for(k=0;k<6;k++){
							if(tahmin[i]==renk[k])
							skala++;
						}
					}
					i=0; k=0;
					while(i<boy){
						if(tahmin[k]==esas[i]){
							ipucu++;
							i++; k=0;
						}
							else
							  k++;
						if(k==boy){
						i++;	k=0;	
						}			
					}
					
					if(skala<boy)
							printf("\nlutfen verilen skalada renk giriniz\n");	
							
					hak++;		j++;
					
				if(dogru==4 && skala==boy && boy==4)
				cevap=true;
				if(dogru<4 && skala==boy && boy==4){
				printf("\n%d rengin yeri dogrudur. \n",dogru);
					if(ipucu==4)
				printf("ek ipucu : tum renkleri buldunuz... ;)  \n%d rengin yeri ise yanlistir ",ipucu-dogru);
					if(ipucu==0)
				printf("Bu rengi veya renkleri bence tekrar kullanmamalisiniz ;)");
				}
				
				
				// HATALI GÝRÝÞ YAPILDIÐINDA HAKKINIZ GÝTSÝN ÝSTEMÝYORSANIZ ALTTAKÝ YORUM SATIRLARINI AKTÝVE EDÝN...
				/*else {
					hak--; j--;
					} 
					 */
					 
				skala=0;
				dogru=0;
				ipucu=0;
			}
			
		if(cevap==true){		//j++;
			printf("Tebrikler %d . denemenizde dogru cevabi buldunuz tekrar oynamak isterseniz 1 cikmak icin 0 giriniz : \n",j);
		scanf("%d",&tekrar);
		}	
			else if(hak==tahmin_say){
			printf("Cevap %s Bilemediniz ve maalesef sectiðiniz oynama hakkinizi doldurdunuz. Uzulmeyin, tekrar oynamak isterseniz 1 cikmak icin 0 giriniz : \n",esas);
			scanf("%d",&tekrar);
			}
	}while (tekrar==1);
	
	system("PAUSE");
	return 0 ;
}
