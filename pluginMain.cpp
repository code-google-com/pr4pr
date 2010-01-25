#include "particleInterpolateCmd.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "XaMiC", "2010", "Any");

	status = plugin.registerCommand( "particleInterpolate", particleInterpolate::creator, particleInterpolate::mySyntax );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}
	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );
	status = plugin.deregisterCommand( "particleInterpolate" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}
	return status;
}
