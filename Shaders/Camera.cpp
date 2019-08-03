#include "Camera.h"
#include "utility.h"


//****************************************************************
//	Date:		03/10/2012
//	For:		Computer Graphics Programming SEIS 750
//	Class:		Camera
//	Function:	ZoomInLens
//	Inputs:		none
//	Output:		none
//	Purpose:	Logic for Camera Zoom In using lens
//******************************************************************
void	Camera::ZoomInLens()
{
	fovy -= 0.5;
	fovy = Utility::MinMax(fovy,10.0,85.0);
}
//****************************************************************
//	Date:		03/10/2012
//	For:		Computer Graphics Programming SEIS 750
//	Class:		Camera
//	Function:	ZoomOutLens
//	Inputs:		none
//	Output:		none
//	Purpose:	Logic for Camera Zoom Out using lens
//******************************************************************
void	Camera::ZoomOutLens()
{
	fovy += 0.5;

	fovy = Utility::MinMax(fovy,10.0,85.0);
}
//****************************************************************
//	Date:		03/10/2012
//	For:		Computer Graphics Programming SEIS 750
//	Class:		Camera
//	Function:	ZoomInPosition
//	Inputs:		none
//	Output:		none
//	Purpose:	Logic for Camera Zoom In using camera position
//******************************************************************
void	Camera::ZoomInPosition()
{
	eye.z -= 0.5;
	eye.z = Utility::MinMax(eye.z,20.0,100.0);
}
//****************************************************************
//	Date:		03/10/2012
//	For:		Computer Graphics Programming SEIS 750
//	Class:		Camera
//	Function:	ZoomOutPosition
//	Inputs:		none
//	Output:		none
//	Purpose:	Logic for Camera Zoom Out using camera position
//******************************************************************
void	Camera::ZoomOutPosition()
{
	eye.z += 0.5;
	eye.z = Utility::MinMax(eye.z,20.0,100.0);
}



