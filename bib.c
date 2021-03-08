#include "bib.h"


Automate initialiser_automate(Automate A, int nbEtat, int delta, int nbsymboles, int F) {

	A.Q = malloc(nbEtat * sizeof(int));
	//delta est le nombre de transitions
	if (!delta)
		A.delta = NULL;
	else
		A.delta = malloc(delta * sizeof(transition));

	if (!F)
		A.accepteur = NULL;
	else {
		A.accepteur = malloc((F) * sizeof(int));
		A.accepteur[F] = -1;
		A.nbaccept = F;
	}
	if (!nbsymboles)
		A.symbole = "";
	else
		A.symbole = malloc(nbsymboles * sizeof(char));
	
	A.nbEtat = nbEtat;
	A.nbTrans = delta;
	return A;
}

Automate construire_automate_vide() {
	/*Q={0} / Delta=vide / F=vide / s=0 */
	Automate A;
	A = initialiser_automate(A, 1, 0, 0, 0);
	A.Q[0] = 0;
	A.initial = 0;
	return A;

}
Automate construire_automate_mot_vide() {
	/*Q={0} / Delta=vide / F=0 / s=0 */
	Automate A;
	A = initialiser_automate(A, 1, 0, 0, 1);
	A.Q[0] = 0;
	A.accepteur[0] = 0;
	A.nbaccept = 1;
	A.initial = 0;
	return A;

}

Automate construire_automate_caractere(char symbole) {
	/*Q={0,1} Delta={(0,symbole,1)} s={0} F={1}*/
	Automate A;
	A = initialiser_automate(A, 2, 1, 1, 1);
	A.initial = 0;
	A.symbole[0] = symbole;
	A.accepteur[0] = 1;
	A.nbaccept = 1;
	for (int i = 0; i <= A.nbEtat; i++)
		A.Q[i] = i;
	transition tab[A.nbTrans];
	A.delta[0].start = 0;
	A.delta[0].end = 1;
	A.delta[0].symbole = symbole;
	return A;
}
Automate construire_automate_mot(char* symbole) {
	/*Q={0,1} Delta={(0,symbole,1)} s={0} F={1}*/
	Automate A;
	A = initialiser_automate(A, 2, 0, strlen(symbole), 1);
	A.initial = 0;
	A.symbole = symbole;
	A.accepteur[0] = 1;
	A.nbaccept = 1;
	for (int i = 0; i <= A.nbEtat; i++)
		A.Q[i] = i;
	for (int i = 0; i < strlen(symbole); i++)
		A = ajouter_transition(A, 0, symbole[i], 1, 1);
	return A;
}

Automate ajouter_transition(Automate A, int start, char symbole, int end, int alloue) {
	/*
	 * alloue =0 : ne pas allouer la transition a ajouter
	 * alloue =1 : allouer la transition a ajouter
	 */

	
	//verification standard
	if (end == A.initial) {
		printf(
				"impossible de faire la transition (%d,%c,%d) ! L'automate doit etre standard \n ",
				start, symbole, end);
		return A;
	}
	if (alloue == 1)
		A.delta = realloc(A.delta, (A.nbTrans + 1) * sizeof(transition));

	//verification etats
	int verif = 0; //0 vérifications
	
	for (int i = 0; i < A.nbEtat; i++) {
		if (start == A.Q[i])
			verif++;
		if (end == A.Q[i])
			verif++;
		if (verif == 2)
			break;
	}
	if (verif == 2) {
		//verification symbole
		for (int i = 0; i < strlen(A.symbole); i++) {
			if ((int) symbole == (int) A.symbole[i]) {
				verif++;
				break;
			}
		}
		//verification duplication transition
		for (int i = 0; i < A.nbTrans; i++)
			if (A.delta[i].start == start && A.delta[i].symbole == symbole
					&& A.delta[i].end == end) {
				verif = 99;
				break;
			}
	}
	switch (verif) {
	case 0:
	case 1:
		printf(" etat innexistant! \n");
		break;
	case 2:
		printf(" symbole  inexistant \n");
		break;
	case 99:
		printf("transition dupliquee \n");
		break;
	case 3:
		if (start >= 65)
			printf("creation de la transition (%c,%c,%c) \n", start, symbole,
					end);
		else
			printf("creation de la transition (%d,%c,%d) \n", start, symbole,
					end);
		A.delta[A.nbTrans].start = start;
		A.delta[A.nbTrans].symbole = symbole;
		A.delta[A.nbTrans].end = end;
		A.nbTrans++;
		break;
	default:
		printf("erreur fonctin transition\n");
	}

	return A;

}

