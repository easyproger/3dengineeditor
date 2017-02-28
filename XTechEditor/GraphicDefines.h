#ifndef __GraphicsDefines__
#define __GraphicsDefines__
#define MODE_PICK_OBJECT 0
#define MODE_CAMERA_FREE 1


enum ModeControlls {pick,camera_free};
enum AxisType {AXIS_X,AXIS_Y,AXIS_Z, AXIS_XY, AXIS_XZ, AXIS_YZ, AXIS_XYZ,AXIS_NONE, AxisType_axisSize};
enum ManipulatorType {noManipulator,translate,rotation,scale,manipulatorsCount};
#endif