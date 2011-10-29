#pragma once 
#include "ofMain.h"
#include "GSlider.h"
#include "GCheckbox.h"
#include "GObject.h"
#include "GButton.h"
#include "GSeparator.h"
#include "GINI.h"
#include "GGuis.h"

#ifndef eglGetError
	#define eglGetError()
#endif 

// Converting texture coordinates (image width/height = 64)
#define GTCX(col) 	((float)(col)/448)
#define GTCY(row)	((float)(row)/1536.0f)

#define GMOUSE_INSIDE(x,y,i,j,w,h)	((x>=i) && (x<=(i+w)) && (y >= j) && y <= (j+h))
#define GINSIDE_OBJECT(obj,x,y)	GMOUSE_INSIDE(x,y,obj->mouse_x, obj->mouse_y,obj->mouse_width, obj->mouse_height)

// default offsets for a GObject
struct GMetrics {
	float label_x_offset;
	float label_y_offset;
	float label_width;
	float row_width;
	float row_height;
};

class GGui {
public:
	enum GGuiStates {
		STATE_NONE
		,STATE_HIDING
		,STATE_SHOWING
		,STATE_VISIBLE
		,STATE_HIDDEN
	};
	GGui();
	
	void save(string filename);
	void load(string filename);
	
	void update(ofEventArgs& args);
	void draw();	

	GSlider&	addFloat(string title, float& value);
	GCheckbox&	addBool(string title, bool& flag);
	GButton& addButton(string title, string message);
	GSeparator& addSeparator();
		
	int addVertex(float x, float y);
	int addTexCoord(float s, float t);	
	void addIndex(int index);

	void setDataIsChanged(bool isChanged = true);
	ofVec2f* getVertexPtr(int index);
	ofVec2f* getTexCoordPtr(int index);
	
	void drawLabel(string label, int x, int y);
	void drawValue(string label, int x, int y);
	
	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	
	void enableEventListeners();
	void disableEventListeners();

	float x, y;
	
	void	setName(string name);
	bool	hasCleanName();
	string	getCleanName();
	
	bool isVisible();
	void hide();
	void show();
	void toggleVisibility();
		
private:
	string createCleanName(string ugly);
	void setup();
	void checkAssetFiles();
	void createGuiTexture();
	void addObject(GObject* obj);	
	void updateVertices();
	void positionObjects();
	void createOrthoProjectionMatrix();
	string name;
	string clean_name; // lowercased version of name, no spaces 
	bool is_visible;
	int end_tween_on; // we tween in and out of view; this is millis when ended
	int state;

public:
	GMetrics metrics;
	bool is_setup;
	bool is_data_changed;
	vector<GObject*>	objects;
	vector<ofVec2f> 	texcoords;
	vector<int> 		indices;
	vector<ofVec2f> 	vertices;

	GLuint		vbo_vertices;
	GLuint		vbo_texcoords;
	GLuint		gui_texture;
	
	ofTrueTypeFont label_font;
	ofTrueTypeFont value_font;
	
	float ortho_projection[16];
};

extern GGui gui;

inline int GGui::addVertex(float x, float y) {
	vertices.push_back(ofVec2f(x,y));
	return vertices.size()-1;
}
	
inline int GGui::addTexCoord(float s, float t) {
	texcoords.push_back(ofVec2f(s,t));
	return texcoords.size()-1;
}
	

inline void GGui::addIndex(int index) {
	indices.push_back(index);
}

inline void GGui::setDataIsChanged(bool isChanged) {
	is_data_changed = isChanged;
}

inline ofVec2f* GGui::getVertexPtr(int index) {
	return &vertices[index];
}

inline ofVec2f* GGui::getTexCoordPtr(int index) {
	return &texcoords[index];
}

inline void GGui::drawLabel(string label, int x, int y) {
	label_font.drawString(label, x, y);
}

inline void GGui::drawValue(string label, int x, int y) {
	value_font.drawString(label, x, y);
}

// will generate a clean name that is used as filename.
inline void GGui::setName(string n) {
	name = n;
	clean_name = createCleanName(n);
}

inline bool GGui::hasCleanName() {
	return clean_name.size() > 0;
}

inline string GGui::getCleanName() {
	return clean_name;
}

inline string GGui::createCleanName(string ugly) {
	string clean_name;
	for(int i = 0; i < ugly.size(); ++i) {
		if(isalnum(ugly[i])) {
			clean_name.push_back(tolower(ugly[i]));
		}
	}
	return clean_name;
}

inline void GGui::show() {
	end_tween_on = ofGetElapsedTimeMillis() + 300;
	state = STATE_SHOWING;
}
inline void GGui::hide() {
	end_tween_on = ofGetElapsedTimeMillis() + 300;
	state = STATE_HIDING;
}

inline void GGui::toggleVisibility() {
	if(state == STATE_NONE || state == STATE_HIDDEN) {
		show();
	}
	else if(state == STATE_VISIBLE) {
		hide();
	}
}
