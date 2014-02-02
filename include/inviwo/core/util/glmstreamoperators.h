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
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#ifndef IVW_GLMOPERATORS_H
#define IVW_GLMOPERATORS_H

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tvec2<T> const& v) {
    s << glm::to_string(v);
    return s;
}

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tvec3<T> const& v) {
    s << glm::to_string(v);
    return s;
}

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tvec4<T> const& v) {
    s << glm::to_string(v);
    return s;
}

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tmat2x2<T> const& m) {
    s << glm::to_string(m);
    return s;
}

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tmat3x3<T> const& m) {
    s << glm::to_string(m);
    return s;
}

template<typename T>
std::ostream& operator << (std::ostream& s, glm::detail::tmat4x4<T> const& m) {
    s << glm::to_string(m);
    return s;
}

#endif
