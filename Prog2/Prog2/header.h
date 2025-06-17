#define _CRT_SECURE_NO_WARNINGS

#ifndef HEADER_H
#define HEADER_H

#include "pitanje.c"
#include "rezultati.c"

typedef enum {
    IZBORNIK_ZAPOCNI_KVIZ = 1,
    IZBORNIK_DODAJ_PITANJE,
    IZBORNIK_PRIKAZI_PITANJA,
    IZBORNIK_IZMJENI_PITANJE,
    IZBORNIK_OBRISI_PITANJE,
    IZBORNIK_SPREMI_REZULTATE,
    IZBORNIK_IZLAZ = 0
} IzbornikOpcije;

// Deklaracije fja
inline void ocistiBuffer(void);

void prikaziPitanja(void);
void izmijeniPitanje(void);
void obrisiPitanje(void);

void ucitajPitanja(const char* imeDatoteke);
void spremiPitanja(const char* imeDatoteke);
void dodajPitanje(void);
void spremiRezultate(const char* imeDatoteke);
void pokreniKviz(void);

void oslobodiMemoriju(void);

#endif
