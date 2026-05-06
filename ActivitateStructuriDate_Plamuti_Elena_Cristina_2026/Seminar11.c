#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* dr;
	struct Nod* stg;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* root) {
	if (root) {
		return fmax(calculeazaInaltimeArbore(root->stg), calculeazaInaltimeArbore(root->dr))+1;
	}
	else
	return 0;
}

char calculGE(Nod* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->stg) - calculeazaInaltimeArbore(root->dr);
	}
	else
		return 0;
}

void rotireStg(Nod** root) {
	Nod* aux = (*root)->dr;
	(*root)->dr = aux->stg;
	aux->stg = *root;
	(*root) = aux;
}

void rotireDr(Nod** root) {
	Nod* aux = (*root)->stg;
	(*root)->stg = aux->dr;
	aux->dr = (*root);
	(*root) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** root,  Masina masinaNoua) {
	if (*root) {
		if (masinaNoua.id < (*root)->info.id) {
			adaugaMasinaInArboreEchilibrat(&((*root)->stg), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*root)->dr), masinaNoua);
		}
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->stg = NULL;
		nou->dr = NULL;
		(*root) = nou;
	}
		int grad = calculGE((*root));
		if (grad == 2) {//dezechilibrat in partea stg
			if (calculGE((*root)->stg) == -1) {
				rotireStg(&(*root)->stg);
				rotireDr(&(*root));//se anuleaza si nu mai trebuie &(*root) merge doar root
			}
			else {
				rotireDr(&(*root));
			}
		}
		else if (grad == -2) {//dezechilibrat in partea dr
			if (calculGE((*root)->dr) == 1) {
				rotireDr(&(*root)->dr);
			}
			rotireStg(root);
		}
	
	
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* root=NULL;
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
	fclose(f);
	return root;
}

void afisareMasiniDinArbore(Nod* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->stg);
		afisareMasiniDinArbore(root->dr);
	}
}

void dezalocareArboreDeMasini(Nod** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->stg);
		dezalocareArboreDeMasini(&(*root)->dr);

		free((*root)->info.model);
		free((*root)->info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(Nod* radacina,int id) {
	Masina m;
		m.id = -1;
	
		if (radacina != NULL) {
			if (radacina->info.id == id)
				m = radacina->info;
			else {
				if (id < radacina->info.id)
					m = getMasinaByID(radacina->stg, id);
				else
					m = getMasinaByID(radacina->dr, id);
			}
		}
	
		return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL)
				return 0;
		
			int nrStg = determinaNumarNoduri(radacina->stg);
			int nrDr = determinaNumarNoduri(radacina->dr);
		
			return 1 + nrStg + nrDr;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
				return 0;
		
			float sumaStg = calculeazaPretTotal(radacina->stg);
			float sumaDr = calculeazaPretTotal(radacina->dr);
		
			return radacina->info.pret + sumaStg + sumaDr;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* root, const char* numeSofer) {
	float pret = 0;
	if (root) {
		pret = calculeazaPretulMasinilorUnuiSofer(root->stg, numeSofer);
		pret += calculeazaPretulMasinilorUnuiSofer(root->dr, numeSofer);
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			pret += root->info.pret;
		}
	}
	return pret;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);

	printf("\nPretul masinilor lui Ionescu este: %.2f\n",calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));
	dezalocareArboreDeMasini(&arbore);

	return 0;
}