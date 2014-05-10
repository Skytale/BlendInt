/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/TableLayout.hpp>

namespace BlendInt {

	TableLayout::TableLayout (int rows, int columns)
			: AbstractLayout(), m_rows(rows), m_columns(columns)
	{
	}

	TableLayout::~TableLayout ()
	{
	}

	void TableLayout::add_widget (AbstractWidget* widget, int row, int column,
	        int width, int height)
	{
		AppendSubWidget(widget);

		for (int i = 0; i < width; i++)
		{
			m_items[m_columns * row + column + i] = widget;
		}

		for (int i = 0; i < height; i++)
		{
			m_items[m_columns * (row + i) + column] = widget;
		}

		//Update(FormSize, 0);
	}

	void TableLayout::add_layout (AbstractLayout* layout, int row, int column,
	        int width, int height)
	{
		AppendSubWidget(layout);

		for (int i = 0; i < width; i++)
		{
			m_items[m_columns * row + column + i] = layout;
		}

		for (int i = 0; i < height; i++)
		{
			m_items[m_columns * (row + i) + column] = layout;
		}

		//Update(FormSize, 0);
	}

	void TableLayout::Update (int property_type)
	{
		generate_default_layout();
	}

	void TableLayout::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* new_pos = static_cast<const Point*>(request.data());

					int x = new_pos->x() - position().x();
					int y = new_pos->y() - position().y();

					MoveSubWidgets(x, y);

