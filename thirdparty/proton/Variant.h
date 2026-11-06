//  ***************************************************************
//  Variant - Creation date: 04/13/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************
#pragma once

#define C_VAR_SPACE_BYTES 16 //enough to fit a rect

typedef struct {
	float x, y;
} CL_Vec2f;

typedef struct {
	float x, y, z;
} CL_Vec3f;

typedef enum
{
	TYPE_UNUSED,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_VECTOR2,
	TYPE_VECTOR3,
	TYPE_UINT32,
	TYPE_ENTITY,
	TYPE_COMPONENT,
	TYPE_RECT,
	TYPE_INT32
} eType;

typedef struct {
	eType m_type;
	
	union
	{
		unsigned char m_var[C_VAR_SPACE_BYTES]; //large enough so we can use the space for all the datatypes we care about
		
		//don't actually use these, these unions help me look at vars when debugging easier
		float m_as_floats[4];
		unsigned int m_as_uint32s[4];
		int m_as_int32s[4];
	};
	const char *m_string;
} Variant;

void setVarient(Variant *variant, float var)
{
	variant->m_type = TYPE_FLOAT; *((float*)variant->m_var) = var;
}
void setVarient(Variant *variant, unsigned int var)
{
	variant->m_type = TYPE_UINT32; *((unsigned int*)variant->m_var) = var;
}
void setVarient(Variant *variant, int var)
{
	variant->m_type = TYPE_INT32; *((int*)variant->m_var) = var;
}
void setVarient(Variant *variant, const char *var)
{
	variant->m_type = TYPE_STRING; variant->m_string = var;
}
void setVarient(Variant *variant, CL_Vec2f var)
{
	variant->m_type = TYPE_VECTOR2; *((CL_Vec2f*)variant->m_var) = var;
}
void setVarient(Variant *variant, CL_Vec3f var)
{
	variant->m_type = TYPE_VECTOR3; *((CL_Vec3f*)variant->m_var) = var;
}

//a VariantList holds a group of variants, we pass these when we don't know in advance how many variants we want to use

#define C_MAX_VARIANT_LIST_PARMS 8

typedef struct {
	Variant m_variant[C_MAX_VARIANT_LIST_PARMS];
} VariantList;
extern unsigned char * SerializeToMem(const VariantList *vlist, unsigned int *pSizeOut, unsigned char *pDest);

extern int GetSizeOfData(eType type);
