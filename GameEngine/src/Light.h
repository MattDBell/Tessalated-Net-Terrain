#ifndef LIGHT_H
#define LIGHT_H

#include "Matrix.h"
#include "UniformBufferObject.h"

const int MAXLIGHTS = 10; // MUST BE THE SAME IN ANY SHADER

class Light{

	enum MASKS{
		TYPEMASK		= 7,
		ENABLEDMASK		= 1
	};
	enum SHIFTS{
		TYPESHIFT		= 1,
		ENABLEDSHIFT	= 0
	};
	
	struct Lights{
		MVector<4>	position[MAXLIGHTS];
		MVector<4>	color[MAXLIGHTS];
		int			type[MAXLIGHTS];
	};

	static UniformBufferObject<Lights>* lightUBO;
	static Lights lights;
	
	MVector<4>			position;
	MVector<4>			color;
	int					type;
	unsigned __int8		slot; // > MAXLIGHTS if not set

public:
	Light(MVector<4> position, MVector<4> color, int type, unsigned __int8  mySlot);
	void MoveTo(MVector<4> newPosition);
	void ChangeColor(MVector<4> newColor);
	void ChangeType (int newType);
	void ApplyTo(unsigned __int8  newSlot);
};


#endif//LIGHT_H