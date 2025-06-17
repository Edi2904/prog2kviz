#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "quiz.h"


void obrisiDatotekuPitanja(void);

int main(void) {
    ucitajPitanja(DATOTEKA_PITANJA);

    IzbornikOpcije izbor;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("%d - Zapocni kviz\n", IZBORNIK_ZAPOCNI_KVIZ);
        printf("%d - Dodaj pitanje\n", IZBORNIK_DODAJ_PITANJE);
        printf("%d - Prikazi pitanja\n", IZBORNIK_PRIKAZI_PITANJA);
        printf("%d - Izmijeni pitanje\n", IZBORNIK_IZMJENI_PITANJE);
        printf("%d - Obrisi pitanje\n", IZBORNIK_OBRISI_PITANJE);
        printf("%d - Spremi rezultate\n", IZBORNIK_SPREMI_REZULTATE);
        printf("%d - Obrisi datoteku pitanja\n", 7);  // Nova opcija
        printf("%d - Izlaz\n", IZBORNIK_IZLAZ);
        printf("Unesi izbor: ");

        int tmp;
        if (scanf("%d", &tmp) != 1) {
            ocistiBuffer();
            printf("Neispravan unos, pokusajte ponovno.\n");
            continue;
        }
        ocistiBuffer();

        izbor = (IzbornikOpcije)tmp;

        switch (izbor) {
        case IZBORNIK_ZAPOCNI_KVIZ:
            pokreniKviz();
            break;
        case IZBORNIK_DODAJ_PITANJE:
            dodajPitanje();
            break;
        case IZBORNIK_PRIKAZI_PITANJA:
            prikaziPitanja();
            break;
        case IZBORNIK_IZMJENI_PITANJE:
            izmijeniPitanje();
            break;
        case IZBORNIK_OBRISI_PITANJE:
            obrisiPitanje();
            break;
        case IZBORNIK_SPREMI_REZULTATE:
            spremiRezultate(DATOTEKA_REZULTATA);
            break;
        case 7:
            obrisiDatotekuPitanja();
            oslobodiMemoriju();
            brojPitanja = 0;
            printf("Datoteka pitanja je obrisana i memorija osvjezena.\n");
            break;
        case IZBORNIK_IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor, pokusajte ponovno.\n");
            break;
        }
    } while (izbor != IZBORNIK_IZLAZ);

    spremiPitanja(DATOTEKA_PITANJA);

    oslobodiMemoriju();

    return 0;
}
