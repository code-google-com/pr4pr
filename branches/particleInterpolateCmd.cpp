//
// Copyright (C) XaMiC
// 

#include "particleInterpolateCmd.h"
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MVectorArray.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <maya/MArgDatabase.h>


MStatus particleInterpolate::doIt( const MArgList& args)
{
	MStatus stat;
// Get command arguments.
	MArgDatabase argData( syntax(), args, &stat);
	CHECKRESULT(stat, "Could not construct MArgDatabase.");
	argData.getFlagArgument("-sd", 0, searchingDistance);
	argData.getFlagArgument("-ic", 0, interpolateCount);
	argData.getFlagArgument("-v", 0, visualize);

	cout << endl << "Searching distance is " << searchingDistance << "." << endl;
	cout << "Interpolate count is " << interpolateCount << "." << endl;

	// Get particle node from selection.
	MGlobal::getActiveSelectionList(oriSelList);
	return redoIt();
}

// The core function, it is all about it.
MPointArray particleInterpolate::interpolate(MPointArray &points)
{
	MPointArray outputPoints;
	for(unsigned i = 0; i < points.length(); ++i)
	{

		for(unsigned j = (i+1); j < (points.length()); ++j)
		{
			
			MPoint currentPoint(points[i]);
			MPoint nextPoint(points[j]);
			
			double xCurrent = currentPoint.x;
			double yCurrent = currentPoint.y;
			double zCurrent = currentPoint.z;
			double xNext = nextPoint.x;
			double yNext = nextPoint.y;
			double zNext = nextPoint.z;

			double xMin = xCurrent - searchingDistance;
			double xMax = xCurrent + searchingDistance;
			double yMin = yCurrent - searchingDistance;
			double yMax = yCurrent + searchingDistance;
			double zMin = zCurrent - searchingDistance;
			double zMax = zCurrent + searchingDistance;

			if( ((xNext>xMin) && (xNext<xMax)) && ((yNext>yMin) && (yNext<yMax)) && ((zNext>zMin) && (zNext<zMax)) )
			{
				double nBetween = (double)interpolateCount + 1.0;
				double xStep = (xNext - xCurrent) / nBetween;
				double yStep = (yNext - yCurrent) / nBetween;
				double zStep = (zNext - zCurrent) / nBetween;

				for(unsigned k = 1; k <= interpolateCount; ++k)
				{
					MPoint resultPoint(xCurrent+k*xStep, yCurrent+k*yStep, zCurrent+k*zStep, 1.0);
					outputPoints.append(resultPoint);
				}
			}
		}
	}
	
	return outputPoints;
}

MStatus particleInterpolate::redoIt()

{
// Get selected particle node.	
	MStatus stat;
	MItSelectionList list(oriSelList, MFn::kParticle, &stat);
	CHECKRESULT(stat, "Could not create selection list iterator.");
	if(list.isDone())
	{
		MGlobal::displayError("No particle selected.");
		return MS::kFailure;
	}
	stat = list.getDependNode(particleNode);
	CHECKRESULT(stat, "Could not get particle node from selection.");

// Attach function set to original particle node.
	MFnParticleSystem originalPs(particleNode, &stat);
	CHECKRESULT (stat,"Attach function set to original particle failed.");
	if(!originalPs.isValid())
	{
		MGlobal::displayError("The function set is invalid.");
		return MS::kFailure;
	}

// Feed positions into a new MVectorArray.
	const MFnParticleSystem::RenderType oriPsType = originalPs.renderType();	
	MVectorArray posVec;
	if(oriPsType == MFnParticleSystem::kTube)
		originalPs.position0(posVec);
	else
		originalPs.position(posVec);

// Extract positon informations to MPointArray.
	MPointArray pos;
	for(unsigned i = 0; i < posVec.length(); ++i)
	{
		MPoint point(posVec[i]);
		pos.append(point);
	}

// Interpolate.
	MPointArray finalPos = interpolate(pos);
	cout << "Done interpolating " << finalPos.length() << " points." << endl;

// Create new particle system ondemand.
	if(visualize)
	{
		MFnParticleSystem tempPs;	
		particleNode = tempPs.create(&stat);
		CHECKRESULT(stat, "Could not create new particle system.");
		MFnParticleSystem resultPs(particleNode, &stat);
		CHECKRESULT(stat, "Could not attach new particle system function set.");
		stat = resultPs.emit(finalPos);
		CHECKRESULT(stat, "Could not emit particles.");
	}


	return MS::kSuccess;
}

MStatus particleInterpolate::undoIt()
{
	MStatus stat;
	MFnDagNode dagFn(particleNode, &stat);
	CHECKRESULT(stat, "Could not attach MFnDagNode to particleNode.");
	MObject particleTransform = dagFn.parent(0, &stat);
	CHECKRESULT(stat, "Could not get particle tranform node.");
	MGlobal::deleteNode(particleNode);
	MGlobal::deleteNode(particleTransform);
	MGlobal::setActiveSelectionList(oriSelList, MGlobal::kReplaceList);
	return MS::kSuccess;
}

void* particleInterpolate::creator()
{
	return new particleInterpolate();
}

particleInterpolate::particleInterpolate():searchingDistance(1.0), interpolateCount(4), visualize(false)
{}

particleInterpolate::~particleInterpolate()
{}

bool particleInterpolate::isUndoable() const
{
	if(visualize)
		return true;
	else
		return false;
}

MSyntax particleInterpolate::mySyntax()
{
	MSyntax syntax;
	syntax.addFlag("-sd", "-searchingDistance", MSyntax::kDouble);
	syntax.addFlag("-ic", "-interpolateCount", MSyntax::kUnsigned);
	syntax.addFlag("-v", "-visualize", MSyntax::kBoolean);
	return syntax;
}
