/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_PROGRESSBAROWNER_H
#define IVW_PROGRESSBAROWNER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/progressbar.h>

namespace inviwo {

/** \class ProgressBarOwner 
*
* Class to support processors owning a ProgressBar. 
* This class allows a progress bar contained within a processor to be visible in the network.
*
* \section example Example
* Example of how to apply it to a processor.
* @code
*    class IVW_XXX_API MyProcessor: public Processor, public ProgressBarOwner {
*    public:
*        MyProcessor(): Processor(), ProgressBarOwner() {};
*        // Need to overload serialize/deseralize
*        virtual void serialize(IvwSerializer& s) const { 
*           Processor::serialize(s); 
*           s.serialize("ProgressBar", getProgressBar());
*        }
*        virtual void deserialize(IvwDeserializer& d) { 
*           Processor::deserialize(d); 
*           d.deserialize("ProgressBar", getProgressBar());
*        }
*    };
*
* @endcode
* @see ProgressBar
* @see ProgressBarObservable
*/
class IVW_CORE_API ProgressBarOwner  {
public:
    ProgressBarOwner() {};
    virtual ~ProgressBarOwner() {};

    ProgressBar& getProgressBar() { return progressBar_; };
    const ProgressBar& getProgressBar() const { return progressBar_; };

    // Helper function
    void updateProgress(float progress) { progressBar_.updateProgress(progress); }

protected:
    ProgressBar progressBar_;
};


} // namespace

#endif // IVW_PROGRESSBAROWNER_H
