#include "Light.h"

UniformBufferObject<Light::Lights>* Light::lightUBO = NULL;
Light::Lights Light::lights = {0}; 

Light::Light(MVector<3> position, MVector<3> color, int type, unsigned __int8  slot)
	: slot(slot)
{
	info.position = position;
	info.color = color;
	info.type = type;
	if(lightUBO == NULL){
		lightUBO = new UniformBufferObject<Lights>("Lights");
		for(int i = 0; i < MAXLIGHTS; ++i){
			lights.lights[i].type = 0;
		}
		lightUBO->Update(&lights);
	}
	ApplyTo(slot);
}

void Light::ApplyTo(unsigned __int8 slot){
	this->slot = slot;
	if(slot > MAXLIGHTS)
		return;
	lights.lights[slot] = info;
	//lightUBO->Update(&lights);
	lightUBO->SubUpdate( OFFSETOF(Lights, lights[slot]), sizeof(ShaderLight), (void*)(&lights.lights[slot]));
}

void Light::MoveTo(MVector<3> newPosition){
	info.position = newPosition;
	ApplyTo(slot);
}
void Light::ChangeColor(MVector<3> newColor){
	info.color = newColor;
	ApplyTo(slot);
}
void Light::ChangeType (int newType){
	info.type = newType;
	ApplyTo(slot);
}