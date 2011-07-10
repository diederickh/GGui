#include "GButton.h"
#include "GGui.h"
GButton::GButton(GGui* gui)
	:GObject(gui)
	,num_texcoords(0)
	,state(GBUTTON_NONE)
	,message("")
{
}

void GButton::setup() {
	float string_width = gui->label_font.stringWidth(label);
	bar_width = 6;
	button_height = 29;
	button_width = string_width + 20;

	x += gui->metrics.label_width;
	y += 3;
	mouse_x = x;
	mouse_y = y;
	mouse_width = button_width;
	mouse_height = button_height;
	

	// vertices: left bar
	addVertex(x,y);
	addVertex(x+bar_width,y);
	addVertex(x+bar_width,y+button_height);
	addVertex(x, y+button_height);

	// vertices: button fill
	addVertex(x+bar_width,y);
	addVertex(x+bar_width+button_width-bar_width,y);
	addVertex(x+bar_width+button_width-bar_width,y+button_height);
	addVertex(x+bar_width, y+button_height);

	// vertices: rigth bar
	float start_x = x+bar_width+button_width-bar_width;
	addVertex(start_x,y);
	addVertex(start_x+bar_width,y);
	addVertex(start_x+bar_width,y+button_height);
	addVertex(start_x, y+button_height);

	x -= gui->metrics.label_width;
	
	// texcoord: left part of buton.
	texcoords.push_back(addTexCoord(GTCX(64),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(64+bar_width),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(64+bar_width),GTCY(button_height)));
	texcoords.push_back(addTexCoord(GTCX(64),GTCY(button_height)));
	
	// texcoord: button fill
	texcoords.push_back(addTexCoord(GTCX(71),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(71),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(71),GTCY(button_height)));
	texcoords.push_back(addTexCoord(GTCX(71),GTCY(button_height)));

	// texcoord: right bar
	texcoords.push_back(addTexCoord(GTCX(73),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(81),GTCY(0)));
	texcoords.push_back(addTexCoord(GTCX(81),GTCY(button_height)));
	texcoords.push_back(addTexCoord(GTCX(71),GTCY(button_height)));
	
	num_texcoords = texcoords.size();
	flagChanged();
	y -= 3;
}

void GButton::setMessage(string msg) {
	message = msg;
}

void GButton::draw() {
	float by = (state == GBUTTON_DOWN) ? 1 : 0;
	gui->drawLabel(
		label
		,x+gui->metrics.label_x_offset+gui->metrics.label_width
		,y+gui->metrics.label_y_offset + 6 + by
	);
}


void GButton::mouseMoved(int x, int y) {
	if(is_mouse_inside && state == GBUTTON_NONE) {
		int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x += GTCX(31);
		}
		state = GBUTTON_HOVER;
		flagChanged();
		return;
	}
	
	if(!is_mouse_inside && state == GBUTTON_HOVER) {
		int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x -= GTCX(31);
		}
		state = GBUTTON_NONE;
		flagChanged();
		return;
	}
	
	if(!is_mouse_inside && state == GBUTTON_DOWN) {
			int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x -= GTCX(62);
		}
		state = GBUTTON_NONE;
		flagChanged();
		return;
	}
}

void GButton::mouseDragged(int x, int y) {
}

void GButton::mousePressed(int x, int y) {
	if(is_mouse_inside && state == GBUTTON_HOVER) {
		int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x += GTCX(31);
		}
		state = GBUTTON_DOWN;
		flagChanged();
	}
}

void GButton::mouseReleased(int x, int y) {
	if(is_mouse_inside && state == GBUTTON_DOWN) {
		int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x -= GTCX(31);
		}
		state = GBUTTON_HOVER;
		flagChanged();
		return;
	}
	if(!is_mouse_inside && state == GBUTTON_DOWN) {
		int* ptr = &texcoords.front();
		for(int i = 0; i < num_texcoords; ++i) {
			gui->getTexCoordPtr(ptr[i])->x -= GTCX(62);
		}
		state = GBUTTON_NONE;
		flagChanged();
		return;
	}
}

void GButton::mouseClick(int x, int y) {
	ofSendMessage(message);	
}