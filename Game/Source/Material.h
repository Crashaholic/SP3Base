#ifndef MATERIAL_H
#define MATERIAL_H
struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
};
//M: Added hsv / rgb converters
Component rgbtohsv(Color rgb)
{
	//Thanks to David H from StackOverflow
	Component hsv;
	float min, max, delta;
	min = Math::Min(rgb.r, Math::Min(rgb.g, rgb.b));
	max = Math::Max(rgb.r, Math::Max(rgb.g, rgb.b));
	delta = max - min;

	hsv.b = max;

	if (delta < 0.00001)
	{
		hsv.g = 0;
		hsv.r = 0;
		return hsv;
	}

	if (max > 0.0)
	{
		hsv.g = (delta / max);
	}
	else
	{
		hsv.g = 0.0;
		hsv.r = 0;
		return hsv;
	}

	if (rgb.r >= max)
		hsv.r = (rgb.g - rgb.b) / delta;
	else
		if (rgb.g >= max)
			hsv.r = 2.0 + (rgb.b - rgb.r) / delta;
		else
			hsv.r = 4.0 + (rgb.r - rgb.g) / delta;

	hsv.r *= 60.0;

	if (hsv.r < 0.0)
		hsv.r += 360.0;

	return hsv;
}

Component hsvtorgb(Component hsv)
{
	//Thanks to David H from StackOverflow
	float hh;
	float p, q, t, ff;
	int i;
	Component rgb;

	if (hsv.b <= 0.0f)
	{
		rgb.Set(hsv.b, hsv.b, hsv.b);
		return rgb;
	}
	hh = (float)hsv.r;
	hh = Math::Min(hh, 360.0f);
	hh /= 60.0f;
	i = (int)hh;
	ff = hh - (float)i;
	p = hsv.b * (1.0 - hsv.g);
	q = hsv.b * (1.0 - (hsv.g * ff));
	t = hsv.b * (1.0 - (hsv.g * (1.0 - ff)));

	switch (i) {
	case 0:
		rgb.r = hsv.b;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = hsv.b;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = hsv.b;
		rgb.b = t;
		break;

	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = hsv.b;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = hsv.b;
		break;
	case 5:
	default:
		rgb.r = hsv.b;
		rgb.g = p;
		rgb.b = q;
		break;
	}
	return rgb;

}
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;

	Material()
	{
		//some default values
		kAmbient.Set(0.1f, 0.1f, 0.1f);
		kDiffuse.Set(0.6f, 0.6f, 0.6f);
		kSpecular.Set(0.3f, 0.3f, 0.3f);
		kShininess = 5.f;
	}
};

#endif