/** $Id$
 * @file
 * @author Jakob van Santen <jakob.van.santen@desy.de>
 *
 * $Revision$
 * $Date$
 */

#include <simclasses/detail/polygon.h>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <simclasses/delaunay/delaunay.h>
#include <phys-services/I3RandomService.h>

// specialize boost::checked_delete to teach boost::scoped_ptr how to release
// handles
namespace boost {

template <>
void checked_delete(delaunay2d_t* t)
{
	if (t != NULL)
		delaunay2d_release(t);
}

template <>
void checked_delete(tri_delaunay2d_t* t)
{
	if (t != NULL)
		tri_delaunay2d_release(t);
}

}

namespace simclasses { namespace polygon {

namespace {

/// A counterclockwise curve is the basic building block of a convex hull
class ccw_curve : public std::vector<vec2> {
public:
	// Add a point to the curve
	void operator()(const vec2 &p)
	{
		// Remove points until the curve will be counterclockwise
		while (size() >= 2 && !ccw((*this)[size()-2], (*this)[size()-1], p))
			pop_back();
		push_back(p);
	}
	static bool
	ccw(const vec2 &o, const vec2 &a, const vec2 &b)
	{
		// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
		// positive, if OAB makes a counter-clockwise turn,
		// negative for clockwise turn, and zero if the points are collinear.
		return cross(o, a, b) > 0;
	}
	static double
	cross(const vec2 &o, const vec2 &a, const vec2 &b)
	{
		return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
	}
};

double
operator*(const vec2 &a, const vec2 &b)
{
	return a.x*b.x + a.y*b.y;
}

vec2
operator*(double a, const vec2 &b)
{
	return vec2(a*b.x, a*b.y);
}

vec2
operator-(const vec2 &a, const vec2 &b)
{
	return vec2(a.x-b.x, a.y-b.y);
}

vec2
operator+(const vec2 &a, const vec2 &b)
{
	return vec2(a.x+b.x, a.y+b.y);
}

vec2
operator/(const vec2 &a, double b)
{
	return vec2(a.x/b, a.y/b);
}

class closed_polygon {
private:
	typedef std::vector<vec2>::const_iterator vec_iter;
	std::vector<vec2> x_, dx_;
public:
	closed_polygon(const std::vector<vec2> &points) : x_(points)
	{
		for (vec_iter p = x_.begin(); p != x_.end(); p++) {
			vec_iter np = boost::next(p);
			if (np == x_.end())
				np = x_.begin();
			dx_.push_back(*np - *p);
		}
	}
	
