// Program name
//
// This program was written by Nathan Lue (z5477755)
// on [date]
//
// TODO: Description of program


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Contants       ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define ID_SIZE 6

// Provided enums
enum carriage_type {INVALID_TYPE, PASSENGER, BUFFET, RESTROOM, FIRST_CLASS};

// TODO: Your #defines/enums can go here:




////////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



// A Train Carriage
struct carriage {
    // carriage id in the form #"N1002", unique, null terminated
    char carriage_id[ID_SIZE];
    //  Type of the carriage
    enum carriage_type type;
    // Maximum number of passengers 
    int capacity;
    // Current number of passengers
    int occupancy;

    struct carriage *next;
};

// TODO: Any additional structs you want to add can go here:




////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void print_usage(void);
void print_carriage(struct carriage *carriage);
void scan_id(char id_buffer[ID_SIZE]);
enum carriage_type scan_type(void);
void print_train_summary(
    int is_selected, 
    int n, 
    int capacity, 
    int occupancy,
    int num_carriages
);
int compare_double(double n1, double n2);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
char *type_to_string(enum carriage_type type);
enum carriage_type string_to_type(char *type_str);





////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct carriage *create_carriage(
    char id[ID_SIZE], 
    enum carriage_type type,
    int capacity
);

int check_same_ids(
    char c_id[ID_SIZE],
    char id[ID_SIZE]
);

int carriage_error_check(
    struct carriage *train,
    char id[6],
    int capacity,
    enum carriage_type type
);

void append_carriage(
    struct carriage **train, 
    struct carriage **last_carriage,
    int *size
);

void print_train(
    struct carriage *train
);

void insert_carriage(
    struct carriage **train, 
    struct carriage **last_carriage,
    int *size
);

