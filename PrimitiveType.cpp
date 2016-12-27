#include "PrimitiveType.h"
#include <iostream>


std::ostream& operator<<(std::ostream &os, const PrimitiveType &p)
{
	switch (p)
	{
	case NOT_OBJECT: os << "not a object";	break;
	case INF_PLANE: os << "Infinite Plane";	break;
	case F_PLANE: os << "Finite Plane";	break;
	case SPHERE: os << "Sphere"; break;
	case TRIANGLE: os << "Triangle";	break;
	case MESH: os << "Mesh";	break;
	default: os << "Unknown " << p;	break;
	}
	return os;
}