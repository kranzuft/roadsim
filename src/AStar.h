//
// Created by Aden Prior on 19/4/2023.
//

#ifndef ROADS_ASTAR_H
#define ROADS_ASTAR_H

#include "model.h"

Points aStar(const PointPtr &start, const PointPtr &goal,
                                          const Points &points);

#endif //ROADS_ASTAR_H
