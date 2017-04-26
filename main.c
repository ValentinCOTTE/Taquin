#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct
{
    int tab[3][3];
    int ligne;  //positon x de la case vide
    int colonne;    //position y de la case vide
} Etat;


struct EtatFr
{
    Etat etat;
    char* chemin;
    int f;  //valeur de la distance g()+h()
    struct EtatFr* next;   //contient un noeud avec une valeur de f est inférieur
};

struct EtatFr* Cree_EtatFr(Etat etat, char* chemin, int f, struct EtatFr* next){
    struct EtatFr* newEtatFr = malloc(sizeof(struct EtatFr));

    newEtatFr->etat = etat;

    newEtatFr->chemin =malloc(30*sizeof(char));
    newEtatFr->chemin= chemin;



    newEtatFr->f = f;

    newEtatFr->next = next;


    return newEtatFr;
}

struct EtatFr* addFr(struct EtatFr* etatFr,Etat etat,char* chemin,int f){
    if (f<=etatFr->f){
        struct EtatFr* nouveau=Cree_EtatFr(etat,chemin,f,etatFr);
        return nouveau;
    }
    else{
        if(etatFr->next!=NULL){
            etatFr->next=addFr(etatFr->next,etat,chemin,f);
        }
        else{
            struct EtatFr* nouveau=Cree_EtatFr(etat,chemin,f,etatFr);
            etatFr->next=nouveau;
            return etatFr;
        }
    }
}

struct EtatFr* suppTete(struct EtatFr* tete){
    tete=tete->next;
    return tete;
}

Etat getFr(struct EtatFr* frontiere){
    return frontiere->etat;
}

struct EtatEx{
    Etat etat;
    int h;  //valeur de la distance h() la distance de manhattan
    struct EtatEx *gauche;   //contient un noeud avec une valeur de f est inférieur
    struct EtatEx *droite;   //contient un noeud avec une valeur de f est supérieur
};


struct EtatEx* Cree_EtatEx(Etat etat,int h,struct EtatEx* gauche,struct EtatEx* droite){
    struct EtatEx* newEtatEx = malloc(sizeof(struct EtatEx));

    newEtatEx->etat =etat;
    newEtatEx->h=h;

    newEtatEx->gauche = gauche;

    newEtatEx->droite = droite;

    return newEtatEx;
}

struct EtatEx* addEx(struct EtatEx* etatEx, Etat etat,int h){
    if (h<=etatEx->h){
        if(etatEx->gauche==NULL){
            struct EtatEx* nouveau=Cree_EtatEx(etat,h,NULL,NULL);
            etatEx->gauche=nouveau;
        }
        else{
            etatEx->gauche=addEx(etatEx->gauche,etat,h);
        }
    }
    else{
        if(etatEx->droite==NULL){
            struct EtatEx* nouveau=Cree_EtatEx(etat,h,NULL,NULL);
            etatEx->droite=nouveau;
        }
        else{
            etatEx->droite=addEx(etatEx->droite,etat,h);
        }
    }
    return etatEx;
}

//retourn 1 si les etats sont égaux, 0 sinon
int etatsEgaux(Etat a, Etat b){
    if(a.colonne!=b.colonne||a.ligne!=b.ligne){
        return 0;
    }
    int i,j;
    for ( i = 0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if(a.tab[i][j]!=b.tab[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

//retourne 1 si l'état à déjà été exploré, 0 sinon
int isEx(struct EtatEx* listeEx, Etat etat, int h){
    if(listeEx==NULL){
        return 0;
    }
    if(h==listeEx->h){
        if(etatsEgaux(listeEx->etat,etat)==1){
            return 1;
        }
    }
    if(listeEx->h<h){
        return 0;
    }
    return isEx(listeEx->gauche,etat,h);
}


//Affiche l'etat du taquin
void afficher (Etat etat)
{
    int i,j;
    for ( i = 0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if (etat.ligne == i && etat.colonne == j)
            {
                printf("X ");
            }
            else
            {
                printf("%d ",etat.tab[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

//la case vide est-elle a gauche?
bool testG(Etat tab)
{
    if (tab.colonne == 0) return false;
    else {return true;}
}
//droite?
bool testD(Etat tab)
{
    if (tab.colonne == 2) return false;
    else {return true;}
}
//en haut?
bool testH(Etat tab)
{
    if (tab.ligne == 0) return false;
    else {return true;}
}
//en bas?
bool testB(Etat tab)
{
    if (tab.ligne == 2) return false;
    else {return true;}
}

//deplace la case vide à gauche
//donc on va échanger sa valeur avec la case à sa gauche
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

//deplace la case vide à droite...
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

//deplace la case vide en haut...
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

//deplace la case vide en bas...
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
//Calcule la distance manhattan (de l'etat actuel à l'état final)
int manhattan (Etat init)
{
    int a,i,j, result=0;
    for ( a = 0; a < 9; a++)
    {
        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                if (init.tab [i][j] == a)
                {
                     result += distance(a%3, a/3, i,j);  //a%3 ligne de la valeur a avec un taquin resolu
                }                                        //a/3 colonne de la valeur a avec un taquin resolu
            }
        }
    }
    return result;
}

// crée un taquin aleatoire valide
//on va simplement crée un taquin resolu, et le mélanger aléatoirement
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

int longueurChaine(char* chaine)
{
    int nombreDeCaracteres = 0;
    char i;
    char caractereActuel = 0;

    do
    {
        caractereActuel = chaine[nombreDeCaracteres];
        nombreDeCaracteres++;

    }
    while(caractereActuel != '\0'); // On boucle tant qu'on n'est pas arrivé à l'\0

    nombreDeCaracteres--; // On retire 1 caractère de long pour ne pas compter le caractère \0

    return nombreDeCaracteres;
}

int main()
{
    afficher(resolu());
    Etat tab = aleatoire();
    afficher(tab);
    printf("%d", manhattan(tab));


    return 0;
}
