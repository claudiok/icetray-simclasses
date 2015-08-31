/** $Id$
 * @file
 * @author Jakob van Santen <jakob.van.santen@desy.de>
 *
 * $Revision$
 * $Date$
 */

#ifndef SIMCLASSES_POLYGON_H_INCLUDED
#define SIMCLASSES_POLYGON_H_INCLUDED

#include <dataclasses/I3Direction.h>

class I3Position;
class I3RandomService;

namespace simclasses { namespace polygon {

struct vec2 {
	vec2(double xi, double yi);
	static vec2 from_I3Position(const I3Position &pos);
	static vec2 normalized(double xi, double yi);

	double x, y;

private:
	vec2() {};
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive &, unsigned);
};

struct side {
	side(const vec2 &point, const vec2 &next_point);
	vec2 origin;
	vec2 vector;
	double length;
	I3Direction normal;
};

class triangle2 {
public:
	triangle2(const vec2 &a, const vec2 &b, const vec2 &c);
	
	vec2 center() const;
	double area() const;
	vec2 sample_point(I3RandomService &rng) const;
private:
	vec2 vertex_, side1_, side2_;
};

/// @brief Computes the convex hull of a set of 2D points.
/// Input: an iterable sequence of (x, y) pairs representing the points.
/// Output: a list of vertices of the convex hull in counter-clockwise order,
///   starting from the vertex with the lexicographically smallest coordinates.
/// Implements Andrew's monotone chain algorithm. O(n log n) complexity.
std::vector<vec2> convex_hull(const std::vector<I3Position> &positions);

/// @brief Computes the hull of a set of 2D points by a heuristic
///
/// param[in] positions to compute a hull for
/// param[in] max_edge_length maximum distance between two positions to
///           consider "inside" the polygon
/// param[in] border_tolerance tolerance for simplifying the polygon. If a
///           vertex is less than this distance from a line joining the
///           previous and nextvertex, it will be removed.
///
/// The hull is computed in 4 steps:
/// 1. Compute the Delaunay triangulation of the set
/// 2. Remove an simplices with edges longer than *max_edge_length* m.
/// 3. Select outer edges of the reduced triangulation
/// 4. Simplify the outline by removing vertices that move the outline by less
///    than *border_tolerance* m.
std::vector<vec2> delaunay_hull(const std::vector<I3Position> &positions,
   double max_edge_length, double border_tolerance);

/// @brief Triangulate a closed, simple polygon
std::vector<triangle2> triangulate(const std::vector<vec2> &polygon);

/// @brief Expand the x-y footprint by moving each edge out by a distance
/// *padding*.
/// 
/// A convex polygon can be offset by moving each vertex parallel to the
/// edges by a distance that is inversely proportional to the sine of the
/// counterclockwise angle between the edges that meet at each vertex.
/// This breaks down for edges that are [anti]parallel or, but neither
/// case should occur for maximally simplified polygons.
std::vector<vec2> expand_polygon(const std::vector<vec2> &hull, double padding);

class triangulated_polygon {
public:
	triangulated_polygon(const std::vector<vec2> &edge);
	double area() const;
	vec2 sample_point(I3RandomService &rng) const;
private:
	std::vector<triangle2> triangles_;
	std::vector<double> cumulative_area_;
};

}}

#endif // SIMCLASSES_POLYGON_H_INCLUDED
