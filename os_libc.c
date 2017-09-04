#include "easyos.h"

void* osmemset(void *dest,U8 value,U32 size)
{
	if(dest == NULL)
	{
		return NULL;
	}
	U8 *dst = (U8 *)dest;
	U32 num = size;
	U8 val = value;
	while(num-->0) 
	{
		*dst++ = val;
	}
	return dest;
}


void* osmemcpy(void *dest,void *src,U32 size)
{
	if(dest == NULL || src == NULL)
	{
		return NULL;
	} 
    U8 * psrc = (U8 *)src;
    U8 * pdst = (U8 *)dest; 
	U32 num = size; 
    while(num--> 0) 
	{
		*pdst++ = *psrc++;
	}  
    return dest;
}

void * osmemmv(void *dest, void *src, U32 size)  
{  
    if (dest == NULL || src == NULL)  
	{
          return NULL; 
	} 
    U8 * psrc = (U8 *)src;
    U8 * pdst = (U8 *)dest; 
	U32 num = size;   
    if (pdst > psrc && pdst < psrc+num)  
    {  
		U32 i;
        for(i=num-1;i!=-1;--i)  
        {  
			pdst[i] = psrc[i];  
        }  
    }  
    else  
    {  
		while(num--> 0)
		{
			*pdst++ = *psrc++;
		}
    }  
      
    return dest;  
} 
