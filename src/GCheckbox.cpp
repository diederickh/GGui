#include "GCheckbox.h"
#include "GGui.h"
#include "GINI.h"
#include <GLUT/glut.h> // for cursor

GCheckbox::GCheckbox(GGui* gui, bool* val) 
	:GObject(gui)
	,value_ptr(val)
{
}

void GCheckbox::setup() {
	box_width = 21;
	box_height = 18;
	mouse_x = x+gui->metrics.label_width;
	mouse_y = y;
	mouse_width = gui->metrics.row_width - (mouse_x);
	mouse_height = gui->metrics.row_height;
		
	// Vertices
	x += gui->metrics.label_width;
	y += 3;
	
	addVertex(x,y);
	addVertex(x+box_width,y);
	addVertex(x+box_width,y+box_height);
	addVertex(x,y+box_height);
	
	x -= gui->metrics.label_width;	
	y -= 3;
	
	// Texcoords 
	tex_a = addTexCoord(GTCX(0),GTCY(64));
	tex_b = addTexCoord(GTCX(box_width),GTCY(64));
	tex_c = addTexCoord(GTCX(box_width),GTCY(64+box_height));
	tex_d = addTexCoord(GTCX(0),GTCY(64+box_height));

	flagChanged();
}

void GCheckbox::draw() {
	gui->drawLabel(
		label
		,x+gui->metrics.label_x_offset+gui->metrics.label_width+box_width
		,y+gui->metrics.label_y_offset
	);
}


void GCheckbox::mouseMoved(int x, int y) {

}

void GCheckbox::mouseDragged(int x, int y) {
}

void GCheckbox::mousePressed(int x, int y) {
}

void GCheckbox::mouseReleased(int x, int y) {
}

void GCheckbox::mouseEnter(int x, int y) {
	glutSetCursor(GLUT_CURSOR_INFO);

}

void GCheckbox::mouseLeave(int x, int y) {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void GCheckbox::mouseClick(int x, int y) {
	*value_ptr = !(*value_ptr);
	updateTexCoords();
	flagChanged();
}

void GCheckbox::updateTexCoords() {
	if(*value_ptr) {
		gui->getTexCoordPtr(tex_a)->x = GTCX(32);
		gui->getTexCoordPtr(tex_b)->x = GTCX(32+box_width);
		gui->getTexCoordPtr(tex_c)->x = GTCX(32+box_width);
		gui->getTexCoordPtr(tex_d)->x = GTCX(32);
	}
	else {
		gui->getTexCoordPtr(tex_a)->x = GTCX(0);
		gui->getTexCoordPtr(tex_b)->x = GTCX(box_width);
		gui->getTexCoordPtr(tex_c)->x = GTCX(box_width);
		gui->getTexCoordPtr(tex_d)->x = GTCX(0);
	}
}

void GCheckbox::save(GINI* ini) {
	ini->setBool(clean_name, *value_ptr);
}

void GCheckbox::load(GINI* ini) {
	*value_ptr = ini->getBool(clean_name, false);
	updateTexCoords();
}