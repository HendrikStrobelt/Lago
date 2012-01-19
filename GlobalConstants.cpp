#include "GlobalConstants.hpp"
#include <string>

//startoptions
int WINDOW_WIDTH = 1500;
int WINDOW_HEIGHT = 750;

//deactivate the cration of the EDGE_HIERARCHY
//less storage consumption theoretical increase in speed but
//seems to be contra productive
bool EDGE_HIERARCHY_FLAT = true;

//min and max values for coordinates
int MIN_COORD = -10000000;
int MAX_COORD = 10000000;

//gauss textures consts
double GAUSS_TEXTURE_PI = 3.1415926535897;

//painter commander
int POINT_INIT_STEP = 1000;
int PARTS_INIT_STEP = 15;

//texture examiner
int TE_KERNEL_SIZE = 16;
int TE_MAX_ITERATIONS = 1;


//zooom & quad exponent should be equal to enable direct zoom middle mouse button
float ZOOM_BASE = 1.3f;
float SIDE_BASE = 1.3f;

//dividedLines
int ANGLE_STEP = 1; // 1..45 \in N

//off rendering
int OFF_ZOOM = 2;  //2 times zoomed out
int OFF_SHRINK = 1; //calculated in a x times smaller field


//labels
int START_MAX_LABELS = 20;

//ordering tool
bool ORDER_LABELS = false;

std::string OL_START = "";
std::string OL_PARAM = "";
std::string OL_BOX_IDENTIFIER_START = "";
std::string OL_BOX_IDENTIFIER_END = "";
std::string OL_BOX_CENTER_START = "";
std::string OL_BOX_CENTER_DELIM = "";
std::string OL_BOX_CENTER_END = "";
std::string OL_BOX_WIDTH_START = "";
std::string OL_BOX_WIDTH_END = "";
std::string OL_BOX_HEIGHT_START = "" ;
std::string OL_BOX_HEIGHT_END = "";
std::string OL_ADDITIONAL_BOX_PARAMETERS = "";
std::string OL_END = "";

std::string OL_RETURN_START = "node";
//expected return mode   "OL_RETURN_START id x y"

std::string ORDERING_TOOL_PATH = "";
std::string ORDERING_TOOL_PARAMETERS = "";