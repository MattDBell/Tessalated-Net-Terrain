#include "Light.h"

UniformBufferObject<Light::Lights>* Light::lightUBO = NULL;
Light::Lights Light::lights = {0}; 

Light::Light(MVector<4> position, MVector<4> color, int type, unsigned __int8  slot)
	: slot(slot), position(position), color(color), type(type)
{
	if(lightUBO == NULL){
		lightUBO = new UniformBufferObject<Lights>("Lights");
		memset(&lights.type[0], 0, sizeof(lights.type));
		lightUBO->Update(&lights);
	}
	ApplyTo(slot);
}

void Light::ApplyTo(unsigned __int8 slot){
	this->slot = slot;
	if(slot > MAXLIGHTS)
		return;
	lights.position[slot]	= position;
	lights.color[slot]		= color;
	lights.type[slot]		= type;
	
	lightUBO->Update(&lights);
}

void Light::MoveTo(MVector<4> newPosition){
	position = newPosition;
	ApplyTo(slot);
}
void Light::ChangeColor(MVector<4> newColor){
	color = newColor;
	ApplyTo(slot);
}
void Light::ChangeType (int newType){
	type = newType;
	ApplyTo(slot);
}