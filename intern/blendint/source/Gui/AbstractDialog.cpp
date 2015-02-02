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

#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLHeader.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

#include <BlendInt/Gui/AbstractDialog.hpp>

namespace BlendInt {
    
    AbstractDialog::AbstractDialog()
    : Frame(),
      focused_widget_(0),
      hovered_widget_(0),
      cursor_position_(InsideRectangle),
      dialog_flags_(0)
    {
		set_size(400, 300);
		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_refresh(true);
		//set_modal(modal);

        InitializeAbstractDialog();
    }
    
    AbstractDialog::AbstractDialog(int width, int height)
    : Frame(width, height),
    focused_widget_(0),
    hovered_widget_(0),
    cursor_position_(InsideRectangle),
    dialog_flags_(0)
    {
		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_refresh(true);
		//set_modal(modal);

        InitializeAbstractDialog();
    }

    AbstractDialog::~AbstractDialog()
    {
		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &AbstractDialog::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &AbstractDialog::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
    }

	void AbstractDialog::PerformSizeUpdate (
	        const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			projection_matrix_  = glm::ortho(
				0.f,
				0.f + (float)size().width(),
				0.f,
				0.f + (float)size().height(),
				100.f, -100.f);

			if(buffer_) {
				buffer_->Resize(size());
			}

			UpdateLayout();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool AbstractDialog::PreDraw (AbstractWindow* context)
	{
		if(!visiable()) return false;

		context->register_active_frame(this);

		if(refresh() && buffer_) {
			RenderSubFramesToTexture(this,
					context,
					projection_matrix_,
					model_matrix_,
					buffer_->texture());
		}

		return true;
	}

	ResponseType AbstractDialog::Draw (AbstractWindow* context)
	{
		if(buffer_) {

			AbstractWindow::shaders->frame_image_program()->use();

			glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
			glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
			glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);
			buffer_->Draw();
		}

		return Finish;
	}

	void AbstractDialog::PostDraw (AbstractWindow* context)
	{
	}

	void AbstractDialog::PerformFocusOn (AbstractWindow* context)
	{
	}

	void AbstractDialog::PerformFocusOff (AbstractWindow* context)
	{
	}

	void AbstractDialog::PerformHoverIn (AbstractWindow* context)
	{
	}

	void AbstractDialog::PerformHoverOut (AbstractWindow* context)
	{
	}

	ResponseType AbstractDialog::PerformKeyPress (
	        AbstractWindow* context)
	{
		ResponseType response = Ignore;

		if(context->GetKeyInput() == Key_Escape) {
			RequestRedraw();
			delete this;
			return Finish;
		}

		if(focused_widget_) {
			const_cast<AbstractWindow*>(context)->register_active_frame(this);
			response = DispatchKeyEvent(focused_widget_, context);
		}

		return response;
	}

	ResponseType AbstractDialog::PerformContextMenuPress (
	        AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType AbstractDialog::PerformContextMenuRelease (
	        AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType AbstractDialog::PerformMousePress (
	        AbstractWindow* context)
	{
		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		if(cursor_position_ == InsideRectangle) {

			last_position_ = position();
			cursor_point_ = context->GetCursorPosition();

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, context);

				if(widget == 0) {
					//DBG_PRINT_MSG("%s", "widget 0");
					set_mouse_button_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
				}

			} else {
				set_mouse_button_pressed(true);
			}

			if(!modal()) {
				const_cast<AbstractWindow*>(context)->MoveFrameToTop(this);
			}

			return Finish;

		} else if (cursor_position_ != OutsideRectangle) {

			set_mouse_button_pressed(true);

			last_position_ = position();
			last_size_ = size();
			cursor_point_ = context->GetCursorPosition();

			return Finish;
		}

		if(modal()) {
			return Finish;
		}

		return Ignore;
	}

