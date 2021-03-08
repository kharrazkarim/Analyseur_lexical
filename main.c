#include "bib.h"
#include <string.h>
int main(int argc, char const *argv[]) {
	Automate A, B ;

// Test rapport !!
	//	A=construire_automate_vide();
		//A=construire_automate_mot_vide();
		//printf("Construction de A \n");
		//A=construire_automate_caractere('k');
		//afficher_automate(A, 0);

		//A=fermeture_it_kleene(A);
		//A=ajouter_transition(A,0,'a',0,1);
		//A=ajouter_transition(A,0,'a',1,1);
		//A=ajouter_transition(A,0,'k',1,1);
		//printf("Construction de B\n");

		//B=construire_automate_caractere('a');
		 //afficher_automate(B, 0);


		//printf("Construction de C la réunion de A et B \n");

		//C=reunion_automate(A,B);
		//afficher_automate(A, 0);

///////////////////////////////////////////
//  Construction de l'automate du cours 

	A = construire_automate_caractere('a');
	B = construire_automate_caractere('b');
	A = concat_automate(A, B);
	A = reunion_automate(A, B);
	A.Q[A.nbEtat] = 4;
	A.nbEtat++;
	A.accepteur[A.nbaccept] = 4;
	A.nbaccept++;
	A.accepteur[A.nbaccept] = 0;
	A.nbaccept++;
	A = ajouter_transition(A, 0, 'a', 4, 1);
	A = ajouter_transition(A, 2, 'a', 1, 1);
	A = ajouter_transition(A, 2, 'b', 3, 1);
	A = ajouter_transition(A, 2, 'a', 4, 1);
	A = ajouter_transition(A, 3, 'a', 4, 1);
	A = ajouter_transition(A, 3, 'b', 3, 1);
	A = ajouter_transition(A, 3, 'a', 1, 1);
	afficher_automate(A, 0);

	A = determiniser(A);
	afficher_automate(A, 1);
	A = minimiser(A);
	afficher_automate(A, 0);

	printf("#### test acceptation mots ###\n");
	 		executer_mot(A, "abab");
	 		executer_mot(A, "abaa");
	 		executer_mot(A, "abababbb");
	 	


	return 0;
}


