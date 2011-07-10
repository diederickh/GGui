#pragma once

#include "ofMain.h"

/**
 * This class stores all instantiaded GUI objects, it has one
 * instance which is defined as global and extern. This container
 * is used to save, load, show, hide, etc.. the created GGui instances
 *
 * We use the same keyboard shortcuts:
 * .	(dot)		Will save the Gui data
 * ,	(comma)		Will toggle visibility
 *
 */
class GGui;

class GGuis {
public:
	GGuis();
	static GGuis* getInstance();
	void addGui(GGui* gui);
	void setup(ofEventArgs& args);
	void onUpdateOnce(ofEventArgs& args); 
	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(ofKeyEventArgs& args);
	
private:	
	void enableEventListeners();
	void generateNames();
	void loadSettings();
	void saveSettings();
	vector<GGui*> created_guis;
	bool added_events;
};


