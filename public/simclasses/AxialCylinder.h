/** $Id$
 * @file
 * @author Jakob van Santen <vansanten@wisc.edu>
 *
 * $Revision$
 * $Date$
 */

#ifndef SIMCLASSES_AXIALCYLINDER_H_INCLUDED
#define SIMCLASSES_AXIALCYLINDER_H_INCLUDED

#include <simclasses/SamplingSurface.h>
#include <dataclasses/I3Position.h>

namespace simclasses {

/**
 * @brief A cylinder aligned with the incoming particle axis a la NuGen
 */
class AxialCylinder : public SamplingSurface {
public:
	virtual ~AxialCylinder();
	AxialCylinder(double length, double radius, I3Position center=I3Position(0,0,0) );
	AxialCylinder(double lengthBefore, double lengthAfter, double radius, I3Position center=I3Position(0,0,0) );
	
	virtual std::pair<double, double> GetIntersection(const I3Position &p, const I3Direction &dir) const;
	
	// SamplingSurface interface
	virtual double GetArea(const I3Direction &) const;
	virtual double GetMaximumArea() const;
	virtual double GetAcceptance(double cosMin=0, double cosMax=1) const;
	
	virtual I3Direction SampleDirection(I3RandomService &rng, double cosMin=0, double cosMax=1) const;
	virtual I3Position SampleImpactPosition(const I3Direction &dir, I3RandomService &rng) const;
private:
	AxialCylinder() {}
	
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive &, unsigned);
	
	std::pair<double, double> length_;
	double radius_;
	I3Position center_;
};

I3_POINTER_TYPEDEFS(AxialCylinder);

}

BOOST_CLASS_VERSION(simclasses::AxialCylinder, 0);

#endif // SIMCLASSES_AXIALCYLINDER_H_INCLUDED
