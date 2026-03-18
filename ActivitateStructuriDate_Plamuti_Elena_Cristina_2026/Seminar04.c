//
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//
//struct StructuraMasina {
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
////creare structura pentru un nod dintr-o lista simplu inlantuita
//typedef struct Nod Nod;
//
//struct Nod {
//	Masina info;
//	Nod* next;
//};
//
//Masina citireMasinaDinFisier(FILE* file) {
//	char buffer[100];
//	char sep[3] = ",\n";
//	fgets(buffer, 100, file);
//	char* aux;
//	Masina m1;
//	aux = strtok(buffer, sep);
//	m1.id = atoi(aux);
//	m1.nrUsi = atoi(strtok(NULL, sep));
//	m1.pret= atof(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	m1.model = malloc(strlen(aux) + 1);
//	strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//	aux = strtok(NULL, sep);
//	m1.numeSofer = malloc(strlen(aux) + 1);
//	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//	m1.serie = *strtok(NULL, sep);
//	return m1;
//}
//
//void afisareMasina(Masina masina) {
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
//	Nod* nou = malloc(sizeof(Nod));
//	nou->info = masinaNoua;//shallow copy
//	nou->next = NULL;
//	Nod* aux = *lista;
//	if ((*lista) != NULL) {
//		while (aux->next != NULL) {
//			aux = aux->next;
//		}
//		aux->next = nou;
//	}
//	else
//		*lista = nou;
//}
//
//void afisareListaMasini(Nod* cap) {
//	//afiseaza toate elemente de tip masina din lista simplu inlantuita
//	//prin apelarea functiei afisareMasina()
//	while (cap != NULL) {
//		afisareMasina(cap->info);
//		cap = cap->next;
//	}
//}
//
//void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
//	//adauga la inceputul listei o noua masina pe care o primim ca parametru
//	Nod* nou = malloc(sizeof(Nod));
//	if (nou != NULL) {
//		nou->info = masinaNoua;
//		nou->next = *lista;
//		*lista = nou;
//	}
//}
//
//void* citireListaMasiniDinFisier(const char* numeFisier) {
//	
//	FILE* file = fopen(numeFisier, "r");
//	Nod* lista = NULL;
//	while (!feof(file)) {
//		adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
//	}
//	fclose(file);
//	return lista;
//}
//
//void dezalocareListaMasini(Nod** lista) {
//	//sunt dezalocate toate masinile si lista de elemente
//	Nod* head=*lista;
//	Nod* next = NULL;
//	while (head != NULL) {
//		next = head->next;
//		free(head->info.model);
//		free(head->info.numeSofer);
//		free(head);
//		head = next;
//	}
//	*lista = NULL;
//}
//
//float calculeazaPretMediu(Nod* lista) {
//	float suma = 0;
//	int nrMasini = 0;
//	while (lista != NULL) {
//		suma += lista->info.pret;
//		nrMasini++;
//		lista = lista->next;
//	}
//
//	if (nrMasini != NULL)
//		return suma / nrMasini;
//	else
//		return 0;
//}
//
//void stergeMasiniDinSeria(Nod**lista, char serieCautata) {
//	
//	while (*lista!=NULL && (*lista)->info.serie == serieCautata) {
//		Nod* gasit = *lista;
//		*lista = (*lista)->next;
//
//		free(gasit->info.model);
//		free(gasit->info.numeSofer);
//		free(gasit);
//	}
//
//	if (*lista == NULL)
//		return;
//	else {
//		Nod* ramas = *lista;
//		while (ramas != NULL && ramas->next != NULL) {
//			if (ramas->next->info.serie == serieCautata) {
//				Nod* gasit = ramas->next;
//
//				ramas->next = gasit->next;
//
//				free(gasit->info.model);
//				free(gasit->info.numeSofer);
//				free(gasit);
//			}
//			else {
//				ramas = ramas->next;
//			}
//		}
//	}
//}
//
//float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
//	float sumaTotala = 0;
//
//	while (lista != NULL) {
//
//		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
//
//			sumaTotala += lista->info.pret;
//		}
//
//		lista = lista->next;
//	}
//
//	return sumaTotala;
//
//	return 0;
//}
//
//int main() {
//	Nod* lista = citireListaMasiniDinFisier("masini.txt");
//	printf("--- Lista initiala din fisier ---\n");
//	afisareListaMasini(lista);
//
//	Masina masinaNoua;
//	masinaNoua.id = 999;
//	masinaNoua.nrUsi = 2;
//	masinaNoua.pret = 50000.0;
//	masinaNoua.serie = 'Z';
//
//	const char* textModel = "Porsche";
//	masinaNoua.model = malloc(strlen(textModel) + 1);
//	strcpy_s(masinaNoua.model, strlen(textModel) + 1, textModel);
//
//	const char* textSofer = "Mihai";
//	masinaNoua.numeSofer = malloc(strlen(textSofer) + 1);
//	strcpy_s(masinaNoua.numeSofer, strlen(textSofer) + 1, textSofer);
//
//	adaugaLaInceputInLista(&lista, masinaNoua);
//
//	printf("\n--- Lista DUPA adaugarea la inceput ---\n");
//	afisareListaMasini(lista);
//	
//	float pretMediu=calculeazaPretMediu(lista);
//	printf("\nPretul mediu este: %f", pretMediu);
//
//	const char* soferCautat = "Gigel";
//	float pretSofer = calculeazaPretulMasinilorUnuiSofer(lista, soferCautat);
//	printf("\nPret total pentru soferul %s este %.2f\n", soferCautat, pretSofer);
//
//	char serieDeSters = 'A';
//	printf("\nStergem masinile cu seria %c", serieDeSters);
//	stergeMasiniDinSeria(&lista, serieDeSters);
//	printf("\n---Lista dupa stergere---\n");
//	afisareListaMasini(lista);
//
//	dezalocareListaMasini(&lista);
//	printf("\nDupa dezalocare:\n");
//	afisareListaMasini(lista);
//	return 0;
//}
//
////lista- tablou unidim cu acelasi tip de date care nu ocupa o zona de memorie contigua si fiecare nod il retine pe urm 
////vector- tablou unidim cu acelasi tip de date care ocupa o zona de memorie contigua cu acces direct
////void pointer- pointer la orice (malloc e asta); stramosul lui template; merge pt orice functie care returneaza un pointer