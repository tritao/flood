/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Gizmo.h"
#include "GizmoPlugin.h"
#include "UndoOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoOperation : public UndoOperation
{
public:

	GizmoOperation();
	
	void undo();
	void redo();
	void process( bool undo );

	NodeWeakPtr weakNode;
	GizmoTool::Enum tool;
	GizmoAxis::Enum axis;
	GizmoPtr gizmo;

	Vector3 prevTranslation;
	EulerAngles prevRotation;
	Vector3 prevScale;

	Vector3 translation;
	EulerAngles rotation;
	Vector3 scale;
};

//-----------------------------------//

} } // end namespaces