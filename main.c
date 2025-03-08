#include <stdio.h>
#include <stdlib.h>

#define MAX_PATIENTS 100

// Hasta yapisi
typedef struct {
    char name[50];
    char surname[50];
    int age;
    char gender[10];
    char id[20];
    int priority; // 1: Yüksek, 2: Orta, 3: Düþük
    char prescriptionNumber[20]; // Reçete numarasý
} Patient;

Patient patients[MAX_PATIENTS]; // Hastalarý tutan dizi
int patientCount = 0; // Kayýtlý hasta sayýsý

// Hasta ekleme fonksiyonu
void addPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Hasta kapasitesi doldu! Daha fazla hasta eklenemez.\n");
        return;
    }

    Patient newPatient;

    printf("\n--- Yeni Hasta Ekle ---\n");
    printf("Ad: ");
    scanf("%s", newPatient.name);
    printf("Soyad: ");
    scanf("%s", newPatient.surname);
    printf("Yas: ");
    scanf("%d", &newPatient.age);
    printf("Cinsiyet: ");
    scanf("%s", newPatient.gender);
    printf("Kimlik No: ");
    scanf("%s", newPatient.id);
    printf("Oncelik Seviyesi (1: Yuksek, 2: Orta, 3: Dusuk): ");
    scanf("%d", &newPatient.priority);

    // Reçete numarasi uretme (RX001, RX002, ...)
    sprintf(newPatient.prescriptionNumber, "RX%03d", patientCount + 1);

    // Hastayi diziye ekle
    patients[patientCount++] = newPatient;
    printf("Hasta basariyla eklendi! Recete No: %s\n", newPatient.prescriptionNumber);
}

// Hastalari listeleme fonksiyonu
void listPatients() {
    if (patientCount == 0) {
        printf("Kayitli hasta bulunmamaktadýr.\n");
        return;
    }

    printf("\n--- Kayitli Hastalar ---\n");
    int i;
    for (i = 0; i < patientCount; i++) {
        printf("Hasta %d:\n", i + 1);
        printf("  Ad: %s\n", patients[i].name);
        printf("  Soyad: %s\n", patients[i].surname);
        printf("  Yas: %d\n", patients[i].age);
        printf("  Cinsiyet: %s\n", patients[i].gender);
        printf("  Kimlik No: %s\n", patients[i].id);
        printf("  Oncelik Seviyesi: %d\n", patients[i].priority);
        printf("  Recete No: %s\n", patients[i].prescriptionNumber);
    }
}

// Hastalari oncelik sirasina gore siralama fonksiyonu
void prioritizePatients() {
    if (patientCount == 0) {
        printf("Siralanacak hasta bulunmamaktadýr.\n");
        return;
    }

    // Basit bir siralama algoritmasi (kabarcik siralama)
    int i,j;
    for ( i = 0; i < patientCount - 1; i++) {
        for (j = 0; j < patientCount - i - 1; j++) {
            if (patients[j].priority > patients[j + 1].priority) {
                // Hastalarý yer deðiþtir
                Patient temp = patients[j];
                patients[j] = patients[j + 1];
                patients[j + 1] = temp;
            }
        }
    }

    printf("Hastalar oncelik sirasina gore siralandi!\n");
    listPatients();
}

// HL7 formatinda rapor olusturma fonksiyonu
void generateHL7Report() {
    if (patientCount == 0) {
        printf("Rapor oluþturmak için hasta bulunmamaktadýr.\n");
        return;
    }

    // HL7 raporunu ekrana yazdýr
    int i;
    printf("\n--- HL7 Raporu (Ekran Ciktisi) ---\n");
    for ( i = 0; i < patientCount; i++) {
        printf("MSH|^~\\&|HOSPITAL|ACIL|HL7|20231020||ADT^A01|%d|P|2.5.1\n", i + 1);
        printf("PID|1|%s||%s^%s||%d|%s\n", patients[i].id, patients[i].name, patients[i].surname, patients[i].age, patients[i].gender);
        printf("PV1|1|%d|||ACIL||||||||||||||||||||||||||||||||||||||||%s\n", patients[i].priority, patients[i].prescriptionNumber);
        printf("\n"); // Her hasta arasýnda boþluk býrak
    }

    // HL7 raporunu dosyaya yazdýr
    FILE *file = fopen("hasta_raporu.hl7", "w");
    if (file == NULL) {
        printf("HL7 raporu olusturulamadi! Dosya acilamadi.\n");
        return;
    }

    for ( i = 0; i < patientCount; i++) {
        fprintf(file, "MSH|^~\\&|HOSPITAL|ACIL|HL7|20231020||ADT^A01|%d|P|2.5.1\n", i + 1);
        fprintf(file, "PID|1|%s||%s^%s||%d|%s\n", patients[i].id, patients[i].name, patients[i].surname, patients[i].age, patients[i].gender);
        fprintf(file, "PV1|1|%d|||ACIL||||||||||||||||||||||||||||||||||||||||%s\n", patients[i].priority, patients[i].prescriptionNumber);
    }

    fclose(file);
    printf("HL7 raporu basariyla olusturuldu! (hasta_raporu.hl7)\n");
}

// Ana menu fonksiyonu
int main() {
    int choice;

    do {
        printf("\n--- Acil Servis Sistemi ---\n");
        printf("1. Hasta Ekle\n");
        printf("2. Hastalari Listele\n");
        printf("3. Hastalari Oncelik Sirasina Gore Sirala\n");
        printf("4. HL7 Raporu Olustur\n");
        printf("5. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                listPatients();
                break;
            case 3:
                prioritizePatients();
                break;
            case 4:
                generateHL7Report();
                break;
            case 5:
                printf("Cikis yapiliyor...\n");
                break;
            default:
                printf("Gecersiz secim! Lutfen 1-5 arasinda bir sayi girin.\n");
        }
    } while (choice != 5);

    return 0;
}
