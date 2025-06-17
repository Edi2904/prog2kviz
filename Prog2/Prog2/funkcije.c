#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "quiz.h"

// Globalne var.
Pitanje* pitanja = NULL;
int brojPitanja = 0;

inline void ocistiBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


static void ukloniNoviRed(char* tekst) {
    if (!tekst) return;
    size_t len = strlen(tekst);
    if (len > 0 && tekst[len - 1] == '\n') {
        tekst[len - 1] = '\0';
    }
}


static int usporediPitanja(const void* a, const void* b) {
    const Pitanje* p1 = (const Pitanje*)a;
    const Pitanje* p2 = (const Pitanje*)b;
    return p1->id - p2->id;
}


static int usporediPitanjaPoID(const void* key, const void* elem) {
    const int* id = (const int*)key;
    const Pitanje* p = (const Pitanje*)elem;
    return *id - p->id;
}

void ucitajPitanja(const char* imeDatoteke) {
    if (!imeDatoteke) {
        fprintf(stderr, "Ime datoteke nije definirano!\n");
        return;
    }
    FILE* f = fopen(imeDatoteke, "r");
    if (!f) {
        perror("Greska pri otvaranju datoteke za ucitavanje pitanja");
        return;
    }

    if (pitanja) free(pitanja);
    pitanja = malloc(sizeof(Pitanje) * MAX_PITANJA);
    if (!pitanja) {
        perror("Greska pri alokaciji memorije za pitanja");
        fclose(f);
        return;
    }
    brojPitanja = 0;

    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);

    while (brojPitanja < MAX_PITANJA && ftell(f) < filesize) {
        if (fscanf(f, "%d\n", &pitanja[brojPitanja].id) == EOF) break;
        if (!fgets(pitanja[brojPitanja].pitanje, MAX_TEKST, f)) break;

        for (int i = 0; i < BROJ_OPCIJA; i++) {
            if (!fgets(pitanja[brojPitanja].opcije[i], MAX_TEKST, f)) break;
        }

        if (fscanf(f, "%d\n", &pitanja[brojPitanja].tocnaOpcija) == EOF) break;

        ukloniNoviRed(pitanja[brojPitanja].pitanje);
        for (int i = 0; i < BROJ_OPCIJA; i++) {
            ukloniNoviRed(pitanja[brojPitanja].opcije[i]);
        }

        brojPitanja++;
    }

    fclose(f);
    qsort(pitanja, brojPitanja, sizeof(Pitanje), usporediPitanja);
}

void spremiPitanja(const char* imeDatoteke) {
    if (!imeDatoteke) {
        fprintf(stderr, "Ime datoteke nije definirano!\n");
        return;
    }
    FILE* f = fopen(imeDatoteke, "w");
    if (!f) {
        perror("Greska pri otvaranju datoteke za spremanje pitanja");
        return;
    }

    for (int i = 0; i < brojPitanja; i++) {
        fprintf(f, "%d\n%s\n", pitanja[i].id, pitanja[i].pitanje);
        for (int j = 0; j < BROJ_OPCIJA; j++) {
            fprintf(f, "%s\n", pitanja[i].opcije[j]);
        }
        fprintf(f, "%d\n", pitanja[i].tocnaOpcija);
    }
    fclose(f);
}

void dodajPitanje(void) {
    if (!pitanja) {
        pitanja = malloc(sizeof(Pitanje) * MAX_PITANJA);
        if (!pitanja) {
            perror("Greska pri alokaciji memorije za pitanja");
            return;
        }
    }

    if (brojPitanja >= MAX_PITANJA) {
        printf("Dosegnut maksimalan broj pitanja.\n");
        return;
    }

    Pitanje* p = &pitanja[brojPitanja];
    p->id = brojPitanja + 1;

    printf("Unesi pitanje: ");
    if (!fgets(p->pitanje, MAX_TEKST, stdin)) return;
    ukloniNoviRed(p->pitanje);

    for (int i = 0; i < BROJ_OPCIJA; i++) {
        printf("Opcija %d: ", i + 1);
        if (!fgets(p->opcije[i], MAX_TEKST, stdin)) return;
        ukloniNoviRed(p->opcije[i]);
    }

    do {
        printf("Tocan broj opcije (1-4): ");
        if (scanf("%d", &p->tocnaOpcija) != 1) {
            ocistiBuffer();
            continue;
        }
        ocistiBuffer();
    } while (p->tocnaOpcija < 1 || p->tocnaOpcija > BROJ_OPCIJA);

    brojPitanja++;
    qsort(pitanja, brojPitanja, sizeof(Pitanje), usporediPitanja);
}

