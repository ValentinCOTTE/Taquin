#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

struct Etat
{
    int tab[3][3];
    int ligne;  //positon x de la case vide
    int colonne;    //position y de la case vide
};


struct EtatFr
{
    struct Etat etat;
    char* chemin;
    int f;  //valeur de la distance g()+h()
    struct EtatFr* next;   //contient un noeud avec une valeur de f est inf�rieur
};

struct EtatFr* Cree_EtatFr(struct Etat etat, char* chemin, int f, struct EtatFr* next){
    struct EtatFr* newEtatFr = malloc(sizeof(struct EtatFr));

    newEtatFr->etat = etat;

    newEtatFr->chemin =malloc(30*sizeof(char));
    newEtatFr->chemin= chemin;



    newEtatFr->f = f;

    newEtatFr->next = next;


    return newEtatFr;
}

struct EtatFr* addFr(struct EtatFr* etatFr,struct Etat etat,char* chemin,int f){
    if(etatFr==NULL){
        struct EtatFr* nouveau=Cree_EtatFr(etat,chemin,f,NULL);
        return nouveau;
    }
    if(etatsEgaux(etatFr->etat,etat)==1){
        return etatFr;
    }
    if (f<=etatFr->f){
        struct EtatFr* nouveau=Cree_EtatFr(etat,chemin,f,etatFr);
        return nouveau;
    }
    else{
        if(etatFr->next!=NULL){
            etatFr->next=addFr(etatFr->next,etat,chemin,f);
        }
        else{
            struct EtatFr* nouveau=Cree_EtatFr(etat,chemin,f,NULL);
            etatFr->next=nouveau;
            return etatFr;
        }
    }
}

struct EtatFr* suppTete(struct EtatFr* tete){
    tete=tete->next;
    return tete;
}

struct EtatEx{
    struct Etat etat;
    int h;  //valeur de la distance h() la distance de manhattan
    struct EtatEx *gauche;   //contient un noeud avec une valeur de f est inf�rieur
    struct EtatEx *droite;   //contient un noeud avec une valeur de f est sup�rieur
};


struct EtatEx* Cree_EtatEx(struct Etat etat,int h,struct EtatEx* gauche,struct EtatEx* droite){
    struct EtatEx* newEtatEx = malloc(sizeof(struct EtatEx));

    newEtatEx->etat =etat;
    newEtatEx->h=h;

    newEtatEx->gauche = gauche;

    newEtatEx->droite = droite;

    return newEtatEx;
}

struct EtatEx* addEx(struct EtatEx* etatEx, struct Etat etat,int h){
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

//retourn 1 si les etats sont �gaux, 0 sinon
int etatsEgaux(struct Etat a, struct Etat b){
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

//retourne 1 si l'�tat � d�j� �t� explor�, 0 sinon
int isEx(struct EtatEx* listeEx, struct Etat etat, int h){
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
void afficher (struct Etat etat)
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

//la case vide est-elle a gauche? 1 si oui, 0 sinon
int testG(struct Etat tab)
{
    if (tab.colonne == 0) return 0;
    else {return 1;}
}
//droite?
int testD(struct Etat tab)
{
    if (tab.colonne == 2) return 0;
    else {return 1;}
}
//en haut?
int testH(struct Etat tab)
{
    if (tab.ligne == 0) return 0;
    else {return 1;}
}
//en bas?
int testB(struct Etat tab)
{
    if (tab.ligne == 2) return 0;
    else {return 1;}
}

//deplace la case vide � gauche
//donc on va �changer sa valeur avec la case � sa gauche
struct Etat gauche (struct Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne][colonne-1];
    etat.tab[ligne][colonne-1] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.colonne -= 1;
    return etat;
}

//deplace la case vide � droite...
struct Etat droite (struct Etat etat)
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
struct Etat haut (struct Etat etat)
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
struct Etat bas (struct Etat etat)
{
    int colonne = etat.colonne;
    int ligne = etat.ligne;
    int temp = etat.tab[ligne+1][colonne];
    etat.tab[ligne+1][colonne] = etat.tab[ligne][colonne];
    etat.tab[ligne][colonne] = temp;
    etat.ligne += 1;
    return etat;
}
// cr�e un taquin resolu
struct Etat resolu()
{
    struct Etat tabFinal;
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
//Calcule la distance manhattan (de l'etat actuel � l'�tat final)
int h(struct Etat ini)
{
    int h=6;
    int pi[6][9] = {{36,12,12,4,1,1,4,1,0},
                {8,7,6,5,4,3,2,1,0},
                {8,7,6,5,4,3,2,1,0},
                {8,7,6,5,3,2,4,1,0},
                {8,7,6,5,3,2,4,1,0},
                {1,1,1,1,1,1,1,1,0}};

    int a,i,j, result=0;
    for ( a = 0; a < 8; a++)
    {
        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                if (ini.tab [i][j] == a)
                {
                     result += (distance(a/3, a%3, i,j) * pi[h-1][a]);
                }
            }
        }
    }
    if ((h%2) == 0) return result;
    else return result/4;
}

// cr�e un taquin aleatoire valide
//on va simplement cr�e un taquin resolu, et le m�langer al�atoirement
struct Etat aleatoire()
{
    struct Etat tab = resolu();

