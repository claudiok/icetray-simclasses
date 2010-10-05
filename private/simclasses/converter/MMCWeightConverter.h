/**
 * copyright  (C) 2010
 * The Icecube Collaboration
 *
 * $Id: I3CorsikaShowerInfoConverter.h 66214 2010-08-17 16:39:23Z kislat $
 *
 * @version $Revision: 66214 $
 * @date $LastChangedDate: 2010-08-17 18:39:23 +0200 (Tue, 17 Aug 2010) $
 * @author Fabian Kislat <fabian.kislat@desy.de> $LastChangedBy: kislat $
 */

#ifndef SIMCLASSES_MMCWEIGHTCONVERTER_H_INCLUDED
#define SIMCLASSES_MMCWEIGHTCONVERTER_H_INCLUDED

#include <tableio/I3ConverterFactory.h>
#include "simclasses/MMCWeight.h"

class MMCWeightConverter : public I3ConverterImplementation< MMCWeight > {
public:
  MMCWeightConverter();

private:
  I3TableRowDescriptionPtr CreateDescription(const MMCWeight& mmcweight);
  size_t FillRows(const MMCWeight& mmcweight, I3TableRowPtr rows);
};


#endif // SIMCLASSES_MMCWEIGHTCONVERTER_H_INCLUDED
