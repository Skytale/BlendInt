/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BILO/Button.hpp>
#include <BILO/ShaderManager.hpp>

namespace BILO {

	Button::Button ()
			: AbstractButton()
	{
		set_roundcorner(RoundCornerAll);
		resize(90, 25);
	}

	Button::Button (const String& text)
			: AbstractButton()
	{
		set_roundcorner(RoundCornerAll);
		set_text(text);
	}

	Button::Button (Drawable* parent)
			: AbstractButton(parent)
	{
		set_roundcorner(RoundCornerAll);
		resize (90, 25);
	}

	Button::Button (const String& text, Drawable* parent)
			: AbstractButton(parent)
	{
		set_roundcorner(RoundCornerAll);
		set_text(text);
	}

	Button::~Button ()
	{

	}

	void Button::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Theme* tm = Theme::instance();

		// draw inner, simple fill
		if(m_status_down) {
			glColor4ub(tm->themeUI()->regular.inner_sel.r(),
					tm->themeUI()->regular.inner_sel.g(),
					tm->themeUI()->regular.inner_sel.b(),
					tm->themeUI()->regular.inner_sel.a());
		} else {
			if(m_status_hover) {
				glColor4ub(tm->themeUI()->regular.inner.highlight_red(),
						tm->themeUI()->regular.inner.highlight_green(),
						tm->themeUI()->regular.inner.highlight_blue(),
						tm->themeUI()->regular.inner.a());
			} else {
				glColor4ub(tm->themeUI()->regular.inner.r(),
						tm->themeUI()->regular.inner.g(),
						tm->themeUI()->regular.inner.b(),
						tm->themeUI()->regular.inner.a());
			}
		}

		m_buffer.set_index(0);

		m_buffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_POLYGON, 0, m_buffer.vertices());

		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

		// draw outline
		m_buffer.set_index(1);
		unsigned char tcol[4] = { tm->themeUI()->regular.outline.r(),
		        tm->themeUI()->regular.outline.g(),
		        tm->themeUI()->regular.outline.b(),
		        tm->themeUI()->regular.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		m_buffer.bind();

		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

		// Draw text
		FontCache::create(m_font)->print(
		        m_text_outline.left() + m_padding.left(),
		        m_padding.bottom() + std::abs(m_text_outline.bottom()), m_text);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
