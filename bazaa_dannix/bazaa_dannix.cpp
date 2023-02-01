#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person { // tworzenie struktury osoby
    char surname[100];
    char name[100];
    int age;
    char pesel[12];
    char gender;
    struct Person* next;
    struct Person* prev;
} Person;

Person* head = NULL; // stworzenie wskaznika glowy
Person* tail = NULL; // stworzenie wskaznika ogon

void addPerson(char* surname, char* name, int age, char* pesel, char gender) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson != NULL) {
        strcpy_s(newPerson->surname, strlen(surname) + 1, surname);
        strcpy_s(newPerson->name, strlen(name) + 1, name);
        newPerson->age = age;
        strcpy_s(newPerson->pesel, strlen(pesel) + 1, pesel);
        newPerson->gender = gender;
        newPerson->next = NULL;
        newPerson->prev = NULL;
        if (head == NULL) {
            head = tail = newPerson;
        }
        else {
            tail->next = newPerson;
            newPerson->prev = tail;
            tail = newPerson;
        }
    }
}

Person* searchPerson(char* nameOrSurname) { // funkcja wyszukujaca osobe po nazwisku
    Person* current = head;
    while (current != NULL) {
        if (strcmp(current->name, nameOrSurname) == 0 || strcmp(current->surname, nameOrSurname) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void deletePerson(char* surname) { // funkcja kasujaca osobe po nazwisku
    Person* current = head;
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            if (current == head) {
                head = current->next;
            }
            if (current == tail) {
                tail = current->prev;
            }
            free(current);
            break;
        }
        current = current->next;
    }
}

void displayList(int direction) { //funkcja wyuswietlajaca liste od glowy do ogona badz odwrotnie
    Person* current;
    if (direction == 0) {
        current = head;
        while (current != NULL) {
            printf("%s %s %d %s %c\n", current->surname, current->name, current->age, current->pesel, current->gender);
            current = current->next;
        }
    }
    else {
        current = tail;
        while (current != NULL) {
            printf("%s %s %d %s %c\n", current->surname, current->name, current->age, current->pesel, current->gender);
            current = current->prev;
        }
    }
}

void readFromFile() {
    FILE* file;
    fopen_s(&file, "data.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char surname[100], name[100], pesel[12];
    int age;
    char gender;
    while (fscanf_s(file, "%s %s %d %s %c", surname, (unsigned)_countof(surname), name, (unsigned)_countof(name), &age, pesel, (unsigned)_countof(pesel), &gender) == 5) {
        addPerson(surname, name, age, pesel, gender);
    }
    fclose(file);
}

void writeToFile() { // zapis do pliku
    FILE* file;
    fopen_s(&file, "data.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    Person* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %d %s %c\n", current->surname, current->name, current->age, current->pesel, current->gender);
        current = current->next;
    }
    fclose(file);
}

void sortList() { // sortowanie babelkowe
    int sorted;
    do {
        sorted = 1;
        Person* current = head;
        while (current->next != NULL) {
            if (strcmp(current->surname, current->next->surname) > 0) {
                char tempSurname[100], tempName[100], tempPesel[12];
                int tempAge;
                char tempGender;
                strcpy_s(tempSurname, current->surname);
                strcpy_s(tempName, current->name);
                tempAge = current->age;
                strcpy_s(tempPesel, current->pesel);
                tempGender = current->gender;
                strcpy_s(current->surname, current->next->surname);
                strcpy_s(current->name, current->next->name);
                current->age = current->next->age;
                strcpy_s(current->pesel, current->next->pesel);
                current->gender = current->next->gender;
                strcpy_s(current->next->surname, tempSurname);
                strcpy_s(current->next->name, tempName);
                current->next->age = tempAge;
                strcpy_s(current->next->pesel, tempPesel);
                current->next->gender = tempGender;
                sorted = 0;
            }
            current = current->next;
        }
    } while (!sorted);
}

int main() { // funkkcja main z menu
    int choice;
    char surname[100], fileName[100], name[100], pesel[12], gender;
    int age, direction;
    while (1) {
        printf("1. Dodaj osobe\n");
        printf("2. Wyszukaj osobe\n");
        printf("3. Usun osobe\n");
        printf("4. Wyswietl liste\n");
        printf("5. Odczytaj z pliku\n");
        printf("6. Zapisz do pliku\n");
        printf("7. Posortuj liste\n");
        printf("8. Wyjdz\n");
        printf("Wybierz opcje: ");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1:
        {
            printf("Podaj nazwisko: ");
            scanf_s("%s", surname, sizeof(surname));
            printf("Podaj imie: ");
            scanf_s("%s", name, sizeof(name));
            printf("Podaj wiek: ");
            scanf_s("%d", &age);
            printf("Podaj PESEL: ");
            scanf_s("%s", pesel, sizeof(pesel));
            printf("Podaj plec (M/F): ");
            scanf_s(" %c", &gender, sizeof(gender));
            addPerson(surname, name, age, pesel, gender);
        }
        break;
        case 2:
        {
            printf("Podaj imie lub nazwisko do wyszukania: ");
            scanf_s("%s", surname, sizeof(surname));
            Person* result = searchPerson(surname);
            if (result != NULL) {
                printf("%s %s %d %s %c\n", result->surname, result->name, result->age, result->pesel, result->gender);
            }
            else {
                printf("Osoba nie znaleziona\n");
            }
        }
        break;
        case 3:
        {
            printf("Podaj nazwisko osoby do usuniecia: ");
            scanf_s("%s", surname, sizeof(surname));
            deletePerson(surname);
        }
        break;
        case 4:
        {
            printf("Podaj kierunek wyswietlania (0 dla przodu, 1 dla tylu): ");
            scanf_s("%d", &direction);
            displayList(direction);
        }
        break;
        case 5:
        {
            readFromFile();
        }
        break;
        case 6:
        {
            writeToFile();
        }
        break;
        case 7:
            sortList();
            break;
        case 8:
            exit(0);
        default:
            printf("Nieprawidlowy wybor\n");
        }
    }
    return 0;
}