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



template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tvec2<T> const &v){
	s <<v.x << " " <<v.y;
	return s;
}

template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tvec3<T> const &v){
	s <<v.x << " " <<v.y << " " <<v.z;
	return s;
}

template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tvec4<T> const &v){
	s <<v.x << " " <<v.y << " " <<v.z << " " <<v.w;
	return s;
}

template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tmat2x2<T> const & m){
	int size = 2;
	for(int i = 0;i<size;i++){
		s << "[";
		for(int j = 0;j<size;j++){
			if(j!=0)
				s << " ";
			s << m[i][j];
		}
		s << "]";
	}
    return s;
}

template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tmat3x3<T> const & m){
	int size = 3;
	for(int i = 0;i<size;i++){
		s << "[";
		for(int j = 0;j<size;j++){
			if(j!=0)
				s << " ";
			s << m[i][j];
		}
		s << "]";
	}
    return s;
}

template<typename T>
std::ostream & operator << (std::ostream & s, glm::detail::tmat4x4<T> const & m){
	int size = 4;
	for(int i = 0;i<size;i++){
		s << "[";
		for(int j = 0;j<size;j++){
			if(j!=0)
				s << " ";
			s << m[i][j];
		}
		s << "]";
	}
    return s;
}



#endif