void spremiRezultate(const char* imeDatoteke) {
    if (!imeDatoteke) {
        fprintf(stderr, "Ime datoteke nije definirano!\n");
        return;
    }
    FILE* f = fopen(imeDatoteke, "a");
    if (!f) {
        perror("Greska pri otvaranju datoteke za spremanje rezultata");
        return;
    }

    Rezultat r;
    printf("Unesi ime igraca: ");
    if (!fgets(r.imeIgraca, MAX_IME_IGRACA, stdin)) {
        fclose(f);
        return;
    }
    ukloniNoviRed(r.imeIgraca);

    printf("Unesi broj osvojenih bodova: ");
    scanf("%d", &r.bodovi);
    ocistiBuffer();

    fprintf(f, "%s %d\n", r.imeIgraca, r.bodovi);
    fclose(f);
}

// rf
static void prikaziPitanjaRekurzivno(int index) {
    if (index >= brojPitanja) return;

    printf("%d: %s\n", pitanja[index].id, pitanja[index].pitanje);
    for (int j = 0; j < BROJ_OPCIJA; j++) {
        printf("   %c) %s\n", 'A' + j, pitanja[index].opcije[j]);
    }
    printf("Tocna opcija: %d\n\n", pitanja[index].tocnaOpcija);

    prikaziPitanjaRekurzivno(index + 1);
}

void prikaziPitanja(void) {
    if (brojPitanja == 0) {
        printf("Nema pitanja.\n");
        return;
    }
    prikaziPitanjaRekurzivno(0);
}

void izmijeniPitanje(void) {
    if (brojPitanja == 0) {
        printf("Nema pitanja za izmjenu.\n");
        return;
    }

    int id;
    printf("Unesi ID pitanja za izmjenu: ");
    if (scanf("%d", &id) != 1) {
        ocistiBuffer();
        printf("Neispravan unos.\n");
        return;
    }
    ocistiBuffer();

    Pitanje* p = bsearch(&id, pitanja, brojPitanja, sizeof(Pitanje), usporediPitanjaPoID);
    if (!p) {
        printf("Pitanje s ID %d ne postoji.\n", id);
        return;
    }

    printf("Unesi novo pitanje: ");
    if (!fgets(p->pitanje, MAX_TEKST, stdin)) return;
    ukloniNoviRed(p->pitanje);

    for (int i = 0; i < BROJ_OPCIJA; i++) {
        printf("Opcija %d: ", i + 1);
        if (!fgets(p->opcije[i], MAX_TEKST, stdin)) return;
        ukloniNoviRed(p->opcije[i]);
    }

    do {
        printf("Tocan broj opcije (1-4): ");
        if (scanf("%d", &p->tocnaOpcija) != 1) {
            ocistiBuffer();
            continue;
        }
        ocistiBuffer();
    } while (p->tocnaOpcija < 1 || p->tocnaOpcija > BROJ_OPCIJA);
}

void obrisiPitanje(void) {
    if (brojPitanja == 0) {
        printf("Nema pitanja za brisanje.\n");
        return;
    }

    int id;
    printf("Unesi ID pitanja za brisanje: ");
    if (scanf("%d", &id) != 1) {
        ocistiBuffer();
        printf("Neispravan unos.\n");
        return;
    }
    ocistiBuffer();

    int index = -1;
    for (int i = 0; i < brojPitanja; i++) {
        if (pitanja[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Pitanje s ID %d ne postoji.\n", id);
        return;
    }

    for (int i = index; i < brojPitanja - 1; i++) {
        pitanja[i] = pitanja[i + 1];
    }
    brojPitanja--;
}

void pokreniKviz(void) {
    if (brojPitanja == 0) {
        printf("Nema pitanja za kviz.\n");
        return;
    }

    int score = 0;
    for (int i = 0; i < brojPitanja; i++) {
        printf("%d: %s\n", pitanja[i].id, pitanja[i].pitanje);
        for (int j = 0; j < BROJ_OPCIJA; j++) {
            printf("   %c) %s\n", 'A' + j, pitanja[i].opcije[j]);
        }

        int odgovor;
        do {
            printf("Unesi broj tocne opcije (1-4): ");
            if (scanf("%d", &odgovor) != 1) {
                ocistiBuffer();
                continue;
            }
            ocistiBuffer();
        } while (odgovor < 1 || odgovor > BROJ_OPCIJA);

        if (odgovor == pitanja[i].tocnaOpcija) {
            score++;
        }
    }

    printf("Osvojili ste %d od %d bodova.\n", score, brojPitanja);
    spremiRezultate(DATOTEKA_REZULTATA);
}

void oslobodiMemoriju(void) {
    if (pitanja) {
        free(pitanja);
        pitanja = NULL;
    }
}

void obrisiDatotekuPitanja(void) {
    if (remove(DATOTEKA_PITANJA) == 0) {
        printf("Datoteka '%s' uspjesno obrisana.\n", DATOTEKA_PITANJA);
    }
    else {
        perror("Neuspjesno brisanje datoteke");
    }
}