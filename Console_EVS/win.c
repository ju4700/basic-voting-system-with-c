#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_USERS 100
#define MAX_ID_LENGTH 32
#define USER_FILE "voters.txt"

// Structure to represent a candidate
struct Candidate {
    char *name;
    int votes;
};

// Global variables
struct Candidate *candidates = NULL;
char votedUsers[MAX_USERS][MAX_ID_LENGTH] = {0}; // 2D array to store voted user IDs
int numCandidates = 0;

// Function prototypes
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
void setConsoleColor(int color);

// Main function
int main() {
    loadUserIDs(); // Load existing user IDs
    welcome();
    while (1) {
        displayMenu();
    }

    return 0;
}

// Display welcome message
void welcome() {
    setConsoleColor(14); // Yellow text
    printf("\n\t\t\t\t*****************************************\n");
    printf("\t\t\t\t*  WELCOME TO ELECTRONIC VOTING SYSTEM  *\n");
    printf("\t\t\t\t*****************************************\n");
    setConsoleColor(7); // Reset to default color
}

// Display the main menu and handle user input
void displayMenu() {
    int choice;

    setConsoleColor(11); // Light cyan text
    printf("\n1. Manage Candidates\n");
    printf("2. Start Voting\n");
    printf("3. Exit\n\n");
    setConsoleColor(15); // White text
    printf("Enter your choice (1, 2, or 3): ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        system("cls");
        manageCandidates();
        break;
    case 2:
        if (numCandidates == 0) {
            setConsoleColor(12); // Light red text
            printf("\nNo candidates available. Please add candidates first.\n");
            setConsoleColor(7); // Reset to default color
            break;
        }
        system("cls");
        // Only call displayCandidates() once before entering the voting process
        displayCandidates();
        vote();
        break;
    case 3:
        setConsoleColor(10); // Light green text
        printf("\nThank you for using the Electronic Voting System. Goodbye!\n");
        for (int i = 0; i < numCandidates; ++i) {
            printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
            free(candidates[i].name);
        }
        saveUserIDs(); // Save user IDs before exit
        free(candidates);
        setConsoleColor(7); // Reset to default color
        exit(0);
    default:
        setConsoleColor(12); // Light red text
        printf("\nInvalid choice. Please enter 1, 2, or 3.\n");
        setConsoleColor(7); // Reset to default color
        exit(1);
    }
}

// Manage candidates - add or view candidates
void manageCandidates() {
    int subChoice;
    do {
        setConsoleColor(11); // Light cyan text
        printf("\n\t\t\t\t********** CANDIDATES MANAGEMENT **********\n\n");
        printf("1. Add Candidate\n");
        printf("2. View Candidates\n");
        printf("3. Back to Main Menu\n\n");
        setConsoleColor(15); // White text
        printf("Enter your choice (1, 2, or 3): ");
        scanf("%d", &subChoice);

        switch (subChoice) {
        case 1:
            addCandidate();
            break;
        case 2:
            displayCandidates();
            break;
        case 3:
            break; // Return to the main menu
        default:
            setConsoleColor(12); // Light red text
            printf("\nInvalid choice. Please enter 1, 2, or 3.\n");
            setConsoleColor(7); // Reset to default color
        }
    } while (subChoice != 3);
}

// Display the list of candidates
void displayCandidates() {
    setConsoleColor(11); // Light cyan text
    printf("\n\t\t\t\t*************** CANDIDATES ****************\n\n");
    if (numCandidates == 0) {
        setConsoleColor(12); // Light red text
        printf("No candidates available.\n\n");
        setConsoleColor(7); // Reset to default color
    } else {
        for (int i = 0; i < numCandidates; ++i) {
            printf("%d. %s\n", i + 1, candidates[i].name);
        }
    }
}

// Check if a user has already voted
int hasVoted(const char *userID) {
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strcmp(votedUsers[i], userID) == 0) {
            return 1; // User has voted
        }
    }
    return 0; // User has not voted
}

// Record a user's vote
void recordVote(const char *userID) {
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strlen(votedUsers[i]) == 0) {
            strncpy(votedUsers[i], userID, MAX_ID_LENGTH - 1);
            votedUsers[i][MAX_ID_LENGTH - 1] = '\0'; // Ensure null-termination
            break;
        }
    }
}

// Save user IDs to a file
void saveUserIDs() {
    FILE *file = fopen(USER_FILE, "w");
    if (file == NULL) {
        setConsoleColor(12); // Light red text
        printf("Error opening user ID file for writing.\n");
        setConsoleColor(7); // Reset to default color
        return;
    }

    for (int i = 0; i < MAX_USERS; ++i) {
        if (strlen(votedUsers[i]) > 0) {
            fprintf(file, "%s\n", votedUsers[i]);
        }
    }

    fclose(file);
}

