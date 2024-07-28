Detailed README file for your C language election voting system:

---

# Election Voting System for College

This is a simple console-based election voting system for a college, written in C. The system allows students to register and vote for candidates, while an election officer can manage the election process, including viewing results and deleting illegal votes.

## Features

1. **Student Registration and Login:**
   - Students can register using their roll number, username, and password.
   - Students can log in and vote for a candidate.
   - The system ensures that each student can vote only once.

2. **Admin (Election Officer) Functions:**
   - Admin can log in using a predefined username and password.
   - Admin can start a new election by entering candidate details.
   - Admin can delete illegal votes.
   - Admin can view election results, including the percentage of votes each candidate received and NOTA (None of the Above) votes.

3. **Vote Management:**
   - Votes are recorded in separate files for each candidate.
   - Votes for NOTA are recorded in a separate file.
   - A voted file keeps track of roll numbers of students who have already voted.

## Files

- `main.c`: Contains the main function and the primary menu for student and admin logins.
- `admin.c`: Contains functions related to the admin operations.
- `student.c`: Contains functions related to student registration, login, and voting.
- `shared.c`: Contains functions shared between admin and student operations.
- `voted.txt`: Stores roll numbers of students who have voted.
- `nota_votes.txt`: Stores roll numbers of students who voted for NOTA.
- `candidates.txt`: Stores the list of candidates.

## Compilation and Execution

### Prerequisites

- A C compiler (GCC, Clang, etc.)
- `conio.h` library for Windows (For Linux, this may need adjustments as `conio.h` is specific to Windows)

### Compilation

Use the following command to compile the program:

```sh
gcc -o election_system main.c
```

### Execution

Run the compiled program using:

```sh
./election_system
```

## Usage

### Main Menu

1. **Student Voter Login:**
   - Register as a new student.
   - Log in as a registered student and vote.

2. **Election Officer Login:**
   - Log in as an admin.
   - Start a new election by entering candidate details.
   - Delete illegal votes.
   - View election results.

3. **Exit:**
   - Exit the program.

### Admin Operations

- **New Election:**
  - Enter the number of candidates.
  - Enter the name of each candidate (must end with a number).

- **Delete Illegal Vote:**
  - Enter the roll number of the student whose vote needs to be deleted.

- **Result:**
  - View the total votes and percentage for each candidate and NOTA.

### Student Operations

- **Voter Register:**
  - Enter roll number, username, and password.
  - Roll number must be in the format `XXUGXXXXX`.

- **Voter Login:**
  - Enter roll number, username, and password.
  - Vote for a candidate or NOTA.

## Functions

### Admin Functions

- `void adminLogin()`: Admin login and menu.
- `int validateAdmin(const char *username, const char *passwd)`: Validate admin credentials.
- `void loadCandidateData()`: Load candidate data from file.
- `void saveCandidateData()`: Save candidate data to file.
- `int isValidCandidateName(const char *name)`: Validate candidate name.

### Student Functions

- `void studentLogin()`: Student login and registration menu.
- `void writeUserToFile(struct User user)`: Write registered user data to file.
- `int findUserByRollNumber(char *rollNumber, char *username, char *passwd)`: Find user by roll number.
- `bool isValidRollNumber(const char *rollNumber)`: Validate roll number format.
- `bool isRollNumberRegistered(const char *rollNumber)`: Check if roll number is already registered.
- `void writeVotedRollNumber(const char *rollNumber)`: Record roll number of a student who has voted.
- `bool hasVoted(const char *rollNumber)`: Check if a student has already voted.

### Shared Functions

- `void getPassword(char *passwd)`: Get password input without echoing characters.

## Data Files

- `Users.txt`: Stores registered users' data (roll number, username, password).
- `candidates.txt`: Stores the list of candidates.
- `voted.txt`: Stores roll numbers of students who have voted.
- `nota_votes.txt`: Stores roll numbers of students who voted for NOTA.
- `<candidate_name>.txt`: Stores roll numbers of students who voted for a specific candidate.

## Notes

- Ensure the roll number format and candidate name format are followed correctly to avoid errors.
- The program uses `conio.h` for `getch()` function which is specific to Windows. For Unix-like systems, consider using alternatives for password masking.
