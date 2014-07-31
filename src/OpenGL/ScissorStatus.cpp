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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <BlendInt/OpenGL/ScissorStatus.hpp>

namespace BlendInt {

	void ScissorCropShape::Crop ()
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor(area().x(), area().y(), area().width(), area().height());
	}

	void ScissorCropShape::Reset ()
	{
		glDisable (GL_SCISSOR_TEST);
	}

	void CropStack::Push (const Rect& area)
	{
		Push(area.x(), area.y(), area.width(), area.height());
	}

	void CropStack::Push (int x, int y, int w, int h)
	{
		int xmax = x + w;
		int ymax = y + h;

		int top_xmax = xmax;
		int top_ymax = ymax;
		int new_x = x;
		int new_y = y;

		if(stack_.size()) {
			new_x = std::max(x, stack_.top()->area().x());
			new_y = std::max(y, stack_.top()->area().y());
			top_xmax = stack_.top()->area().x() + stack_.top()->area().width();
			top_ymax = stack_.top()->area().y() + stack_.top()->area().height();
		}

		int new_width = std::min(xmax, top_xmax) - new_x;
		int new_height = std::min(ymax, top_ymax) - new_y;

		if(new_width < 0)
			new_width = 0;

		if(new_height < 0)
			new_height = 0;

		ScissorCropShape* p = new ScissorCropShape(new_x, new_y, new_width, new_height);
		stack_.push(p);

		p->Crop();
	}

	void CropStack::Push (const Point& pos, const Size& size, int round_type,
			float radius)
	{
		// TODO use stencil for crop
	}

	void CropStack::Pop ()
	{
		if(stack_.size()) {
			AbstractCropShape* p = stack_.top();
			stack_.pop();
			delete p;
			p = 0;

			if(stack_.size()) {
				p = stack_.top();
				p->Crop();
			}
		}
	}

	AbstractCropShape* CropStack::GetCurrent () const
	{
		if(stack_.size()) {
			return stack_.top();
		} else {
			return 0;
		}
	}

	ScissorStatus::ScissorStatus ()
	{
	}

	ScissorStatus::~ScissorStatus ()
	{
	}

	void ScissorStatus::Push (const Rect& area)
	{
		m_area_stack.push(area);
	}

	void ScissorStatus::Pop ()
	{
		m_area_stack.pop();
	}

	void ScissorStatus::Push (int x, int y, int w, int h)
	{
		int xmax = x + w;
		int ymax = y + h;

		int top_xmax = xmax;
		int top_ymax = ymax;
		int new_x = x;
		int new_y = y;

		if(m_area_stack.size()) {
			new_x = std::max(x, m_area_stack.top().x());
			new_y = std::max(y, m_area_stack.top().y());
			top_xmax = m_area_stack.top().x() + m_area_stack.top().width();
			top_ymax = m_area_stack.top().y() + m_area_stack.top().height();
		}

		int new_width = std::min(xmax, top_xmax) - new_x;
		int new_height = std::min(ymax, top_ymax) - new_y;

		if(new_width < 0)
			new_width = 0;

		if(new_height < 0)
			new_height = 0;

		Rect right_rect;
		right_rect.set_x(new_x);
		right_rect.set_y(new_y);
		right_rect.set_width(new_width);
		right_rect.set_height(new_height);

		m_area_stack.push(right_rect);
	}

	void ScissorStatus::Enable ()
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_area_stack.top().x(),
				m_area_stack.top().y(),
				m_area_stack.top().width(),
				m_area_stack.top().height());
	}

	void ScissorStatus::Disable()
	{
		if(m_area_stack.empty())
			glDisable(GL_SCISSOR_TEST);
	}

}
