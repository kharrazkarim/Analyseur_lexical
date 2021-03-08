#ifndef BIB_H_
#define BIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct transition //Transition d'un automate
{
	int start; //etat de start
	char symbole; // symbole
	int end; //etat d'end
} transition;

typedef struct Automate {
	int *Q;  //ensemble des etats
	char* symbole; //ensemble des symboles 
	transition *delta; //Ensemble des transitions
	int nbTrans; // nombre des transitions
	int initial; // l'etat inital
	int *accepteur;  // Ensemble des etats accepteurs
	int nbEtat; // nombre des etats
	int nbaccept; // nombre des etats accepteurs
} Automate;

typedef struct deterministe {
	char nom;  //nom de l'etat
	int* etat_nd; // etats non deterministes
	int nbEtat; //nombre d'etats
} deterministe;


/*
 * CONSTRUCTION
 */

Automate initialiser_automate(Automate A, int nbEtat, int delta, int nbsymboles, int F);
Automate construire_automate_vide();
Automate construire_automate_mot_vide();
Automate construire_automate_caractere(char symbole);
Automate construire_automate_mot(char* symbole);
Automate ajouter_transition(Automate A, int start, char symbole, int end,int alloue);

/*
 * FONCTIONS
 */
Automate reunion_automate(Automate A, Automate B);
Automate concat_automate(Automate A, Automate B);
Automate fermeture_it_kleene(Automate A);
Automate copier_automate(Automate B);


/*
 * AFFICHAGE
 */
void afficher_automate(Automate A, int dt);
void afficher_etat_dterministe(deterministe A);
void afficher_matrice(int ** mat , int n , int k) ;


/*
 * DETERMINISATION
 */
void determiniser_etat(Automate Automate, deterministe* etat_dt,
		transition* delta, int *dernier_ajoute_dt, int *idxdelta,
		deterministe etat_a_determiner);
int verfier_etat_dt(deterministe* etat, deterministe tmp,
		int *dernier_ajoute_dt);
char nextsymbole(deterministe* etat, int dernier_ajoute_dt);
deterministe prochain_etat_dt(Automate Automate, deterministe etat_a_determiner,
		char nom, char symbole);
Automate determiniser(Automate autom);
int in_tableau(int* tab, int nbtab, int val);

/*
 * EXECUTION MOT ET VERIFICATION
 */

int verifier_detrerminisme(Automate autom);
int executer_mot(Automate A, char* mot);
int executer_caractere(Automate A,int start ,char consomme);

/*
 * MINIMISATION
 */
Automate minimiser(Automate A);
int ** initialiser_mat(int ** Mat,int n , int k);
int val_mat (Automate A, int ** mat , int etat , char symbole);
void remplir_mat(Automate A, int ** mat , int n , int k);
void  exist_eps(int ** matn ,int ** mato , int n , int k);
int nouveau_etat (int ** mato , int ** matn , int n , int k);
int exist (transition * nouveau , int start , char symbole , int end, int index);
int accepter (int etat , int* tab, int n);


#endif /* BIB_H_ */
