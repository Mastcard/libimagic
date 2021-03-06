//
//  characterization.c
//  imagic
//
//  Created by itzseven on 09/02/2016.
//  Copyright © 2016 itzseven. All rights reserved.
//

#include "characterization.h"

#include <stdio.h>

charact_t *characterizegray8i(gray8i_t *src, labels_t *labels);

regchar_t *regcharalloc(uint32_t id)
{
    regchar_t *reg = (regchar_t *)malloc(sizeof(regchar_t));
    reg->id = id;
    
    reg->size = 0;
    
    reg->sdx = 0;
    reg->sdy = 0;
    reg->sdxy = 0;
    reg->mdir = 0;
    
    reg->redav = 0;
    reg->greenav = 0;
    reg->blueav = 0;
    reg->grayav = 0;
    
    reg->redhist.count = 0;
    reg->greenhist.count = 0;
    reg->bluehist.count = 0;
    reg->grayhist.count = 0;
    
    reg->bounds.start.x = UINT32_MAX;
    reg->bounds.start.y = UINT32_MAX;
    reg->bounds.end.x = 0;
    reg->bounds.end.y = 0;
    
    reg->gravity.x = 0;
    reg->gravity.y = 0;
    
    return reg;
}

regchar_t *regcharcpy(regchar_t *src)
{
    regchar_t *dst = regcharalloc(src->id);
    
    dst->size = src->size;
    
    dst->sdx = src->sdx;
    dst->sdy = src->sdy;
    dst->sdxy = src->sdxy;
    dst->mdir = src->mdir;
    
    dst->redav = src->redav;
    dst->greenav = src->greenav;
    dst->blueav = src->blueav;
    dst->grayav = src->grayav;
    
    dst->redhist.count = src->redhist.count;
    dst->greenhist.count = src->greenhist.count;
    dst->bluehist.count = src->bluehist.count;
    dst->grayhist.count = src->grayhist.count;
    
    memcpy(dst->redhist.data, src->redhist.data, 256);
    memcpy(dst->greenhist.data, src->greenhist.data, 256);
    memcpy(dst->bluehist.data, src->bluehist.data, 256);
    memcpy(dst->grayhist.data, src->grayhist.data, 256);
    
    dst->bounds = src->bounds;
    dst->gravity = src->gravity;
    
    return dst;
}

charact_t *charactalloc(uint32_t count)
{
    charact_t *charact = (charact_t *)malloc(sizeof(charact_t));
    
    charact->count = count;
    charact->data = (regchar_t **)malloc(sizeof(regchar_t *) * count);
    
    uint32_t i = 0;
    
    for (i = 0; i < count; i++)
    {
        regchar_t *reg = regcharalloc(i + 1);
        charact->data[i] = reg;
    }
    
    return charact;
}

charact_t *characterize(rgb8i_t *rgbi, gray8i_t *grayi, labels_t *labels)
{
    charact_t *ch = NULL;
    
    if (rgbi == NULL)
        ch = characterizegray8i(grayi, labels);
    
    return ch;
}

charact_t *characterizegray8i(gray8i_t *src, labels_t *labels)
{
    charact_t *ch = NULL;
    
    if (labels->count > 0)
    {
        ch = charactalloc(labels->count);
        
        uint16_t width = src->width;
        
        uint32_t labelCount = labels->count, labvalue = 0, regidx = 0;
        
        pt2d_t *gravityacc = (pt2d_t *)calloc(labelCount, sizeof(pt2d_t));
        
        unsigned int *grayavacc = (unsigned int *)calloc(labelCount, sizeof(unsigned int));
        
        unsigned int i = 0, j = 0, k = 0;
        
        for (i = 1; i < src->height; i++)
        {
            for (j = 1; j < src->width; j++)
            {
                labvalue = labels->data[PXL_IDX(width, j, i)];
                
                if (labvalue > 0)
                {
                    regidx = labvalue - 1;
                    
                    regchar_t *reg = ch->data[regidx];
                    reg->size++;
                    
                    if (i < reg->bounds.start.y)
                        reg->bounds.start.y = i;
                    
                    if (i > reg->bounds.end.y)
                        reg->bounds.end.y = i;
                    
                    if (j < reg->bounds.start.x)
                        reg->bounds.start.x = j;
                    
                    if (j > reg->bounds.end.x)
                        reg->bounds.end.x = j;
                    
                    gravityacc[regidx].x += j;
                    gravityacc[regidx].y += i;
                }
            }
        }
        
        for (k = 0;  k < ch->count; k++)
        {
            regchar_t *reg = ch->data[k];
            uint32_t size = reg->size;
            
            reg->gravity.x = (uint16_t)(gravityacc[k].x / (uint16_t)size);
            reg->gravity.y = (uint16_t)(gravityacc[k].y / (uint16_t)size);
        }
        
        free(grayavacc);
        free(gravityacc);
    }
    
    return ch;
}

void charactfree(charact_t *charact)
{
    int i = 0;
    
    for (i = 0; i < charact->count; i++)
    {
        free(charact->data[i]);
    }
    
    free(charact->data);
    free(charact);
}