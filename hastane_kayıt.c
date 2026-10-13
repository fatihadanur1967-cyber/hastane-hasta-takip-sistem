#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Hasta {
    int id;
    char isim[50];
    char durum[20];
    struct Hasta* next;
} Hasta;

typedef struct {
    Hasta* front;
    Hasta* rear;
} Kuyruk;

typedef struct {
    Hasta* top;
} Yigin;

typedef struct {
    Hasta* head;
    Hasta* tail;
} Gecmis;


void enqueue(Kuyruk* q, int id, char* isim) {
    Hasta* yeni = (Hasta*)malloc(sizeof(Hasta));
    if (yeni == NULL) {
        printf("Bellek ayrilamadi!\n");
        return;
    }
    yeni->id = id;
    strncpy(yeni->isim, isim, sizeof(yeni->isim) - 1);
    yeni->isim[sizeof(yeni->isim) - 1] = '\0';
    strcpy(yeni->durum, "bekliyor");
    yeni->next = NULL;

    if (q->rear == NULL) {
        q->front = yeni;
        q->rear = yeni;
    } else {
        q->rear->next = yeni;
        q->rear = yeni;
    }
}

Hasta* dequeue(Kuyruk* q) {
    if (q->front == NULL) {
        printf("Bekleme kuyrugu bos, muayeneye alinacak hasta yok!\n");
        return NULL;
    }
    Hasta* hasta = q->front;
    q->front = hasta->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    hasta->next = NULL;
    return hasta;
}

void kuyrukYazdir(Kuyruk* q) {
    printf("--- Bekleyen Hastalar ---\n");
    if (q->front == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Hasta* current = q->front;
    while (current != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n",
               current->id, current->isim, current->durum);
        current = current->next;
    }
}


void push(Yigin* s, Hasta* hasta) {
    strcpy(hasta->durum, "muayenede");
    hasta->next = s->top;
    s->top = hasta;
}

Hasta* pop(Yigin* s) {
    if (s->top == NULL) {
        printf("Muayene odasi bos, tamamlanacak hasta yok!\n");
        return NULL;
    }
    Hasta* hasta = s->top;
    s->top = hasta->next;
    hasta->next = NULL;
    return hasta;
}

void yiginYazdir(Yigin* s) {
    printf("--- Muayene Odasindaki Hastalar ---\n");
    if (s->top == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Hasta* current = s->top;
    while (current != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n",
               current->id, current->isim, current->durum);
        current = current->next;
    }
}


void gecmiseEkle(Gecmis* g, Hasta* hasta) {
    strcpy(hasta->durum, "tamamlandi");
    hasta->next = NULL;

    if (g->head == NULL) {
        g->head = hasta;
        g->tail = hasta;
    } else {
        g->tail->next = hasta;
        g->tail = hasta;
    }
}

void gecmisYazdir(Gecmis* g) {
    printf("--- Tamamlanan Hastalar ---\n");
    if (g->head == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Hasta* current = g->head;
    while (current != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n",
               current->id, current->isim, current->durum);
        current = current->next;
    }
}



void durumYazdir(Kuyruk* q, Yigin* s, Gecmis* g) {
    kuyrukYazdir(q);
    yiginYazdir(s);
    gecmisYazdir(g);
}


void muayeneyeAl(Kuyruk* q, Yigin* s) {
    Hasta* hasta = dequeue(q);
    if (hasta != NULL) {
        push(s, hasta);
        printf("%s muayeneye alindi.\n", hasta->isim);
    }
}

void muayeneyiTamamla(Yigin* s, Gecmis* g) {
    Hasta* hasta = pop(s);
    if (hasta != NULL) {
        gecmiseEkle(g, hasta);
        printf("%s muayenesi tamamlandi.\n", hasta->isim);
    }
}

void listeTemizle(Hasta* head) {
    while (head != NULL) {
        Hasta* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    Kuyruk kuyruk = {NULL, NULL};
    Yigin yigin = {NULL};
    Gecmis gecmis = {NULL, NULL};

    enqueue(&kuyruk, 1, "Ahmet");
    enqueue(&kuyruk, 2, "Mehmet");
    enqueue(&kuyruk, 3, "Ayse");

    printf("=== Baslangic Durumu ===\n\n");
    durumYazdir(&kuyruk, &yigin, &gecmis);

    printf("\n=== Hasta Aktarim Islemleri ===\n");
    muayeneyeAl(&kuyruk, &yigin);
    muayeneyiTamamla(&yigin, &gecmis);

    muayeneyeAl(&kuyruk, &yigin);
    muayeneyiTamamla(&yigin, &gecmis);

    printf("\n=== Tum Islemler Sonrasi ===\n\n");
    durumYazdir(&kuyruk, &yigin, &gecmis);

    listeTemizle(kuyruk.front);
    listeTemizle(yigin.top);
    listeTemizle(gecmis.head);

    return 0;
}
