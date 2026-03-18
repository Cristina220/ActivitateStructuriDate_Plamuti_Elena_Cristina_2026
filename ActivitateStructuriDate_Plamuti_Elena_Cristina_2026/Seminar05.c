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

typedef struct Nod Nod;
struct Nod{
	Masina info;
	Nod* next;
	Nod* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

//creare structura pentru un nod dintr-o lista dublu inlantuita

//creare structura pentru Lista Dubla 

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

void afisareListaMasini(ListaDubla lista) {
	
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

//Nod* p = lista.prim;
//while (p) {
//	afisareMasina(p->info);
//	p = p->prev;
//}

void adaugaMasinaInLista(ListaDubla* ld, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim) {
		ld->ultim->next = nou;
	}
	else {
		ld->prim = nou;
	}
	ld->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* ld,  Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = ld->prim;
	nou->prev = NULL;
	if (ld->prim) {
		ld->prim->prev = nou;
	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;

}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}
}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.prim == NULL) return 0.0f; 

	float suma = 0;
	int count = 0;
	Nod* temp = lista.prim;

	while (temp != NULL) {
		suma += temp->info.pret;
		count++;
		temp = temp->next;
	}

	return (count > 0) ? (suma / count) : 0.0f;
}
void stergeMasinaDupaID(ListaDubla* ld, int id) {
	/*Nod* temp = ld->prim;

	while (temp != NULL) {
		if (temp->info.id == id) {
			if (temp->prev != NULL) {
				temp->prev->next = temp->next;
			}
			else {
				ld->prim = temp->next;
			}

			if (temp->next != NULL) {
				temp->next->prev = temp->prev;
			}
			else {
				ld->ultim = temp->prev;
			}

			free(temp->info.model);
			free(temp->info.numeSofer);

			free(temp);
			return;
		}
		temp = temp->next;
	}*/

	if (ld->prim && ld->prim->info.id == id) {
		free(ld->prim->info.model);
		free(ld->prim->info.numeSofer);
		ld->prim = ld->prim->next;
			if (ld->prim) {
				free(ld->prim->prev);
			}
			else{
				free(ld->ultim);
				ld->ultim = NULL;
			}
			return;
	}
	Nod* p = ld->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
		}
		if (p->next) {
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			ld->ultim = p->prev;
		}
		free(ld->prim->info.model);
		free(ld->prim->info.numeSofer);
		free(p);
	}
}


	char* getNumeSoferMasinaScumpa(ListaDubla lista) {
		if (lista.prim == NULL) {
			return NULL;
		}

		Nod* p = lista.prim;
		float pretMaxim = p->info.pret;
		char* numeSoferScump = p->info.numeSofer;

		while (p != NULL) {
			if (p->info.pret > pretMaxim) {
				pretMaxim = p->info.pret;
				numeSoferScump = p->info.numeSofer;
			}
			p = p->next;
		}

		return numeSoferScump;
	}
	


int main() {

	ListaDubla lista=citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	//printf("Pretul mediu: %.2f",calculeazaPretMediu(lista));

	Masina mNoua;
	mNoua.id = 100;
	mNoua.nrUsi = 4;
	mNoua.pret = 15000.75f;
	mNoua.serie = 'X';

	char tempModel[] = "Dacia Logan";
	mNoua.model = (char*)malloc(strlen(tempModel) + 1);
	if (mNoua.model) {
		strcpy(mNoua.model, tempModel);
	}

	char tempSofer[] = "Popescu Ion";
	mNoua.numeSofer = (char*)malloc(strlen(tempSofer) + 1);
	if (mNoua.numeSofer) {
		strcpy(mNoua.numeSofer, tempSofer);
	}

	/*adaugaLaInceputInLista(&lista, mNoua);
	printf("\nDupa adaugare:\n");
	afisareListaMasini(lista);

	stergeMasinaDupaID(&lista, 100);
	printf("\nDupa stergere:\n");
	afisareListaMasini(lista);*/

	printf("\nNumele soferului cu masina cea mai scumpa: %s",getNumeSoferMasinaScumpa(lista));
	return 0;
}