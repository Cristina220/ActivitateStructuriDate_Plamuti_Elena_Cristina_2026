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

struct Nod {
	Masina info;
	struct Nod* stg;
	struct Nod* dr;
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina == NULL) {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->info = masinaNoua;
		nodNou->dr = NULL;
		nodNou->stg = NULL;
		*radacina = nodNou;
	}
	else {
		if (masinaNoua.id < (*radacina)->info.id)
			adaugaMasinaInArbore((&(*radacina)->stg), masinaNoua);

		else if (masinaNoua.id > (*radacina)->info.id)
			adaugaMasinaInArbore((&(*radacina)->dr), masinaNoua);
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	if (f) {
		while (!feof(f)) {
			Masina masinaNoua = citireMasinaDinFisier(f);
			adaugaMasinaInArbore(&radacina, masinaNoua);
		}
		fclose(f);
	}
	return radacina;
}

void afisareInordineMasiniDinArbore(Nod* radacina) {
	//INORDINE(S - R - D)->Afiseaza sortat
	if (radacina != NULL) {
		afisareMasiniDinArbore(radacina->stg);
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->dr);
	}

}

	void afisarePreordineMasiniDinArbore(Nod* radacina) {
		//  PREORDINE (R-S-D)
		if (radacina != NULL) {
			afisareMasina(radacina->info);
			afisareMasiniDinArbore(radacina->stg);
			afisareMasiniDinArbore(radacina->dr);
		}
	}

	void afisarePostordineMasiniDinArbore(Nod* radacina) {
		// POSTORDINE (S-D-R)
		if (radacina != NULL) {
			afisareMasiniDinArbore(radacina->stg);
			afisareMasiniDinArbore(radacina->dr);
			afisareMasina(radacina->info);
		}
	}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (radacina != NULL && *radacina != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->stg));
		dezalocareArboreDeMasini(&((*radacina)->dr));

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);

		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
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

	/*if (radacina == NULL) return -1;
	else {
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}


	return 0;*/
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL)
		return 0;

	int hStg = calculeazaInaltimeArbore(radacina->stg);
	int hDr = calculeazaInaltimeArbore(radacina->dr);

	if (hStg > hDr)
		return 1 + hStg;
	else
		return 1 + hDr;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
		return 0;

	float sumaStg = calculeazaPretTotal(radacina->stg);
	float sumaDr = calculeazaPretTotal(radacina->dr);

	return radacina->info.pret + sumaStg + sumaDr;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL)
		return 0;

	float sumaStg = calculeazaPretulMasinilorUnuiSofer(radacina->stg, numeSofer);
	float sumaDr = calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);

	if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
		return radacina->info.pret + sumaStg + sumaDr;
	else
		return 0 + sumaStg + sumaDr;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInordineMasiniDinArbore(arbore);
	afisarePostordineMasiniDinArbore(arbore);
	afisarePreordineMasiniDinArbore(arbore);

	printf("\nNr de noduri din arbore masini este: %d\n", determinaNumarNoduri(arbore));
	printf("\nInaltimea arborelui de masini este: %d\n", calculeazaInaltimeArbore(arbore));

	printf("\nPretul total al masinilor: %.2f\n", calculeazaPretTotal(arbore));
	const char* sofer = "Gigel";
	printf("\nPretul total al masinilor soferului %s este: %.2f\n", sofer, calculeazaPretulMasinilorUnuiSofer(arbore, sofer));

	dezalocareArboreDeMasini(&arbore);
	return 0;
}