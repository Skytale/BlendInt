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

#ifndef _BLENDINT_FORM_HPP_
#define _BLENDINT_FORM_HPP_

#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * @brief A Normal form
	 */
	class Widget: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		Widget (AbstractWidget* parent);

		virtual ~Widget();

//		void set_emboss (bool emboss);

//		bool emboss () const {return m_emboss;}

		void set_border_width (float width = 1.0);

		float border_width () const {return m_border_width;}

	protected:

		virtual void update (int type, const void* data);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		/**
		 * @brief draw vertices without buffer
		 */
		void draw_outline (const float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], int num);

		/**
		 * @brief draw vertices without buffer
		 */
		void draw_inner (const float inner_v[WIDGET_SIZE_MAX][2], int num);

		/**
		 * @brief draw the GL Buffer in render()
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h, 
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_inner_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_POLYGON);

		/**
		 * @brief draw shaded GL buffer in render()
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_shaded_inner_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_POLYGON);

		/**
		 * @brief draw the GL Buffer in render() with anti-alias
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_outline_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_QUAD_STRIP);

		void generate_form_buffer (const Size* size, bool emboss, int round_type, float radius, GLBuffer* buffer);

		void generate_form_buffer (const Size* size, bool emboss, GLBuffer* buffer);

	private:

//		void update_shape (const Size* size);

		/**
		 * @brief border width
		 */
		DRAWABLE_PROPERTY float m_border_width;

		/**
		 * @brief If draw emboss
		 *
		 * @todo only menu and scroll bar set emboss off, remove this property later
		 */
//		DRAWABLE_PROPERTY bool m_emboss;

	};
}

#endif /* _BLENDINT_FORM_HPP_ */
