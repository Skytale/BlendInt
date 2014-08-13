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
#endif  // __UNIX__

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Section.hpp>

namespace BlendInt {

	Margin AbstractButton::default_button_padding = Margin(2, 2, 2, 2);

	AbstractButton::AbstractButton ()
	: AbstractWidget(),
	  text_length_(0)
	{
	}

	AbstractButton::~AbstractButton ()
	{

	}

	Size AbstractButton::GetPreferredSize () const
	{
		Size preferred_size;

		int radius_plus = 0;

		if ((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += round_radius();
		}

		if ((round_type() & RoundTopRight)
				|| (round_type() & RoundBottomRight)) {
			radius_plus += round_radius();
		}

		int max_font_height = font_.GetHeight();

		preferred_size.set_height(
				max_font_height + default_button_padding.vsum()); // top padding: 2, bottom padding: 2

		if (text_.empty()) {
			preferred_size.set_width(
					max_font_height + default_button_padding.hsum()
							+ radius_plus);
		} else {
			int width = font().GetTextWidth(text());
			preferred_size.set_width(
					width + default_button_padding.hsum() + radius_plus); // left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	void AbstractButton::SetText (const String& text)
	{
		text_ = text;
		text_length_ = UpdateTextPosition(size(), round_type(), round_radius(), text, font_);
	}

	void AbstractButton::SetFont (const Font& font)
	{
		font_ = font;
		text_length_ = UpdateTextPosition(size(), round_type(), round_radius(), text_, font_);
	}

	void AbstractButton::UpdateTextPosition(const Size& size, int round_type, float radius, const String& text)
	{
		text_length_ = UpdateTextPosition(size, round_type, radius, text, font_);
	}

	int AbstractButton::UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, Font& font)
	{
		if(text.length() == 0) {
			return 0;
		}

		int text_length = 0;

		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		float radius_plus = 0.f;
		int x = DefaultButtonPadding().left(); int y = DefaultButtonPadding().bottom();

		if((round_type & RoundTopLeft) || (round_type & RoundBottomLeft)) {
			radius_plus += radius;
			x += (int)radius;
		}

		if((round_type & RoundTopRight) || (round_type & RoundBottomRight)) {
			radius_plus += radius;
		}

		int valid_width = size.width() - DefaultButtonPadding().hsum() - (int)radius_plus;
		int valid_height = size.height() - DefaultButtonPadding().vsum();

		if(valid_width <= 0 || valid_height <= 0) {
			return 0;
		}

		Rect text_outline = font.GetTextOutline(text);

		if(valid_height < text_outline.height()) {
			text_length = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(valid_width < text_outline.width()) {
				text_length = GetValidTextLength(text, font, valid_width);
			} else {
				text_length = text.length();
				x = (size.width() - text_outline.width()) / 2;
			}
			y = (size.height() - font.GetHeight()) / 2 + std::abs(font.GetDescender());
		}

		font.set_pen(x, y);

		return text_length;
	}

	ResponseType AbstractButton::CursorEnterEvent(bool entered)
	{
		if(entered) {

			if(m_status[ButtonPressed]) {
				m_status[ButtonDown] = 1;

				if(m_status[ButtonCheckable]) {
					m_status[ButtonChecked] = !m_status[ButtonChecked];
				}
			}

			Refresh();
		} else {

			if(m_status[ButtonPressed]) {
				m_status[ButtonDown] = 0;

				if(m_status[ButtonCheckable]) {
					m_status[ButtonChecked] = !m_status[ButtonChecked];
				}

			}

			Refresh();
		}

		return Accept;
	}

	ResponseType AbstractButton::MousePressEvent (const MouseEvent& event)
	{
		if (event.button() == MouseButtonLeft) {
			m_status.set(ButtonPressed);
			m_status.set(ButtonDown);

			if (m_status[ButtonCheckable]) {
				m_status[ButtonLastChecked] = m_status[ButtonChecked];
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}

			Refresh();

			pressed_.fire();
		}

		return Accept;
	}

	ResponseType AbstractButton::MouseReleaseEvent(const MouseEvent& event)
	{
		if (event.button() == MouseButtonLeft) {
			int fire_event = 0;	// 0: no event, 1: click event, 2: toggled event

			if (m_status[ButtonCheckable]) {
				if (m_status[ButtonPressed]) {
					fire_event = 2;
				}
			} else {
				if (m_status[ButtonPressed] && m_status[ButtonDown]) {
					fire_event = 1;
				}
			}

			Refresh();

			switch (fire_event) {

				case 0:
					break;

				case 1:
					clicked_.fire();
					break;

				case 2: {
					if (m_status[ButtonChecked]
									!= m_status[ButtonLastChecked]) {
						toggled_.fire(m_status[ButtonChecked]);
					}
					break;
				}

				default:
					break;
			}

			m_status.reset(ButtonPressed);
			m_status.reset(ButtonDown);

			released_.fire();

			return Accept;
		}

		return Ignore;
	}

	ResponseType AbstractButton::MouseMoveEvent (const MouseEvent& event)
	{
		/*
		if (m_status[ButtonDown]) {
			event->accept(this);
			return;
		}
		*/
		return Accept;
	}
	
	void AbstractButton::SetDown (bool down)
	{
		if(m_status[ButtonCheckable]) {
			if(m_status[ButtonChecked] != down)
				Refresh();

			m_status[ButtonChecked] = down ? 1 : 0;
		} else {

			if(m_status[ButtonDown] != down)
				Refresh();

			m_status[ButtonDown] = down ? 1 : 0;
		}
	}

	void AbstractButton::SetCheckable (bool checkable)
	{
		if(!checkable) {
			m_status[ButtonChecked] = false;
		}

		m_status[ButtonCheckable] = checkable ? 1 : 0;
	}

	void AbstractButton::SetChecked (bool checked)
	{
		if(m_status[ButtonCheckable]) {

			if(m_status[ButtonChecked] == checked)
				return;

			m_status[ButtonChecked] = checked ? 1 : 0;
			Refresh();

			toggled_.fire(m_status[ButtonChecked]);
		}
	}

	ResponseType AbstractButton::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType AbstractButton::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractButton::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractButton::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	int AbstractButton::GetValidTextLength(const String& text, const Font& font, int max_width)
	{
		int width = 0;
		int str_len = text.length();

		width = font.GetTextWidth(text, str_len, 0);

		if(width > max_width) {
			while(str_len > 0) {
				width = font.GetTextWidth(text, str_len, 0);
				if(width < max_width) break;
				str_len--;
			}
		}

		return str_len;
	}

} /* namespace BlendInt */
