#include "image.h"
#include <iostream>

using namespace std;
using namespace lw;

void main()
{
	Image image = Image::fromFile("grace_cross.hdr");
	image.save("grace_cross.png");
	cout << image.width() << endl
		<< image.height() << endl;
}