int ** initialiser_mat(int ** mat, int n, int k) {
	mat = (int**) malloc(n * k * sizeof(int*));
	for (int i = 0; i <= n * k; i++)
		mat[i] = (int*) malloc(n * k * sizeof(int));
	return mat;
}

void afficher_matrice(int ** mat, int n, int k) {
	for (int i = 0; i <= n; i++) {
		printf("\n");
		for (int j = 0; j < k; j++) {
			if (j == 0 && i == 1)
				printf("|e|");
			else if (j == 0 && i == 0)
				printf("| |");

			else if (i == 0 || j == 0)
				printf("|%c|", mat[i][j]);
			else
				printf("|%d|", mat[i][j]);
		}
	}
	printf("\n");
}
int verifier_accepteur(Automate A, int etat) {
	for (int i = 0; i <= A.nbaccept; i++) {
		if (A.accepteur[i] == etat)
			return 1;
	}
	return 0;
}
int valeur_mat(Automate A, int ** mat, int etat, char symbole) {
	int end;
	for (int i = 0; i < A.nbTrans; i++)
		if (etat == A.delta[i].start && symbole == A.delta[i].symbole) {
			end = A.delta[i].end;
			break;
		}
	for (int i = 1; i <= A.nbEtat; i++)
		if (end == mat[0][i])
			return mat[1][i];
}
void remplir_mat(Automate A, int ** mat, int n, int k) {
	for (int i = 2; i <= n; i++)
		for (int j = 1; j < k; j++) {
			mat[i][j] = valeur_mat(A, mat, mat[0][j], mat[i][0]);
		}
}

void exist_eps(int ** matn, int ** mato, int n, int k) {
	typedef struct ma_chaine {
		int epsilon;
		char * mot;
	} ma_chaine;

	int verif = -1;
	int e = 0;
	ma_chaine* chaine = malloc(k * sizeof(ma_chaine));
	for (int i = 1; i < k; i++)
		chaine[i].mot = malloc(n * sizeof(char));

	char aux[2];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j < k; j++) {
			sprintf(aux, "%d", mato[i][j]);
			strcat(chaine[j].mot, aux);
			chaine[j].epsilon = matn[1][j];
		}

	int compt = 2;
	chaine[1].epsilon = e;
	while (compt < k) {
		verif = -1;
		for (int i = compt - 1; i >= 1; i--) {
			if (!strcmp(chaine[compt].mot, chaine[i].mot)) {
				chaine[compt].epsilon = chaine[i].epsilon;
				matn[1][compt] = chaine[i].epsilon;
				verif = 1;
				break;
			}
		}
		if (verif < 0) {
			e++;
			chaine[compt].epsilon = e;
			matn[1][compt] = e;
		}
		compt++;
	}
}

int nouveau_etat(int ** mato, int ** matn, int n, int k) {
	int max1 = 0;
	int max2 = max1;

	for (int j = 1; j < k; j++) {
		if (max1 < mato[1][j]) {
			max1 = mato[1][j];
		}
		if (max2 < matn[1][j]) {
			max2 = matn[1][j];
		}
	}
	if (max1 == max2)
		return 0;
	else
		return 1;
}

int exist(transition * nouveau, int start, char symbole, int end, int index) {
	int verif = 1;
	for (int i = 0; i <= index; i++) {
		if ((nouveau[i].start == start) && (nouveau[i].symbole == symbole)
				&& (nouveau[i].end == end)) {
			verif = 0;
			break;
		}
	}
	return verif;
}

