#pragma once

#include <memory>
#include <vector>

struct WidgetData
{
	using Ptr = std::shared_ptr<WidgetData>;

	const char* name;

	size_t index;
	size_t shapeIndex;
	bool hasShape;

	Ptr parent;
	std::vector<Ptr> children;
};

using WidgetPtr = std::shared_ptr<WidgetData>;
