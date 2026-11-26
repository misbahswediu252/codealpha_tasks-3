#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"
#define NAME_LEN 50

typedef struct {
    int roll;
    char name[NAME_LEN];
    float cgpa;
} Student;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void readName(char *buf, int size) {
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

int rollExists(int roll) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    if (!fp) return 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addStudent(void) {
    Student s;
    FILE *fp;

    printf("Enter roll number: ");
    if (scanf("%d", &s.roll) != 1) { clearInput(); printf("Invalid input!\n"); return; }
    clearInput();

    if (rollExists(s.roll)) {
        printf("A student with this roll number already exists!\n");
        return;
    }

    printf("Enter name: ");
    readName(s.name, NAME_LEN);
    printf("Enter CGPA: ");
    if (scanf("%f", &s.cgpa) != 1) { clearInput(); printf("Invalid input!\n"); return; }
    clearInput();

    fp = fopen(FILE_NAME, "ab");
    if (!fp) { printf("Error opening file!\n"); return; }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayStudents(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    int count = 0;

    if (!fp) { printf("No records found.\n"); return; }

    printf("\n%-8s %-25s %-6s\n", "Roll", "Name", "CGPA");
    printf("------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-8d %-25s %-6.2f\n", s.roll, s.name, s.cgpa);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("No records found.\n");
}

void searchStudent(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    int roll, found = 0;

    if (!fp) { printf("No records found.\n"); return; }

    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) { clearInput(); fclose(fp); printf("Invalid input!\n"); return; }
    clearInput();

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\nStudent found:\n");
            printf("Roll: %d\nName: %s\nCGPA: %.2f\n", s.roll, s.name, s.cgpa);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Student not found.\n");
}

void updateStudent(void) {
    FILE *fp = fopen(FILE_NAME, "rb+");
    Student s;
    int roll, found = 0;

    if (!fp) { printf("No records found.\n"); return; }

    printf("Enter roll number to update: ");
    if (scanf("%d", &roll) != 1) { clearInput(); fclose(fp); printf("Invalid input!\n"); return; }
    clearInput();

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("Enter new name: ");
            readName(s.name, NAME_LEN);
            printf("Enter new CGPA: ");
            if (scanf("%f", &s.cgpa) != 1) { clearInput(); fclose(fp); printf("Invalid input!\n"); return; }
            clearInput();

            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    printf(found ? "Record updated successfully.\n" : "Student not found.\n");
}

void deleteStudent(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *tmp;
    Student s;
    int roll, found = 0;

    if (!fp) { printf("No records found.\n"); return; }

    printf("Enter roll number to delete: ");
    if (scanf("%d", &roll) != 1) { clearInput(); fclose(fp); printf("Invalid input!\n"); return; }
    clearInput();

    tmp = fopen("temp.dat", "wb");
    if (!tmp) { fclose(fp); printf("Error creating temp file!\n"); return; }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll)
            found = 1;
        else
            fwrite(&s, sizeof(Student), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("Record deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Student not found.\n");
    }
}

int main(void) {
    int choice;

    do {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Update Student\n");
        printf("4. Search Student\n");
        printf("5. Display All Students\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInput();
            choice = 0;
        } else {
            clearInput();
        }

        switch (choice) {
        case 1: addStudent(); break;
        case 2: deleteStudent(); break;
        case 3: updateStudent(); break;
        case 4: searchStudent(); break;
        case 5: displayStudents(); break;
        case 6: printf("Goodbye!\n"); break;
        default: printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}