					break;
				}

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					if(sub_widget_size())
						MakeLayout(size_p, &margin(), space());

					break;

				}

				case ContainerMargin: {
					const Margin* margin_p = static_cast<const Margin*>(request.data());
					if(sub_widget_size())
						MakeLayout(&size(), margin_p, space());
					break;
				}

				default: {
					break;
				}
			}

		}
	}

	ResponseType TableLayout::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	void TableLayout::AddItem(AbstractWidget* widget)
	{
		/*
		// don't fire events when adding a widget into a layout
		widget->deactivate_events();
		deactivate_events();

		Size min_size = minimal_size();
		Size prefer_size = preferred_size();
		Size current_size = size();
		// TODO: count max size

		//unsigned int w_plus = margin().left() + margin().right();
		//unsigned int h_plus = margin().top() + margin().bottom();

		if (sub_widget_size() == 0) {
			min_size.add_width(widget->minimal_size().width());
			min_size.add_height(widget->minimal_size().height());
			prefer_size.add_width(widget->preferred_size().width());
			prefer_size.add_height(widget->preferred_size().height());
		} else {
			min_size.add_width(widget->minimal_size().width() + space());
			prefer_size.add_width(widget->preferred_size().width() + space());


		}

		if (current_size.width() < prefer_size.width()) {
			current_size.set_width(prefer_size.width());
		}
		if (current_size.height() < prefer_size.height()) {
			current_size.set_height(prefer_size.height());
		}

		SetPreferredSize(prefer_size);
		SetMinimalSize(min_size);

		AppendSubWidget(widget);

		if( !(current_size == size()) )
			Resize(current_size);
		else
			MakeLayout(&current_size, &margin(), space());

		activate_events();
		widget->activate_events();
		*/
	}

	void TableLayout::RemoveItem(AbstractWidget* object)
	{

	}

	bool TableLayout::generate_layout(const Size* new_size)
	{
		/*
		if(new_size->width() < minimal_size().width() ||
				new_size->height() < minimal_size().height())
			return false;
			*/

		unsigned int total_width = 0;
		unsigned int total_height = 0;

		AbstractWidget* child = 0;

		std::vector<int> row_height(m_rows, 0);
		std::vector<int> column_width(m_columns, 0);

		int w = 0;
		int h = 0;

		int column_num = 0;
		int row_num = 0;

		column_num = total_fixed_width(&w);
		row_num = total_fixed_height(&h);

		if(new_size->width() > static_cast<unsigned int>(w)) {

			for (int j = 0; j < m_columns; j++)
			{
				column_width[j] = fixed_column_width(j);
				if(column_width[j] > 0)
					total_width += column_width[j];
			}

			if(column_num < m_columns) {
				int single_expandable_width = (new_size->width() - margin().left() - margin().right() - space() * (m_columns - 1)- total_width) / (m_columns - column_num);

				for (int j = 0; j < m_columns; j++)
				{
					if(column_width[j] < 0)
						column_width[j] = single_expandable_width;
				}
			}

		} else {

			for (int j = 0; j < m_columns; j++)
			{
				column_width[j] = fixed_column_width(j);
				if(column_width[j] < 0) {
					column_width[j] = minimal_column_width(j);
				}

				total_width += column_width[j];
			}

			if(column_num > 0) {
				int single_fixed_width_diff = size().width() - new_size->width() / column_num;

				for (int j = 0; j < m_columns; j++)
				{
					if(!(fixed_column_width(j) < 0)) {
						column_width[j] = column_width[j] - single_fixed_width_diff;
					}
				}
			}

		}

		if(new_size->height() > static_cast<unsigned int>(h)) {

			for (int i = 0; i < m_rows; i++)
			{
				row_height[i] = fixed_row_height(i);
				if(row_height[i] > 0)
					total_height += row_height[i];
			}

			if(row_num < m_rows) {
				int single_expandable_height = (new_size->height() - margin().top() - margin().bottom() - space() * (m_rows - 1)- total_height) / (m_rows - row_num);

				for (int i = 0; i < m_rows; i++)
				{
					if(row_height[i] < 0)
						row_height[i] = single_expandable_height;
				}
			}

		} else {

			for (int i = 0; i < m_rows; i++)
			{
				row_height[i] = fixed_row_height(i);
				if(row_height[i] < 0) {
					row_height[i] = minimal_row_height(i);
				}

				total_height += row_height[i];
			}

			if(row_num > 0) {
				int single_fixed_height_diff = size().height() - new_size->height() / row_num;

				for (int i = 0; i < m_rows; i++)
				{
					if(!(fixed_row_height(i) < 0)) {
						row_height[i] = row_height[i] - single_fixed_height_diff;
					}
				}
			}

		}

#ifdef DEBUG
		std::cout << "        ";
		for(int j = 0; j < m_columns; j++)
		{
			std::cout << column_width[j] << " ";
		}
		std::cout << std::endl;

		for(int i = 0; i < m_rows; i++)
		{
			std::cout << row_height[i] << std::endl;
		}
#endif

		int x = position().x() + margin().left();
		int y = position().y() + new_size->height() - margin().top();
		for(int i = 0; i < m_rows; i++)
		{
			y = y - row_height[i];

			for (int j = 0; j < m_columns; j++)
			{
				child = m_items[i * m_columns + j];
				if(!child) continue;

				SetSubWidgetPosition(child, x, y);

				if(child->expand_y()) {
					ResizeSubWidget(child, child->size().width(), row_height[i]);
				}
				if (child->expand_x()) {
					ResizeSubWidget(child, column_width[j], child->size().height());
				}

				if (alignment() & AlignTop) {
					SetSubWidgetPosition(child, child->position().x(),
						        child->position().y() + row_height[i] - child->size().height());
				} else if (alignment() & AlignBottom) {
					SetSubWidgetPosition(child, child->position().x(),
								child->position().y());
				} else if (alignment() & AlignHorizontalCenter) {
					SetSubWidgetPosition(child, child->position().x(),
								child->position().y() + (row_height[i] - child->size().height()) / 2);
				}
				x = x + column_width[j] + space();
			}

			x = position().x() + margin().left();
			y = y - space();
		}

		return true;
	}

	void TableLayout::generate_default_layout ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = 0;

		AbstractWidget* child = 0;

		std::vector<unsigned int> row_height(m_rows, 0);
		std::vector<unsigned int> column_width(m_columns, 0);

		for(int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_columns; j++)
			{
				child = m_items[i * m_columns + j];
				if(child) {
					column_width[j] = std::max(column_width[j], child->size().width());
					row_height[i] = std::max(row_height[i], child->size().height());
				}
			}
		}

		for(int i = 0; i < m_rows; i++) total_height += row_height[i];
		for(int j = 0; j < m_columns; j++) total_width += column_width[j];

		total_width += margin().left() + margin().right() + space() * (m_columns - 1);
		total_height += margin().top() + margin().bottom() + space() * (m_rows - 1);

		int x = position().x() + margin().left();
		int y = position().y() + total_height - margin().top();
		for(int i = 0; i < m_rows; i++)
		{
			y = y - row_height[i];

			for (int j = 0; j < m_columns; j++)
			{
				child = m_items[i * m_columns + j];
				if(!child) continue;

				SetSubWidgetPosition(child, x, y);

				if(child->expand_y()) {
					ResizeSubWidget(child, child->size().width(), row_height[i]);
				}
				if (child->expand_x()) {
					ResizeSubWidget(child, column_width[j], child->size().height());
				}

				if (alignment() & AlignTop) {
					SetSubWidgetPosition(child, child->position().x(),
						        child->position().y() + row_height[i] - child->size().height());
				} else if (alignment() & AlignBottom) {
					SetSubWidgetPosition(child, child->position().x(),
								child->position().y());
				} else if (alignment() & AlignHorizontalCenter) {
					SetSubWidgetPosition(child, child->position().x(),
								child->position().y() + (row_height[i] - child->size().height()) / 2);
				}
				x = x + column_width[j] + space();
			}

			x = position().x() + margin().left();
			y = y - space();
		}

		Resize(total_width, total_height);
	}

	int TableLayout::fixed_column_width(int column)
	{
		int fixed_width = -1;	// the return value
		AbstractWidget* child = 0;

		for (int i = 0; i < m_rows; i++)
		{
			child = m_items[i * m_columns + column];
			if(child) {
				if(!child->expand_x()) {
					fixed_width = std::max(fixed_width, static_cast<int>(child->size().width()));
				}
			}
		}

		return fixed_width;
	}

	int TableLayout::fixed_row_height(int row)
	{
		int fixed_height = -1;	// the return value
		AbstractWidget* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_items[row * m_columns + j];
			if(child) {
				if(!child->expand_y()) {
					fixed_height = std::max(fixed_height, static_cast<int>(child->size().height()));
				}
			}
		}

		return fixed_height;
	}

	unsigned int TableLayout::minimal_column_width(int column)
	{
		unsigned int minimal_width = 0;	// the return value

		/*
		AbstractWidget* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_items[j * m_columns + column];
			if(child) {
				minimal_width = std::max(minimal_width, child->minimal_size().width());
			}
		}
		*/

		return minimal_width;
	}
	
	void TableLayout::MakeLayout (const Size* size, const Margin* margin,
					int space)
	{
	}
	
	void TableLayout::CountExpandableNumber (unsigned int* cols,
					unsigned int* rows)
	{
		//unsigned int col_num = 0;
		//unsigned int row_num = 0;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{

		}
	}

	unsigned int TableLayout::minimal_row_height(int row)
	{
		unsigned int minimal_height = 0;	// the return value

		/*
		AbstractWidget* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_items[row * m_columns + j];
			if(child) {
				minimal_height = std::max(minimal_height, child->minimal_size().height());
			}
		}
		*/

		return minimal_height;

	}

	void TableLayout::debug_print()
	{
		int fixed_width = 0;
		int fixed_height = 0;
		int columns = 0;
		int rows = 0;

		columns = total_fixed_width(&fixed_width);

		rows = total_fixed_height(&fixed_height);

		std::cout << "total fixed width, columns: " << columns << " width: " << fixed_width << std::endl;
		std::cout << "total fixed height, rows: " << rows << " height: " << fixed_height << std::endl;
	}

	int TableLayout::total_fixed_width(int* width)
	{
		int total = 0;
		int columns_with_fixed_width = 0;
		int column_width = 0;

		for(int j = 0; j < m_columns; j++)
		{
			column_width = fixed_column_width(j);
			if(column_width < 0) {
				column_width = minimal_column_width(j);
			} else {
				columns_with_fixed_width++;
			}

			total += column_width;
		}

		total += margin().left() + margin().right() + space() * (m_columns - 1);

		*width = total;

		return columns_with_fixed_width;
	}

	int TableLayout::total_fixed_height(int* height)
	{
		int total = 0;
		int rows_with_fixed_height = 0;
		int row_height = 0;

		for(int i = 0; i < m_rows; i++)
		{
			row_height = fixed_row_height(i);
			if(row_height < 0) {
				row_height = minimal_row_height(i);
			} else {
				rows_with_fixed_height++;
			}

			total += row_height;
		}

		total += margin().top() + margin().bottom() + space() * (m_rows - 1);

		*height = total;

		return rows_with_fixed_height;
	}

}
