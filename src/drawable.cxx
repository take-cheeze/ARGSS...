#include <GL/gl.h>

#include "drawable.hxx"

void Drawable::setBlendFunc(Blend::Type type)
{
	switch(type) {
		default: // Blend::NORMAL
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Blend::ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case Blend::SUBTRACT:
			glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_COLOR);
			break;
	}
}
