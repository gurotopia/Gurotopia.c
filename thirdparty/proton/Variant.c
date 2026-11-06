#include "Variant.h"

#include <stdlib.h>

int GetSizeOfData(eType type)
{
    switch(type)
	{
        case TYPE_UNUSED:
        case TYPE_COMPONENT:
        case TYPE_ENTITY:
            return 0;
            break;

        case TYPE_UINT32:
        case TYPE_INT32:
        case TYPE_FLOAT:
            return 4;
        case TYPE_VECTOR2:
            return sizeof(CL_Vec2f);
        case TYPE_VECTOR3:
            return sizeof(CL_Vec3f);

	}

	return 0;
}

unsigned char * SerializeToMem(const VariantList *vlist, unsigned int *pSizeOut, unsigned char *pDest)
{
    int varsUsed = 0;
	int memNeeded = 0;

    int tempSize;

    for (int i=0; i < C_MAX_VARIANT_LIST_PARMS; i++)
    {
        if (vlist->m_variant[i].m_type == TYPE_STRING)
		{
			tempSize = (int)sizeof vlist->m_variant[i].m_string + 4; //the 4 is for an int showing how long the string will be when writing
		} else
		{
			tempSize = GetSizeOfData(vlist->m_variant[i].m_type);
		}
        if (tempSize > 0)
		{
			varsUsed++;
			memNeeded += tempSize;
		}
    }

    int totalSize = memNeeded + 1 + (varsUsed*2);

    if (!pDest)
	{
		pDest = malloc(totalSize*sizeof *pDest); //1 is to write how many are coming
	}
    
	//write it

	
	unsigned char *pCur = pDest;

    pCur[0] = (unsigned char)(varsUsed);
	pCur++;

    unsigned char type;

    for (int i=0; i < C_MAX_VARIANT_LIST_PARMS; i++)
    {
        if (vlist->m_variant[i].m_type == TYPE_STRING)
        {
            type = i;
			memcpy(pCur, &type, 1); pCur += 1; //index

			type = TYPE_STRING;
			memcpy(pCur, &type, 1); pCur += 1; //type

			
			unsigned int s = (int)sizeof vlist->m_variant[i].m_string;
			memcpy(pCur, &s, 4); pCur += 4; //length of string
			memcpy(pCur, vlist->m_variant[i].m_string, s); pCur += s; //actual string data
        } else
		{
			tempSize = GetSizeOfData(vlist->m_variant[i].m_type);

			if (tempSize > 0)
			{
				type = i;
				memcpy(pCur, &type, 1); pCur += 1; //index

				type = vlist->m_variant[i].m_type;
				memcpy(pCur, &type, 1); pCur += 1; //type
				
				memcpy(pCur, vlist->m_variant[i].m_var, tempSize); pCur += tempSize;

			}
		}
    }
    *pSizeOut = totalSize;
	return pDest;
}

