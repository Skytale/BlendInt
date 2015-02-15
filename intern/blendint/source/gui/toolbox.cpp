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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <opengl/gl-framebuffer.hpp>

#include <gui/flow-layout.hpp>
#include <gui/toolbox.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	ToolBox::ToolBox (AbstractLayout* layout)
	: AbstractRoundFrame(),
	  focused_widget_(0),
	  hovered_widget_(0),
	  space_(1),
	  margin_(2, 2, 2, 2),
	  cursor_position_(0),
	  layout_(0)
	{
		if(layout == nullptr) {
			layout_ = Manage(new FlowLayout);
		} else {
			layout_ = layout;
		}

		PushBackSubView(layout_);
		set_size(layout_->size());

		InitializeToolBoxOnce();
	}

	ToolBox::ToolBox (int width, int height, AbstractLayout* layout)
	: AbstractRoundFrame(width, height),
	  focused_widget_(0),
	  hovered_widget_(0),
	  space_(1),
	  cursor_position_(0),
	  layout_(0)
	{
		if(layout == nullptr) {
			layout_ = Manage(new FlowLayout);
		} else {
			layout_ = layout;
		}

		PushBackSubView(layout_);
		ResizeSubView(layout_, size());

		InitializeToolBoxOnce();
	}

	ToolBox::~ToolBox()
	{
		glDeleteVertexArrays(3, vao_);

		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void ToolBox::AddWidget (AbstractWidget* widget)
	{
		layout_->AddWidget(widget);
	}

	bool ToolBox::IsExpandX () const
	{
		return layout_->IsExpandX();
	}

	bool ToolBox::IsExpandY () const
	{
		return layout_->IsExpandY();
	}

	Size ToolBox::GetPreferredSize () const
	{
		return layout_->GetPreferredSize();
	}

	AbstractView* ToolBox::GetFocusedView() const
	{
		return focused_widget_;
	}

	bool ToolBox::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	void ToolBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			projection_matrix_  = glm::ortho(
				0.f,
				0.f + (float)request.size()->width(),
				0.f,
				0.f + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;
			GenerateVertices(size(), 1.f * AbstractWindow::theme->pixel(), RoundNone, 0.f, &inner_verts, &outer_verts);

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			vbo_.bind(0);
			float* ptr = (float*)vbo_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			vbo_.unmap();

			vbo_.reset();

			ResizeSubView(layout_, size());

			RequestRedraw();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool ToolBox::PreDraw (AbstractWindow* context)
	{
		if(!visiable()) return false;

		context->register_active_frame(this);

		if(refresh()) {
			RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_, &texture_buffer_);
		}

		//texture_buffer_.bind();
		//texture_buffer_.WriteToFile("file.png");
		//texture_buffer_.reset();

		return true;
	}

	Response ToolBox::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		AbstractWindow::shaders->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glBindVertexArray(vao_[1]);

		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.576f, 0.576f, 0.576f, 1.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 6);

		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.4f, 0.4f, 0.4f, 1.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        AbstractWindow::shaders->frame_image_program()->use();
        
        texture_buffer_.bind();
        glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);
        
        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Finish;
	}

	void ToolBox::PostDraw (AbstractWindow* context)
	{
	}

	void ToolBox::PerformFocusOn (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus in");
	}

	void ToolBox::PerformFocusOff (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus out");

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
			hovered_widget_ = 0;
		}
	}

	void ToolBox::PerformHoverIn (AbstractWindow* context)
	{
	}

	void ToolBox::PerformHoverOut (AbstractWindow* context)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, context);
			hovered_widget_ = 0;
		}
	}

	Response ToolBox::PerformKeyPress (AbstractWindow* context)
	{
		context->register_active_frame(this);

		Response response = Ignore;

		if(focused_widget_) {
			delegate_key_press_event(focused_widget_, context);
		}

		return response;
	}

	Response ToolBox::PerformMousePress (AbstractWindow* context)
	{
		context->register_active_frame(this);

		if(cursor_position_ == InsideRectangle) {

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, context);

				if(widget == 0) {
					DBG_PRINT_MSG("%s", "widget 0");
					set_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
				}

			} else {
				set_pressed(true);
				// SetFocusedWidget(0);
			}

		} else {
			set_pressed(false);
		}

		return Finish;
	}

	Response ToolBox::PerformMouseRelease (AbstractWindow* context)
	{
		cursor_position_ = InsideRectangle;
		set_pressed(false);

		if(focused_widget_) {
			context->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	Response ToolBox::PerformMouseMove (AbstractWindow* context)
	{
		Response retval = Ignore;

		if(focused_widget_) {
			context->register_active_frame(this);
			retval = delegate_mouse_move_event(focused_widget_, context);
		}

		return retval;
	}

	Response ToolBox::PerformMouseHover (AbstractWindow* context)
	{
		if(pressed_ext()) return Finish;

		if(Contain(context->GetCursorPosition())) {

			cursor_position_ = InsideRectangle;

			if(!hover()) {
				set_hover(true);
				PerformHoverIn(context);
			}

			AbstractWidget* new_hovered_widget = DispatchHoverEventsInWidgets(hovered_widget_, context);

			if(new_hovered_widget != hovered_widget_) {

				if(hovered_widget_) {
					hovered_widget_->destroyed().disconnectOne(this,
							&ToolBox::OnHoverWidgetDestroyed);
				}

				hovered_widget_ = new_hovered_widget;
				if(hovered_widget_) {
					events()->connect(hovered_widget_->destroyed(), this,
							&ToolBox::OnHoverWidgetDestroyed);
				}

			}

			return Finish;

		} else {

			cursor_position_ = OutsideRectangle;

			if(hover()) {
				set_hover(false);
				PerformHoverOut(context);
			}

			return Ignore;
		}
	}

	void ToolBox::InitializeToolBoxOnce()
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;
		GenerateVertices(size(), 1.f * AbstractWindow::theme->pixel(), RoundNone, 0.f, &inner_verts, &outer_verts);

		buffer_.generate();
		glGenVertexArrays(3, vao_);

		glBindVertexArray(vao_[0]);
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[2]);

		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		vbo_.generate();
		vbo_.bind(0);
		vbo_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				AttributeCoord);
		glEnableVertexAttribArray (
				AttributeUV);
		glVertexAttribPointer (AttributeCoord,
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (AttributeUV, 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_.reset();

		set_refresh(true);
	}

	void ToolBox::SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
			events()->connect(focused_widget_->destroyed(), this, &ToolBox::OnFocusedWidgetDestroyed);
		}
	}

	void ToolBox::OnFocusedWidgetDestroyed (AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void ToolBox::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

}