#pragma once
#include <vector>
#include "GObject.h"

using namespace std;

class GGui;
class GSeparator : public GObject {
public:
	GSeparator(GGui* gui);
	virtual void setup();
	virtual void draw();
	virtual float getHeight();
private:


};

inline float GSeparator::getHeight() {
	return 25;
}