	ResponseType AbstractDialog::PerformMouseRelease (
	        AbstractWindow* context)
	{
		cursor_position_ = InsideRectangle;
		set_mouse_button_pressed(false);

		if(focused_widget_) {
			context->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	ResponseType AbstractDialog::PerformMouseMove (
	        AbstractWindow* context)
	{
		ResponseType retval = Ignore;

		if(mouse_button_pressed()) {

			int ox = context->GetCursorPosition().x() - cursor_point_.x();
			int oy = context->GetCursorPosition().y() - cursor_point_.y();

			switch(cursor_position_) {

				case InsideRectangle: {
					set_position(last_position_.x() + ox, last_position_.y() + oy);
					break;
				}

				case OnLeftBorder: {
					set_position(last_position_.x() + ox, last_position_.y());
					Resize(last_size_.width() - ox, last_size_.height());
					break;
				}

				case OnRightBorder: {
					Resize(last_size_.width() + ox, last_size_.height());
					break;
				}

				case OnTopBorder: {
					Resize(last_size_.width(), last_size_.height() + oy);
					break;
				}

				case OnBottomBorder: {
					set_position(last_position_.x(), last_position_.y() + oy);
					Resize(last_size_.width(), last_size_.height() - oy);
					break;
				}

				case OnTopLeftCorner: {
					set_position(last_position_.x() + ox, last_position_.y());
					Resize(last_size_.width() - ox, last_size_.height() + oy);
					break;
				}

				case OnTopRightCorner: {
					Resize(last_size_.width() + ox, last_size_.height() + oy);
					break;
				}

				case OnBottomLeftCorner: {
					set_position(last_position_.x() + ox, last_position_.y() + oy);
					Resize(last_size_.width() - ox, last_size_.height() - oy);
					break;
				}

				case OnBottomRightCorner: {
					set_position(last_position_.x(), last_position_.y() + oy);
					Resize(last_size_.width() + ox, last_size_.height() - oy);
					break;
				}

				default: {
					return Finish;
					break;
				}

			}

			if(superview()) {
				superview()->RequestRedraw();
			}
			retval = Finish;

		} else {

			if(focused_widget_) {

				context->register_active_frame(this);
				retval = delegate_mouse_move_event(focused_widget_, context);

			}
		}

		return retval;
	}

	ResponseType AbstractDialog::DispatchHoverEvent (
	        AbstractWindow* context)
	{
		if(mouse_button_pressed()) return Finish;

		ResponseType retval = Finish;
		int border = 4;

		Rect valid_rect(position().x() - border, position().y() - border,
			size().width() + 2 * border, size().height() + 2 * border);

		if(valid_rect.contains(context->GetCursorPosition())) {

			if(Contain(context->GetCursorPosition())) {

				cursor_position_ = InsideRectangle;

				// DBG_PRINT_MSG("Cursor position: (%d, %d)", context->GetCursorPosition().x(), context->GetCursorPosition().y());

				AbstractWidget* new_hovered_widget = DispatchHoverEventsInWidgets(hovered_widget_, context);

				if(new_hovered_widget != hovered_widget_) {

					if(hovered_widget_) {
						hovered_widget_->destroyed().disconnectOne(this,
								&AbstractDialog::OnHoverWidgetDestroyed);
					}

					hovered_widget_ = new_hovered_widget;
					if(hovered_widget_) {
						events()->connect(hovered_widget_->destroyed(), this,
								&AbstractDialog::OnHoverWidgetDestroyed);
					}

				}

//				if(hovered_widget_) {
//					 DBG_PRINT_MSG("hovered widget: %s", hovered_widget_->name().c_str());
//				}

				// set cursor shape
				if(cursor_on_border()) {
					set_cursor_on_border(false);
					context->PopCursor();
				}

			} else {

				set_cursor_on_border(true);
				cursor_position_ = InsideRectangle;

				if(context->GetCursorPosition().x() <= position().x()) {
					cursor_position_ |= OnLeftBorder;
				} else if (context->GetCursorPosition().x() >= (position().x() + size().width())) {
					cursor_position_ |= OnRightBorder;
				}

				if (context->GetCursorPosition().y() >= (position().y() + size().height())) {
					cursor_position_ |= OnTopBorder;
				} else if (context->GetCursorPosition().y () <= position().y()) {
					cursor_position_ |= OnBottomBorder;
				}

				// set cursor shape
				switch(cursor_position_) {

					case OnLeftBorder:
					case OnRightBorder: {
						context->PushCursor();
						context->SetCursor(SplitHCursor);

						break;
					}

					case OnTopBorder:
					case OnBottomBorder: {
						context->PushCursor();
						context->SetCursor(SplitVCursor);
						break;
					}

					case OnTopLeftCorner:
					case OnBottomRightCorner: {
						context->PushCursor();
						context->SetCursor(SizeFDiagCursor);
						break;
					}

					case OnTopRightCorner:
					case OnBottomLeftCorner: {
						context->PushCursor();
						context->SetCursor(SizeBDiagCursor);
						break;
					}

					default:
						break;
				}

			}

		} else {
			cursor_position_ = OutsideRectangle;

			// set cursor shape
			if(cursor_on_border()) {
				set_cursor_on_border(false);
				context->PopCursor();
			}

			retval = Ignore;
		}

		// a modal dialog always return Finish
		if(modal()) {
			return Finish;
		} else {
			return retval;
		}
	}

	void AbstractDialog::SetFocusedWidget (AbstractWidget* widget,
	        AbstractWindow* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &AbstractDialog::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
			events()->connect(focused_widget_->destroyed(), this, &AbstractDialog::OnFocusedWidgetDestroyed);
		}
	}

	void AbstractDialog::OnFocusedWidgetDestroyed (
	        AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &AbstractDialog::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void AbstractDialog::OnHoverWidgetDestroyed (
	        AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &AbstractDialog::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

	void AbstractDialog::InitializeAbstractDialog ()
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

        applied_.reset(new Cpp::Event<AbstractDialog*>);
        canceled_.reset(new Cpp::Event<AbstractDialog*>);

        set_buffered(true);

        buffer_.reset(new ViewBuffer(size().width(), size().height()));
	}

}
