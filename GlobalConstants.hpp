#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H


//startoptions
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 750

//deactivate the cration of the EDGE_HIERARCHY
//less storage consumption depending on data less speed
#define EDGE_HIERARCHY_FLAT true   //EdgeHierarchy may have a defect with aggregated edges of a level
								   //may also be just a visual effect not sure

//min and max values for coordinates
#define MIN_COORD -10000000
#define MAX_COORD  10000000

//gauss textures consts
#define GAUSS_TEXTURE_PI 3.1415926535897

//painter commander
#define POINT_INIT_STEP 1000
#define PARTS_INIT_STEP 8

//texture examiner
#define TE_KERNEL_SIZE 16
#define TE_MAX_ITERATIONS 1


//zooom
#define ZOOM_BASE 1.5

//Quads
#define SIDE_BASE 1.3

//dividedLines
#define ANGLE_STEP 1 // 1..45 \in N

//off rendering
#define OFF_ZOOM 2  //2 times zoomed out
#define OFF_SHRINK 1 //calculated in a x times smaller field

//BLENDING
#define COLOR_BLEND_ON true
#define BLEND_MARGIN 0.1

#endif