#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Heap {
	Masina* vector;
	int lungime; 
	int nrElemente; 
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) return (Masina) { 0 };

	char* aux;
	Masina m1;

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);

	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		return (Masina) { 0 };
	}
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = strtok(NULL, sep)[0];
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d, Usi: %d, Pret: %.2f, Model: %s, Sofer: %s, Serie: %c\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.nrElemente = 0;
	h.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int maxIdx = pozitieNod;
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;

	if (stanga < heap.nrElemente && heap.vector[stanga].id > heap.vector[maxIdx].id) {
		maxIdx = stanga;
	}
	if (dreapta < heap.nrElemente && heap.vector[dreapta].id > heap.vector[maxIdx].id) {
		maxIdx = dreapta;
	}

	if (maxIdx != pozitieNod) {
		Masina aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[maxIdx];
		heap.vector[maxIdx] = aux;
		filtreazaHeap(heap, maxIdx);
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return initializareHeap(0);

	char buffer[10];
	fgets(buffer, 10, f);
	int nr = atoi(buffer); 

	Heap h = initializareHeap(nr);
	for (int i = 0; i < nr; i++) {
		h.vector[i] = citireMasinaDinFisier(f);
		h.nrElemente++;
	}

	// Construirea heap-ului (Bottom-up)
	for (int i = (h.nrElemente / 2) - 1; i >= 0; i--) {
		filtreazaHeap(h, i);
	}

	fclose(f);
	return h;
}

void afisareHeap(Heap heap) {
	printf("Elemente vizibile in Heap (%d):\n", heap.nrElemente);
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	printf("Toate elementele din vector (inclusiv cele extrase/ascunse):\n");
	for (int i = 0; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	if (heap->nrElemente > 0) {
		Masina extras = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemente - 1];
		heap->vector[heap->nrElemente - 1] = extras;
		heap->nrElemente--; 
		filtreazaHeap(*heap, 0); 
		return extras;
	}
	return (Masina) { 0 };
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	 Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	 afisareHeap(h);
	 Masina m = extrageMasina(&h);
	 afisareHeap(h);
	 dezalocareHeap(&h);
	return 0;
}