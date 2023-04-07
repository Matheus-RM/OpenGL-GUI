#include "Widget.hpp"

#include "Interface.hpp"

Widget::Widget(Interface* interface, WidgetPtr data) : mInterface(interface), mData(data)
{

}


Widget::~Widget()
{

}


void Widget::destroy()
{
	mInterface->removeWidget(mData);
	mData.reset();
}

void Widget::addChild(const std::string& name)
{
	auto child = mInterface->createWidget(name);
	mData->children.push_back(child.mData);
}

void Widget::setShape(ShapePtr shape)
{
	mInterface->setShape(mData, std::move(shape));
}
