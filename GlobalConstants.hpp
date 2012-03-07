#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <string>

//startoptions
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//deactivate the cration of the EDGE_HIERARCHY
//less storage consumption depending on data less speed
extern bool EDGE_HIERARCHY_FLAT;   //EdgeHierarchy may have a defect with aggregated edges of a level
								   //may also be just a visual effect not sure

extern std::string STANDARD_NODE;
extern std::string STANDARD_EDGE;

//min and max values for coordinates
extern int MIN_COORD;
extern int MAX_COORD;

//gauss textures consts
extern double GAUSS_TEXTURE_PI;

//painter commander
extern int POINT_INIT_STEP;
extern int PARTS_INIT_STEP;

//texture examiner
extern int TE_KERNEL_SIZE;
extern int TE_MAX_ITERATIONS;


//zooom & quad exponent should be equal to enable direct zoom middle mouse button
extern float ZOOM_BASE;
extern float SIDE_BASE;

//dividedLines
extern int ANGLE_STEP; // 1..45 \in N

//off rendering
extern int OFF_ZOOM;  //2 times zoomed out
extern int OFF_SHRINK; //calculated in a x times smaller field

//labels
extern int START_MAX_LABELS;

//ordering tool
extern bool ORDER_LABELS;

extern std::string OL_START;
extern std::string OL_PARAM;
extern std::string OL_BOX_IDENTIFIER_START;
extern std::string OL_BOX_IDENTIFIER_END;
extern std::string OL_BOX_CENTER_START;
extern std::string OL_BOX_CENTER_DELIM;
extern std::string OL_BOX_CENTER_END;
extern std::string OL_BOX_WIDTH_START;
extern std::string OL_BOX_WIDTH_END;
extern std::string OL_BOX_HEIGHT_START;
extern std::string OL_BOX_HEIGHT_END;
extern std::string OL_ADDITIONAL_BOX_PARAMETERS;
extern std::string OL_END;

extern std::string OL_RETURN_START;
//expected return mode   "OL_RETURN_START id x y"

extern std::string ORDERING_TOOL_PATH;
extern std::string ORDERING_TOOL_PARAMETERS;

#endif