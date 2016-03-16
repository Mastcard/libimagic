//
//  interests.h
//  imagic
//
//  Created by itzseven on 24/02/2016.
//  Copyright © 2016 itzseven. All rights reserved.
//

#ifndef interests_h
#define interests_h

#include "core.h"
#include "geometry.h"

pt2darray_t *itrtharris(gray8i_t *src, float lambda, uint32_t count);
pt2darray_t *itrtgrad(gray8i_t *src, uint32_t count);

#endif /* interests_h */
