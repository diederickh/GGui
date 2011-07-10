#include "GObject.h"
#include "GGui.h"

GObject::GObject(GGui* pGui) 
	:gui(pGui)
	,x(0)
	,y(0)
	,width(0)
	,height(0)
	,is_mouse_inside(false)
	,is_mouse_down_inside(false)
	,state(GSTATE_NONE)
{
}

int GObject::addVertex(float x, float y) {
	return gui->addVertex(x,y);
}

int GObject::addTexCoord(float s, float t) {
	return gui->addTexCoord(s,t);
}

void GObject::addIndex(int index) {
	gui->addIndex(index);
}

void GObject::flagChanged() {
	gui->setDataIsChanged(true);
}

ofVec2f* GObject::getVertexPtr(int index) {
	return gui->getVertexPtr(index);
}