#include "LightType.h"
#include <iostream>


std::ostream& operator<<(std::ostream &os, const LightType &light)
{
	switch (light)
	{
	case POINT_LIGHT:	os << "Point Light";	break;
	case AREA_LIGHT:	os << "Area Light";		break;
	default:	break;
	}
	return os;
}