#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TAILLE 9//nombre de ligne et de colonne

//variable globale
int grille[TAILLE][TAILLE];//matrice contenant les chiffres en base 10
int grille2[TAILLE][TAILLE];//matrice contenant les chiffres en binaires
int grille3[TAILLE][TAILLE];



//calcul du poid
int poids(unsigned long x)
{
int tmp=0;
while (x>0) {
		if(x&1L) tmp++;
		x=x>>1;
     }
return tmp;
}

//conversion entier en binaire
int conversion_entierbinaire(int nb) {
   int Value = 0;
   int i=0;
 int division = nb;

 for ( i=0 ; division > 0; i++) {
               if (division % 2 == 1)
                            Value += pow(10,i);
                            division = division /2;
                            }

 return Value;
}


//conversion d'un binaire de poids 1 en sa representation reel ex:000100000 =>7
int conversion_poids1(int nb) {
     int cpt=0;
     int tmp;
     tmp=nb;
     while((tmp>>1)>0) {
                       tmp=tmp/2;
                       cpt++;
                       }




     return cpt;
     }




//lecture du fichier texte et stockage dans la matrice grille et grille4
void lire_fichier(char* argv) {
     int i,j;
     	FILE* fichier = NULL;
      fichier = fopen(argv, "r");
      if (fichier != NULL)
      {
         for(i=0;i<TAILLE;i++)
		                   for(j=0;j<TAILLE;j++)
			                                fscanf(fichier, "%d", &grille[i][j] );
                                   }
       fclose(fichier);

}


//affichage de la grille (pour des sudoku 9x9)
void affiche_grille() {
 int i,j;
         printf("\n ----------------------\n");
	for(i=0;i<TAILLE;i++) {
		for(j=0;j<TAILLE;j++) {

		      if(j==0  ) printf("| ");
			printf("%d ",grille[i][j]);
            if(j==2 | j==5 |j==8 ) printf("| ");


			}
		if(i==2 | i==5) printf("\n ----------------------\n");

		else printf("\n");

	}
	printf(" ----------------------\n  ");


 }





void remplir_matricebinaire() {
int i,j;
     for(i=0;i<TAILLE;i++)  {
		for(j=0;j<TAILLE;j++) {
                           if((grille[i][j]!=0)&&(grille[i][j]<10)) grille2[i][j]=(1<<(grille[i][j]-1));
                           else grille2[i][j]=511;
                           }
     }





     }





void contraite_unicitecol(int u,int col) {
 int i;
   int tmp=511;
int cpt=1;

for(i=0;i<TAILLE;i++) {
		 if((poids(grille2[i][col])!=1)&&(i!=u)) {
                   tmp=tmp&(511 -grille2[i][col]);


                    cpt++;}
		}
if((poids(grille2[u][col])!=1)&&((grille2[u][col]&tmp)!=0)) {
                                                grille2[u][col]=tmp&grille2[u][col];

}

}








void contraite_uniciteligne(int ligne,int v) {
 int j;
int tmp=511;
for(j=0;j<TAILLE;j++) {
if((poids(grille2[ligne][j])!=1)&&(j!=v)) {

					tmp=tmp&(511 -grille2[ligne][j]);}
		}
if((poids(grille2[ligne][v])!=1)&&((grille2[ligne][v]&tmp)!=0)) grille2[ligne][v]=tmp&grille2[ligne][v];


}




void contraite_col(int col)
{

     int i;
     int tmp=0;
     int tmp2;
     for(i=0;i<TAILLE;i++) {



                      if(poids(grille2[i][col])==1) {
                                                tmp=tmp|grille2[i][col];
                                                }


                      }


     for(i=0;i<TAILLE;i++) {
                    if(poids(grille2[i][col])!=1) {


                                               grille2[i][col]=(511-tmp)&(grille2[i][col]);

                                                }
                    }

    for(i=0;i<TAILLE;i++) {
                             contraite_unicitecol(i,col);
    }
}


void contraite_ligne(int ligne)
{


     int j;
     int tmp=0;
     for(j=0;j<TAILLE;j++) {


                      if(poids(grille2[ligne][j])==1) {
                                                tmp=tmp|grille2[ligne][j];
                                                }


                      }


     for(j=0;j<TAILLE;j++) {
                    if(poids(grille2[ligne][j])!=1) {


                                              grille2[ligne][j]=(511-tmp)&(grille2[ligne][j]);
                                                }
                    }
    for(j=0;j<TAILLE;j++) {
                        contraite_uniciteligne(ligne,j);
    }

}


void contraite_carre(int ligne,int col) {

   int i,j;
   int tmp=0;
     for(i=ligne-3;i<ligne;i++) {
                            for(j=col-3;j<col;j++) {
                                                       if(poids(grille2[i][j])==1) {
                                                                                     tmp=tmp|grille2[i][j];}

                                                       }
                                }


      for(i=ligne-3;i<ligne;i++) {
                            for(j=col-3;j<col;j++) {
                                                  if(poids(grille2[i][j])!=1)
                                                                                     grille2[i][j]=(511-tmp)&(grille2[i][j]);
                                                }
                    }




}

//test si la  grille est resolu en verifiant que les 81 cases ont un poid de 1
int nb_resolu() {
int i,j;
int cpt=0;
 for(i=0;i<TAILLE;i++) {
		for(j=0;j<TAILLE;j++) {
		    if(poids(grille2[i][j])==1) cpt++;
		}
 }

 return cpt;
}




  //Resoud le sudoku en appliquant les contraites
void resolution() {
     int nb_res=0;
     printf("\n\n\nResolution\n\n\n");
     nb_res=0;
     //tant qu'il y a des du changement dans la matrice on utilise les contraites
     while( nb_res!=81) {
     nb_res=nb_resolu();
      int i,j;
      int tmp=0;
      for(i=0;i<TAILLE;i++) {
                            contraite_ligne(i);
                             contraite_col(i);
      }
     for(i=0;i<TAILLE;i++) {
          if((i==2) || (i==5) || (i==8))   { contraite_carre(i+1,3);
                                             contraite_carre(i+1,6);
                                             contraite_carre(i+1,9);
          }

      }

     }
printf("Nombre de cases resolu:%d sur 81 \n\n",nb_res);





}





//converti les chiffres en binaires de la matrice binaire grille2
//en entier dans la matrice grille1
void remplir_resultat() {


int i,j;
int tmp;
int cpt;
cpt=0;
 for(i=0;i<TAILLE;i++) {
		for(j=0;j<TAILLE;j++) {
		    if(poids(grille2[i][j])==1) {      cpt=0;             tmp=grille2[i][j];
                                                              while(tmp > 0) {

                                                                   tmp=tmp>>1;
                                                                   cpt++;
                                                                   }

                                                            grille[i][j]=cpt;
                                        }
		}

 }
}


int main(int argc,char **argv) {
 int i,j;
 int tmp;
if(argc==2)
{

     lire_fichier(argv[1]);
    affiche_grille();

    remplir_matricebinaire();


resolution();


remplir_resultat();
	affiche_grille();


}

else {
	printf("Grille sudoku : \n");
	for(i=0;i<TAILLE;i++)
	{

		printf("ligne %d de la grille\n",i+1);
		for(j=0;j<TAILLE;j++)
		{
		scanf("%d",&grille[i][j]);
		}
		printf("\n");
	}
	for(i=0;i<TAILLE;i++) {
		for(j=0;j<TAILLE;j++) {

			printf("%d",grille[i][j]);
			}
			printf("\n");
	}


}


return 0;
}

