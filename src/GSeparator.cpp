#include "GSeparator.h"
#include "GGui.h"
GSeparator::GSeparator(GGui* gui)
	:GObject(gui)
{
}

void GSeparator::setup() {
	x += gui->metrics.label_x_offset;
	y += 10;
	
	mouse_x = 0;
	mouse_y = 0;
	mouse_width = 0;
	mouse_height = 0;
	float w = 273;
	float m = gui->metrics.row_width * 0.5;
	float start_x = m - (w*0.5);

	addVertex(start_x,y);
	addVertex(start_x+w,y);
	addVertex(start_x+w,y+6);
	addVertex(start_x,y+6);
	
	x -= gui->metrics.label_x_offset;
	y -= 10;
	
	// texcoord: left part of buton.
	addTexCoord(GTCX(178),GTCY(0));
	addTexCoord(GTCX(178+270),GTCY(0));
	addTexCoord(GTCX(178+270),GTCY(6));
	addTexCoord(GTCX(178),GTCY(6));
	
	flagChanged();

}


void GSeparator::draw() {
}