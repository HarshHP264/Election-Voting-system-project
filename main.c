// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // taken For getch() on Windows
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// Define maximum lengths
#define MAX_USERNAME_LEN 50
#define MAX_ROLL_NUM_LEN 12
#define MAX_PASSWD_LEN 50
#define MAX_NAME_LEN 50 // Maximum length of candidate name + 1 for null terminator
    time_t currentTime;
struct tm* localTime;
bool hasVoted(const char *rollNumber);
void writeVotedRollNumber(const char *rollNumber);
// Function prototypes
void writeVotedRollNumberToNota(const char *rollNumber);
#define VOTED_FILE "voted.txt"
void adminLogin();
void studentLogin();
void loadCandidateData();
void saveCandidateData();
int loadAllowedRollNumbers(char allowedRollNumbers[][MAX_ROLL_NUM_LEN]);

int numCandidates;
char candidateNames[100][MAX_NAME_LEN]; // Assuming max 100 candidates
char filename[MAX_NAME_LEN + 4];
bool isEligible(const char *rollNumber, char allowedRollNumbers[][MAX_ROLL_NUM_LEN], int numAllowedRollNumbers);

int main()
{
    loadCandidateData();

    while (1)
    {
        printf("\n");
        printf("*********************************************\n");
        printf("*    ELECTION VOTING SYSTEM FOR COLLEGE     *\n");
        printf("*********************************************\n");
        printf("* %-10s %s \t\t\t    *\n", "OPTION", "\tACTION");
        printf("*-------------------------------------------*\n");
        printf("*   %-10d %s          *\n", 1, "STUDENT VOTER LOGIN");
        printf("*   %-10d %s       *\n", 2, "ELECTION OFFICER LOGIN");
        printf("*   %-10d %s \t\t\t    *\n", 3, "EXIT");
        printf("*********************************************\n");
        printf("SELECT CHOICE   :");
        int input;
        scanf(" %d", &input);
        system("cls");
        switch (input)
        {
        case 1:
            studentLogin();
            break;
        case 2:
            adminLogin();
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("\nInvalid option");
            break;
        }
    }
    return 0;
}

// admin.c
// Function prototypes
int validateAdmin(const char *username, const char *passwd);
void getPassword(char *passwd);
int isValidCandidateName(const char *name);

