#include "UniformBufferObject.h"

UBOStatics* UBOStatics::instance = NULL;

UBOStatics* UBOStatics::Get(){
	if(instance == NULL)
		UBOStatics::instance = new UBOStatics();
	return instance;
}

std::vector<UBOinterface *>& UBOStatics::GetBuffers(){  return buffers;}

void UBOStatics::BindAll(unsigned int program){
	for(auto it = buffers.begin(); it != buffers.end(); it++)
		(*it)->BindToShader(program);
}