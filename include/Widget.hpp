#pragma once

#include "Base/WidgetData.hpp"
#include "Base/Shape.hpp"

#include <string>

class Interface;

class Widget
{
	public:
		Widget(Interface* interface, WidgetPtr data);
		~Widget();

	public:
		void destroy();
		void addChild(const std::string& name);
		void setShape(ShapePtr shape);


	private:
		Interface* mInterface;
		WidgetPtr mData;
};