    int i = 0;
	int nombre_aleatoire = 0;
	srand(time(NULL)); // initialisation de rand
	for(i=0; i<10000; i++){
		nombre_aleatoire = rand()%4;
		if (nombre_aleatoire == 0 && testD(tab)) tab = droite(tab);
		if (nombre_aleatoire == 1 && testG(tab)) tab = gauche(tab);
		if (nombre_aleatoire == 2 && testH(tab)) tab = haut(tab);
		if (nombre_aleatoire == 3 && testB(tab)) tab = bas(tab);
	}
	return tab;
}

char* ecrire(char* chaine,char c){
    char* nouvelle=malloc(sizeof(char)*100);
    nouvelle[0]=c;
    int i = 0;
    while(chaine[i] != '\0') // On boucle tant qu'on n'est pas arriv� � la fin F
    {
        nouvelle[i+1]=chaine[i];
        i++;
    }
    nouvelle[i+1]='\0';
    return nouvelle;
}

int longueurChaine(char* chaine)
{
    int nombreDeCaracteres = 0;
    char caractereActuel = chaine[0];

    while(caractereActuel != '\0') // On boucle tant qu'on n'est pas arriv� � la fin F
    {
        caractereActuel = chaine[nombreDeCaracteres];
        nombreDeCaracteres++;
    }
    return nombreDeCaracteres;
}

void afficherChaine(char* chaine)
{
    int i = 0;
    char caractereActuel = chaine[i];

    i=longueurChaine(chaine);
    i=i-3;

    for(i;i>=0;i--)
    {
        printf("%c",chaine[i]);

    }

}


char* solution(struct Etat initial){

    int nbetats=0;

    char* chemin=malloc(sizeof(char)*100);
    chemin[0]=' ';
    chemin[1]='\0';
    struct Etat actu=initial;
    struct EtatEx* listeEx=Cree_EtatEx(initial,h(initial),NULL,NULL);
    struct EtatFr* listeFr=NULL;

    if(testB(initial)==1){
        struct Etat a=bas(initial);
        char* b=ecrire(chemin,'B');
        int c=1+h(initial);
        listeFr=addFr(listeFr,a,b,c);
        nbetats++;
    }
    if(testH(initial)==1){
        listeFr=addFr(listeFr,haut(initial),ecrire(chemin,'H'),1+h(initial));
        nbetats++;
    }
    if(testG(initial)==1){
        listeFr=addFr(listeFr,gauche(initial),ecrire(chemin,'G'),1+h(initial));
        nbetats++;
    }
    if(testD(initial)==1){
        listeFr=addFr(listeFr,droite(initial),ecrire(chemin,'D'),1+h(initial));
        nbetats++;
    }
    while(etatsEgaux(actu,resolu())!=1){
        actu=listeFr->etat;
        chemin=listeFr->chemin;
        listeFr=suppTete(listeFr);

        if(testB(actu)==1&&chemin[0]!='H'){
        struct Etat a1=bas(actu);
        char* b1=ecrire(chemin,'B');
        int c1=1+longueurChaine(chemin)+h(actu);
        listeFr=addFr(listeFr,a1,b1,c1);
        nbetats++;
        }
        if(testH(actu)==1&&chemin[0]!='B'){
            struct Etat a2=haut(actu);
            char* b2=ecrire(chemin,'H');
            int c2=1+longueurChaine(chemin)+h(actu);
            listeFr=addFr(listeFr,a2,b2,c2);
            nbetats++;
        }
        if(testG(actu)==1&&chemin[0]!='D'){
            listeFr=addFr(listeFr,gauche(actu),ecrire(chemin,'G'),1+longueurChaine(chemin)+h(actu));
            nbetats++;
        }
        if(testD(actu)==1&&chemin[0]!='G'){
            listeFr=addFr(listeFr,droite(actu),ecrire(chemin,'D'),1+longueurChaine(chemin)+h(actu));
            nbetats++;
        }
    }
    printf("\n%d Etats explor�s",nbetats);
    printf("\n%d longueur du chemin",longueurChaine(chemin));
    printf("\nSolution:\n");
    return chemin;
}

int main()
{
    //struct Etat etat = aleatoire();

 //Saisie manuel
    struct Etat etat;
    etat.tab[0][0]=2;etat.tab[0][1]=0;etat.tab[0][2]=8;
    etat.tab[1][0]=3;etat.tab[1][1]=1;etat.tab[1][2]=7;
    etat.tab[2][0]=4;etat.tab[2][1]=5;etat.tab[2][2]=6;
    etat.ligne=0;
    etat.colonne=2;

    afficher(etat);
    printf("\n");
    afficherChaine(solution(etat));
    printf("\n");
    return 0;
}
