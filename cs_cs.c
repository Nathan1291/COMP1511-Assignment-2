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
#define TRUE 1
#define FALSE 0



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

// struct for the linked list of trains
struct train {
    struct carriage *carriages;
    
    int size;
    
    // Keeps a tracker of the tail of the list in order to save time
    struct carriage *last_carriage;

    struct train *next;
};

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

void print_current_train(
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

void remove_passengers(
    struct carriage *train 
);

void count_all_passengers(
    struct carriage *train 
);

void count_passengers(
    struct carriage *train 
);

void move_passengers(
    struct carriage *train 
);

void create_train(
    struct train **trains_head,
    struct train *selected
);

void select_next_train(
    struct train **selected
);

void select_prev_train(
    struct train *trains_head,
    struct train **selected
);

void print_all_train(
    struct train *trains_head,
    struct train *selected
);

void remove_carriage(
    struct train *selected
);

void remove_train(
    struct train **trains_head,
    struct train **selected
);

void free_all(
    struct train **trains_head
);

void merge_train(
    struct train *selected
);

void split_train(
    struct train *selected
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main(void) {
    printf("Welcome to Carriage Simulator\n");
    printf("All aboard!\n");

    // new code
    struct train *trains_head = malloc(sizeof(struct train));
    trains_head->carriages = NULL;
    trains_head->last_carriage = NULL;
    trains_head->size = 0;
    trains_head->next = NULL;

    struct train *selected = trains_head;

    // initialises the command variable to store the values for the command loop
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
            append_carriage(&selected->carriages, 
                            &selected->last_carriage, 
                            &selected->size);
        }
        // Stage 1.4: Logic for printing the whole train
        if (command == 'p') {
            print_current_train(selected->carriages);
        }
        // Stage 2.1: Logic for inserting carriages
        if (command == 'i') {
            insert_carriage(&selected->carriages, 
                            &selected->last_carriage, 
                            &selected->size);
        }
        // Stage 2.2: Logic for adding passengers
        if (command == 's') {
            add_passenger(selected->carriages);
        }
        // Stage 2.3: Logic for removing passengers
        if (command == 'd') {
            remove_passengers(selected->carriages);
        }
        // Stage 2.4: Logic for counting all passengers
        if (command == 'T') {
            count_all_passengers(selected->carriages);
        }
        // Stage 2.4: Logic for counting passengers in a given range 
        if (command == 'c') {
            count_passengers(selected->carriages);
        }
        // Stage 2.5: Logic for moving passengers
        if (command == 'm') {
            move_passengers(selected->carriages);
        }
        // Stage 3.1: Logic for creating a new train
        if (command == 'N') {
            create_train(&trains_head, selected);
        }
        // Stage 3.1: Logic for selecting the previous train
        if (command == '>') {
            select_next_train(&selected);
        }
        // Stage 3.1: Logic for selecting the next train
        if (command == '<') {
            select_prev_train(trains_head, &selected);
        }
        // Stage 3.2: Logic for printing the train sequence
        if (command == 'P') {
            print_all_train(trains_head, selected);
        }
        // Stage 3.3: Removing a selected carriage
        if (command == 'r') {
            remove_carriage(selected);
        }
        // Stage 3.4: Removing the selected train
        if (command == 'R') {
            remove_train(&trains_head, &selected);
        }
        // Stage 4.1: Merge the next train with the current train
        if (command == 'M') {
            merge_train(selected);
        }
        // Stage 4.2: Splitting the train at given the given carriage ids
        if (command == 'S') {
            split_train(selected);
        }
        printf("Enter command: ");
    }

    // frees all the memory that we have malloced
    free_all(&trains_head);
    printf("\nGoodbye\n");

    return 0;
}






