#version 330 core

layout (location=0) out vec4 fragmentColor;
layout (location=1) out uint id;

in vec2 fPosition;
in vec4 fColor;

uniform uint shapeId;
uniform int radius;
uniform ivec2 size;

ivec2 circleCenter;

bool checkIfIsInCornerY(void)
{
	if(fPosition.y < radius)
	{
		circleCenter.y = radius;
	}
	else if (fPosition.y > size.y - radius)
	{
		circleCenter.y = size.y - radius;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool checkIfIsInCornerX(void)
{
	if(fPosition.x < radius)
	{
		circleCenter.x = radius;
	}
	else if (fPosition.x > size.x - radius)
	{
		circleCenter.x = size.x - radius;
	}
	else
	{
		return false;
	}
	
	return true;
}

void main(void)
{
	id = shapeId;
	fragmentColor = fColor.xyzw;
	
	if(checkIfIsInCornerX() && checkIfIsInCornerY())
	{
		if( distance(fPosition, circleCenter) > radius)
		{
			discard;
		}
	}
}
