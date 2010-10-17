/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Hungarian Method algorithm.
 * Source code taken from:
 * http://www.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm
 *
 * @author  Kevin Anceau <anceau@kth.se>
 * @author  Andrea Baisero <baisero@kth.se>
 * @author  Carlos Alberto Colmenares <cacol@kth.se>
 * @author  Manuel Parras Ruiz De Azúa <mprda@kth.se>
 *
 */

#include<iostream>
#include<string.h>

using namespace std;

/**
 * Hungarian Algorithm, given a cost matrix NxN
 * calculates the maximum possible assingment of
 * cells. (For further information, go to
 *  http://www.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm)
 *  
 *  @param box_goal_distance    The cost matrix from one box to each
 *                              goal. This costs must be negative, so,
 *                              when the hungarian algorithm finds the
 *                              maximum assingment, it is actually
 *                              taking the minimum.
 *
 * @param num_boxes             The number of boxes on the board
 *
 * @return                      The cost of the minimum assingment
 *                              between boxes and goals.
 */
int hungarian_tc(int (*box_goal_distance)[55], int num_boxes);
