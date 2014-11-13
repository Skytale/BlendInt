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

#include <BlendInt/Gui/VBlockLayout.hpp>

namespace BlendInt {

	VBlockLayout::VBlockLayout ()
	: Layout()
	{
		set_size(80, 60);
		set_margin(0, 0, 0, 0);
	}

	VBlockLayout::~VBlockLayout ()
	{
	}

	void VBlockLayout::Prepend (Widget* widget)
	{
		AbstractInteractiveForm* orig_first = first_child();

		if(PushBackSubWidget(widget)) {

			if(orig_first) {
				widget->SetEmboss(false);
				orig_first->SetRoundType(orig_first->round_type() & ~(RoundBottomLeft | RoundBottomRight));
				widget->SetRoundType(RoundBottomLeft | RoundBottomRight);
			} else {
				widget->SetEmboss(true);
				widget->SetRoundType(RoundAll);
			}

			FillInVBlock(size(), margin());

		}
	}

	void VBlockLayout::Append (Widget* widget)
	{
		AbstractInteractiveForm* orig_last = last_child();

		if(PushBackSubWidget(widget)) {

			widget->SetEmboss(true);

			if(orig_last) {
				orig_last->SetEmboss(false);
				orig_last->SetRoundType(orig_last->round_type() & ~(RoundBottomLeft | RoundBottomRight));
				widget->SetRoundType(RoundBottomLeft | RoundBottomRight);
			} else {
				widget->SetRoundType(RoundAll);
			}

			FillInVBlock(size(), margin());

		}
	}

	bool VBlockLayout::IsExpandX() const
	{
		bool expand = false;

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool VBlockLayout::IsExpandY () const
	{
		bool expand = false;

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size VBlockLayout::GetPreferredSize () const
	{
		Size preferred_size;

		if(first_child() == 0) {

			preferred_size.set_width(80);
			preferred_size.set_height(60);

		} else {

			Size tmp;
			int max_width = 0;
			int max_height = 0;
			int sum = 0;

			for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
			{
				if(p->visiable()) {
					sum++;
					tmp = p->GetPreferredSize();

					max_width = std::max(max_width, tmp.width());
					max_height = std::max(max_height, tmp.height());
				}
			}
			preferred_size.set_width(max_width);
			preferred_size.set_height(sum * (max_height - 1));

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	void VBlockLayout::PerformMarginUpdate(const Margin& request)
	{
		FillInVBlock(size(), request);
	}

	bool VBlockLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->parent() == this) {
			return false;
		}

		return true;
	}

	bool VBlockLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->parent() == this) {
			return false;
		}

		return true;
	}

	void VBlockLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillInVBlock(*request.size(), margin());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void VBlockLayout::FillInVBlock (const Size& out_size,
					const Margin& margin)
	{
		int x = margin.left();
		int y = margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		FillInVBlock(x, y, w, h);
	}

	void VBlockLayout::FillInVBlock (int x, int y, int w, int h)
	{
		int count = subs_count();
		if(count == 0) return;
		int average_height = h / count + 1;

		y = y + h;
		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			ResizeSubWidget(p, w, average_height);
			y = y - average_height + 1;
			SetSubWidgetPosition(p, x, y);
		}
	}

}