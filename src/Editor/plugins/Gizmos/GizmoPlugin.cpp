/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoPlugin.h"
#include "GizmoOperation.h"
#include "GizmoTranslate.h"
#include "GizmoRotate.h"
#include "GizmoScale.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"
#include "UndoManager.h"
#include "Viewframe.h"
#include "PluginManager.h"

#include "../Selection/SelectionPlugin.h"
#include "../Selection/SelectionManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

GizmoPlugin::GizmoPlugin( EditorFrame* frame )
	: Plugin(frame)
	, editorScene(frame->getEditorScene())
	, op(nullptr)
{
	PluginManager* plugins = editor->getPluginManager();
	
	SelectionPlugin* sp = (SelectionPlugin*) plugins->getPlugin("Selection");
	assert( sp != nullptr );

	selections = sp->getSelectionManager();
	assert( selections != nullptr );
}

//-----------------------------------//

PluginMetadata GizmoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Gizmos";
	metadata.description = "Provides translate, rotate and scale tools";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void GizmoPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();

	//addTool( toolBar->AddSeparator() );

	wxBitmap iconCamera = wxMEMORY_BITMAP(camera);
	buttonCamera = toolBar->AddTool( GizmoTool::Camera, "Camera",
		iconCamera, "Selects the Camera View tool", wxITEM_RADIO );
	addTool(buttonCamera);

	wxBitmap iconTranslate = wxMEMORY_BITMAP(move);
	buttonTranslate = toolBar->AddTool( GizmoTool::Translate, "Move",
		iconTranslate, "Selects the Move tool", wxITEM_RADIO );
	addTool(buttonTranslate);

	wxBitmap iconRotate = wxMEMORY_BITMAP(rotate2);
	buttonRotate = toolBar->AddTool( GizmoTool::Rotate, "Rotate",
		iconRotate, "Selects the Rotate tool", wxITEM_RADIO );
	addTool(buttonRotate);

	wxBitmap iconScale = wxMEMORY_BITMAP(scale);
	buttonScale = toolBar->AddTool( GizmoTool::Scale, "Scale",
		iconScale, "Selects the Scale tool", wxITEM_RADIO );
	addTool(buttonScale);
}

//-----------------------------------//

void GizmoPlugin::onPluginDisable()
{
}

//-----------------------------------//

void GizmoPlugin::onToolSelect( int id )
{
	tool = (GizmoTool::Enum) id;

	SelectionOperation* selection = selections->getSelection();

	if( !selection )
		return;

	if( selection->mode == SelectionMode::None )
		return;
	
	for( uint i = 0; i < selection->selections.size(); i++ )
	{
		onEntityUnselect( selection->selections[i].entity );
		onEntitySelect( selection->selections[i].entity );
	}
}

//-----------------------------------//

void GizmoPlugin::onToolUnselect( int id )
{
	if(id >= GizmoTool::Camera && id <= GizmoTool::Scale)
		return;

	SelectionOperation* sel = selections->getSelection();
	
	if( !sel || sel->mode == SelectionMode::None )
		return;

	for( uint i = 0; i < sel->selections.size(); i++ )
	{
		onEntityUnselect( sel->selections[i].entity );
	}

	//std::vector<EntityPtr> entities;

	//GizmoMap::const_iterator it;
	//for( it = gizmos.cbegin(); it != gizmos.cend(); it++ )
	//	entities.push_back( it->first );	
	//
	//for( uint i = 0; i < entities.size(); i++ )	
	//	removeGizmo( entities[i] );

	//assert( gizmos.empty() );
}

//-----------------------------------//

void GizmoPlugin::onEntitySelect( const EntityPtr& entity )
{
	editor->redrawView();

	if( isTool(GizmoTool::Camera) )
		return;
	
	bool gizmoExists = gizmos.find(entity) != gizmos.end();
	
	if( !gizmoExists )
			createGizmo( entity );
}

//-----------------------------------//

void GizmoPlugin::onEntityUnselect( const EntityPtr& entity )
{
	removeGizmo( entity );
	editor->redrawView();
}

//-----------------------------------//

Plane GizmoPlugin::getGizmoPickPlane()
{
	const EntityPtr entityObject( op->weakEntity );
	const TransformPtr& transObject = entityObject->getTransform();

	Vector3 gizmoAxis = gizmo->getAxisVector(op->axis);
	Vector3 gizmoPosition = transObject->getPosition();
	
	Plane planeX( Vector3::UnitX, gizmoPosition.x );
	Plane planeY( Vector3::UnitY, gizmoPosition.y );
	Plane planeZ( Vector3::UnitZ, gizmoPosition.z );

	RenderView* renderView = viewframe->getView();
	const CameraPtr& camera = renderView->getCamera();
	
	Vector2i viewCenter = renderView->getSize() / 2.0f;
	Ray ray = camera->getRay(viewCenter.x, viewCenter.y);
	
	float lengthX = planeX.project(ray.direction).lengthSquared();
	float lengthY = planeY.project(ray.direction).lengthSquared();
	float lengthZ = planeZ.project(ray.direction).lengthSquared();

	if( lengthX < lengthY && lengthX < lengthZ )
		return planeX;
	else if( lengthY < lengthX && lengthY < lengthZ )
		return planeY;
	else
		return planeZ;
}

//-----------------------------------//

bool GizmoPlugin::getGizmoPickPoint(int x, int y, Vector3& pickPoint)
{
	RenderView* renderView = viewframe->getView();
	const CameraPtr& camera = renderView->getCamera();	

	Plane pickPlane = getGizmoPickPlane();	
	Ray ray = camera->getRay(x, y);
	
	float distance;
	if( !pickPlane.intersects(ray, distance) )
		return false;

	pickPoint = ray.getPoint(distance);
	
	return true;
}

