#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("\nID: %d", masina.id);
	printf("\nNr usi: %d", masina.nrUsi);
	printf("\nPret: %f", masina.pret);
	printf("\nModel: %s", masina.model);
	printf("\nNume sofer: %s", masina.numeSofer);
	printf("\nSerie: %c", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
		afisareMasina(masini[i]);
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	(*nrMasini)++;
	Masina* aux = malloc(sizeof(Masina) * (*nrMasini));
	for (int i = 0; i < (*nrMasini) - 1; i++) {
		aux[i] = (*masini)[i];//shallow copy
	}
	aux[(*nrMasini) - 1] = masinaNoua;
	free(*masini);
	*masini = aux;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[50];
	fgets(linie, 50, file);

	char delimitatori[3] = ",\n";
	Masina masina;
	masina.id = atoi(strtok(linie, delimitatori));
	masina.nrUsi = atoi(strtok(NULL, delimitatori));
	masina.pret = atoi(strtok(NULL, delimitatori));

	char* model = strtok(NULL, delimitatori);
	masina.model = malloc(strlen(model) * sizeof(char));
	strcpy(masina.model, model);
	//free(model); nu e nevoie pentru ca sirul de caractere e pe stiva,
	//             folosim free cand alocam noi malloc

	char* numeSofer = strtok(NULL, delimitatori);
	masina.numeSofer = malloc(strlen(numeSofer) * sizeof(char));
	strcpy(masina.numeSofer, numeSofer);

	char* serie = strtok(NULL, delimitatori);
	masina.serie = serie[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	Masina* masini = NULL;
	(*nrMasiniCitite) = 0;
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);

}

int main() {

	//FILE* file = fopen("masini.txt", "r");
	//Masina masina = citireMasinaFisier(file);
	//afisareMasina(masina);

	Masina* masini;
	int nr = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(masini, nr);

	return 0;
}