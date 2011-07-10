#pragma once
#include "ofMain.h"
class GINI;
class GGui;
class GObject {
public:
	enum GObjectStates {
		 GSTATE_NONE
		,GSTATE_ENTER
		,GSTATE_LEAVE
	};
	
	GObject(GGui* gui);
	virtual void setup() = 0;
	virtual void draw() = 0;
	virtual void mouseMoved(int x, int y) { }
	virtual void mouseDragged(int x, int y) { }
	virtual void mousePressed(int x, int y) { } 
	virtual void mouseReleased(int x, int y) { }
	virtual void mouseClick(int x, int y) { } // when clicked inside and released inside
	virtual void mouseEnter(int x, int y) { }
	virtual void mouseLeave(int x, int y) { }
	virtual float getHeight() { }; 
	
	virtual void save(GINI* ini) { }
	virtual void load(GINI* ini) { }
	
	int addVertex(float x, float y);
	int addTexCoord(float x, float y);
	ofVec2f* getVertexPtr(int index);
	void addIndex(int index);
	void flagChanged();

	
	void setLabel(string);
	void setCleanName(string);
	
	float x;
	float y;
	float width;
	float height;	
	string label;
	
	// Each GObject defined a mouse area which triggers calling 
	// onMouseOver, onMouseOut, onMouseDown, etc..
	float mouse_x;
	float mouse_y;
	float mouse_width;
	float mouse_height;
	bool is_mouse_inside;
	bool is_mouse_down_inside;
	int state;
	string clean_name;
protected:	
	
	GGui* gui;
};

inline void GObject::setLabel(string sLabel) {
	label = sLabel;
}

inline void GObject::setCleanName(string name)  {
	clean_name = name;
}
