#pragma once
#include "GObject.h"
#include "ofMain.h"
class GINI;
class GGui;
class GSlider : public GObject {
public:

	GSlider(GGui* gui, float* value_ptr);
	virtual void setup();
	virtual void draw();
	virtual void mouseMoved(int x, int y);
	virtual void mouseDragged(int x, int y);
	virtual void mousePressed(int x, int y);
	virtual void mouseEnter(int x, int y);
	virtual void mouseLeave(int x, int y);
	
	virtual void save(GINI* ini);
	virtual void load(GINI* ini);
	virtual float getHeight();
		
	GSlider& min(float n);
	GSlider& max(float n);
	GSlider& step(float n);
	void updateStep();
	
	float knob_width;
	float knob_height;
	float half_knob_width;
	float arc_width;
	float bar_width;
	float bar_height;
	float percentage; // percentage of slide [0-1]
	float min_value;
	float max_value;
	float step_size;
	int num_steps;
	float* value_ptr;

private:
	void updateVertices();
	
	int bar_a, bar_b, bar_c, bar_d;
	int arc_a, arc_b, arc_c, arc_d;
	int knob_a, knob_b, knob_c, knob_d;
	int tex_knob_a, tex_knob_b, tex_knob_c, tex_knob_d;
	vector<int> texcoords_fill;
};

inline void GSlider::updateStep() {
	if(step_size <= 0.0) {
		step_size = 0.01;
	}
	if(step_size != 0) {
		num_steps = (max_value-min_value)/step_size;
	}
}

inline GSlider& GSlider::min(float n) {
	min_value = n;
	updateStep();
	return *this;
}

inline GSlider& GSlider::max(float n) {
	max_value = n;
	updateStep();
	return *this;

}

inline GSlider& GSlider::step(float n) {
	step_size = n;
	updateStep();
	return *this;
}

inline float GSlider::getHeight() {
	return 25;
}
