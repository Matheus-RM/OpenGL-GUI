#include "Interface.hpp"

#include <utility>
#include <stdexcept>

Interface::Interface()
{

}

Interface::~Interface()
{

}


void Interface::start(const glm::ivec2& size)
{
	mRenderer.start(size);
}

void Interface::setClearColor(const glm::vec3& color)
{
	mRenderer.setClearColor(color);
}


void Interface::draw() const
{
	mRenderer.draw(mShapes,	mShapesCount);
}

Widget Interface::createWidget(const std::string& name)
{
	auto newWidgetData = std::make_shared<WidgetData>();
	insertWidget(name, newWidgetData);

	return Widget(this, newWidgetData);
}

Widget Interface::createWidget(const std::string& name, ShapePtr shape)
{
	auto newWidgetData = std::make_shared<WidgetData>();
	insertWidget(name, newWidgetData, std::move(shape));

	return Widget(this, newWidgetData);
}


void Interface::insertWidget(const std::string& name, WidgetPtr widget, ShapePtr shape)
{
	insertWidget(name, widget);
	insertShape(widget, std::move(shape));
}

void Interface::insertWidget(const std::string& name, WidgetPtr widget)
{
	if(mWidgetCount < mShapes.size())
	{
		mWidgets[mWidgetCount] = widget;
	}
	else
	{
		mWidgets.push_back(widget);
	}

	widget->index = mWidgetCount++;
	widget->hasShape = false;

	auto it = mNameToWidget.emplace(name, widget).first;
	widget->name = it->first.c_str();
}

void Interface::insertShape(WidgetPtr widget, ShapePtr shape)
{
	shape->id = widget->index + 1;
	shape->priority = -1.0f + (shape->priority * 2.0f / (float)mMaxPriority);  // clamp to the range [-1, 1]

	if(mShapesCount < mShapes.size())
	{
		mShapes[mShapesCount] = *shape;
	}
	else
	{
		mShapes.push_back(*shape);
	}

	widget->shapeIndex = mShapesCount++;
	widget->hasShape = true;
}


void Interface::removeWidget(WidgetPtr widget)
{
	removeShape(widget);

	if(widget->index > mWidgetCount)
		throw std::domain_error("Renderer: trying to remove a out of bound shape.");

	mNameToWidget.erase(std::string(widget->name));

	auto lastWidgetIt = mWidgets.end() - 1;
	auto emptyWidgetIt = mWidgets.begin() + widget->index;

	(*lastWidgetIt)->index = widget->index;
	emptyWidgetIt->reset();

	std::iter_swap(lastWidgetIt, emptyWidgetIt);
}

void Interface::removeShape(WidgetPtr widget)
{
	if(!widget->hasShape)
		return;

	if(widget->shapeIndex > mShapesCount)
		throw std::domain_error("Renderer: trying to remove a out of bound shape.");

	auto lastIt = mShapes.end() - 1;
	auto toRemoveIt = mShapes.begin() + widget->shapeIndex;

	std::iter_swap(lastIt, toRemoveIt);

	mShapesCount--;
	widget->hasShape = false;
}

ShapeData& Interface::getShape(WidgetPtr widget)
{
	return mShapes.at(widget->shapeIndex);
}

void Interface::setShape(WidgetPtr widget, ShapePtr shape)
{
	if(widget->hasShape)
	{
		mShapes[widget->shapeIndex] = *shape;
		return;
	}

	insertShape(widget, std::move(shape));
}


void Interface::setMaxPriority(unsigned int level)
{
	mMaxPriority = level;
}

unsigned int Interface::getShapeId(const glm::ivec2& position) const
{
	return mRenderer.getShapeId(position);
}