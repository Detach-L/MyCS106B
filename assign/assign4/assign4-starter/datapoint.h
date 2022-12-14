#pragma once

#include <string>
#include <ostream>
#include <istream>

/* A type representing a data point. This type is used throughout this assignment
 * to represent a piece of information (here, a string label) with its associated
 * priority (here, a double). In principle, you could imagine having much more
 * complex structures consisting of some key (general information) with an
 * associated priority, where the keys are sorted by priority.
 */
struct DataPoint {
    std::string label;
    double priority;
};

/* Some nice utility operators for working with DataPoints. */
bool operator== (const DataPoint& lhs, const DataPoint& rhs);
bool operator!= (const DataPoint& lhs, const DataPoint& rhs);

/* Write a DataPoint to a stream. */
std::ostream& operator<< (std::ostream& out, const DataPoint& pt);

/* Read a DataPoint from a stream. */
std::istream& operator>> (std::istream& in, DataPoint& result);

/* You should not be directly comparing DataPoints against one another using relational
 * operators. If you're getting a compiler error around here, there's a good chance that
 *
 *   1. You are trying to compare DataPoint objects against one another, as in
 *
 *         if (dp1 < dp2) { ... bad! ... }
 *
 *      If that's the case, you'll need to specify what it is that you want to
 *      compare against one another. Did you mean, for example, to compare their
 *      priorities?
 *
 *         if (dp1.priority < dp2.priority) { ... good! ... }
 *
 *   2. You are trying to form a Set<DataPoint> or a Map<DataPoint, Something>.
 *      For the purposes of this assignment, you shouldn't need to do either of these
 *      things. See if there's a different way to accomplish your particular goal.
 */
bool operator<  (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator<= (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator>= (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator>  (const DataPoint& lhs, const DataPoint& rhs) = delete;
