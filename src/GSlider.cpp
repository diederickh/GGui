#include "ofMain.h"
#include "GSlider.h"
#include "GGui.h"
#include "GINI.h"

#include <GLUT/glut.h> // for cursor

GSlider::GSlider(GGui* gui, float* pValue)
	 :max_value(1)
	,min_value(0)
	,step_size(0.01)
	,value_ptr(pValue)
	,percentage(0)
	,arc_width(4)
	,bar_height(9)
	,knob_width(20)
	,knob_height(20)
	,half_knob_width(knob_width*0.5)
	,GObject(gui)
{
	updateStep();
}

void GSlider::setup() {
	bar_width = 160 - (arc_width*2); // arc width is 4, we've got 2 arcs  (left,right)
	mouse_x = x+gui->metrics.label_width - half_knob_width;
	mouse_y = y;
	mouse_width = bar_width + knob_width;
	mouse_height = knob_height;
		
	// create copies.
	x += gui->metrics.label_width; 
	float slider_offset_y = 8;
	y += slider_offset_y;
	
	// Create background
	// -------------------------------------------------------------------------
	// left arc
	addVertex(x, y);		
	addVertex(x+arc_width,y);
	addVertex(x+arc_width,y+bar_height);
	addVertex(x,y+bar_height);

	// bar 
	addVertex(x+arc_width, y);		
	addVertex(x+arc_width+bar_width,y);
	addVertex(x+arc_width+bar_width,y+bar_height);
	addVertex(x+arc_width,y+bar_height);

	// right arc	
	addVertex(x+arc_width+bar_width, y);		
	addVertex(x+arc_width+bar_width+arc_width,y);
	addVertex(x+arc_width+bar_width+arc_width,y+bar_height);
	addVertex(x+arc_width+bar_width,y+bar_height);
	
	// left arc
	addTexCoord(GTCX(0),GTCY(0));
	addTexCoord(GTCX(4),GTCY(0));
	addTexCoord(GTCX(4),GTCY(9));
	addTexCoord(GTCX(0),GTCY(9));

	// bar
	addTexCoord(GTCX(4),GTCY(0));
	addTexCoord(GTCX(4),GTCY(0));
	addTexCoord(GTCX(4),GTCY(9));
	addTexCoord(GTCX(4),GTCY(9));
	
	// right arc.
	addTexCoord(GTCX(15),GTCY(0));
	addTexCoord(GTCX(18),GTCY(0));
	addTexCoord(GTCX(18),GTCY(9));
	addTexCoord(GTCX(15),GTCY(9));
	
	// Create fill
	//--------------------------------------------------------------------------
	// fill left arc
	addVertex(x+1, y+1);		
	addVertex(x+1+arc_width,y+1);
	addVertex(x+1+arc_width,y+1+bar_height);
	addVertex(x+1,y+1+bar_height);
	
	// fill bar 
	bar_a = addVertex(x+arc_width, y+1);		
	bar_b = addVertex(x+arc_width+bar_width,y+1);
	bar_c = addVertex(x+arc_width+bar_width,y+1+bar_height);
	bar_d = addVertex(x+arc_width,y+1+bar_height);

	// right arc	
	arc_a = addVertex(x+arc_width+bar_width, y+1);		
	arc_b = addVertex(x+arc_width+bar_width+arc_width-1,y+1);
	arc_c = addVertex(x+arc_width+bar_width+arc_width-1,y+1+bar_height);
	arc_d = addVertex(x+arc_width+bar_width,y+1+bar_height);
	
	// texcoord fill arc left
	texcoords_fill.push_back(addTexCoord(GTCX(0),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16+9)));
	texcoords_fill.push_back(addTexCoord(GTCX(0),GTCY(16+9)));

	// texcoord fill bar 
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16+9)));
	texcoords_fill.push_back(addTexCoord(GTCX(4),GTCY(16+9)));

	// right arc.
	texcoords_fill.push_back(addTexCoord(GTCX(15),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(18),GTCY(16)));
	texcoords_fill.push_back(addTexCoord(GTCX(18),GTCY(16+9)));
	texcoords_fill.push_back(addTexCoord(GTCX(15),GTCY(16+9)));
	
	
	// Create knob
	//--------------------------------------------------------------------------
	y -= slider_offset_y;
	y+= 3;
	knob_a = addVertex(x-half_knob_width,y);
	knob_b = addVertex(x+half_knob_width, y);
	knob_c = addVertex(x+half_knob_width, y+knob_height);
	knob_d = addVertex(x-half_knob_width, y + knob_height);

	tex_knob_a = addTexCoord(GTCX(0),GTCY(32));
	tex_knob_b = addTexCoord(GTCX(20),GTCY(32));
	tex_knob_c = addTexCoord(GTCX(20),GTCY(32+20));
	tex_knob_d = addTexCoord(GTCX(0),GTCY(32+20));
	y-=3;
	
	flagChanged();
	
	x -= gui->metrics.label_width; 

	updateVertices();
}

