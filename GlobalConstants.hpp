#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H


//startoptions
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 750

//deactivate the cration of the EDGE_HIERARCHY
//less storage consumption depending on data less speed
#define EDGE_HIERARCHY_FLAT false

//min and max values for coordinates
#define MIN_COORD -10000000
#define MAX_COORD  10000000

//gauss textures consts (move to a broader area in the float rep)
#define GAUSS_TEXTURE_PI 31415.926535897
#define GAUSS_TEXTURE_VALUE_MULT 100000.0

//painter commander
#define POINT_INIT_STEP 500

//texture examiner
#define TE_KERNEL_SIZE 16
#define TE_MAX_ITERATIONS 1


//zooom
#define ZOOM_BASE 1.5

//Quads
#define SIDE_BASE 1.2

#endif