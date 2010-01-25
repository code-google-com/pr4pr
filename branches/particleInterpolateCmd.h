#ifndef _particleInterpolateCmd
#define _particleInterpolateCmd

#include <maya/MPxCommand.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MPointArray.h>
#include <maya/MSyntax.h>

#define CHECKRESULT(stat, msg) \
	if (MS::kSuccess != stat) \
	{ \
		cerr << msg << endl; \
		return stat; \
	}


class MArgList;

class particleInterpolate : public MPxCommand
{

public:
				particleInterpolate();
	virtual		~particleInterpolate();

	MStatus		doIt( const MArgList& );
	MStatus		redoIt();
	MStatus		undoIt();
	bool		isUndoable() const;
	
	static MSyntax mySyntax();
	static		void* creator();

private:

	MObject particleNode;
	MSelectionList oriSelList;
	double searchingDistance;
	unsigned interpolateCount;
	MPointArray interpolate(MPointArray &points);
	bool visualize;
};

#endif
