#include "GGuis.h"
#include "GGui.h"
#include <sstream>

GGuis::GGuis() 
	:added_events(false)
{
	ofAddListener(ofEvents.setup, this, &GGuis::setup);
}

GGuis* GGuis::getInstance() {
	static GGuis instance;
	return &instance;
}

void GGuis::setup(ofEventArgs& args) {
}


void GGuis::onUpdateOnce(ofEventArgs& args) {
	ofRemoveListener(ofEvents.update, this, &GGuis::onUpdateOnce);
	generateNames();
	loadSettings();
}

void GGuis::keyPressed(ofKeyEventArgs& args) {

}

void GGuis::keyReleased(ofKeyEventArgs& args) {
	if(args.key == '.') {
		saveSettings();
	}
	else if(args.key == ',') {
		// for now we just hide/show all guis.
		int num = created_guis.size();
		GGui** ptr = (num == 0) ? NULL : &created_guis.front();
		for(int i = 0; i < num; ++i) {
				ptr[i]->toggleVisibility();
		}
	}
}

void GGuis::addGui(GGui* gui) {
	created_guis.push_back(gui);
	if(!added_events) {
		enableEventListeners();
	}
}


void GGuis::enableEventListeners() {
	added_events = true;
	ofRegisterKeyEvents(this);
	ofAddListener(ofEvents.update, this, &GGuis::onUpdateOnce);
}

void GGuis::loadSettings() {
	int num = created_guis.size();
	GGui** ptr = (num == 0) ? NULL : &created_guis.front();
	for(int i = 0; i < num; ++i) {
		if(ptr[i]->hasCleanName()) {
			ptr[i]->load(ptr[i]->getCleanName() +".ini");
		}
	}
}

void GGuis::generateNames() {
	int num = created_guis.size();
	GGui** ptr = (num == 0) ? NULL : &created_guis.front();
	string base = "ggui_settings";
	for(int i = 0; i < num; ++i) {
		if(!ptr[i]->hasCleanName()) {
			stringstream name;
			name << base << i;
			ptr[i]->setName(name.str());
		}
	}
}

void GGuis::saveSettings() {
	int num = created_guis.size();
	GGui** ptr = (num == 0) ? NULL : &created_guis.front();
	for(int i = 0; i < num; ++i) {
		if(ptr[i]->hasCleanName()) {
			ptr[i]->save(ptr[i]->getCleanName() +".ini");
		}		
	}
}