void GSlider::updateVertices() {
	x += gui->metrics.label_width; 
	
	// new position for fill.
	float new_width = bar_width * percentage;
	getVertexPtr(bar_b)->x = x+arc_width+new_width;
	getVertexPtr(bar_c)->x = x+arc_width+new_width;
	
	
	// do we need to draw the right arc?
	if((x+arc_width+new_width) < (x+arc_width+bar_width)){
		getVertexPtr(arc_b)->x = x+arc_width+bar_width;
		getVertexPtr(arc_c)->x = x+arc_width+bar_width;		
	}
	
	// new position for knob.
	getVertexPtr(knob_a)->x = x+(new_width)-half_knob_width;
	getVertexPtr(knob_b)->x = x+(new_width)+half_knob_width;	
	getVertexPtr(knob_c)->x = x+(new_width)+half_knob_width;	
	getVertexPtr(knob_d)->x = x+(new_width)-half_knob_width;
	
	x -= gui->metrics.label_width; 
	flagChanged();
}

void GSlider::draw() {
	gui->drawLabel(
		label
		,x+gui->metrics.label_x_offset
		,y+gui->metrics.label_y_offset
	);
	gui->drawValue(
		ofToString(*value_ptr,3)
		,x+gui->metrics.row_width - 60
		,y+gui->metrics.label_y_offset
	);
}
void GSlider::mouseEnter(int x, int y) {
	//glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
	gui->getTexCoordPtr(tex_knob_a)->x += GTCX(32);
	gui->getTexCoordPtr(tex_knob_b)->x += GTCX(32);
	gui->getTexCoordPtr(tex_knob_c)->x += GTCX(32);
	gui->getTexCoordPtr(tex_knob_d)->x += GTCX(32);
	
	int* ptr = &texcoords_fill.front();
	int num = texcoords_fill.size();
	for(int i = 0; i < num; ++i) {
		gui->getTexCoordPtr(ptr[i])->x += GTCX(32);
	}
	flagChanged();
}

void GSlider::mouseLeave(int x, int y) {
	//glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	gui->getTexCoordPtr(tex_knob_a)->x -= GTCX(32);
	gui->getTexCoordPtr(tex_knob_b)->x -= GTCX(32);
	gui->getTexCoordPtr(tex_knob_c)->x -= GTCX(32);
	gui->getTexCoordPtr(tex_knob_d)->x -= GTCX(32);

	int* ptr = &texcoords_fill.front();
	int num = texcoords_fill.size();
	for(int i = 0; i < num; ++i) {
		gui->getTexCoordPtr(ptr[i])->x -= GTCX(32);
	}
	flagChanged();
}

void GSlider::mouseMoved(int x, int y) {

}

void GSlider::mouseDragged(int x, int y) {
	if(is_mouse_inside && is_mouse_down_inside) {
		float xx = CLAMP(x,mouse_x, mouse_x+mouse_width);
		percentage = (xx-mouse_x)/mouse_width;
		updateVertices();
		*value_ptr = min_value + (num_steps * percentage) * step_size;
	}
}

void GSlider::mousePressed(int x, int y) {
	if(is_mouse_inside && is_mouse_down_inside) {
		float xx = CLAMP(x,mouse_x, mouse_x+mouse_width);
		percentage = (xx-mouse_x)/mouse_width;
		*value_ptr = min_value + (num_steps * percentage) * step_size;
		updateVertices();
	}
}

void GSlider::save(GINI* ini) {
	ini->setFloat(clean_name, percentage);
}

void GSlider::load(GINI* ini) {
	percentage = ini->getFloat(clean_name,0);
	*value_ptr = min_value + (num_steps * percentage) * step_size;
	updateVertices();
}