void adminLogin()
{
    char username[MAX_USERNAME_LEN];
    char passwd[MAX_PASSWD_LEN];
    // Maximum length of filename + 4 for ".txt" extension
admin_login:
    loadCandidateData();
    // Input admin username
    printf("*********************************************\n");
    printf("*           ELECTION OFFICER LOGIN          *\n");
    printf("*********************************************\n");
    printf("Enter Officer Username    :");
    scanf("%s", username);

    // Input admin password without echoing characters
    printf("Enter Officer Password    :");
    getPassword(passwd);
    printf("\n");

    // Validate admin credentials
    if (validateAdmin(username, passwd))
    {
        printf("Officer login successful!\n");
        printf("Please tap the (ENTER) key to initiate the operation.");
        getch();
        system("cls");
        int adminChoice;
        int voteCounts[numCandidates];
        int totalvotes;
        // Find the candidate with the most votes
        int maxVotesIndex = 0;
        do
        {
            
            // Display admin menu options
            printf("\n");
            printf("*********************************************\n");
            printf("*                 MAIN MENU                 *\n");
            printf("*********************************************\n");
            printf("* %-10s %s \t\t\t    *\n", "OPTION", "\tACTION");
            printf("*-------------------------------------------*\n");
            printf("*   %-10d %s \t\t    *\n", 1, "New Election");
            printf("*   %-10d %s \t    *\n", 2, "Delete Illegal Vote");
            printf("*   %-10d %s \t\t\t    *\n", 3, "Result");
            printf("*   %-10d %s \t\t\t    *\n", 4, "Logout");
            printf("*********************************************\n");
            printf("SELECT CHOICE   :");
            scanf("%d", &adminChoice);

            switch (adminChoice)
            {
            case 1:
                system("cls");
                time(&currentTime);
    localTime = localtime(&currentTime);

    printf("Date: %s", asctime(localTime));
                // Prompt admin to enter the number of candidates
                printf("*********************************************\n");
                printf("*              CANDIDATES DETAILS           *\n");
                printf("*********************************************\n");
                printf("Enter the number of candidates: ");
                scanf("%d", &numCandidates);
                getchar(); // Consume newline character

                if (numCandidates <= 0)
                {
                    printf("Invalid number of candidates. Exiting...\n");
                    return;
                }

                printf("====================================================================\n");
                printf("NOTE :Each name must not exceed 50 characters and end with a number\n");
                printf("====================================================================\n");

                for (int i = 0; i < numCandidates; i++)
                {
                    printf("Candidate %d: ", i + 1);
                    fgets(candidateNames[i], sizeof(candidateNames[i]), stdin);
                    candidateNames[i][strcspn(candidateNames[i], "\n")] = '\0'; // Remove newline character
                    if (!isValidCandidateName(candidateNames[i]))
                    {
                        printf("Invalid candidate name. Name must not exceed 50 characters and end with a number.\n");
                        i--; // Prompt for the same candidate again
                    }
                }

                // Create a separate text file for each candidate
                for (int i = 0; i < numCandidates; i++)
                {
                    sprintf(filename, "%s.txt", candidateNames[i]);
                    FILE *file = fopen(filename, "w");
                    if (file == NULL)
                    {
                        printf("Error creating file for candidate %s. Skipping...\n", candidateNames[i]);
                    }
                    else
                    {
                        printf("File created for candidate %s.\n", candidateNames[i]);
                        fclose(file);
                    }
                }
                saveCandidateData();
                printf("Kindly press (ENTER) to proceed to MAIN MENU.");
                getch();
                system("cls");
                break;
            case 2:
                system("cls");
                printf("*********************************************\n");
                printf("*            DELETE ILLEGEL VOTE            *\n");
                printf("*********************************************\n");

                // Prompt admin to enter the roll number of the student
                char rollNumberToDelete[MAX_ROLL_NUM_LEN];
                printf("Enter the roll number of the student whose vote needs to be deleted: ");
                scanf("%s", rollNumberToDelete);

                // Iterate through each candidate's txt file and remove the roll number if found
                for (int i = 0; i < numCandidates; i++)
                {
                    sprintf(filename, "%s.txt", candidateNames[i]);
                    FILE *file = fopen(filename, "r");
                    if (file != NULL)
                    {
                        // Create a temporary file to store the updated contents
                        FILE *tempFile = fopen("temp.txt", "w");
                        if (tempFile == NULL)
                        {
                            printf("Error creating temporary file.\n");
                            fclose(file);
                            return;
                        }
                        char line[1024];
                        while (fgets(line, sizeof(line), file))
                        {
                            // Check if the current line contains the roll number to delete
                            if (strstr(line, rollNumberToDelete) == NULL)
                            {
                                // If not found, write the line to the temporary file
                                fputs(line, tempFile);
                            }
                        }
                        fclose(file);
                        fclose(tempFile);

                        // Replace the original file with the temporary file
                        remove(filename);
                        rename("temp.txt", filename);
                    }
                    else
                    {
                        printf("Error opening file for candidate %s.\n", candidateNames[i]);
                    }
                }
// Remove the roll number from the "voted.txt" file
                FILE *votedFile = fopen("voted.txt", "r");
                if (votedFile != NULL)
                {
                    // Create a temporary file to store the updated contents
                    FILE *tempFile = fopen("temp_voted.txt", "w");
                    if (tempFile == NULL)
                    {
                        printf("Error creating temporary file for voted.\n");
                        fclose(votedFile);
                        return;
                    }
                    char line[1024];
                    while (fgets(line, sizeof(line), votedFile))
                    {
                        // Check if the current line contains the roll number to delete
                        if (strstr(line, rollNumberToDelete) == NULL)
                        {
                            // If not found, write the line to the temporary file
                            fputs(line, tempFile);
                        }
                    }
                    fclose(votedFile);
                    fclose(tempFile);

                    // Replace the original "voted.txt" file with the temporary file
                    remove("voted.txt");
                    rename("temp_voted.txt", "voted.txt");
                }
                else
                {
                    printf("Error opening voted file.\n");
                }
                printf("Illegal vote with roll number %s deleted successfully.\n", rollNumberToDelete);
                printf("Kindly press (ENTER) to proceed to MAIN MENU.");
                getch();
                system("cls");
                break;

                break;

            case 3:
                system("cls");
                 time(&currentTime);
    localTime = localtime(&currentTime);

  printf("Date: %s", asctime(localTime));
printf("*********************************************\n");
printf("*              ELECTION RESULT              *\n");
printf("*********************************************\n");

// Array to store the vote counts for each candidate
totalvotes = 0;
for (int i = 0; i < numCandidates; i++)
{
    voteCounts[i] = 0;
}

// Count votes for each candidate
for (int i = 0; i < numCandidates; i++)
{
    sprintf(filename, "%s.txt", candidateNames[i]);
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        char line[1024];
        while (fgets(line, sizeof(line), file))
        {
            voteCounts[i]++;
            totalvotes++;
        }
        fclose(file);
    }
}

// Count votes for NOTA from "nota_votes.txt" file
FILE *notaFile = fopen("nota_votes.txt", "r");
int notaCount = 0; // Initialize NOTA vote count
if (notaFile != NULL)
{
    char line[MAX_ROLL_NUM_LEN];
    while (fgets(line, sizeof(line), notaFile))
    {
        // Check if the line is not empty
        if (line[0] != '\n') {
            notaCount++;
        }
    }
    fclose(notaFile);
}

// Display the results
printf("Candidate\tVotes\tPercentage\n");
for (int i = 0; i < numCandidates; i++)
{
    float percentage = (float)voteCounts[i] / totalvotes * 100;
    printf("%s\t\t%d\t%.2f%%\n", candidateNames[i], voteCounts[i], percentage);
}
// Display NOTA only if there are votes
if (notaCount > 0) {
    printf("NOTA\t\t%d\t%.2f%%\n", notaCount, (float)notaCount / totalvotes * 100); // Display NOTA vote count and percentage
}
printf("=============================================\n");
printf("Winner: %s\n", candidateNames[maxVotesIndex]);
printf("=============================================\n");
printf("Kindly press (ENTER) to proceed to MAIN MENU.");
getch();
system("cls"); 
break;

            case 4:
                if (numCandidates > 0)
                {
                    printf("Continuing previous election...\n");
                    // Load candidate data from files if available
                    FILE *candidatesFile = fopen("candidates.txt", "r");
                    if (candidatesFile != NULL)
                    {
                        fscanf(candidatesFile, "%d\n", &numCandidates);
                        for (int i = 0; i < numCandidates; i++)
                        {
                            fgets(candidateNames[i], sizeof(candidateNames[i]), candidatesFile);
                            candidateNames[i][strcspn(candidateNames[i], "\n")] = '\0'; // Remove newline character
                        }
                        fclose(candidatesFile);
                        printf("Candidate data loaded successfully.\n");
                    }
                    else
                    {
                        printf("Error opening candidates file!\n");
                    }
                    // Add any other necessary data loading for continuing the previous election
                    // Now you can proceed with other operations related to the previous election
                }
                else
                {
                    printf("No candidates data available. Please load candidates for a new election.\n");
                }
                break;
            case 5:
                printf("Logging out...\n");
                system("cls");
                break;

            default:
                system("cls");
                printf("Invalid option! Please select a valid option.\n");
                break;
            }
        } while (adminChoice != 4);
    }
    else
    {
        int choice2 = 0;
        printf("Invalid username or password. Access denied!\n");
        printf("Press 1 to try again | Press 2 for Home page    :");
        scanf("%d", &choice2);
        system("cls");
        switch (choice2)
        {
        case 1:
            goto admin_login;
            break;
        case 2:
            break;
        default:
            printf("Invalid Choice! Code Exited..\n");
            exit(0);
            break;
        }
    }
}