void add_passenger(
    struct carriage *train 
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////






int main(void) {
    printf("Welcome to Carriage Simulator\n");
    printf("All aboard!\n");

    // Initialises the head of the linked list, called train
    struct carriage *train = NULL;
    // Keeps a tracker of the tail of the list in order to save time
    struct carriage *last_carriage = NULL;
    // Keeps a track of the size of the linked list
    int size = 0;
    // initialises the command variable to store hte values for the command loop
    char command;
    printf("Enter command: ");

    // Beginning the command loop
    while (scanf(" %c", &command) != EOF) {
        // Stage 1.2: Logic for the help command
        if (command == '?') {
            print_usage();
        }
        // Stage 1.3: Logic for appending a carriage to the train
        if (command == 'a') {
            append_carriage(&train, &last_carriage, &size);
        }
        // Stage 1.4: Logic for printing the whole train
        if (command == 'p') {
            print_train(train);
        }
        // Stage 2.1: Logic for inserting carriages
        if (command == 'i') {
            insert_carriage(&train, &last_carriage, &size);
        }
        // Stage 2.2: Logic for adding passengers
        if (command == 's') {
            add_passenger(train);
        }

        printf("Enter command: ");
    }

    printf("\nGoodbye\n");

    return 0;
}






////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// STAGE 1.1, creates an empty carraige that isnt linked to the linked list
struct carriage *create_carriage(
    char id[ID_SIZE], 
    enum carriage_type type,
    int capacity
) {
    struct carriage *new_carriage = malloc(sizeof(struct carriage));
    
    for (int i = 0; i < ID_SIZE; i++) {
        new_carriage->carriage_id[i] = id[i];
    }

    new_carriage->type = type;
    new_carriage->capacity = capacity;    
    new_carriage->occupancy = 0;
    new_carriage->next = NULL;

    return new_carriage; 
}

// Stage 1.5, function to check if the ids are the same,
// Had to make this to pass styling guide
int check_same_ids(
    char c_id[ID_SIZE],
    char id[ID_SIZE]
) {
    int diff_ids = 0;
    for (int i = 0; i < ID_SIZE; i++) {
        if (c_id[i] != id[i]) {
            diff_ids = 1;
        }
    }
    return diff_ids;
}

// Stage 1.5, checks for errors when appending new carriages
int carriage_error_check(
    struct carriage *train,
    char id[ID_SIZE],
    int capacity,
    enum carriage_type type
) {
    // originally has no errors
    int has_error = 0;
    
    // error check for correct type
    if (type == INVALID_TYPE) {
        has_error = 1;
        printf("ERROR: Invalid carriage type\n");
    }
    // error check for valid capacity
    else if (capacity > 999 || capacity <= 0) {
        has_error = 1;
        printf("ERROR: Capacity should be between 1 and 999\n");
    }
    // error check for overlapping ids
    else {
        // iterating through the linked list using a temporary pointer
        struct carriage *temp = train;
        while (temp != NULL) {
            // Comparing each value to see if they are a different ID
            // diff_ids stays 0 if they are not different IDs
            // since it always fails the check
            int diff_ids = check_same_ids(temp->carriage_id, id);

            // printing the error message if they are not different IDs
            if (!diff_ids) {
                has_error = 1;
                printf("ERROR: a carriage with id: '%s' already exists in this train\n", 
                        id);
            }
            temp = temp->next;
        }
    }
    return has_error;
}

// Stage 1.3, appends a new train at the end of the linked list
void append_carriage(
    struct carriage **train, 
    struct carriage **last_carriage,
    int *size
) {
    // scanning in all the required data
    char carriage_id[6];
    scan_id(carriage_id);
    enum carriage_type type = scan_type();
    int capacity;
    scanf("%d", &capacity);

    int has_error = carriage_error_check(*train, carriage_id, capacity, type);

    // if there is no errors, add the carriage onto the train
    if (!has_error) {
        // Creating the new carriage struct
        struct carriage *new_carriage = create_carriage(carriage_id, type, capacity);
        new_carriage->next = NULL;

        // if the list is empty then make the head point to the new carriage
        // and make the end point to the new carriage as well
        if (*train == NULL) {
            *train = new_carriage;
            *last_carriage = new_carriage;
        }
        // if the list isnt empty, change the last carraige to point to the new carriage
        // we dereference the last carriage pointer &
        // change the next value then make last carraige the new carriage
        else {
            struct carriage *temp = *last_carriage;
            temp->next = new_carriage;
            *last_carriage = new_carriage;
        }
        *size += 1;
        printf("Carriage: '%s' attached!\n", carriage_id);
    }
}


// Stage 1.4, prints the linked list of train 
void print_train(
    struct carriage *train
) {
    struct carriage *temp = train;
    if (temp == NULL) {
        printf("This train is empty!\n");
    }
    while (temp != NULL) {
        print_carriage(temp);
        temp = temp->next;
    }
}

// Stage 2.1, inserting a carriage in the given integer position
void insert_carriage(
    struct carriage **train, 
    struct carriage **last_carriage,
    int *size
) {
    // Scanning all dependencies
    int carriage_pos;
    scanf("%d", &carriage_pos);
    char carriage_id[6];
    scan_id(carriage_id);
    enum carriage_type type = scan_type();
    int capacity;
    scanf("%d", &capacity);

    int has_error;
    // Error check just in case n is not valid
    if (carriage_pos < 0) {
        has_error = 1;
        printf("ERROR: n must be at least 0\n");
    }
    // Error check just like append_carriages
    else {
        has_error = carriage_error_check(*train, carriage_id, capacity, type);
    }
    
    // if there is no errors, inser the carriage
    if (!has_error) {
        // Creating the new carriage struct
        struct carriage *new_carriage = create_carriage(carriage_id, type, capacity);
        // Case where there isnt a carriage existing
        if (*size == 0) {
            *train = new_carriage;
            *last_carriage = new_carriage;
            *size += 1;
        }
        // case where we insert it at the head
        else if (carriage_pos == 0) {
            new_carriage->next = *train;
            *train = new_carriage;
            *size += 1;
        }
        // Case where we need to append it to the end
        else if (carriage_pos >= *size) {
            struct carriage *temp = *last_carriage;
            if (temp == NULL) {
            }   
            temp->next = new_carriage;
            *last_carriage = new_carriage;
            *size += 1;
        }
        // Case where we need to insert it to some position      
        else {
            struct carriage *temp = *train;

            for (int i = 0; i < carriage_pos-1; i++) {
                temp = temp->next;
            }

            new_carriage->next = temp->next;
            temp->next = new_carriage;
            *size += 1;
        }
        printf("Carriage: '%s' inserted!\n", carriage_id);
    }
}

void add_passenger(
    struct carriage *train 
) {
    // scanning all the dependencies
    char id[6];
    scan_id(id);
    int num_passengers;
    scanf("%d", &num_passengers);

    struct carriage *temp = train;

    int has_error = 0;
    if (num_passengers <= 0) {
        has_error = 1;
        printf("ERROR: n must be a positive integer\n");
    }


    // tracker to see if we have found the carriage with the id or not
    // used to create the logic for the olverflow
    int carriage_found = 0;

    while (temp != NULL && num_passengers != 0 && !has_error) {
        int diff_ids = check_same_ids(temp->carriage_id, id);
        
        // if the carriage with the id is already found and we need to do a overflow
        if (carriage_found && num_passengers != 0) {
            // if the carriage is already full, do nothing and go to the next carriage
            if (temp->capacity == temp->occupancy) {
                // deliberately empty
            }
            else if (temp->capacity - temp->occupancy >= num_passengers) {
                temp->occupancy += num_passengers;
                printf("%d passengers added to %s\n", 
                       num_passengers, 
                       temp->carriage_id
                );
                num_passengers = 0;
            }
            // if there isn't enough space for all the passengers
            // use up all available passengers then reduce waiting passengers
            else if (temp->capacity - temp->occupancy < num_passengers) {
                num_passengers -= temp->capacity - temp->occupancy;
                printf("%d passengers added to %s\n", 
                       temp->capacity - temp->occupancy, 
                       temp->carriage_id
            );
                temp->occupancy = temp->capacity;
            }
        }

        // If the carriage with the id is first found, then add the passengers
        if (!diff_ids) {
            carriage_found = 1;
            // if the carriage is already full, do nothing and go to the next carriage
            if (temp->capacity == temp->occupancy) {
                // deliberately empty
            }
            // if there is enough space for all the passengers
            else if (temp->capacity - temp->occupancy >= num_passengers) {
                temp->occupancy += num_passengers;
                printf("%d passengers added to %s\n", 
                       num_passengers, 
                       temp->carriage_id
                );
                num_passengers = 0;
            }
            // if there isn't enough space for all the passengers
            // use up all available passengers then reduce waiting passengers
            else if (temp->capacity - temp->occupancy < num_passengers) {
                num_passengers -= temp->capacity - temp->occupancy;
                printf("%d passengers added to %s\n", 
                       temp->capacity - temp->occupancy, 
                       temp->carriage_id
                );
                temp->occupancy = temp->capacity;
            }
        }
        temp = temp->next;
    }

    if (carriage_found == 0 && !has_error) {
        printf("ERROR: No carriage exists with id: '%s'\n", id);
    }
    else if (num_passengers > 0) {
        printf("%d passengers could not be seated\n", num_passengers);
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints the Carriage simulator usage instructions,
// displaying the different commands and their arguments.
//
void print_usage(void) {
    printf(
        "=====================[ Carriage Simulator ]=====================\n"
        "      ===============[     Usage Info     ]===============      \n"
        "  a [carriage_id] [type] [capacity]                             \n"
        "    Add a carriage to the train                                 \n"
        "  p                                                             \n"
        "    Print out all of the carriages in the train                 \n"
        "  i [n] [carriage_id] [type] [capacity]                         \n"
        "    Insert a carriage into the train at position `n`            \n"
        "                                                                \n"
        "  s [carriage_id] [n]                                           \n"
        "    Seat `n` passengers onto the train starting from carriage   \n"
        "    `carriage_id`                                               \n"
        "  d [carriage_id] [n]                                           \n"
        "    Remove `n` passengers from carriage `carriage_id`           \n"
        "  T                                                             \n"
        "    Display the total number of passengers and empty seats on   \n"
        "    the train                                                   \n"
        "  c [start_id] [end_id]                                         \n"
        "    Display the number of passengers and empty seats on between \n"
        "    carriage `start_id` and carriage `end_id`                   \n"
        "  m [source_id] [destination_id] [n]                            \n"
        "    Move `n` passengers from one carrige to another, without    \n"
        "    kicking anyone off the train.                               \n"
        "  h [carriage_id]                                               \n"
        "    Display the happiness of passengers in carriage             \n"
        "    `carriage_id`                                               \n"
        "  H                                                             \n"
        "    Display the average happiness of all passengers on the train\n"
        "                                                                \n"
        "  N                                                             \n"
        "    Create a new empty train                                    \n"
        "  >                                                             \n"
        "    Select the next train in the train list.                    \n"
        "  <                                                             \n"
        "    Select the previous train in the train list.                \n"
        "  P                                                             \n"
        "    Display the train list.                                     \n"
        "  r [carriage_id]                                               \n"
        "    Remove carriage `carriage_id` from the selected train.      \n"
        "  R                                                             \n"
        "    Remove the selected train.                                  \n"
        "                                                                \n"
        "  M                                                             \n"
        "    Merge the selected train with the train after it.           \n"
        "  S [n]                                                         \n"
        "    Split the current train into smaller trains.                \n"
        "  O                                                             \n"
        "    Rearrange passengers on the selected train to optimise      \n"
        "    happiness.                                                  \n"
        "  ?                                                             \n"
        "    Show help                                                   \n"
        "================================================================\n"
    );
}


// Scan in the a carriage id string into the provided buffer, placing 
// '\0' at the end.
//
// Parameters:
//      id_buffer - a char array of length ID_SIZE, which will be used
//                  to store the id.
// 
// Usage: 
// ```
//      char id[ID_SIZE];
//      scan_id(id);
// ```
void scan_id(char id_buffer[ID_SIZE]) {
    scan_token(id_buffer, ID_SIZE);
}


// Scans a string and converts it to a carriage_type.
//
// Returns:
//      The corresponding carriage_type, if the string was valid,
//      Otherwise, returns INVALID_TYPE.
// 
// Usage: 
// ```
//      enum carriage_type type = scan_type();
// ```
//
enum carriage_type scan_type(void) {
    // This 20 should be #defined, but we've kept it like this to
    // avoid adding additional constants to your code.
    char type[20];
    scan_token(type, 20);
    return string_to_type(type);
}


// Formats and prints out a train carriage struct,
//
// Parameters:
//      carriage - The struct carriage to print.
// 
void print_carriage(struct carriage *carriage) {
    int line_length = 20;

    char *id = carriage->carriage_id;
    char *type = type_to_string(carriage->type);

    printf(" ---------\\/--------- \n");

    int padding = line_length - strlen(id);
    printf("|%*s%s%*s|\n", padding / 2, "", id, (padding + 1) / 2, "");

    padding = line_length - 2 - strlen(type);
    printf("|%*s(%s)%*s|\n", padding / 2, "", type, (padding + 1) / 2, "");

    printf("| Occupancy: %3d/%-3d |\n", 
            carriage->occupancy, 
            carriage->capacity);
    printf(" ---------||--------- \n");
}


// Formats and prints out various information about a given train.
//
// Parameters:
//      is_selected - 1, if this train is the currently selected train, 
//                    0, otherwise.
//      n           - The position of the given train in the list of trains, 
//                    starting from 0.
//      capacity    - The total capacity of the given train.
//      capacity    - The total occupancy of the given train
//      num_carriages   - The number of carriages in the given train.
//
void print_train_summary(
    int is_selected, 
    int n, 
    int capacity, 
    int occupancy,
    int num_carriages
) {
    if (is_selected) {
        printf("--->Train #%d\n", n);
    } else  {
        printf("    Train #%d\n", n);
    }

    printf("        Carriages: %3d\n", num_carriages);
    printf("        Capacity : %3d/%-3d\n", occupancy, capacity);
    printf("    ----------------------\n");

}



// Compares two double (floating point) values. Value are considered
// equal if there is a less than 0.01 difference between them.
// Note: You should use this function if you need to compare doubles
//       to eachother, as it reduces inconsistencies caused by double
//       imprecision.
//
// Parameters:
//      n1 - a floating point value
//      n2 - a floating point value
// 
// Returns:
//      0, if the two values are considered equal.
//      a negative number, if n1 is less than n2,
//      a positive number, if n2 is less than n1,
//
// Usage: 
// ```
//      if (compare_double(n1, n2) > 0) {
//          printf("n1 greater than n2\n");
//      } else if (compare_double(n1, n2) == 0) {
//          printf("n1 is equal to n2\n");
//      } else {
//          printf("n1 is less than n2\n");
//      }
// ```
int compare_double(double n1, double n2) {
    double delta = 0.01;

    double difference = n1 - n2;
    // abs(n1 - n2) < delta 
    if (difference < delta && difference > -delta) {
        return 0;
    }
    if (n1 < n2) {
        return -1;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////// ADDITIONAL PROVIDED FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

enum carriage_type string_to_type(char *type_str) {
    int len = strlen(type_str);

    if (strncasecmp(type_str, "passenger", len) == 0) {
        return PASSENGER;
    } 
    if (strncasecmp(type_str, "buffet", len) == 0) {
        return BUFFET;
    } 
    if (strncasecmp(type_str, "restroom", len) == 0) {
        return RESTROOM;
    }
    if (strncasecmp(type_str, "first_class", len) == 0) {
        return FIRST_CLASS;
    } 

    return INVALID_TYPE;
}


char *type_to_string(enum carriage_type type) {
    if (type == PASSENGER) {
        return "PASSENGER";
    } else if (type == BUFFET) {
        return "BUFFET";
    } else if (type == FIRST_CLASS) {
        return "FIRST CLASS";
    } else if (type == RESTROOM) {
        return "RESTROOM";
    }

    return "INVALID";
}

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // consume all leading whitespace
    scanf(" ");

    // Scan in characters until whitespace
    while (i < buffer_size - 1
        && (num_scanned = scanf("%c", &c)) == 1 
        && !isspace(c)) {

        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}
