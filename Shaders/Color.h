#ifndef _DEFECT_VIEWER_COLOR_H
#define _DEFECT_VIEWER_COLOR_H

class Color
{
public:
	enum Names {BLACK = 0, RED, ORANGE, YELLOW, GREEN, LIGHT_BLUE, BLUE, GRAY, MAGENTA, LIGHT_GRAY};

	static vec4 GetForeground(Names e)
	{
		vec4 colorV;
		switch (e)
		{
			case RED:
				colorV = vec4(1.0f,0,0,1.0f);
				break;
			case ORANGE:
				colorV = vec4(1.0f,0.5f,0,1.0f);
				break;
			case YELLOW:
				colorV = vec4(1.0f,1.0f,0,1.0f);
				break;
			case GREEN:
				colorV = vec4(0.0f,1,0,1.0f);
				break;
			case LIGHT_BLUE:
				colorV = vec4(0.0f,1.0f,1.0f,1.0f);
				break;
			case BLUE:
				colorV = vec4(0.0f,0.0f,1.0f,1.0f);
				break;	
			case GRAY:
				colorV = vec4(0.25f,0.25f,0.25f,1.0f);
				break;
			case LIGHT_GRAY:
				colorV = vec4(0.5f,0.5f,0.5f,1.0f);
				break;
			case MAGENTA:
				colorV = vec4(0.63f,0.29f,0.64f,1.0f);
				break;
			default://BLACK
				colorV = vec4(0.0f,0.0f,0.0f,1.0f);
				break;	
		}

		return colorV;
	}

	static Names GetNextColor(Names e)
	{
		Names ret = BLACK;

		if (e == BLACK)
			ret = RED;
		else
			ret = (Names)(e + 1);

	}

	static vec4 GetBackground(Names e)
	{
		vec4 colorV;
		switch (e)
		{
			case RED:
				colorV = vec4(0.5f,0,0,1.0f);
				break;
			case ORANGE:
				colorV = vec4(0.5f,0.25f,0,1.0f);
				break;
			case YELLOW:
				colorV = vec4(1.0f,0.5f,0,1.0f);
				break;
			case GREEN:
				colorV = vec4(0.0f,0.25f,0,1.0f);
				break;
			case LIGHT_BLUE:
				colorV = vec4(0.0f,0.25f,0.5f,1.0f);
				break;
			case BLUE:
				colorV = vec4(0.0f,0.0f,0.25f,1.0f);
				break;	
			case GRAY:
				colorV = vec4(0.75f,0.75f,0.75f,1.0f);
				break;
			default://BLACK
				colorV = vec4(0.0f,0.0f,0.0f,1.0f);
				break;	
		}
		return colorV;
	}
};

#endif
