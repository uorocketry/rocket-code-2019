#include <stdio.h>

typedef struct { //initialize structure
    float altitude;
    float velocity;
} state;

float time = 2;
float prev_altitude;

// function prototypes
void update_state(state *,float *); // pointer to struct
void compute_velocity(state *,float, float);


void main(){
state init_state = {15.0, 20.0}; // random initial value
printf("struct before update_state and compute_velocity\naltitude: %f, velocity: %f\n\n", init_state.altitude, init_state.velocity);

update_state(&init_state, &prev_altitude); // pass memory address of init_state, pass memory address of prev_altitude
compute_velocity(&init_state, prev_altitude, time); //pass memory address of init_state, pass value of prev_altitude

printf("struct after update_state and compute_velocity\naltitude: %f, velocity: %f\n\n", init_state.altitude, init_state.velocity);
}

void update_state(state *state_ptr, float *altitude_ptr){ // point to memory address (get contents of mem address)
*altitude_ptr = state_ptr->altitude;
state_ptr->altitude = state_ptr->altitude + 5; // the altitude field in the struct that state_ptr points to
// alternative syntax (*state_ptr).altitude
//alternative syntax (*state_ptr).velocity
}

void compute_velocity(state *state_ptr,float prev_altitude, float time){
float current_altitude;
current_altitude = state_ptr->altitude;
state_ptr->velocity = (current_altitude - prev_altitude) / time;
}