int accepter(int etat, int* tab, int n) {
	int verif = 0;
	for (int i = 0; i < n; i++)
		if (etat == tab[i]) {
			verif = 1;
			break;
		}
	return verif;
}

Automate minimiser(Automate A) {
	Automate M;
	int ** Matold;
	int ** Matnew;
	int verif;
	Matold = initialiser_mat(Matold, A.nbEtat + 1, strlen(A.symbole) + 1);
	Matnew = initialiser_mat(Matnew, A.nbEtat + 1, strlen(A.symbole) + 1);

	//initialisation de la matrice de minimisation 
	for (int i = 0; i < A.nbEtat; i++) {
		Matold[0][i + 1] = A.Q[i];
	}
	for (int i = 0; i < strlen(A.symbole); i++) {
		Matold[i + 2][0] = A.symbole[i];
	}

	printf("\n");
	//initialisation de l'epsilone en fonction de l'etat accepteur
	for (int i = 1; i <= A.nbEtat; i++) {
		if (verifier_accepteur(A, Matold[0][i]))
			Matold[1][i] = 1;
		else
			Matold[1][i] = 0;
	}
	//Remplissage du tableau en fonction des transitions de l'automate
	remplir_mat(A, Matold, strlen(A.symbole) + 1, A.nbEtat + 1);
	afficher_matrice(Matold, strlen(A.symbole) + 1, A.nbEtat + 1);

	//Initialisation de la nouvelle matrice de minimisation
	for (int i = 0; i < A.nbEtat; i++) {
		Matnew[0][i + 1] = A.Q[i];
	}
	for (int i = 0; i < strlen(A.symbole); i++) {
		Matnew[i + 2][0] = A.symbole[i];
	}
	//Calcul du nouvel epsilone et remplissage de la nouvelle Matrice de minimisation
	exist_eps(Matnew, Matold, strlen(A.symbole) + 1, A.nbEtat + 1);
	remplir_mat(A, Matnew, strlen(A.symbole) + 1, A.nbEtat + 1);
	printf("\n");
	afficher_matrice(Matnew, strlen(A.symbole) + 1, A.nbEtat + 1);
	
	//Refaire le meme travail jusqu'à ce qu'on obtient plus un nouvel etat au niveau de l'epsilone
	while (nouveau_etat(Matold, Matnew, strlen(A.symbole) + 1, A.nbEtat + 1) > 0) {
		Matold = Matnew;
		Matnew = initialiser_mat(Matnew, A.nbEtat + 1, strlen(A.symbole) + 1);
		for (int i = 0; i < A.nbEtat; i++) {
			Matnew[0][i + 1] = A.Q[i];
		}
		for (int i = 0; i < strlen(A.symbole); i++) {
			Matnew[i + 2][0] = A.symbole[i];
		}
		exist_eps(Matnew, Matold, strlen(A.symbole) + 1, A.nbEtat + 1);
		remplir_mat(A, Matnew, strlen(A.symbole) + 1, A.nbEtat + 1);
		afficher_matrice(Matnew, strlen(A.symbole) + 1, A.nbEtat + 1);
	}
	printf(" Finalisation de la minimisation \n");
	printf("Les nouvelles transitions \n");

	//Création du nouveau Automate minimisé
	transition *nouveau = malloc(
			strlen(A.symbole) * A.nbEtat * sizeof(transition));
	int index = 0;
	for (int i = 2; i <= strlen(A.symbole) + 1; i++)
		for (int j = 1; j < A.nbEtat + 1; j++) {
			if (exist(nouveau, Matnew[1][j], Matnew[i][0], Matnew[i][j],
					index)) {
				nouveau[index].start = Matnew[1][j];
				nouveau[index].symbole = Matnew[i][0];
				nouveau[index].end = Matnew[i][j];
				//printf("{%d,%c,%d} ", nouveau[index].start,nouveau[index].symbole,nouveau[index].end);
				index++;
			}
		}
	M.nbTrans = index;

	M.symbole = malloc(strlen(A.symbole) * sizeof(char));
	strcpy(M.symbole, A.symbole);
	M.initial = 0;
	M.Q = malloc(A.nbEtat * sizeof(int));
	M.Q[0] = 0;
	index = 0;
	for (int i = 2; i <= A.nbEtat; i++) {
		verif = 1;
		for (int j = 0; j <= index; j++)
			if (Matnew[1][i] == M.Q[j]) {
				verif = 0;
				break;
			}
		if (verif) {
			index++;
			M.Q[index] = Matnew[1][i];
		}
	}
	M.nbEtat = index + 1;

	M.delta = malloc(strlen(A.symbole) * A.nbEtat * sizeof(transition));

	for (int i = 0; i < M.nbTrans; i++) {
		M.delta[i].start = nouveau[i].start;
		M.delta[i].end = nouveau[i].end;
		M.delta[i].symbole = nouveau[i].symbole;
		printf("{%d,%c,%d} ", M.delta[i].start, M.delta[i].symbole,
				M.delta[i].end);
	}

	M.nbaccept = 0;
	M.accepteur = malloc(A.nbEtat * sizeof(int));

	//Les etats accepteurs
	for (int i = 1; i <= A.nbEtat; i++) {
		if (accepter(Matnew[0][i], A.accepteur, A.nbaccept)) {
			if (!accepter(Matnew[1][i], M.accepteur, M.nbaccept)) {
				M.accepteur[M.nbaccept] = Matnew[1][i];
				M.nbaccept++;
			}
		}
	}

	printf("\n");

	return M;
}




