
#include <tableio/converter/pybindings.h>
#include <tableio/converter/I3VectorConverter.h>
#include <dataclasses/physics/I3Particle.h>
#include <icetray/I3Frame.h>
#include <simclasses/I3MMCTrack.h>

// I3ConverterRegistry::GetDefaultConverter() requires tableio >= r91320

namespace convert {

struct I3MMCTrack {
	typedef ::I3MMCTrack booked_type;
	I3ConverterPtr base_;
	I3FramePtr dummyFrame_;
	I3MMCTrack() : dummyFrame_(new I3Frame)
	{
		base_ = I3ConverterRegistry::GetDefaultConverter<I3Particle>();
		if (!base_)
			log_fatal("No converter registered for I3Particle!");
	} 
		
        void AddFields(I3TableRowDescriptionPtr desc, const booked_type &item = booked_type())
	{
		*desc << *(base_->GetDescription(item.GetI3Particle()));
		#warning add I3MMCTrack fields!
	}
		
	void FillSingleRow(const booked_type &item, I3TableRowPtr row)
	{
		size_t nrows = base_->Convert(item.GetI3Particle(), row, dummyFrame_);
		assert(nrows == 1);
		#warning fill I3MMCTrack fields!
	}
};

}

void register_simclasses_converters()
{
	I3CONVERTER_NAMESPACE(simclasses);
	typedef I3VectorConverter< convert::I3MMCTrack > I3MMCTrackListConverter;
	I3CONVERTER_EXPORT_DEFAULT(I3MMCTrackListConverter, "Dumps an I3Vector of I3MMCTracks");
}
