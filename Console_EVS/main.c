#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_ID_LENGTH 32
#define USER_FILE "voters.txt"

struct Candidate {
    char *name;
    int votes;
};

struct Candidate *candidates = NULL;
char votedUsers[MAX_USERS][MAX_ID_LENGTH] = {0};
int numCandidates = 0;

void welcome();
void displayMenu();
void manageCandidates();
void displayCandidates();
void vote();
void addCandidate();
int hasVoted(const char *userID);
void recordVote(const char *userID);
void saveUserIDs();
void loadUserIDs();

int main() {
    loadUserIDs();
    welcome();
    while (1) {
        displayMenu();
    }

    return 0;
}
void welcome() {
    printf("\033[1;33m\n*****************************************\n");
    printf("*  WELCOME TO ELECTRONIC VOTING SYSTEM  *\n");
    printf("*****************************************\033[0m\n");
}

void displayMenu() {
    int choice;

    printf("\033[1;36m\n1. Manage Candidates\n");
    printf("2. Start Voting\n");
    printf("3. Exit\n\n");
    printf("Enter your choice (1, 2, or 3): \033[0m");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        system("clear || cls");
        manageCandidates();
        break;
    case 2:
        if (numCandidates == 0) {
            printf("\033[1;31m\nNo candidates available. Please add candidates first.\033[0m\n");
            break;
        }
        system("clear || cls");
        displayCandidates();
        vote();
        break;
    case 3:
        printf("\033[1;32m\nThank you for using the Electronic Voting System. Goodbye!\n");
        for (int i = 0; i < numCandidates; ++i) {
            printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
            free(candidates[i].name);
        }
        saveUserIDs();
        free(candidates);
        exit(0);
    default:
        printf("\033[1;31mInvalid choice. Please enter 1, 2, or 3.\033[0m\n");
        exit(1);
    }
}

void manageCandidates() {
    int subChoice;
    do {
        printf("\033[1;34m\n********** CANDIDATES MANAGEMENT **********\n\n");
        printf("1. Add Candidate\n");
        printf("2. View Candidates\n");
        printf("3. Back to Main Menu\n\n");
        printf("Enter your choice (1, 2, or 3): \033[0m");
        scanf("%d", &subChoice);

        switch (subChoice) {
        case 1:
            addCandidate();
            break;
        case 2:
            displayCandidates();
            break;
        case 3:
            break;
        default:
            printf("\033[1;31mInvalid choice. Please enter 1, 2, or 3.\033[0m\n");
        }
    } while (subChoice != 3);
}

void displayCandidates() {
    printf("\033[1;34m\n*************** CANDIDATES ****************\n\n");
    if (numCandidates == 0) {
        printf("\033[1;31mNo candidates available.\033[0m\n\n");
    } else {
        for (int i = 0; i < numCandidates; ++i) {
            printf("%d. %s\n", i + 1, candidates[i].name);
        }
    }
}

int hasVoted(const char *userID) {
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strcmp(votedUsers[i], userID) == 0) {
            return 1;
        }
    }
    return 0;
}

void recordVote(const char *userID) {
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strlen(votedUsers[i]) == 0) {
            strncpy(votedUsers[i], userID, MAX_ID_LENGTH - 1);
            votedUsers[i][MAX_ID_LENGTH - 1] = '\0';
            break;
        }
    }
}

void saveUserIDs() {
    FILE *file = fopen(USER_FILE, "w");
    if (file == NULL) {
        printf("\033[1;31mError opening user ID file for writing.\033[0m\n");
        return;
    }
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strlen(votedUsers[i]) > 0) {
            fprintf(file, "%s\n", votedUsers[i]);
        }
    }
    fclose(file);
}

void loadUserIDs() {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        return;
    }
    char userID[MAX_ID_LENGTH];
    while (fscanf(file, "%s", userID) == 1) {
        for (int i = 0; i < MAX_USERS; ++i) {
            if (strlen(votedUsers[i]) == 0) {
                strncpy(votedUsers[i], userID, MAX_ID_LENGTH - 1);
                votedUsers[i][MAX_ID_LENGTH - 1] = '\0';
                break;
            }
        }
    }
    fclose(file);
}

void vote() {
    char userID[MAX_ID_LENGTH];
    int choice;
    while (1) {
        printf("\033[1;36m\nEnter your user ID (up to %d characters): \033[0m", MAX_ID_LENGTH);
        scanf("%s", userID);
        if (hasVoted(userID)) {
            printf("\033[1;31mYou have already voted.\033[0m\n");
        } else {
            printf("\033[1;36mEnter your choice (1-%d) or %d to exit: \033[0m", numCandidates, numCandidates + 1);
            scanf("%d", &choice);
            if (choice >= 1 && choice <= numCandidates) {
                printf("\033[1;32m\nYou voted for %s.\n\n", candidates[choice - 1].name);
                candidates[choice - 1].votes++;
                recordVote(userID);
            } else if (choice == numCandidates + 1) {
                break;
            } else {
                printf("\033[1;31mInvalid choice. Please enter a number between 1 and %d.\033[0m\n", numCandidates + 1);
            }
        }
        printf("\033[1;36m1. Vote\n");
        printf("2. Show Results\n");
        printf("Enter your choice (1 or 2): \033[0m");
        scanf("%d", &choice);
        if (choice == 2) {
            printf("\033[1;32m\nThank you for voting!\n\n");
            printf("Results:\n");
            for (int i = 0; i < numCandidates; ++i) {
                printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
                free(candidates[i].name);
            }
            printf("\n");
            free(candidates);
            saveUserIDs();
            return;
        }
    }
}

void addCandidate() {
    int newNumCandidates;
    printf("\033[1;36m\nEnter the number of candidates: \033[0m");
    scanf("%d", &newNumCandidates);
    if (newNumCandidates <= 0) {
        printf("\033[1;31mInvalid number of candidates. Exiting...\033[0m\n");
        exit(1);
    }
    candidates = (struct Candidate *)realloc(candidates, (numCandidates + newNumCandidates) * sizeof(struct Candidate));
    if (candidates == NULL) {
        printf("\033[1;31mMemory allocation error. Exiting...\033[0m\n");
        exit(1);
    }
    while (getchar() != '\n');
    for (int i = numCandidates; i < numCandidates + newNumCandidates; ++i) {
        printf("\033[1;36mEnter the name of candidate %d (up to 32 characters): \033[0m", i + 1);
        candidates[i].name = (char *)malloc((MAX_ID_LENGTH + 1) * sizeof(char));
        fgets(candidates[i].name, MAX_ID_LENGTH + 1, stdin);
        size_t len = strlen(candidates[i].name);
        if (len > 0 && candidates[i].name[len - 1] == '\n') {
            candidates[i].name[len - 1] = '\0';
        } else {
            while (getchar() != '\n');
        }
        candidates[i].name[MAX_ID_LENGTH] = '\0';
        candidates[i].votes = 0;
    }
    numCandidates += newNumCandidates;
}
