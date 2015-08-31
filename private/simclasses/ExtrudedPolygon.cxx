/** $Id$
 * @file
 * @author Jakob van Santen <jakob.van.santen@desy.de>
 *
 * $Revision$
 * $Date$
 */

#include <simclasses/ExtrudedPolygon.h>
#include <boost/make_shared.hpp>

namespace simclasses {

boost::shared_ptr<ExtrudedPolygon>
ExtrudedPolygon::createFromConvexHull(const std::vector<I3Position> &positions, double padding)
{
	return boost::make_shared<ExtrudedPolygon>(polygon::convex_hull(positions), z_range(positions), padding);
}

boost::shared_ptr<ExtrudedPolygon>
ExtrudedPolygon::createFromTriangulation(const std::vector<I3Position> &positions, double max_edge_length, double border_tolerance, double padding)
{
	return boost::make_shared<ExtrudedPolygon>(polygon::delaunay_hull(positions, max_edge_length, border_tolerance), z_range(positions), padding);
}

ExtrudedPolygon::~ExtrudedPolygon() {}

template <typename Archive>
void
ExtrudedPolygon::serialize(Archive &ar, unsigned version)
{
	if (version > 0)
		log_fatal_stream("Version "<<version<<" is from the future");

	ar & make_nvp("Base", base_object<Base>(*this));
}

}

I3_SERIALIZABLE(simclasses::ExtrudedPolygon);
