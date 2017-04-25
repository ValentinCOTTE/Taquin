#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct
{
    int tab[3][3];
    int ligne;
    int colonne;
} Etat;

void afficher (Etat etat)
{
    int i,j;
    for ( i = 0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if (etat.ligne == i && etat.colonne == j)
            {
                printf("\t");
            }
            else
            {
                printf("%d\t",etat.tab[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool testG(Etat tab)
{
    if (tab.colonne == 0) return false;
    else {return true;}
}

bool testD(Etat tab)
{
    if (tab.colonne == 2) return false;
    else {return true;}
}

bool testH(Etat tab)
{
    if (tab.ligne == 0) return false;
    else {return true;}
}

bool testB(Etat tab)
{
    if (tab.ligne == 2) return false;
    else {return true;}
}

Etat gauche (Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne][colonne-1];
    etat.tab[ligne][colonne-1] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.colonne -= 1;
    return etat;
}

Etat droite (Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne][colonne+1];
    etat.tab[ligne][colonne+1] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.colonne += 1;
    return etat;
}

Etat haut (Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne-1][colonne];
    etat.tab[ligne-1][colonne] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.ligne -= 1;
    return etat;
}

Etat bas (Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne+1][colonne];
    etat.tab[ligne+1][colonne] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.ligne += 1;
    return etat;
}
// crée un taquin resolu
Etat resolu()
{
    Etat tabFinal;
    int j,i,x = 0;
    for(j =0; j<3; j++)
    {
        for (i = 0; i<3; i++)
        {
            tabFinal.tab[j][i] = x;
            x++;
        }
    }

    tabFinal.ligne = 2;
    tabFinal.colonne = 2;
    return tabFinal;
}
//calcule la distance entre deux point
int distance (int ligne1, int colonne1, int ligne2, int colonne2)
{
    return fabs(ligne2 - ligne1) + fabs(colonne2 - colonne1);

}
//Calcule la distance manhattan
int man (Etat ini)
{
    int a,i,j, result=0;
    for ( a = 0; a < 9; a++)
    {
        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                if (ini.tab [i][j] == a)
                {
                     result += distance(a%3, a/3, i,j);
                }
            }
        }
    }
    return result;
}

// crée un taquin aleatoire valide
Etat aleatoire()
{
    Etat tab = resolu();

    int i = 0;
	int nombre_aleatoire = 0;
	srand(time(NULL)); // initialisation de rand
	for(i=0; i<100; i++){
		nombre_aleatoire = rand()%4;
		if (nombre_aleatoire == 0 && testD(tab)) tab = droite(tab);
		if (nombre_aleatoire == 1 && testG(tab)) tab = gauche(tab);
		if (nombre_aleatoire == 2 && testH(tab)) tab = haut(tab);
		if (nombre_aleatoire == 3 && testB(tab)) tab = bas(tab);
	}
	return tab;
}
int main()
{
    afficher(resolu());
    Etat tab = aleatoire();
    afficher(tab);
    printf("%d", man(tab));


    return 0;
}
