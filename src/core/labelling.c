//
//  labelling.c
//  imagic
//
//  Created by Romain Dubreucq on 07/02/2016.
//  Copyright © 2016 Romain Dubreucq. All rights reserved.
//

#include "labelling.h"

labels_t *laballoc(uint16_t width, uint16_t height)
{
    labels_t *labels = (labels_t *)malloc(sizeof(labels_t));
    labels->len = width * height;
    labels->count = 0;
    labels->data = (uint32_t *)calloc(width * height, sizeof(uint32_t));
    
    return labels;
}

labels_t *label(bini_t *src)
{
    labels_t *labels = laballoc(src->width, src->height);
    
    uint16_t width = src->width;
    uint32_t i = 0, j = 0, k = 0, l = 1, m = 1, n = 0, eB = 0, eA = 0, eC = 0, labelCount = 0, pixcount = src->width * src->height, attA, attB, attC;
    unsigned int *T = (unsigned int *)calloc(src->width * src->height, sizeof(unsigned int));
    
    unsigned int TeB = 0;
    
    uint8_t wpix = 0, attA_eq_attB = 0, attA_eq_attC = 0, attB_eq_attC = 0, eA_eq_eB = 0;
    
    uint32_t jm1_i = 0, j_im1 = 0, j_i = 0;
    
    for (k = 0; k < pixcount; k++)
        T[k] = k;
    
    for (i = 1; i < src->height; i++)
    {
        for (j = 1; j < src->width; j++)
        {
            jm1_i = PXL_IDX(width, j - 1, i);
            j_im1 = PXL_IDX(width, j, i - 1);
            j_i = PXL_IDX(width, j, i);
            
            attA = src->data[jm1_i];
            attB = src->data[j_im1];
            attC = src->data[j_i];
            
            eA = labels->data[jm1_i];
            eB = labels->data[j_im1];
            eC = labels->data[j_i];
            
            wpix = attC == 0;
            attA_eq_attB = attA == attB;
            attA_eq_attC = attA == attC;
            attB_eq_attC = attB == attC;
            eA_eq_eB = eA == eB;
            
            TeB = T[eB];
            
            if (attA_eq_attC && attB_eq_attC && !eA_eq_eB)
            {
                uint8_t TeB_gt_eA = TeB > eA;
                eC = labels->data[j_i];
                T[j_i] = (TeB_gt_eA * eA) + (!TeB_gt_eA * TeB);
                T[eC] = eC;
                T[eA] = eC;
                T[(!TeB_gt_eA * eA) + (TeB_gt_eA * TeB)] = eC;
            }
            
            else
            {
                labelCount = labels->count;
                labels->data[j_i] = wpix * ((attA_eq_attC && !attB_eq_attC) * eA) + ((!attA_eq_attC && attB_eq_attC) * TeB) + ((!attA_eq_attC && !attB_eq_attC) * (labels->count += (!attA_eq_attC && !attB_eq_attC))) + ((attA_eq_attC && attB_eq_attC && eA_eq_eB) * eA);
            }
        }
    }
    
    uint32_t Tl = 0;
    uint8_t Tl_eq_l = 0;
    
    for (l = 1; l < pixcount; l++)
    {
        Tl = T[l];
        Tl_eq_l = Tl == l;
        T[l] = (Tl_eq_l * (m++)) + (!Tl_eq_l * Tl);
    }
    
    
    i = 1;
    j = 1;
    
    unsigned int maxLabel = 1;
    uint32_t lab = 0;
    
    for (i = 1; i < src->height; i++)
    {
        for (j = 1; j < src->width; j++)
        {
            n = PXL_IDX(src->width, j, i);
            lab = labels->data[n];
            labels->data[n] = T[lab];
            lab = labels->data[n];
            
            if (lab > maxLabel)
                maxLabel = lab;
        }
    }
    
    labels->count = maxLabel;
    
    free(T);
    
    return labels;
}

void labfree(labels_t *labels)
{
    free(labels->data);
    free(labels);
}