// student.c
struct User
{
    char rollNumber[MAX_ROLL_NUM_LEN];
    char username[MAX_USERNAME_LEN];
    char passwd[MAX_PASSWD_LEN];
};

// Function prototypes
void writeUserToFile(struct User user);
int findUserByRollNumber(char *rollNumber, char *username, char *passwd);
bool isValidRollNumber(const char *rollNumber);
void getPassword(char *passwd);
bool isRollNumberRegistered(const char *rollNumber);

void studentLogin()
{
    int choice;
    struct User user;
    do
    {
        printf("\n");
        printf("****************************************************\n");
        printf("*                 SIGNIN || SIGNUP                 *\n");
        printf("****************************************************\n");
        printf("\n");
        printf("****************************************************\n");
        printf("* %-10s %s \t\t\t   *\n", "OPTION", "\t\tACTION");
        printf("----------------------------------------------------\n");
        printf("*   %-10d %s \t\t   *\n", 1, "\t\tVoter Register");
        printf("*   %-10d %s  \t           *\n", 2, "\t\tVoter Login");
        printf("****************************************************\n");
        printf("SELECT CHOICE   :");
        scanf("%d", &choice);
        if (choice > 2)
        {
            system("cls");
            printf("Please enter a valid option\n");
        }
    } while (choice > 2);
    switch (choice)
    {
    case 1:
        // Registration page
    registertion_page:
            system("cls");
            printf("****************************************************\n");
            printf("*                 REGISTRATION                     *\n");
            printf("****************************************************\n");

            char allowedRollNumbers[100][MAX_ROLL_NUM_LEN];
            int numAllowedRollNumbers = loadAllowedRollNumbers(allowedRollNumbers);

            do {
                printf("\nEnter Roll Number (in the pattern XXUGXXXXX)  :");
                scanf("%s", user.rollNumber);
                while (getchar() != '\n') { ; }

                if (!isEligible(user.rollNumber, allowedRollNumbers, numAllowedRollNumbers)) {
                    printf("You are not eligible for registration.\n");
                    return;
                }
                if (isRollNumberRegistered(user.rollNumber)) {
        printf("You are already registered. Please log in instead.\n");
        return; // Exit the registration process
    }
            } 
            
            while (!isValidRollNumber(user.rollNumber));

            printf("Enter Username  :");
            scanf("%s", user.username);
            printf("Enter Password  :");
            getPassword(user.passwd);

            writeUserToFile(user);

            printf("\n\nRegistration successful.....!\n");
            break;
        
    case 2:
        // Login page
    login_page:
        system("cls");
        printf("****************************************************\n");
        printf("*                   LOGIN                          *\n");
        printf("****************************************************\n");
        char rollNumber[MAX_ROLL_NUM_LEN];
        char username[MAX_USERNAME_LEN];
        char passwd[MAX_PASSWD_LEN];
        printf("Enter Roll Number   :");
        scanf("%s", rollNumber);
        if (findUserByRollNumber(rollNumber, username, passwd))
        {
            char validateUserName[MAX_USERNAME_LEN];
            char validateUserPasswd[MAX_PASSWD_LEN];
            printf("Enter Username      :");
            scanf("%s", validateUserName);
            printf("Enter Password      :");
            getPassword(validateUserPasswd);
            if (strcmp(username, validateUserName) == 0 && strcmp(passwd, validateUserPasswd) == 0)
            {
                printf("\n\t\tLogin Successfully....!");
                // After validating the user's login credentials or registering a new user
                strcpy(user.rollNumber, rollNumber); // Assuming rollNumber is the input obtained from the user
                // Prompt user to apply for vote
                printf("\nPlease! Press (ENTER) to apply for the vote.....");
                getch();
                system("cls");
                getchar();

                // Display the list of candidates

                printf("*********************************************\n");
                printf("*            LIST OF CANDIDATES             *\n");
                printf("*********************************************\n");
                for (int i = 0; i < numCandidates; i++)
                {
                    printf("%d. %s\n", i + 1, candidateNames[i]);
                }

                // Input vote choice
               int voteChoice;
printf("Select Candidate to Vote :");
scanf("%d", &voteChoice);

if (voteChoice == 0) {
        printf("You have chosen NOTA. Your vote will be recorded as None of the Above.\n");
    // Record the vote for NOTA
    writeVotedRollNumberToNota(rollNumber); // Function to write the roll number to "nota_votes.txt"
    printf("Kindly press (ENTER) to proceed to next voting.");
    getch();
    system("cls");

} else if (voteChoice < 1 || voteChoice > numCandidates) {
    printf("Invalid vote choice! Please select a valid candidate number.\n");
} else {
    // Check if the student has already voted for any candidate
    if (hasVoted(rollNumber))
    {
        printf("You have already voted! You cannot vote again.\n");
        goto end_voting;
    }
    // Write student's roll number to the voted file
    writeVotedRollNumber(rollNumber);
    // Write student's roll number and username to the chosen candidate's file
    sprintf(filename, "%s.txt", candidateNames[voteChoice - 1]);
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Error opening file for vote! Please try again later.\n");
    }
    else
    {
        fprintf(file, "%s\n", user.rollNumber);
        fclose(file);
        printf("Vote recorded successfully!\n");
    }
    printf("Kindly press (ENTER) to proceed to next voting.");
    getch();
    system("cls");
}

            end_voting:
                break;
            }
            else
            {
                printf("Invalid username or password!\n");
            }
        }
        else
        {
            int choice2 = 0;
            printf("User not found! Please register to login or try again.\n");
            printf("Press 1 to try again | Press 2 for Registration    :");
            scanf("%d", &choice2);
            switch (choice2)
            {
            case 1:
                goto login_page;
                break;
            case 2:
                goto registertion_page;
                break;
            default:
                printf("Invalid choice!\n");
                break;
            }
        }
        break;
    default:
        printf("Invalid choice!\n");
        break;
    }
}

