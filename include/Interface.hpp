#pragma once

#include "Base/Shape.hpp"
#include "Base/WidgetData.hpp"
#include "Renderer.hpp"

#include "Widget.hpp"

#include <vector>
#include <unordered_map>
#include <string>

class Interface
{
	public:
		Interface();
		~Interface();

		friend class Widget;

	public:
		void start(const glm::ivec2& size);
		void setClearColor(const glm::vec3& color);

		void draw() const;

		void setMaxPriority(unsigned int level);
		unsigned int getShapeId(const glm::ivec2& position) const;

		Widget createWidget(const std::string& name);
		Widget createWidget(const std::string& name, ShapePtr shape);

	private:
		void insertWidget(const std::string& name, WidgetPtr widget);
		void insertWidget(const std::string& name, WidgetPtr widget, ShapePtr shape);
		void insertShape(WidgetPtr widget, ShapePtr shape);

		void removeWidget(WidgetPtr widget);
		void removeShape(WidgetPtr widget);

		ShapeData& getShape(WidgetPtr widget);
		void setShape(WidgetPtr widget, ShapePtr shape);

	private:
		std::vector<ShapeData> mShapes;
		size_t mShapesCount = 0;

		std::vector<WidgetPtr> mWidgets;
		size_t mWidgetCount = 0;

		std::unordered_map<std::string, WidgetPtr> mNameToWidget;

		unsigned int mMaxPriority = 1000;

		Renderer mRenderer;
};