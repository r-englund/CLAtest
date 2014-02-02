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

#ifndef _IVW_QTWIDGETS_DEFINE_H_
#define _IVW_QTWIDGETS_DEFINE_H_

#ifdef INVIWO_ALL_DYN_LINK //DYNAMIC
// If we are building DLL files we must declare dllexport/dllimport
#ifdef IVW_QTWIDGETS_EXPORTS
#ifdef _WIN32
#define IVW_QTWIDGETS_API __declspec(dllexport)
#else //UNIX (GCC)
#define IVW_QTWIDGETS_API __attribute__ ((visibility ("default")))
#endif
#else
#ifdef _WIN32
#define IVW_QTWIDGETS_API __declspec(dllimport)
#else
#define IVW_QTWIDGETS_API
#endif
#endif
#else //STATIC
#define IVW_QTWIDGETS_API
#endif

#endif /* _IVW_QTWIDGETS_DEFINE_H_ */