void afficher_automate(Automate A, int dt) {
	/*
	 * dt = 0 : affichage automate non deterministe
	 * dt = 1 : affichage automate deterministe
	 */
	int idx = 0;
	printf("L'ensemble des etats de l'automate \n");
	printf("(");
	for (int i = 0; i < A.nbEtat; i++) {
		if (dt)
			printf("%c|", A.Q[i]);
		else
			printf("%d|", A.Q[i]);

	}
	printf(")\n");
	printf("Symboles de l'automate \n");
	if (A.symbole == "")
		printf("VIDE \n");
	else {
		printf("(");
		for (int i = 0; i < strlen(A.symbole); i++) {
			printf("%c|", A.symbole[i]);
		}
		printf(")\n");
	}

	printf("L'ensemble des transitions de l'automate \n");
	if (A.nbTrans == 0)
		printf("VIDE \n");
	else {
		printf("(");
		for (int i = 0; i < A.nbTrans; i++) {
			if (dt)
				printf("{%c,%c,%c} ", A.delta[i].start, A.delta[i].symbole,
						A.delta[i].end);
			else
				printf("{%d,%c,%d} ", A.delta[i].start, A.delta[i].symbole,
						A.delta[i].end);
		}
		printf(")\n");
	}
	printf("Etat initial : { %d }\n", A.initial);
	printf("L'ensemble des etats accepteurs de l'automate \n");
	if (A.nbaccept == 0)
		printf("VIDE \n");
	else {
		printf("(");
		while (idx < A.nbaccept) {
			if (dt)
				printf(" %c | ", A.accepteur[idx]);
			else
				printf(" %d | ", A.accepteur[idx]);

			idx++;
		}
		printf(")\n");
	}

}


