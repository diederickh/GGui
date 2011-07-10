#pragma once
#include <vector>
#include "GObject.h"

using namespace std;

class GGui;
class GButton : public GObject {
public:
	enum GButtonStates {
		 GBUTTON_NONE
		,GBUTTON_HOVER
		,GBUTTON_HOVERED
		,GBUTTON_DOWN
	};
	
	GButton(GGui* gui);
	virtual void setup();
	virtual void draw();
	virtual void mouseMoved(int x, int y);
	virtual void mouseDragged(int x, int y);
	virtual void mousePressed(int x, int y);
	virtual void mouseReleased(int x, int y);
	virtual void mouseClick(int x, int y); 
	virtual float getHeight();
	void setMessage(string msg);

	float button_width;
	float button_height;
	float bar_width;

	vector<int> texcoords;
	int num_texcoords;
	int state;
	string message;
private:


};

inline float GButton::getHeight() {
	return 35;
}