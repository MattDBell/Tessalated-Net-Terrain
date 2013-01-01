#ifndef TEXTURE_H
#define TEXTURE_H
class Texture{
public:
	enum TexType{
		TT_GL_TEXTURE_1D					= 0x0DE0,
		TT_GL_TEXTURE_2D					= 0x0DE1,
		TT_GL_TEXTURE_3D					= 0x806F,
		TT_GL_TEXTURE_RECTANGLE				= 0x84F5,
		TT_GL_TEXTURE_BUFFER				= 0x8C2A,
		TT_GL_TEXTURE_CUBE_MAP				= 0x8513,
		TT_GL_TEXTURE_1D_ARRAY				= 0x8C18,
		TT_GL_TEXTURE_2D_ARRAY				= 0x8C1A,
		TT_GL_TEXTURE_CUBE_MAP_ARRAY		= 0x9009,
		TT_GL_TEXTURE_2D_MULTISAMPLE		= 0x9100,
		TT_GL_TEXTURE_2D_MULTISAMPLE_ARRAY	= 0x9102
	};
	enum TextureParams {
		//Params
		TP_GL_TEXTURE_MAG_FILTER	= 0x2800,
		TP_GL_TEXTURE_MIN_FILTER	= 0x2801,
		TP_GL_TEXTURE_WRAP_S		= 0x2802,
		TP_GL_TEXTURE_WRAP_T		= 0x2803,
		TP_GL_TEXTURE_WRAP_R		= 0x8072,
		//Mag Filter
		TP_GL_NEAREST				= 0x2600,
		TP_GL_LINEAR				= 0x2601,
		//Wrap
		TP_GL_CLAMP_TO_EDGE			= 0x812F,
		TP_GL_CLAMP					= 0x2900,
		TP_GL_REPEAT				= 0x2901,
		
		//Formats
		TP_GL_COMPRESSED_RED		= 0x8225,
		TP_GL_COMPRESSED_RG			= 0x8226,
		TP_GL_RG					= 0x8227,
		TP_GL_RG_INTEGER			= 0x8228,
		TP_GL_R8					= 0x8229,
		TP_GL_R16					= 0x822A,
		TP_GL_RG8					= 0x822B,
		TP_GL_RG16					= 0x822C,
		TP_GL_R16F					= 0x822D,
		TP_GL_R32F					= 0x822E,
		TP_GL_RG16F					= 0x822F,
		TP_GL_RG32F					= 0x8230,
		TP_GL_R8I					= 0x8231,
		TP_GL_R8UI					= 0x8232,
		TP_GL_R16I					= 0x8233,
		TP_GL_R16UI					= 0x8234,
		TP_GL_R32I					= 0x8235,
		TP_GL_R32UI					= 0x8236,
		TP_GL_RG8I					= 0x8237,
		TP_GL_RG8UI					= 0x8238,
		TP_GL_RG16I					= 0x8239,
		TP_GL_RG16UI				= 0x823A,
		TP_GL_RG32I					= 0x823B,
		TP_GL_RG32UI				= 0x823C,

		//Channels
		TP_GL_RED					= 0x1903,
		TP_GL_GREEN					= 0x1904,
		TP_GL_BLUE					= 0x1905,
		TP_GL_ALPHA					= 0x1906,
		TP_GL_RGB					= 0x1907,
		TP_GL_RGBA					= 0x1908,
											
		TP_GL_R3_G3_B2				= 0x2A10,
		TP_GL_RGB4					= 0x804F,
		TP_GL_RGB5					= 0x8050,
		TP_GL_RGB8					= 0x8051,
		TP_GL_RGB10					= 0x8052,
		TP_GL_RGB12					= 0x8053,
		TP_GL_RGB16					= 0x8054,
		TP_GL_RGBA2					= 0x8055,
		TP_GL_RGBA4					= 0x8056,
		TP_GL_RGB5_A1				= 0x8057,
		TP_GL_RGBA8					= 0x8058,
		TP_GL_RGB10_A2				= 0x8059,
		TP_GL_RGBA12				= 0x805A,
		TP_GL_RGBA16				= 0x805B
	};
	
	struct TexData{
		unsigned int	target;
		int				level;
		int				width;
		int				height;
		int				depth;
		unsigned int	format;
		unsigned int	type;
		void*			data;
	};

	Texture(TexType t, int format);
	void SetStorage(int width, int height, int depth, int levels);
	void GiveData(TexData &toGive);
	
	void SetParamFloat			(unsigned int	pName, float			param);
	void SetParamInt			(unsigned int	pName, int				param);	
	void SetParamFloatVector	(unsigned int	pName, float*			param);
	void SetParamFloatVector	(unsigned int	pName, int*				param);
	void SetParamIntVector		(unsigned int	pName, int*				param);
	void SetParamIntVector		(unsigned int	pName, unsigned int*	param);

	void GenerateMipMaps();
	void MakeActive(unsigned int slot);
private:
	TexType type;
	int	format;
	unsigned int buffer;
	int boundTo;
	
};

#endif//TEXTURE_H