/** $Id$
 * @file
 * @author Jakob van Santen <jakob.van.santen@desy.de>
 *
 * $Revision$
 * $Date$
 */

#ifndef SIMCLASSES_EXTRUDEDPOLYGON_H_INCLUDED
#define SIMCLASSES_EXTRUDEDPOLYGON_H_INCLUDED

#include <simclasses/SamplingSurface.h>
#include <simclasses/detail/ExtrudedPolygonBase.h>

namespace simclasses {

class ExtrudedPolygon : public ExtrudedPolygonBase<SamplingSurface> {
private:
	typedef ExtrudedPolygonBase<SamplingSurface> Base;
public:
	ExtrudedPolygon(const std::vector<polygon::vec2> &hull, const std::pair<double, double> &zrange, double padding = 0.) : Base(hull, zrange, padding) {};
	~ExtrudedPolygon();
	
	static boost::shared_ptr<ExtrudedPolygon> createFromConvexHull(
	    const std::vector<I3Position> &, double padding=0.);
	static boost::shared_ptr<ExtrudedPolygon> createFromTriangulation(
	    const std::vector<I3Position> &positions, double max_edge_length=200.,
	    double border_tolerance=10., double padding=0.);
	
private:
	ExtrudedPolygon() {}
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive &, unsigned);
};

I3_POINTER_TYPEDEFS(ExtrudedPolygon);

}

BOOST_CLASS_VERSION(simclasses::ExtrudedPolygon, 0);

#endif // SIMCLASSES_EXTRUDEDPOLYGON_H_INCLUDED