Automate concat_automate(Automate A, Automate B) {
	Automate C;
	int nb = 0;
	int k;
	int idx = 0;
	C.initial = A.initial;
	char symbole[1];
	C.symbole = malloc(
			(strlen(A.symbole) + strlen(B.symbole) * sizeof(char)));
	strcpy(C.symbole, A.symbole);
	for (int i = 0; i < strlen(B.symbole); i++)
		if (strchr(A.symbole, B.symbole[i]) == NULL) {
			symbole[0] = B.symbole[i];
			strcat(C.symbole, symbole);
		}

	C.nbEtat = A.nbEtat + B.nbEtat - 1;
	for (int i = 0; i < B.nbTrans; i++) {
		if (B.delta[i].start == B.initial)
			nb++;
	}
	C.nbTrans = A.nbTrans + A.nbaccept * nb + B.nbTrans - nb;
	C.Q = malloc(C.nbEtat * sizeof(int));
	for (int i = 0; i < C.nbEtat; i++) {
		C.Q[i] = i;
	}

	transition tab[C.nbTrans];
	C.delta = malloc(C.nbTrans * sizeof(transition));
	for (int i = 0; i < A.nbTrans; i++) {
		C.delta[i] = A.delta[i];
		printf("création de la transition (%d,%c,%d) \n", C.delta[i].start,
				C.delta[i].symbole, C.delta[i].end);
	}

	k = A.nbTrans;
	while (idx < A.nbaccept) {
		for (int i = 0; i < nb; i++) {
			C.delta[k].start = A.accepteur[idx];
			C.delta[k].end = B.delta[i].end + A.nbEtat - 1;
			C.delta[k].symbole = B.delta[i].symbole;
			printf("création de la transition (%d,%c,%d) \n", C.delta[k].start,
					C.delta[k].symbole, C.delta[k].end);
			k++;

		}
		idx++;
	}
	for (int i = k; i < C.nbTrans; i++) {
		C.delta[i].start = B.delta[i - A.nbTrans].start + A.nbEtat - 1;
		C.delta[i].end = B.delta[i - A.nbTrans].end + A.nbEtat - 1;
		C.delta[i].symbole = B.delta[i - A.nbTrans].symbole;
		printf("création de la transition (%d,%c,%d) \n", C.delta[i].start,
				C.delta[i].symbole, C.delta[i].end);
	}
	if (A.accepteur[A.nbaccept - 1] == A.Q[A.nbEtat - 1]) {
		C.nbaccept = B.nbaccept;
		C.accepteur = malloc(C.nbaccept * sizeof(int));
		for (int i = 0; i < C.nbaccept; i++) {
			C.accepteur[i] = B.accepteur[i] + A.nbEtat - 1;

		}
	} else {
		C.nbaccept = B.nbaccept + A.nbaccept;
		C.accepteur = malloc(C.nbaccept * sizeof(int));
		for (int i = 0; i < A.nbaccept; i++) {
			C.accepteur[i] = A.accepteur[i];
		}
		for (int i = A.nbaccept; i < C.nbaccept; i++) {
			C.accepteur[i] = B.accepteur[i - A.nbaccept] + A.nbaccept;
		}
	}

	return C;

}
Automate reunion_automate(Automate A, Automate B) {
	int k = 0; // idx res
	// calcul des variables
	Automate res;
	res.nbEtat = A.nbEtat + B.nbEtat - 1;
	res.nbTrans = A.nbTrans + B.nbTrans;
	int symbole = strlen(A.symbole) + strlen(B.symbole);
	res.nbaccept = A.nbaccept + B.nbaccept;
	res = initialiser_automate(res, res.nbEtat, res.nbTrans, symbole, res.nbaccept);
	res.initial = A.initial;
	//reunion symbole
	for (int i = 0; i < strlen(A.symbole); i++) {
		res.symbole[k] = A.symbole[i];
		k++;
	}
	for (int i = 0; i < strlen(B.symbole); i++) {
		int j = 0;
		int trouve = 0;
		//verif si existe
		while (j < k && trouve == 0) {
			if (res.symbole[j] == B.symbole[i])
				trouve = 1;
			j++;
		}
		if (trouve == 0) {
			res.symbole[k] = B.symbole[i];
			k++;
		}
	}

	//reunion transition
	transition tab[res.nbTrans];
	k = 0;
	for (int i = 0; i < A.nbTrans; i++) {
		res.delta[k] = A.delta[i];
		k++;
	}
	k = A.nbTrans; // remplit avec tout les transitions de A
	for (int i = 0; i < B.nbTrans; i++) {
		if (B.delta[i].start == B.initial)
			res.delta[k].start = res.initial;
		else
			res.delta[k].start = A.nbEtat + B.delta[i].start - 1;
		res.delta[k].end = A.nbEtat + B.delta[i].end - 1;
		res.delta[k].symbole = B.delta[i].symbole;
		k++;
	}
	// reunion etats :
	for (int i = 0; i < res.nbEtat; i++) {
		res.Q[i] = i;
	}
	// reunion etat accepteur
	k = 0;
	int initaccepteur = 0;
	for (int i = 0; i < A.nbaccept; i++) {
		res.accepteur[k] = A.accepteur[i];
		k++;
		if (A.accepteur[i] == A.initial)
			initaccepteur = 1;
	}
	for (int i = 0; i < B.nbaccept; i++) {
		if (B.accepteur[i] == B.initial && initaccepteur == 0)
			res.accepteur[k] = res.initial;
		else
			res.accepteur[k] = A.nbEtat + B.accepteur[i] - 1;
		k++;

	}
	return res;
}
Automate copier_automate(Automate B) {
	Automate res;
	int symbole = (int) strlen(B.symbole);
	res = initialiser_automate(res, B.nbEtat, B.nbTrans, symbole, B.nbaccept);
	res.initial = B.initial;
	for (int i = 0; i < res.nbaccept; i++) {
		res.accepteur[i] = B.accepteur[i];
	}
	for (int i = 0; i < strlen(B.symbole); i++) {
		res.symbole[i] = B.symbole[i];
	}
	for (int i = 0; i < res.nbEtat; i++) {
		res.Q[i] = B.Q[i];
	}
	for (int i = 0; i < res.nbTrans; i++) {
		res.delta[i].symbole = B.delta[i].symbole;
		res.delta[i].end = B.delta[i].end;
		res.delta[i].start = B.delta[i].start;
	}
	return res;
}
Automate fermeture_it_kleene(Automate A) {
	Automate res;
	res = copier_automate(A);
	//au maximum on aura res.nbTrans*(res.nbEtat-1) qu'on peut ajouter
	res.delta = realloc(res.delta,
			(res.nbTrans + (res.nbTrans * (res.nbEtat - 1)))
					* sizeof(transition));
	for (int i = 0; i < A.nbTrans; i++) {
		if (A.delta[i].start == A.initial) {
			for (int k = 0; k < A.nbaccept; k++) {
				if (A.accepteur[k] != A.initial) {
					res = ajouter_transition(res, A.accepteur[k],
							A.delta[i].symbole, A.delta[i].end, 0);

				}
			}
		}
	}
	// optimiser l'espace alloué en corrigeant avec la vrai taille
	res.delta = realloc(res.delta, res.nbTrans * sizeof(transition));
	return res;
}
void afficher_etat_dterministe(deterministe A) {

	printf("#### %c  ## nbetat= %d \n", A.nom, A.nbEtat);
	for (int i = 0; i < A.nbEtat; i++) {
		printf("sous etat non deterministe =%d \n", A.etat_nd[i]);
	}
}
deterministe prochain_etat_dt(Automate Automate, deterministe etat_a_determiner,
		char nom, char symbole) {
	//determine le prochain etat de etat_a_determiner lorsqu'on consomme symbole
	deterministe res;
	res.nbEtat = 0;
	res.nom = nom;
	res.etat_nd = malloc(500 * sizeof(int));
	// je parcourt toute les transitions pour trouvé une au départ de etat_a_determiner et consomme symbole
	for (int i = 0; i < etat_a_determiner.nbEtat; i++) {
		int* tableau_etat_nd = etat_a_determiner.etat_nd;
		for (int j = 0; j < Automate.nbTrans; j++) {
			transition t = Automate.delta[j];
			if (t.start == tableau_etat_nd[i] && t.symbole == symbole) {
				//verifier si l'etat a jouter n'a pas ete ajouter avant
				if (in_tableau(res.etat_nd, res.nbEtat, t.end)
						|| !res.nbEtat) {
					res.etat_nd[res.nbEtat] = t.end;
					res.nbEtat++;
				}
			}
		}
	}
	if (res.nbEtat == 0)
		res.nom = 'm';
	return res;
}
char nextsymbole(deterministe* etat, int dernier_ajoute_dt) {
	//donne la prochaine lettre de l'etat deterministe
	char c = 'A';
	for (int i = 0; i <= dernier_ajoute_dt; i++) {
		if (etat[i].nom >= c && etat[i].nom < 'a')
			c = etat[i].nom + 1;
	}
	return c;
}
int verifier_determinisme_etat(deterministe* etat, deterministe tmp,
		int *dernier_ajoute_dt) {
	//verifie si l'etat deterministe est deja existant ou non
	int existe = 0;
	for (int i = 0; i <= *dernier_ajoute_dt; i++) {
		if (etat[i].nbEtat == 0 && tmp.nbEtat == 0 && etat[i].nom == tmp.nom) {
			//printf("vide \n");
			return i;
		} else if (etat[i].nbEtat == tmp.nbEtat) {
			int meme_etat = 0;
			for (int j = 0; j < tmp.nbEtat; j++)
				for (int k = 0; k < etat[i].nbEtat; k++)
					if (etat[i].etat_nd[k] == tmp.etat_nd[j])
						meme_etat++;
			if (meme_etat == tmp.nbEtat) {
				existe = i;	//l'état existe
				return existe;
			}
		}

	}

	if (existe == 0) {
		//j'insere dans le tableau et je met a jour l'idx d'etat
		*dernier_ajoute_dt = *dernier_ajoute_dt + 1;
		etat[*dernier_ajoute_dt] = tmp;
		return *dernier_ajoute_dt;
	}
	return existe;
}
void determiniser_etat(Automate Automate, deterministe* etat_dt,

transition* delta, int *dernier_ajoute_dt, int *idxdelta,
		deterministe etat_a_determiner) {
	int new_dt = 0;
	for (int i = 0; i < strlen(Automate.symbole); i++) {

		//determiner le prochain etat de etat_dt[dernier_ajoute_dt] lorsqu'on consomme Automate.symbole[i]
		deterministe tmp = prochain_etat_dt(Automate, etat_a_determiner,
				nextsymbole(etat_dt, *dernier_ajoute_dt), Automate.symbole[i]);
		new_dt = verifier_determinisme_etat(etat_dt, tmp, dernier_ajoute_dt);
		//fin determinisation
		//ajouter transition
		delta[*idxdelta].start = etat_a_determiner.nom;
		delta[*idxdelta].end = etat_dt[new_dt].nom;
		delta[*idxdelta].symbole = Automate.symbole[i];
		*idxdelta += 1;
		// fin ajout transition
	}
}
Automate creer_automate_deterministe(Automate res, Automate a_determiner,
		transition* delta, int idxdelta, deterministe* dt, int nb_dt) {
	res = initialiser_automate(res, nb_dt, idxdelta, strlen(a_determiner.symbole),
			nb_dt);
	//Les compteurs //
	res.nbaccept = 0;
	res.nbEtat = 0;
	res.nbTrans = 0;
	//Les compteurs //

	for (int j = 0; j < nb_dt; j++) {

		// affectation des états accepteurs
		for (int i = 0; i < dt[j].nbEtat; i++) {
			if (!in_tableau(a_determiner.accepteur, a_determiner.nbaccept,
					dt[j].etat_nd[i])) {
				// detecter les doublons
				if (in_tableau(res.accepteur, res.nbaccept, dt[j].nom)
						|| !res.nbaccept) {
					res.accepteur[res.nbaccept] = (int) dt[j].nom;
					res.nbaccept++;
					printf("%c est accepteur \n", dt[j].nom);
				}
			}
		}
		//afectation des etats
		res.Q[res.nbEtat] = dt[j].nom;
		res.nbEtat++;
	}

	// ajout de l'symbole ;
	strcpy(res.symbole, a_determiner.symbole);
	for (int i = 0; i < idxdelta; i++) {
		// Ajout des transitions
		res = ajouter_transition(res, delta[i].start, delta[i].symbole,
				delta[i].end, 0);
	}

	return res;

}
int in_tableau(int* tab, int nbtab, int val) {
	/*
	 * 1 : n'existe pas
	 * 0 : existe
	 */
	int k = 0;
	if (nbtab == 0)
		return 0;
	while (k < nbtab) {
		if (tab[k] == val)
			break;
		k++;
	}
	if (k == nbtab)
		return 1;
	return 0;
}
int verifier_detrerminisme(Automate autom) {
	/*
	 * 0: non deterministe
	 * 1: deterministe
	 */
	int i = 0;
	int res = 0;
	while (i < autom.nbTrans && res >= 0) {
		int k = 0;
		res = 0;
		while (res <= 1 && k < autom.nbTrans) {
			if (autom.delta[i].start == autom.delta[k].start
					&& autom.delta[i].symbole == autom.delta[k].symbole) {
				res++;
			}
			k++;
		}
		if (res >= 2) {
			printf("l'automate est NON DETERMINISTE \n");
			return 0;
		}
		i++;
	}

	printf("l'automate est DETERMINISTE \n");
	return 1;

}
Automate determiniser(Automate autom) {

	printf("##determinisation##\n");
	Automate res;
	deterministe etat_dt[500];
	int dernier_ajoute_dt = 0;
	// transition nouveau autom deterministe
	int idxdelta = 0;
	transition delta[1000];
	//etat_dt intial
	etat_dt[0].nom = 'A';
	etat_dt[0].nbEtat = 1;
	etat_dt[0].etat_nd = malloc(
			etat_dt[dernier_ajoute_dt].nbEtat * sizeof(int));
	etat_dt[0].etat_nd[0] = autom.initial;
	int i = 0; //compteur de parcours de tout les états deterministes
	//change est une variable pour determiner si on a un nouveau etat ou pas
	while (i <= dernier_ajoute_dt || i == 0) {
		determiniser_etat(autom, etat_dt, delta, &dernier_ajoute_dt, &idxdelta,
				etat_dt[i]);
		i++;
	}
	// regler les compteurs
	dernier_ajoute_dt++;

	//affichage
	for (i = 0; i < dernier_ajoute_dt; i++) {
		afficher_etat_dterministe(etat_dt[i]);
	}
	printf("##creation de l'automate deterministe##\n");

	res = creer_automate_deterministe(res, autom, delta, idxdelta, etat_dt,
			dernier_ajoute_dt);
	return res;
}

