#pragma once
#include "GObject.h"

class GINI;
class GGui;
class GCheckbox : public GObject {
public:
	GCheckbox(GGui* gui, bool* value_ptr);
	virtual void setup();
	virtual void draw();
	virtual void mouseMoved(int x, int y);
	virtual void mouseDragged(int x, int y);
	virtual void mousePressed(int x, int y);
	virtual void mouseReleased(int x, int y);
	virtual void mouseClick(int x, int y); 
	virtual void mouseEnter(int x, int y);
	virtual void mouseLeave(int x, int y);
	virtual float getHeight();
	
	virtual void save(GINI* ini);
	virtual void load(GINI* ini);
	
	float box_width;
	float box_height;
	int tex_a, tex_b, tex_c, tex_d;
	bool checked;
	bool* value_ptr;
private:
	void updateTexCoords();
};

inline float GCheckbox::getHeight() {
	return 25;
}