////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// STAGE 1.1, creates an empty carriage that isnt linked to the linked list
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
    int diff_ids = FALSE;
    for (int i = 0; i < ID_SIZE; i++) {
        if (c_id[i] != id[i]) {
            diff_ids = TRUE;
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
    int has_error = FALSE;
    
    // error check for correct type
    if (type == INVALID_TYPE) {
        has_error = TRUE;
        printf("ERROR: Invalid carriage type\n");
    }
    // error check for valid capacity
    else if (capacity > 999 || capacity <= 0) {
        has_error = TRUE;
        printf("ERROR: Capacity should be between 1 and 999\n");
    }
    // error check for overlapping ids
    else {
        // iterating through the linked list using a temporary pointer
        struct carriage *temp = train;
        while (temp != NULL) {
            // Comparing each value to see if they are a different ID
            // diff_ids stays FALSE if they are not different IDs
            // since it always fails the check
            int diff_ids = check_same_ids(temp->carriage_id, id);

            // printing the error message if they are not different IDs
            if (!diff_ids) {
                has_error = TRUE;
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
    struct carriage **train_ptr, 
    struct carriage **last_carriage_ptr,
    int *size
) {
    // scanning in all the required data
    char carriage_id[6];
    scan_id(carriage_id);
    enum carriage_type type = scan_type();
    int capacity;
    scanf("%d", &capacity);

    int has_error = carriage_error_check(*train_ptr, carriage_id, capacity, type);

    // if there is no errors, add the carriage onto the train
    if (!has_error) {
        // Creating the new carriage struct
        struct carriage *new_carriage = create_carriage(carriage_id, type, capacity);
        new_carriage->next = NULL;

        // if the list is empty then make the head point to the new carriage
        // and make the end point to the new carriage as well
        if (*train_ptr == NULL) {
            *train_ptr = new_carriage;
            *last_carriage_ptr = new_carriage;
        }
        // if the list isnt empty, change the last carriage to point to the new carriage
        // we dereference the last carriage pointer &
        // change the next value then make last carriage the new carriage
        else {
            struct carriage *temp = *last_carriage_ptr;
            temp->next = new_carriage;
            *last_carriage_ptr = new_carriage;
        }
        *size += 1;
        printf("Carriage: '%s' attached!\n", carriage_id);
    }
}


// Stage 1.4, prints the linked list of train 
void print_current_train(
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
    struct carriage **train_ptr, 
    struct carriage **last_carriage_ptr,
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
        has_error = TRUE;
        printf("ERROR: n must be at least 0\n");
    }
    // Error check just like append_carriages
    else {
        has_error = carriage_error_check(*train_ptr, carriage_id, capacity, type);
    }
    
    // if there is no errors, inser the carriage
    if (!has_error) {
        // Creating the new carriage struct
        struct carriage *new_carriage = create_carriage(carriage_id, type, capacity);
        // Case where there isnt a carriage existing
        if (*size == 0) {
            *train_ptr = new_carriage;
            *last_carriage_ptr = new_carriage;
            *size += 1;
        }
        // case where we insert it at the head
        else if (carriage_pos == 0) {
            new_carriage->next = *train_ptr;
            *train_ptr = new_carriage;
            *size += 1;
        }
        // Case where we need to append it to the end
        else if (carriage_pos >= *size) {
            struct carriage *temp = *last_carriage_ptr;
            if (temp == NULL) {
            }   
            temp->next = new_carriage;
            *last_carriage_ptr = new_carriage;
            *size += 1;
        }
        // Case where we need to insert it to some position      
        else {
            struct carriage *temp = *train_ptr;

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

// Stage 2.2, adding passengers to the carriages of the train
// if the passengers are greater than the capacity, overflow the passengers
// to the next carriage of the train
void add_passenger(
    struct carriage *train 
) {
    // scanning all the dependencies
    char id[6];
    scan_id(id);
    int num_passengers;
    scanf("%d", &num_passengers);

    struct carriage *temp = train;

    // error check for a valid n value
    int has_error = FALSE;
    if (num_passengers <= 0) {
        has_error = TRUE;
        printf("ERROR: n must be a positive integer\n");
    }

    // tracker to see if we have found the carriage with the id or not
    // used to create the logic for the olverflow
    int carriage_found = FALSE;

    while (temp != NULL && num_passengers != 0 && !has_error) {
        int diff_ids = check_same_ids(temp->carriage_id, id);
        
        // if the carriage with the id is already found and we need to do a overflow
        if (carriage_found && num_passengers != 0) {
            // if the carriage is already full, do nothing and go to the next carriage
            if (temp->capacity == temp->occupancy) {
                // deliberately empty
            }
            // if the carriage is not empty and there is enough space for all passengers
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
            carriage_found = TRUE;
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
    // Error check for the carriage not being found
    if (!carriage_found && !has_error) {
        printf("ERROR: No carriage exists with id: '%s'\n", id);
    }
    // Case where all passengers could not be seated properly
    else if (num_passengers > 0) {
        printf("%d passengers could not be seated\n", num_passengers);
    }
}

// Stage 2.3, remove a given amount of passengers from a carriage
void remove_passengers(
    struct carriage *train 
) {
    // scanning in all dependencies
    char id[6];
    scan_id(id);
    int num_passengers;
    scanf("%d", &num_passengers);

    struct carriage *temp = train;

    // initialising variables to track if we have found the carriage
    // this prepares an error message
    int carriage_found = 0;

    
    // error check for a valid n value
    int has_error = FALSE;
    if (num_passengers <= 0) {
        has_error = TRUE;
        printf("ERROR: n must be a positive integer\n");
    }

    while (temp != NULL && !has_error) {
        int diff_ids = check_same_ids(temp->carriage_id, id);
        // if the carriage with the given id is found
        if (!diff_ids) {
            // if the number of passengers removed is greater
            // than the occupancy
            if (num_passengers > temp->occupancy) {
                printf("ERROR: Cannot remove %d passengers from %s\n",
                       num_passengers,
                       temp->carriage_id
                );
            }
            // if the number of passengers removed is valid
            else {
                temp->occupancy -= num_passengers;
                printf("%d passengers removed from %s\n",
                       num_passengers,
                       temp->carriage_id
                );
            }
            carriage_found = TRUE;
        }
        temp = temp->next;
    }
    // Error handling if the carriage is not found
    if (!carriage_found && !has_error) {
        printf("ERROR: No carriage exists with id: '%s'\n", id);
    }
}

// Stage 2.4, counts all the passengers and unoccupied spots
void count_all_passengers(
    struct carriage *train 
) {
    // initialising variables
    int total_passengers = 0;
    int total_unoccupied = 0;

    struct carriage *temp = train;

    // going through the list and adding the values as we go along
    while (temp != NULL) {
        total_passengers += temp->occupancy;
        total_unoccupied += temp->capacity - temp->occupancy;

        temp = temp->next;
    }

    // printing the results
    printf("Total occupancy: %d\n", total_passengers);
    printf("Unoccupied capacity: %d\n", total_unoccupied);
}


// Stage 2.4, counts all the passengers and unoccupied spots in a given range
void count_passengers(
    struct carriage *train 
) {
    // Scanning in all dependencies
    char start_id[6];
    scan_id(start_id);    
    char end_id[6];
    scan_id(end_id);

    // initialising variables to use later on
    int passengers = 0;
    int unoccupied = 0;

    struct carriage *temp = train;
    
    // variable to check if the carriage is within the given range
    int in_range = FALSE;
    
    // variable to check if the start and end carriages are in the wrong order
    int wrong_order = FALSE;

    while (temp != NULL) {
        // checking if the current carriage is the end or the start carriage
        int diff_start_ids = check_same_ids(temp->carriage_id, start_id);
        int diff_end_ids = check_same_ids(temp->carriage_id, end_id);

        // if the start carriage is found
        if (!diff_start_ids) {
            in_range = TRUE;
        }
        // the carriage is within the given range
        if (in_range) {
            passengers += temp->occupancy;
            unoccupied += temp->capacity - temp->occupancy;
        }
        // if the end carriage is found
        if (!diff_end_ids) {
            // if the end carriage is found before the start carriage
            if (!in_range) {
                wrong_order = TRUE;
            }
            // close out end range so we dont read anymore values
            in_range = FALSE;
        }
        temp = temp->next;
    }

    // Error handing for when the start carriage is found
    // after the end carriage
    if (wrong_order) {
        printf("ERROR: Carriages are in the wrong order\n");
    }
    // Case where the start carriage was not found
    // Logic: if in range is false, but nothing was added, then we know
    // that the start value was not found
    else if (!in_range && passengers == 0 && unoccupied == 0) {
        printf("ERROR: No carriage exists with id: '%s'\n", start_id);
    }
    // Case where the end carriage was not found
    // Logic: if in range is still 1 then we know that the end carriage was not found
    else if (in_range) {
        printf("ERROR: No carriage exists with id: '%s'\n", end_id);
    }
    // print the result if there are no errors
    else {
        printf("Occupancy: %d\n", passengers);
        printf("Unoccupied: %d\n", unoccupied);
    }
}

// Stage 2.5, moves passengers between carriages, determined by moving a
// given amount of passengers from one source carriage to another, with overflow
// if there are unseated passengers, do not perform the movement
void move_passengers(
    struct carriage *train 
) {
    // scanning in all dependencies
    char source_id[6];
    scan_id(source_id);    
    char dest_id[6];
    scan_id(dest_id);
    int num_to_move;
    scanf("%d", &num_to_move);
    
    
    struct carriage *temp = train;

    // error check for a valid n value
    int has_error = FALSE;
    if (num_to_move <= 0) {
        has_error = TRUE;
        printf("ERROR: n must be a positive integer\n");
    }

    // initialising variables to check if the source car or destination car
    // is found within the given linked list
    int source_car_exists = FALSE;
    int dest_car_exists = FALSE;

    int seats_available = 0;

    // going through the linked list mainly for error checking
    while (temp != NULL && !has_error) {
        // Checking if the current car is the same as the source or destination car
        int diff_source_ids = check_same_ids(temp->carriage_id, source_id);
        int diff_dest_ids = check_same_ids(temp->carriage_id, dest_id);

        // if the source carriage is found
        if (!diff_source_ids) {
            // if the number of passengers to move is greater than occupancy
            if (num_to_move > temp->occupancy) {
                has_error = TRUE;
                printf("ERROR: Cannot remove %d passengers from %s\n",
                       num_to_move,
                       source_id);
            }
            source_car_exists = TRUE;
        }

        // if the destination carriage is found
        if (!diff_dest_ids) {
            dest_car_exists = TRUE;
        }

        // counting the number of available seats from the destination car 
        // to see if we have any unseated passengers
        if (dest_car_exists) {
            seats_available += temp->capacity - temp->occupancy;
        }
        temp = temp->next;
    }
    // error check if the source car is not found
    if (!source_car_exists && !has_error) {
        printf("ERROR: No carriage exists with id: '%s'\n",
               source_id
        );
        has_error = TRUE;
    }
    // error check if the destination car is not found
    if (!dest_car_exists && !has_error) {
        printf("ERROR: No carriage exists with id: '%s'\n",
               dest_id
        );
        has_error = TRUE;
    }
    // error check if there are not enough seats for the passengers 
    if (num_to_move > seats_available && !has_error) {
        printf("ERROR: not enough space to move passengers\n");
        has_error = TRUE;
    }

    temp = train;
    // variable used to see if it is in range to have passengers added
    int in_range = FALSE;
    // variable to keep track of the number of passengers we have left to add
    // so that we keep the num_to_move variable untouched
    int passengers_left_to_add = num_to_move;
    // if there are no errors, execute the movement of the passengers
    while (temp != NULL && !has_error) {
        int diff_source_ids = check_same_ids(temp->carriage_id, source_id);
        int diff_dest_ids = check_same_ids(temp->carriage_id, dest_id);

        // if the source carriage is found
        if (!diff_source_ids) {
            temp->occupancy -= num_to_move;
        }
        // if the destination carriage is found, set in_range to 1 so we start adding
        if (!diff_dest_ids) {
            in_range = TRUE;
        }

        if (in_range && passengers_left_to_add > 0) {
            // if the carriage is already full, do nothing and go to next carriage
            if (temp->capacity == temp->occupancy) {
                // deliberately empty
            }
            // if the vacancy in the carriage isnt enough to accomodate 
            // all the passengers we have to move
            else if (passengers_left_to_add > temp->capacity - temp->occupancy) {
                printf("%d passengers moved from %s to %s\n",
                       temp->capacity-temp->occupancy,
                       source_id,
                       temp->carriage_id
                );
                passengers_left_to_add -= temp->capacity - temp->occupancy;
                temp->occupancy = temp->capacity;
            }
            // if the vacancy is enough for all the remaining passengers
            // add them to the current carriage
            else {
                printf("%d passengers moved from %s to %s\n",
                       passengers_left_to_add,
                       source_id,
                       temp->carriage_id

                );
                temp->occupancy += passengers_left_to_add;
                passengers_left_to_add = 0;
            }
        }
        temp = temp->next;
    }
}

// 3.1, creates a new train 
void create_train(
    struct train **trains_head_ptr,
    struct train *selected
) {
    // Creating an empty train
    struct train *new_train = malloc(sizeof(struct train));
    new_train->carriages = NULL;
    new_train->last_carriage = NULL;
    new_train->size = 0;

    // if the selected train is the head of the train
    // we need to insert before the head so we need to change the head
    if (*trains_head_ptr == selected) {
        new_train->next = selected;
        *trains_head_ptr = new_train;
    }
    // if the selected train isnt at the head
    // go through the list until we find the train pointing to our selected train
    // then we insert it between the 2
    else {
        struct train *temp = *trains_head_ptr;
        while (temp != NULL) {
            if (temp->next == selected) {
                temp->next = new_train;
                new_train->next = selected;
            }
            temp = temp->next;
        }
    }
}

// Stage 3.1, Moves to the selected train to the next train
void select_next_train(
    struct train **selected_ptr
) {  
    // we need to deference the pinter so we can access the elements in the struct
    struct train *selected = *selected_ptr;
    // if there is no train after our selected one
    if (selected->next == NULL) {
        // deliberately empty
    }
    // if there is a next train, then make the next train the selected one
    else {
        *selected_ptr = selected->next;
    }
}

// Stage 3.1, Moves to the selected train to the previous train
void select_prev_train(
    struct train *trains_head_ptr,
    struct train **selected
) {
    // if the selected train is the first in the list, do nothing
    if (trains_head_ptr == *selected) {
        // deliberately empty
    }
    // if the selected train isnt first in the list, select the previous train
    else {
        struct train *temp = trains_head_ptr;
        while (temp != NULL) {
            if (temp->next == *selected) {
                *selected = temp;
            }
            temp = temp->next;
        }
    }
}

// Stage 3.2, print all the trains and their corresponding values
void print_all_train(
    struct train *trains_head,
    struct train *selected
) {
    // Initialising variables to store our information
    int is_selected;
    int n = 0;
    int capacity;
    int occupancy;
    int num_carriages;

    // Looping through each train in the linked list of trains
    struct train *temp_train = trains_head;

    while (temp_train != NULL) {
        //  Checking if the train we are looking at is the selected train
        if (temp_train == selected) {
            is_selected = TRUE;
        }
        else {
            is_selected = FALSE;
        }
        // Resetting the information for each train
        capacity = 0;
        occupancy = 0;
        num_carriages = 0;

        // Looping through each carriage of the current train
        struct carriage *temp_carriage = temp_train->carriages;
        while (temp_carriage != NULL) {
            // Updating the info as we go through each carriage
            capacity += temp_carriage->capacity;
            occupancy += temp_carriage->occupancy;
            num_carriages++;
            
            temp_carriage = temp_carriage->next;
        }
        // Once all the data is collected, print all the data
        print_train_summary(is_selected, n, capacity, occupancy, num_carriages);
        // move onto the next train
        temp_train = temp_train->next;
        // Adding 1 to the counter of the train position
        n++;
    }
}

// Stage 3.3, remove a carraige with a given carriage id
void remove_carriage(
    struct train *selected
) {
    // scanning in the carriage id
    char id[6];
    scan_id(id);

    // Keeps track of the pointer of current carriage and the previous carriage
    struct carriage *curr = selected->carriages;
    struct carriage *prev = selected->carriages;

    // Boolean to keep track if we found the carriage for error handling
    int carriage_found = FALSE;


    // Tracker for the carriage we need to remove
    struct carriage *carriage_to_remove = NULL;

    while (curr != NULL && !carriage_found) {
        // A check to see if the ids of the current carraige 
        // matches the carriage we want to remove
        // return false if the same, true if different
        int diff_ids = check_same_ids(curr->carriage_id, id);

        // if the current carriage is carriage we want to remove
        if (!diff_ids) {
            carriage_found = TRUE;

            // if the current carriage tracker is 
            // the same as the previous carriage tracker
            // we know we are on the first carriage 
            // and therefore only need to change the head
            if (curr == prev) {
                selected->carriages = curr->next;
                carriage_to_remove = curr;
            }
            // if they are not the same
            // then we make the previous carriage point to the carriage
            // that the current carriage points to
            else {
                prev->next = curr->next;
                carriage_to_remove = curr;
            }

            // if the last_carraige pointed to the carraige we want to remove
            // change last carriage to point to the one before it
            if (selected->last_carriage == curr) {
                selected->last_carriage = prev;
            }
        }
        // if the current carriage and the previous carriage is the same
        // then let the current carriage go on but keep the previous carriage there
        if (curr == prev) {
            curr = curr->next;
        }
        // if the current carriage and the previous carriage is different
        // then let both of them proceed
        else {
            prev = curr;
            curr = curr->next;
        }
    }
    // if there wasnt a carriage found, do an error check
    if (!carriage_found) {
        printf("ERROR: No carriage exists with id: '%s'\n", id);
    }
    // else free the carriage that we wanted to remove
    else {
        selected->size -= 1;
        free(carriage_to_remove);
    }
}

// Stage 3.4, remove the selected train and select the train before it
// if the train before it is a head, select the next train
void remove_train(
    struct train **trains_head_ptr,
    struct train **selected_ptr
) {
    // initialising variables for later use
    struct train *train_to_remove = NULL;

    // creating variables so that we can remove 
    struct train *curr = *trains_head_ptr;
    struct train *prev = *trains_head_ptr;

    int train_found = FALSE;
    while (curr != NULL && !train_found) {
        // if the current train is the train we want to remove
        if (curr == *selected_ptr) {
            train_found = TRUE;

            // if the current train tracker is the 
            // same as the previous train tracker
            // we know we are on the first train 
            // and therefore only need to change the head
            if (curr == prev) {
                // initialise this variable to we can access the next property
                struct train *trains_head = *trains_head_ptr;
                // Case where the only train in the linked list is the train head
                if (trains_head->next == NULL) {
                    // Creating a new empty train for trains_head to point to
                    struct train *new_train = malloc(sizeof(struct train));
                    new_train->carriages = NULL;
                    new_train->last_carriage = NULL;
                    new_train->size = 0;
                    new_train->next = NULL;

                    // making the trains_head and selected point to this new carriage
                    *trains_head_ptr = new_train;
                    *selected_ptr = *trains_head_ptr;
                    // Preparing the train we removed to be freed
                    train_to_remove = trains_head;
                }
                // Case where we need to remove the head but it points to another train
                else {
                    // Moving the head to point to the next carriage
                    *trains_head_ptr = curr->next;
                    *selected_ptr = *trains_head_ptr;
                    // Preparing the train we removed to be freed
                    train_to_remove = trains_head;
                }
            }
            // if they are not the same, then we make the 
            // previous carriage point to the carriage
            // that the current carriage points to
            else {
                prev->next = curr->next;
                *selected_ptr = prev;
                train_to_remove = curr;
            }
        }

        // Move curr onto the next carriage but keep previous lagging behind by 1
        prev = curr;
        curr = curr->next;
    }

    // removing the train that we have separated from the linked list
    struct carriage *temp = train_to_remove->carriages;
    struct carriage *carriage_to_remove = NULL;
    while (temp != NULL) {
        carriage_to_remove = temp;
        temp = temp->next;
        free(carriage_to_remove);
    }
    free(train_to_remove);
}   

// Stage 3.5: Function to free all of the memory that we have malloced
void free_all(
    struct train **trains_head
) {
    // creaing temporary train so that we can go through the linked list
    struct train *temp_train = *trains_head;
    // Creating a variable so that we can free it after we move onto the next train
    struct train *train_to_free = NULL;

    // Going through each train in the linked list
    while (temp_train != NULL) {
        train_to_free = temp_train;
        // creates variables to prepare to free the carriages
        struct carriage *temp_carriage = train_to_free->carriages;
        struct carriage *carriage_to_free = NULL;
        // freeing the carriage as we go
        while (temp_carriage != NULL) {
            carriage_to_free = temp_carriage;
            temp_carriage = temp_carriage->next;
            free(carriage_to_free);
        }
        temp_train = temp_train->next;
        // freeing the train after every carriage has been freed
        free(train_to_free);
    }
}

// Stage 4.1: Merge the current train with the next train
// error handling: all i need to do is fix the capacity which apparently isnt working
void merge_train(
    struct train *selected
) {
    // Store the next train in a separate variable
    struct train *next_train = selected->next;

    // error handling in case there is no next train
    int has_error = FALSE;
    if (next_train == NULL) {
        has_error = TRUE;
    }
    if (!has_error) {
        // Make the selected train point to the train that the next train points to 
        selected->next = next_train->next;


        struct carriage *temp_selected = selected->carriages;
        struct carriage *temp_next = next_train->carriages;

        // assigning a variable in case we need to free the memory, 
        // tracking the previous carriage
        // we free the memory if there is an overlap in carriage id
        // freeing the carriage in the next train, 
        // and adding the values to the selected train
        struct carriage *prev = temp_next;
        
        while (temp_next != NULL) {
            temp_selected = selected->carriages;
            // Boolean to track if a given carraige id 
            // already exists in the selected train
            int carriage_id_exists = FALSE;
            while (temp_selected != NULL) {
                int diff_ids = check_same_ids(temp_next->carriage_id, 
                                              temp_selected->carriage_id);
                // if a carraige with the same carriage_id is found
                // then add all the passengers and occupancy to the selected train
                if (!diff_ids) {
                    carriage_id_exists = TRUE;
                    temp_selected->capacity += temp_next->capacity;
                    temp_selected->occupancy += temp_next->occupancy;
                }
                temp_selected = temp_selected->next;
            }

            // move the train on but keep a tracker of the previous 
            // train so that we can do operations on it
            prev = temp_next;
            temp_next = temp_next->next;

            // if carriage with the same id was not found
            // add the carriage to the end of the selected train
            if (!carriage_id_exists) {
                if (selected->carriages == NULL) {
                    prev->next = NULL;
                    selected->carriages = prev;
                    selected->last_carriage = prev;
                }
                else {
                    prev->next = NULL;
                    selected->last_carriage->next = prev;
                    selected->last_carriage = prev;
                }
            }

            // if we found a carriage with the same id, 
            // we can free the memory attached to the 
            // carriage in the next train
            if (carriage_id_exists) {
                free(prev);
            }
        }

        // freeing the train pointer
        free(next_train);
    }
}


// Stage 4.2: Split the train off at the given carriage ids
void split_train(
    struct train *selected
) {
    // Scanning in all dependencies
    int num_of_splits;
    scanf("%d", &num_of_splits);
    
    // tracking the current train we should be splitting on
    struct train *curr_train = selected;
    // tracking the train that this train points to
    struct train *next_train = curr_train->next;
    // making the current train point to nothing
    // so that we can manipulate the train easier,
    // we will recconect at the end using next_train pointer
    curr_train->next = NULL;
    
    // error handing if n is not a positive integer
    if (num_of_splits <= 0) {
        printf("ERROR: n must be a positive integer\n");
    }

    printf("Enter ids: \n");
    // looping through each of the carriage ids
    for (int i = 0; i < num_of_splits; i++) {
        // Scanning in the given carriage id
        char id[ID_SIZE];
        scan_id(id);

        // Starting from the first carriage
        curr_train = selected;
        int carriage_found = FALSE;
        while (curr_train != NULL && !carriage_found) {
            // keeping track of the current carrige and the previous carriage
            // so that its easier to manipulate
            struct carriage *curr_carriage = curr_train->carriages;
            struct carriage *prev_carriage = curr_train->carriages;

            // keeping a track of the size of the carraige
            int size = 1;
            
            // going through each carraige of the given train
            while (curr_carriage != NULL && !carriage_found) {
                int diff_ids = check_same_ids(curr_carriage->carriage_id, id);

                // if the carriage is found
                if (!diff_ids) {
                    carriage_found = TRUE;

                    // Making a new train to branch this one off to
                    // may need to implement a size function but i think should be fine 
                    struct train *new_train = malloc(sizeof(struct train));
                    new_train->carriages = curr_carriage;
                    new_train->last_carriage = curr_train->last_carriage;
                    new_train->size = curr_train->size - size;
                    new_train->next = curr_train->next;

                    // if the carriage is at the start of the train
                    // clean the information of the current train
                    if (curr_carriage == prev_carriage) {
                        curr_train->size = 0;
                        curr_train->carriages = NULL;
                        curr_train->last_carriage = NULL;
                        curr_train->next = new_train;
                    }
                    // if the carriage is not at the start
                    // split the given carriage off and end the current train at the previous carriage
                    else {
                        curr_train->size = size;
                        curr_train->last_carriage = prev_carriage;
                        curr_train->next = new_train;
                        prev_carriage->next = NULL;
                    }
                }
                size++;
                prev_carriage = curr_carriage;
                curr_carriage = curr_carriage->next;
            }
            // going onto the next train in the sequence
            curr_train = curr_train->next;
        }
        // error handling if the carriage is not found in any of the trains
        if (!carriage_found) {
            printf("No carriage exists with id: '%s'. Skipping\n", id);
        }
    }

    // fixing the last train to point to the train that was originally next
    // going through the trains until we reach the end and recconnect it to the original train linked list
    curr_train = selected;
    int completed = FALSE;
    while (curr_train != NULL && !completed) {
        if (curr_train->next == NULL) {
            curr_train->next = next_train;
            completed = TRUE;
        }
        curr_train = curr_train->next;
    }
}



////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints the Carriage simulator usage instructions,
// displaying the different commands and their arguments.a N1010 passenger 10

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
