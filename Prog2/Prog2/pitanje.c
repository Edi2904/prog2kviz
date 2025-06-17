#define _CRT_SECURE_NO_WARNINGS

#ifndef PITANJE_C
#define PITANJE_C

#define MAX_PITANJA 100
#define MAX_TEKST 250
#define BROJ_OPCIJA 4

typedef struct {
    int id;
    char pitanje[MAX_TEKST];
    char opcije[BROJ_OPCIJA][MAX_TEKST];
    int tocnaOpcija;
} Pitanje;

// globalne var.
extern Pitanje* pitanja;
extern int brojPitanja;

#endif