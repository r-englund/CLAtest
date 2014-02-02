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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef _IVW_CORE_DEFINE_H_
#define _IVW_CORE_DEFINE_H_

#ifdef INVIWO_ALL_DYN_LINK //DYNAMIC
// If we are building DLL files we must declare dllexport/dllimport
#ifdef IVW_CORE_EXPORTS
#ifdef _WIN32
#define IVW_CORE_API __declspec(dllexport)
#define IVW_CORE_EXT
#else //UNIX (GCC)
#define IVW_CORE_API __attribute__ ((visibility ("default")))
#define IVW_CORE_EXT
#endif
#else
#ifdef _WIN32
#define IVW_CORE_API __declspec(dllimport)
#define IVW_CORE_EXT extern
#else
#define IVW_CORE_API
#define IVW_CORE_EXT extern
#endif
#endif
#else //STATIC
#define IVW_CORE_API
#define IVW_CORE_EXT extern
#endif

#endif /* _IVW_CORE_DEFINE_H_ */