	bool
	contains(const vec2 &pos) const
	{
		unsigned crossings = 0;
		for (vec_iter p = x_.begin(), dx = dx_.begin(); p != x_.end(); p++, dx++) {
			vec_iter np = boost::next(p);
			if (np == x_.end())
				np = x_.begin();
			// only consider segments whose y range spans the current point
			if (!(((p->y > pos.y) && (np->y <= pos.y)) ||
			      ((p->y <= pos.y) && (np->y > pos.y))))
				continue;
			double xc = p->x + (pos.y - p->y)*dx->x/dx->y;
			// is the crossing to the right of the test point?
			if (pos.y < xc)
				crossings++;
		}
		
		return (crossings % 2) == 1;
	}
};

}

/// Lifted from http://code.icecube.wisc.edu/svn/sandbox/ckopper/eventinjector/python/util/__init__.py
std::vector<vec2>
convex_hull(const std::vector<I3Position> &positions)
{
	std::vector<vec2> hull;
	
	// Build a set of unique points, sorted lexicographically
	std::set<vec2> points;
	std::transform(positions.begin(), positions.end(),
	    std::inserter(points, points.end()), vec2::from_I3Position);
	
	// Boring case: 1 point (perhaps repeated)
	if (points.size() <= 1) {
		std::copy(points.begin(), points.end(), std::back_inserter(hull));
		return hull;
	}
	
	// Build lower and upper hulls
	std::vector<vec2> lower = std::for_each(points.begin(), points.end(), ccw_curve());
	std::vector<vec2> upper = std::for_each(points.rbegin(), points.rend(), ccw_curve());
	
	// Concatenation of the lower and upper hulls gives the convex hull.
	// Last point of each list is omitted because it is repeated at the
	// beginning of the other list.
	std::copy(lower.begin(), lower.end()-1, std::back_inserter(hull));
	std::copy(upper.begin(), upper.end()-1, std::back_inserter(hull));
	
	return hull;
}

namespace {

del_point2d_t
vec2_to_point(const vec2 &v)
{
	del_point2d_t d;

	d.x = v.x;
	d.y = v.y;

	return d;
}

vec2
point_to_vec2(const del_point2d_t &d)
{
	return vec2(d.x, d.y);
}

double
side_length(const tri_delaunay2d_t &tri, unsigned a, unsigned b)
{
	const del_point2d_t &p0 = tri.points[a];
	const del_point2d_t &p1 = tri.points[b];
	return hypot(p1.x-p0.x, p1.y-p0.y);
}

double
max_side_length(const tri_delaunay2d_t &tri, unsigned idx)
{
	double ab = side_length(tri, tri.tris[idx*3 + 0], tri.tris[idx*3 + 1]);
	double bc = side_length(tri, tri.tris[idx*3 + 1], tri.tris[idx*3 + 2]);
	double ca = side_length(tri, tri.tris[idx*3 + 2], tri.tris[idx*3 + 0]);
	return std::max(ab, std::max(bc, ca));
}

std::pair<unsigned, unsigned>
make_ordered_pair(unsigned a, unsigned b)
{
	if (a < b)
		return std::make_pair(a, b);
	else
		return std::make_pair(b, a);
}

template <typename T>
std::ostream&
operator<<(std::ostream &o, const std::vector<T> &vec)
{
	o << "[";
	BOOST_FOREACH(const T &t, vec)
		o << t << " ";
	o << "]";
	return o;
}

double
point_line_distance(const vec2 &o, const vec2 &a, const vec2 &b)
{
	vec2 v = vec2::normalized(b.y-a.y, a.x-b.y);
	return std::abs(v*(o-a));
}

}

std::vector<vec2> delaunay_hull(const std::vector<I3Position> &positions,
   double max_edge_length, double border_tolerance)
{
	std::vector<vec2> hull;
	
	// Build a set of unique points, sorted lexicographically
	std::set<vec2> points;
	std::transform(positions.begin(), positions.end(),
	    std::inserter(points, points.end()), vec2::from_I3Position);
	
	// Boring case: 1 point (perhaps repeated)
	if (points.size() <= 1) {
		std::copy(points.begin(), points.end(), std::back_inserter(hull));
		return hull;
	}
	
	// Triangulate the provided points and remove overly long edges d
	std::vector<del_point2d_t> dpoints;
	std::transform(points.begin(), points.end(),
	    std::back_inserter(dpoints), vec2_to_point);
	
	boost::scoped_ptr<delaunay2d_t> del2d(delaunay2d_from(&dpoints.front(), dpoints.size()));
	i3_assert(del2d != NULL);
	boost::scoped_ptr<tri_delaunay2d_t> tri(tri_delaunay2d_from(del2d.get()));
	i3_assert(tri != NULL);
	
	// Find the outline of the polygon by counting unique edges
	typedef std::map<std::pair<unsigned, unsigned>, unsigned> edge_map_t;
	edge_map_t edge_map;
	for (unsigned i=0; i < tri->num_triangles; i++) {
		if (max_side_length(*tri, i) <= max_edge_length) {
			for (unsigned a=0; a < 3; a++) {
				unsigned b = a == 2 ? 0 : a+1;
				// NB: index of the jth vertex of the ith 
				// triangle is tri->tris[i*3 + j]
				edge_map_t::key_type edge =
				    make_ordered_pair(tri->tris[i*3 + a], tri->tris[i*3 + b]);
				edge_map_t::iterator it = edge_map.find(edge);
				if (it == edge_map.end()) {
					edge_map.insert(std::make_pair(edge, 1));
				} else {
					it->second++;
				}
			}
		}
	} 
	
	// Now, sort the resulting edge points in ccw order.
	// First, build a map from each vertex to its neighbors
	typedef std::map<unsigned, std::vector<unsigned> > vertex_map_t;
	vertex_map_t vertices;
	BOOST_FOREACH(const edge_map_t::value_type &edge_count, edge_map) {
		if (edge_count.second == 1) {
			const edge_map_t::key_type &edge = edge_count.first;
			vertices[edge.first].push_back(edge.second);
			vertices[edge.second].push_back(edge.first);
		}
	}
	BOOST_FOREACH(const vertex_map_t::value_type &neighbors, vertices) {
		i3_assert(neighbors.second.size() == 2 && "Polygon has finite width everywhere");
	}
	size_t num_points = vertices.size();
	
	// Start with the leftmost point (guaranteed on the border)
	std::vector<unsigned> order(1, 0);
	vertex_map_t::const_iterator neighbors = vertices.find(0);
	// Pick the neighbor that is in ccw order
	{
		unsigned a = *(neighbors->second.begin());
		unsigned b = *(neighbors->second.begin()+1);
		
		double cross = ccw_curve::cross(point_to_vec2(tri->points[a]),
		    point_to_vec2(tri->points[0]), point_to_vec2(tri->points[b]));
		// ccw turn: b is the next vertex
		if (cross > 0) {
			order.push_back(b);
		// parallel: next vertex is the one with the smallest y coordinate
		} else if (cross == 0) {
			if (tri->points[a].y < tri->points[b].y) {
				order.push_back(a);
			} else {
				order.push_back(b);
			}
		// cw turn: a is the next vertex
		} else {
			order.push_back(a);
		}
		
	}
	vertices.erase(neighbors);
	
	while (vertices.size() > 1) {
		neighbors = vertices.find(order.back());
		if (neighbors == vertices.end())
			log_fatal("Reduced triangulation is not continguous!");
		unsigned a = *(neighbors->second.begin());
		unsigned b = *(neighbors->second.begin()+1);
		// `a` has not yet been consumed
		if (vertices.find(a) != vertices.end()) {
			order.push_back(a);
		} else {
			order.push_back(b);
		}
		vertices.erase(neighbors);
	}
	
	i3_assert(order.size() == num_points && "Ordered list contains all vertices");
	i3_assert(std::set<unsigned>(order.begin(), order.end()).size() == order.size() && "There are no duplicate vertices");
	
	// Simplify the polygon while copying it into `hull`
	for (unsigned i=0; i < order.size(); i++) {
		unsigned prev = i == 0 ? order.back() : order[i-1];
		unsigned next = i == order.size()-1 ? order[0] : order[i+1];
		if (point_line_distance(
		    point_to_vec2(tri->points[prev]),
		    point_to_vec2(tri->points[order[i]]),
		    point_to_vec2(tri->points[next])) > border_tolerance) {
			hull.push_back(point_to_vec2(tri->points[order[i]]));
		}
	}
	
	return hull;
}

std::vector<triangle2>
triangulate(const std::vector<vec2> &points)
{
	std::vector<triangle2> triangles;
	
	// Triangulate
	std::vector<del_point2d_t> dpoints;
	std::transform(points.begin(), points.end(),
	    std::back_inserter(dpoints), vec2_to_point);
	
	boost::scoped_ptr<delaunay2d_t> del2d(delaunay2d_from(&dpoints.front(), dpoints.size()));
	i3_assert(del2d != NULL);
	boost::scoped_ptr<tri_delaunay2d_t> tri(tri_delaunay2d_from(del2d.get()));
	i3_assert(tri != NULL);
	
	// Remove triangles with centers outside the hull
	closed_polygon poly(points);
	for (unsigned i=0; i < tri->num_triangles; i++) {
		triangle2 triangle(points[tri->tris[i*3+0]],
		    points[tri->tris[i*3+1]], points[tri->tris[i*3+2]]);
		if (poly.contains(triangle.center()))
			triangles.push_back(triangle);
	}
	
	return triangles;
}

std::vector<vec2>
expand_polygon(const std::vector<vec2> &hull, double padding)
{
	std::vector<vec2> points;
	for (std::vector<vec2>::const_iterator p = hull.begin(); p != hull.end(); p++) {
		std::vector<vec2>::const_iterator next = boost::next(p);
		if (next == hull.end())
			next = hull.begin();
		std::vector<vec2>::const_iterator prev = boost::prior(
		    p == hull.begin() ? hull.end() : p);
		// normalized vector connecting this vertex to the next one
		vec2 d = vec2::normalized(next->x-p->x, next->y-p->y);
		// and the previous vertex to this one
		vec2 prev_d = vec2::normalized(p->x-prev->x, p->y-prev->y);
		// sine of the inner angle between the segments that meet here
		double det = prev_d.x*d.y - prev_d.y*d.x;
		if (det == 0.)
			log_fatal("Edges can't be [anti]parallel");
		vec2 outwards(prev_d.x-d.x, prev_d.y-d.y);
		points.push_back(vec2(p->x + outwards.x*padding/det, p->y + outwards.y*padding/det));
	}
	
	return points;
}

vec2::vec2(double xi, double yi) : x(xi), y(yi)
{}

template <typename Archive>
void vec2::serialize(Archive &ar, unsigned version)
{
	if (version > 0)
		log_fatal_stream("Version "<<version<<" is from the future");
	
	ar & make_nvp("X", x);
	ar & make_nvp("Y", y);
}

vec2
vec2::from_I3Position(const I3Position &p)
{
	return vec2(p.GetX(), p.GetY());
}

vec2
vec2::normalized(double xi, double yi)
{
	double l = hypot(xi, yi);
	return vec2(xi/l, yi/l);
}

bool
operator<(const vec2 &a, const vec2 &b)
{
	if (a.x < b.x)
		return true;
	else if (a.x > b.x)
		return false;
	else if (a.y < b.y)
		return true;
	else
		return false;
}

side::side(const vec2 &p, const vec2 &np) : origin(p),
    vector(np.x-p.x, np.y-p.y), length(hypot(vector.x, vector.y)),
	normal(vector.y/length, -vector.x/length, 0.)
{}

triangle2::triangle2(const vec2 &a, const vec2 &b, const vec2 &c)
    : vertex_(a), side1_(b-a), side2_(c-a)
{}

vec2
triangle2::center() const
{
	return vertex_ + (side1_+side2_)/3.;
}

double
triangle2::area() const
{
	return std::abs(side1_.x*side2_.y + side1_.y - (side2_.x*side1_.y + side2_.y))/2.;
}

vec2
triangle2::sample_point(I3RandomService &rng) const
{
	// draw a point in a parallelogram with sides side1 and side2
	double p1 = rng.Uniform();
	double p2 = rng.Uniform();
	if (p1+p2 <= 1)
		// point is in the lower triangle. take it.
		return vertex_ + p1*side1_ + p2*side2_;
	else
		// point is in the upper triangle. reflect it around the side
		// opposite `vertex`
		return vertex_ + (1-p1)*side1_ + (1-p2)*side2_;
}

triangulated_polygon::triangulated_polygon(const std::vector<vec2> &edge)
    : triangles_(triangulate(edge))
{
	double area = 0;
	BOOST_FOREACH(const triangle2 &tri, triangles_) {
		area += tri.area();
		cumulative_area_.push_back(area);
	}
	i3_assert(cumulative_area_.size() > 0);
}

double
triangulated_polygon::area() const
{
	return cumulative_area_.back();
}

vec2
triangulated_polygon::sample_point(I3RandomService &rng) const
{
	// Pick a triangle in proportion to its area
	ssize_t offset = std::distance(cumulative_area_.begin(),
	    std::lower_bound(cumulative_area_.begin(), cumulative_area_.end(),
	    rng.Uniform(0, cumulative_area_.back())));
	i3_assert(offset >= 0 && offset < cumulative_area_.size());
	return triangles_.at(offset).sample_point(rng);
}

}}

I3_SERIALIZABLE(simclasses::polygon::vec2);