int executer_caractere(Automate A, int start, char consomme) {
	/*
	 * 0: non executable
	 * 1: executable avec 1 transition (Attention retourne etat end+1)
	 * 2: executable avec 2 transitions (non deterministe)
	 */
	int k = 0;
	int res = 0;
	res = 0;
	while (res < 1 && k < A.nbTrans) {
		if (start == A.delta[k].start && consomme == A.delta[k].symbole) {
			res++;
		}
		k++;
	}
	if (res == 1)
		return (A.delta[k - 1].end) + 1;
	else if (res >= 2)
		return -1;
	else
		return 0;

}
int executer_mot(Automate A, char* mot) {
	int verifie = 0;
	int actuel = A.initial;
	int length = strlen(mot);
	printf("MOT A TESTER =%s\n", mot);
	if (!length) {
		printf("mot vide\n");
		if (!in_tableau(A.accepteur, A.nbaccept, A.initial)) {
			printf("automate accepte test =%d\n",
					in_tableau(A.accepteur, A.nbaccept, A.initial));
			return 1;
		} else {
			printf("l'automate n'accepte pas le mot vide \n");
			return 0;
		}
	}

	int i = 0;
	while (i < length && !verifie && actuel >= 0) {

		verifie = executer_caractere(A, actuel, mot[i]);
		if (verifie == -1) {
			printf("Erreur , automate non deterministe \n");
			break;
		}
		printf("(%d,%c)=%d, ", actuel, mot[i], verifie - 1);
		actuel = verifie - 1;
		verifie = 0;
		i++;
	}
	printf("\n");
	if (i == length && !in_tableau(A.accepteur, A.nbaccept, actuel)
			&& actuel >= 0) {
		printf("mot accepté \n");
		return 1;
	} else {
		printf("caractere %c n'est pas accepte par l'automate\n", mot[i - 1]);
	}
	return verifie;
}

