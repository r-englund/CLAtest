#ifndef IVW_GLMOPERATORS_H
#define IVW_GLMOPERATORS_H

template<typename T>
std::ostream & operator << (std::ostream & stream, glm::detail::tvec2<T> const & vec);

template<typename T>
std::ostream & operator << (std::ostream & stream, glm::detail::tvec3<T> const & vec);

template<typename T>
std::ostream & operator << (std::ostream & stream, glm::detail::tvec4<T> const & vec);

template<typename T> 
std::ostream & operator << (std::ostream & stream, glm::detail::tmat2x2<T> const & mat);

template<typename T> 
std::ostream & operator << (std::ostream & stream, glm::detail::tmat3x3<T> const & mat);

template<typename T> 
std::ostream & operator << (std::ostream & stream, glm::detail::tmat4x4<T> const & mat);

#endif
