#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include<malloc.h>
#include <math.h>
#include<stdlib.h>

/*Structura creata contine coordonatele unui punct*/
typedef struct {
	int x, y;
}punct;

/*Functia converteste un sir de caractere intr-un intreg(implementare atoi)*/
int stringToInt(char str[100])
{
	int i, result = 0;
	for (i = 0; i < strlen(str); i++)
	{
		result = result * 10 + ((int)str[i] - 48);
	}
	return result;
}

/*Se deschide fisierul si configuratia initiala*/
void citireConfiguratie(char input[100], int*** configuratie, int* numarLinii, int* numarColoane)
{
	FILE* fin = fopen(input, "r");
	if (fin == NULL)
	{
		printf("Nu s-a putut deschide fisierul %s", input);
		return;
	}
	fscanf(fin, "%d %d", numarLinii, numarColoane);
	*configuratie = (int**)malloc((*numarLinii) * sizeof(int*));
	int i, j;
	for (i = 0; i < (*numarLinii); i++)
		(*configuratie)[i] = (int*)malloc((*numarColoane) * sizeof(int));
	for (i = 0; i < (*numarLinii); i++)
		for (j = 0; j < (*numarColoane); j++)
			fscanf(fin, "%d", &(*configuratie)[i][j]);
}

/*Se calculeaza numarul de vecini incluzand cazurile particulare(mariginile,colturile)*/
int numarVecini(int** configuratie, int x, int y, int numarLinii, int numarColoane)
{
	int directieLinie[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int directieColoana[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int i, nrVecini = 0, pozitieX, pozitieY;
	for (i = 0; i < 8; i++) {
		pozitieX = (x + directieLinie[i]);
		pozitieY = (y + directieColoana[i]);
		if (pozitieX < 0)
			pozitieX = numarLinii - 1;
		if (pozitieY < 0)
			pozitieY = numarColoane - 1;
		if (pozitieX > numarLinii - 1)
			pozitieX = 0;
		if (pozitieY > numarColoane - 1)
			pozitieY = 0;
		if ((pozitieX < 0) || (pozitieX >= numarLinii) || (pozitieY < 0) || (pozitieY >= numarColoane))
			continue;
		if (configuratie[pozitieX][pozitieY] == 1)
			nrVecini++;
	}
	return nrVecini;
}

/*Se modifica cofiguratia initiala si se pregateste configuratia urmatoare*/
void generatiaUrmatoare(int*** configuratie, int numarLinii, int numarColoane)
{
	int i, j, nrVecini, nrElementeSchimbate = 0;
	punct* modificari =(punct*) malloc(sizeof(punct));
	for (i = 0; i < numarLinii; i++)
		for (j = 0; j < numarColoane; j++) {
			nrVecini = numarVecini((*configuratie), i, j, numarLinii, numarColoane);
			if ((*configuratie)[i][j] == 1) {
				if (nrVecini <= 1 || nrVecini >= 4) {
					nrElementeSchimbate++;
					modificari =(punct*) realloc(modificari, (nrElementeSchimbate + 1) * sizeof(punct));
					modificari[nrElementeSchimbate - 1].x = i;
					modificari[nrElementeSchimbate - 1].y = j;
				}
			}
			else {
				if (nrVecini == 3) {
					nrElementeSchimbate++;
					modificari =(punct*) realloc(modificari, (nrElementeSchimbate + 1) * sizeof(punct));
					modificari[nrElementeSchimbate - 1].x = i;
					modificari[nrElementeSchimbate - 1].y = j;
				}
			}
		}
	for (i = 0; i < nrElementeSchimbate; i++) {
		(*configuratie)[modificari[i].x][modificari[i].y] = abs((*configuratie)[modificari[i].x][modificari[i].y] - 1);
	}
	free(modificari);
}

/*Se construieste fisierul de iesire in care se afiseaza continutul tabloului configuratie*/
void creareConfiguratieFinala(char output[100], int** configuratie, int numarLinii, int numarColoane)
{
	int i, j;
	FILE* fout = fopen(output, "w");
	if (fout == NULL)
		printf("Fisierul output nu se poate deschide");
	for (i = 0; i < numarLinii; i++) {
		for (j = 0; j < numarColoane; j++)
			fprintf(fout, "%d ", configuratie[i][j]);
		fprintf(fout, "\n");
	}
	fclose(fout);
}

int main(int argc, char **argv) {
	int i, numarPasi, superDinamic = 0, comprimat = 0;
	char numeFisierIn[100], numeFisierOut[100];
	/*Se parcurge vectorul de argumente si se verifica fiecare argument*/
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-in") == 0)
			strcpy(numeFisierIn, argv[i + 1]);/*daca argv[i]=="-in" va rezulta ca argv[i+1] va fi numele fisierului de intrare*/
		if (strcmp(argv[i], "-out") == 0)
			strcpy(numeFisierOut, argv[i + 1]);/*daca argv[i]=="-out" va rezulta ca argv[i+1] va fi numele fisierului de intrare*/
		if (strcmp(argv[i], "-steps") == 0)
			numarPasi = stringToInt(argv[i + 1]);/*daca argv[i]=="-steps" va rezulta ca argv[i+1] va fi numele fisierului de intrare*/
		if (strcmp(argv[i], "-super_dinamic"))
			superDinamic = 1;/*daca argv[i]="-super_dinamic" se respecta proprietatea*/
		if (strcmp(argv[i], "-comprimat"))
			comprimat = 1;/*daca argv[i]="-comprimat" se respecta proprietatea*/
	}
	int numarLinii, numarColoane;
	strcpy(numeFisierIn, "file_in.txt");
	FILE *fin = fopen(numeFisierIn, "r");
	FILE *fout = fopen(numeFisierOut, "w");
	//fclose(fout);
	if (fin == NULL) {
		fprintf(fout, "Fisierul de intrare nu s-a putut deschide");
		return 0;
	}
	int** configuratie;
	citireConfiguratie(numeFisierIn, &configuratie, &numarLinii, &numarColoane);
	for (i = 0; i < numarPasi; i++)
		generatiaUrmatoare(&configuratie, numarLinii, numarColoane);
	creareConfiguratieFinala(numeFisierOut, configuratie, numarLinii, numarColoane);
	return 0;
}