// Load user IDs from a file
void loadUserIDs() {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        // If the file does not exist, just return
        return;
    }

    char userID[MAX_ID_LENGTH];
    while (fscanf(file, "%s", userID) == 1) {
        for (int i = 0; i < MAX_USERS; ++i) {
            if (strlen(votedUsers[i]) == 0) {
                strncpy(votedUsers[i], userID, MAX_ID_LENGTH - 1);
                votedUsers[i][MAX_ID_LENGTH - 1] = '\0'; // Ensure null-termination
                break;
            }
        }
    }

    fclose(file);
}

// Perform the voting process
void vote() {
    char userID[MAX_ID_LENGTH];
    int choice;

    while (1) {
        setConsoleColor(11); // Light cyan text
        printf("\nEnter your user ID (up to %d characters): ", MAX_ID_LENGTH);
        scanf("%s", userID);

        if (hasVoted(userID)) {
            setConsoleColor(12); // Light red text
            printf("You have already voted.\n");
            setConsoleColor(7); // Reset to default color
        } else {
            setConsoleColor(11); // Light cyan text
            printf("Enter your choice (1-%d) or %d to exit: ", numCandidates, numCandidates + 1);
            scanf("%d", &choice);

            if (choice >= 1 && choice <= numCandidates) {
                setConsoleColor(10); // Light green text
                printf("\nYou voted for %s.\n", candidates[choice - 1].name);
                candidates[choice - 1].votes++;
                recordVote(userID); // Record the user's vote
                setConsoleColor(7); // Reset to default color
            } else if (choice == numCandidates + 1) {
                // Exit the voting process
                break;
            } else {
                setConsoleColor(12); // Light red text
                printf("Invalid choice. Please enter a number between 1 and %d.\n", numCandidates + 1);
                setConsoleColor(7); // Reset to default color
            }
        }

        // Check if the user wants to continue voting
        setConsoleColor(11); // Light cyan text
        printf("\n1. Vote\n");
        printf("2. Show Results\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);

        if (choice == 2) {
            // Show results and exit the voting process
            setConsoleColor(10); // Light green text
            printf("\nThank you for voting!\n\n");
            printf("Results:\n");
            for (int i = 0; i < numCandidates; ++i) {
                printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
                free(candidates[i].name);
            }
            printf("\n");
            free(candidates);
            saveUserIDs(); // Save user IDs before exit
            setConsoleColor(7); // Reset to default color
            return;
        }
    }
}

// Add candidates to the system
void addCandidate() {
    int newNumCandidates;
    printf("\nEnter the number of candidates: ");
    scanf("%d", &newNumCandidates);

    if (newNumCandidates <= 0) {
        setConsoleColor(12); // Light red text
        printf("Invalid number of candidates. Exiting...\n");
        setConsoleColor(7); // Reset to default color
        exit(1);
    }

    // Allocate memory for the new candidates
    candidates = (struct Candidate *)realloc(candidates, (numCandidates + newNumCandidates) * sizeof(struct Candidate));

    if (candidates == NULL) {
        setConsoleColor(12); // Light red text
        printf("Memory allocation error. Exiting...\n");
        setConsoleColor(7); // Reset to default color
        exit(1);
    }

    // Clear the input buffer
    while (getchar() != '\n');

    for (int i = numCandidates; i < numCandidates + newNumCandidates; ++i) {
        setConsoleColor(11); // Light cyan text
        printf("\nEnter the name of candidate %d (up to 32 characters): ", i + 1);

        // Allocate memory for candidate name
        candidates[i].name = (char *)malloc((MAX_ID_LENGTH + 1) * sizeof(char));

        // Use fgets to allow spaces in candidate names and limit to 32 characters
        fgets(candidates[i].name, MAX_ID_LENGTH + 1, stdin);

        // Remove the newline character from the input
        size_t len = strlen(candidates[i].name);
        if (len > 0 && candidates[i].name[len - 1] == '\n') {
            candidates[i].name[len - 1] = '\0';
        } else {
            // Clear the input buffer in case the name exceeds 32 characters
            while (getchar() != '\n');
        }

        // Limit the name to 32 characters
        candidates[i].name[MAX_ID_LENGTH] = '\0';

        candidates[i].votes = 0;
    }

    numCandidates += newNumCandidates;
}

// Set console text color on Windows
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