//-----------------------------------//

void GizmoPlugin::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !gizmo )
		return;

	if( !(isTool(GizmoTool::Translate)
		|| isTool(GizmoTool::Scale) || isTool(GizmoTool::Rotate)) )
		return;

	// To check if the user picked a gizmo we use two hit test techniques.
	// The first technique is based on bounding volumes so it is not very
	// accurate. If it hits, a more accurate color picking will be done.

	if( pickBoundingTest(moveEvent) && pickImageTest(moveEvent, axis) )
	{
		gizmo->selectAxis(axis);
		
		if( !op )
			createOperation();
		else
			op->axis = axis;
	}
	else
	{
		gizmo->deselectAxis();
		
		delete op;
		op = nullptr;
	}

	editor->redrawView();
}

//-----------------------------------//

void GizmoPlugin::onMouseDrag( const MouseDragEvent& dragEvent )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	if( !op )
		return;
	
	Vector3 pickPoint;
	
	if( !getGizmoPickPoint(dragEvent.x, dragEvent.y, pickPoint) )
		return;

	Vector3 pickAxis = gizmo->getAxisVector(op->axis);
	Vector3 pickOffset = pickAxis.project(pickPoint - firstPickPoint);

	const EntityPtr entityObject( op->weakEntity );
	const TransformPtr& transObject = entityObject->getTransform();
	
	if( isTool(GizmoTool::Translate) )
		transObject->translate( (pickPoint - firstPickPoint)*pickAxis );

	firstPickPoint = pickPoint;

	//else if( isTool(GizmoTool::Scale) )
		//transObject->scale( Vector3(1, 1, 1)+unit*0.05f );

	op->scale = transObject->getScale();
	op->rotation = transObject->getRotation();
	op->translation = transObject->getPosition();

	editor->redrawView();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( isTool(GizmoTool::Camera) )
		return;

	if( mbe.button != MouseButton::Left )
		return;
	
	if( gizmo && gizmo->isAnyAxisSelected() )
	{
		getGizmoPickPoint(mbe.x, mbe.y, firstPickPoint);
		return;
	}
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( !op )
		return;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation( op );
	op = nullptr;
	
	if(gizmo)
		gizmo->deselectAxis();
}

//-----------------------------------//

void GizmoPlugin::createGizmo( const EntityPtr& entity )
{
	assert( entity != nullptr );
	assert( gizmos.find(entity) == gizmos.end() );

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera();

	Gizmo* newGizmo = nullptr;

	// Create new Gizmo for the entity.
	if( isTool(GizmoTool::Translate) )
		newGizmo = new GizmoTranslate(entity, camera);

	else if( isTool(GizmoTool::Rotate) )
		newGizmo = new GizmoRotate(entity, camera);

	else if( isTool(GizmoTool::Scale) )
		newGizmo = new GizmoScale(entity, camera);

	else assert( 0 && "Unknown gizmo tool" );

	gizmo.reset(newGizmo);
	gizmo->buildGeometry();

	// Add the gizmo to the scene.
	EntityPtr entityGizmo( new Entity("Gizmo") );
	entityGizmo->addTransform();
	entityGizmo->addComponent(gizmo);
	editorScene->add(entityGizmo);

	gizmo->updatePositionScale();
	
	// Associate the gizmo with the entity.
	gizmos[entity] = entityGizmo;
}

//-----------------------------------//

void GizmoPlugin::removeGizmo( const EntityPtr& entity )
{
	assert( entity != nullptr );

	// Find the existing gizmo associated with the entity.
	GizmoMap::iterator it = gizmos.find(entity);

	if( it == gizmos.end() )
		return;

	// Find the entity of the gizmo.
	const EntityPtr& entityGizmo = gizmos[entity];
	
	// Remove the gizmo.
	editorScene->remove(entityGizmo);
	gizmos.erase(it);
	gizmo.reset();
}

//-----------------------------------//

bool GizmoPlugin::pickImageTest( const MouseMoveEvent& moveEvent,
								 GizmoAxis::Enum& axis )
{
	RenderView* view = viewframe->getView();
	Vector2i size = view->getSize();

	// We need to flip the Y-axis due to a mismatch between the 
	// OpenGL and wxWidgets coordinate-system origin conventions.
	int mouseX = moveEvent.x;
	int mouseY = size.y - moveEvent.y;

	RenderDevice* device = engine->getRenderDevice();
	Color pick = device->getPixel(mouseX, mouseY);

	axis = gizmo->getAxis(pick);
	return axis != GizmoAxis::None;
}

//-----------------------------------//

bool GizmoPlugin::pickBoundingTest( const MouseMoveEvent& me )
{
	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( me.x, me.y, &outFar );

	// Perform ray casting to find the entities.
	RayQueryResult res;
	
	if( !editorScene->doRayBoxQuery(pickRay, res) )
		return false;

	// Find out if we picked a gizmo...
	GizmoPtr gizmo = res.entity->getComponent<Gizmo>();
	
	return gizmo != nullptr;
}

//-----------------------------------//

void GizmoPlugin::createOperation()
{
	assert( op == nullptr );
	
	EntityPtr entityObject = gizmo->getEntityAttachment();
	TransformPtr transObject = entityObject->getTransform();

	op = new GizmoOperation();

	op->tool = tool;
	op->axis = axis;
	op->gizmo = gizmo;
	op->weakEntity = entityObject;

	op->prevTranslation = transObject->getPosition();
	op->prevScale = transObject->getScale();
	op->prevRotation = transObject->getRotation();
}

//-----------------------------------//

bool GizmoPlugin::isTool(GizmoTool::Enum mode)
{
	return mode == tool;
}

//-----------------------------------//

} } // end namespaces