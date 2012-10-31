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

	struct ShaderLight{
		MVector<3>	position; // 4 * 3 = 12 TOT 12
		MVector<3>	color;	  // 4 * 3 = 12 TOT 24
		int		type;	  // 4			TOT 28
	};
	
	struct Lights{
		ShaderLight lights[MAXLIGHTS]; // MAXLIGHTS(10) * 28 = 280
	};

	static UniformBufferObject<Lights>* lightUBO;
	static Lights lights;
	
	ShaderLight			info;
	unsigned __int8		slot; // > MAXLIGHTS if not set

public:
	Light(MVector<3> position, MVector<3> color, int type, unsigned __int8  mySlot);
	void MoveTo(MVector<3> newPosition);
	void ChangeColor(MVector<3> newColor);
	void ChangeType (int newType);
	void ApplyTo(unsigned __int8  newSlot);
};


#endif//LIGHT_H