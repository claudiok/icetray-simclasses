/**
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: I3CorsikaShowerInfoConverter.cxx 66215 2010-08-17 16:53:26Z kislat $
 *
 * @version $Revision: 66215 $
 * @date $LastChangedDate: 2010-08-17 18:53:26 +0200 (Tue, 17 Aug 2010) $
 * @author Fabian Kislat <fabian.kislat@desy.de> $LastChangedBy: kislat $
 */

#include "simclasses/converter/MMCWeightConverter.h"

MMCWeightConverter::MMCWeightConverter() : I3ConverterImplementation<MMCWeight>() {}

I3TableRowDescriptionPtr MMCWeightConverter::CreateDescription(const MMCWeight& mmcweight)
{
  I3TableRowDescriptionPtr desc = I3TableRowDescriptionPtr(new I3TableRowDescription() );

#warning TODO: Write proper descriptions
  desc->AddField<double>("weight", "", "MMC Weight");
  desc->AddField<double>("distToModIntPoint", "m", "Distance to modified interaction point?");

  return desc;
}

size_t MMCWeightConverter::FillRows(const MMCWeight& mmcweight, I3TableRowPtr rows)
{
  rows->Set<double>("weight", mmcweight.weight);
  rows->Set<double>("distToModIntPoint", mmcweight.distToModIntPoint);

  return 1;
}