// Shared functions
void getPassword(char *passwd)
{
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r')
    {
        // Read characters until Enter (Return) key is pressed
        if (ch == '\b' && i > 0)
        {
            // Handle backspace
            printf("\b \b"); // Move cursor back, print space, move cursor back again
            i--;
            passwd[i] = '\0';
        }
        else if (ch != '\b')
        {
            // Skip backspace character
            passwd[i++] = ch; // Store character in password buffer
            printf("*");      // Print asterisk (*) to the console
        }
    }
    passwd[i] = '\0'; // Null-terminate the password string
}

int validateAdmin(const char *username, const char *passwd)
{
    // Replace with your admin credentials
    const char *adminUsername = "admin";
    const char *adminPasswd = "admin123";

    if (strcmp(username, adminUsername) == 0 && strcmp(passwd, adminPasswd) == 0)
    {
        return 1; // Admin credentials are valid
    }
    else
    {
        return 0; // Admin credentials are invalid
    }
}

int isValidCandidateName(const char *name)
{
    int len = strlen(name);
    if (len > MAX_NAME_LEN - 2 || !isdigit(name[len - 1]))
    {
        return 0; // Invalid name
    }
    return 1; // Valid name
}

void writeUserToFile(struct User user)
{
    FILE *file = fopen("Users.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    // Trim leading and trailing whitespace characters from the password
    char trimmedPasswd[MAX_PASSWD_LEN];
    sscanf(user.passwd, "%s", trimmedPasswd);
    fprintf(file, "%s,%s,%s\n", user.rollNumber, user.username, trimmedPasswd);
    fclose(file);
}

int findUserByRollNumber(char *rollNumber, char *username, char *passwd)
{
    FILE *file = fopen("Users.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        char *token;
        char *temp = strdup(line); // Make a copy of the line for strtok
        token = strtok(temp, ",");
        if (strcmp(token, rollNumber) == 0)
        {
            // Roll number found, extract username and password
            token = strtok(NULL, ",");   // Move to the next token (username)
            strcpy(username, token);     // Copy username to the provided buffer
            token = strtok(NULL, ",");   // Move to the next token (password)
            sscanf(token, "%s", passwd); // Read password and trim leading/trailing whitespace
            fclose(file);
            free(temp); // Free the memory allocated for the copy of the line
            return 1;   // Found
        }
        free(temp); // Free the memory allocated for the copy of the line
    }
    fclose(file);
    return 0; // Not found
}

bool isValidRollNumber(const char *rollNumber)
{
    int length = strlen(rollNumber);
    if (length != 11)
    {
        return false;
    }
    if (strncmp(rollNumber + 2, "UGCS", 4) != 0)
    {
        return false;
    }
    return true;
}

bool hasVoted(const char *rollNumber)
{
    FILE *file = fopen(VOTED_FILE, "r");
    if (file == NULL)
    {
        return false; // If file doesn't exist, assume the student hasn't voted
    }
    char line[MAX_ROLL_NUM_LEN];
    while (fgets(line, sizeof(line), file))
    {
        if (strcmp(line, rollNumber) == 0)
        {
            fclose(file);
            return true; // Roll number found in voted file
        }
    }
    fclose(file);
    return false; // Roll number not found in voted file
}

void writeVotedRollNumber(const char *rollNumber)
{
    FILE *file = fopen(VOTED_FILE, "a");
    if (file == NULL)
    {
        printf("Error opening voted file!\n");
        return;
    }
    fprintf(file, "%s\n", rollNumber);
    fclose(file);
}

// Function to load candidate data from files
void loadCandidateData()
{
    FILE *file = fopen("candidates.txt", "r");
    if (file == NULL)
    {
        printf("Candidates file not found. No candidates loaded.\n");
        return;
    }
    numCandidates = 0;
    while (fgets(candidateNames[numCandidates], sizeof(candidateNames[numCandidates]), file))
    {
        candidateNames[numCandidates][strcspn(candidateNames[numCandidates], "\n")] = '\0'; // Remove newline character
        numCandidates++;
    }
    // Add NOTA as a candidate
    strcpy(candidateNames[numCandidates], "NOTA");
    numCandidates++;
    fclose(file);
    printf("Candidates loaded successfully.\n");
}

// Function to save candidate data to files
void saveCandidateData()
{
    FILE *file = fopen("candidates.txt", "w");
    if (file == NULL)
    {
        printf("Error opening candidates file for writing.\n");
        return;
    }
    for (int i = 0; i < numCandidates; i++)
    {
        fprintf(file, "%s\n", candidateNames[i]);
    }
    fclose(file);
    printf("Candidates saved successfully.\n");
}
// Function to load allowed roll numbers from a file
int loadAllowedRollNumbers(char allowedRollNumbers[][MAX_ROLL_NUM_LEN])
{
    FILE *file = fopen("allowedstudents.txt", "r");
    if (file == NULL)
    {
        printf("Error opening allowed students file!\n");
        exit(1);
    }
    int count = 0;
    while (fgets(allowedRollNumbers[count], MAX_ROLL_NUM_LEN, file) != NULL)
    {
        // Remove newline character
        allowedRollNumbers[count][strcspn(allowedRollNumbers[count], "\n")] = '\0';
        count++;
    }
    fclose(file);
    return count;
}

// Function to check if roll number is eligible
bool isEligible(const char *rollNumber, char allowedRollNumbers[][MAX_ROLL_NUM_LEN], int numAllowedRollNumbers)
{
    for (int i = 0; i < numAllowedRollNumbers; i++)
    {
        if (strcmp(rollNumber, allowedRollNumbers[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isRollNumberRegistered(const char *rollNumber) {
    FILE *file = fopen("Users.txt", "r");
    if (file == NULL) {
        return false; // Assume roll number is not registered if file doesn't exist
    }
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        token = strtok(line, ",");
        if (strcmp(token, rollNumber) == 0) {
            fclose(file);
            return true; // Roll number found in file
        }
    }
    fclose(file);
    return false; // Roll number not found in file
}


// Function definition
void writeVotedRollNumberToNota(const char *rollNumber)
{
    FILE *notaFile = fopen("nota_votes.txt", "a");
    if (notaFile != NULL)
    {
        fprintf(notaFile, "%s\n", rollNumber);
        fclose(notaFile);
    }
    else
    {
        printf("Error opening nota_votes.txt for writing.\n");
    }
}