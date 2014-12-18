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

#ifndef _BLENDINT_GUI_ABSTRACTFRAME_HPP_
#define _BLENDINT_GUI_ABSTRACTFRAME_HPP_

#include <glm/glm.hpp>

#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class FrameSplitter;

	class AbstractFrame: public AbstractView
	{
	public:

		friend class Context;

		AbstractFrame ();

		virtual ~AbstractFrame ();

		virtual AbstractView* GetFocusedView () const = 0;

		Point GetAbsolutePosition (const AbstractWidget* view);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractView* view);

	protected:

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event) = 0;

		ResponseType DispatchKeyEvent (AbstractView* view, const KeyEvent& event);

		AbstractView* DispatchMousePressEvent (AbstractView* view, const MouseEvent& event);

		ResponseType DispatchMouseMoveEvent (AbstractView* view, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (AbstractView* view, const MouseEvent& event);

		AbstractWidget* DispatchHoverEventsInSubWidgets (AbstractWidget* orig, const MouseEvent& event);

		AbstractFrame* CheckHoveredFrame (AbstractFrame* old, const MouseEvent& event);

		void ClearHoverWidgets (AbstractView* hovered_widget);

		void ClearHoverWidgets (AbstractView* hovered_widget, const MouseEvent& event);

		Cpp::ConnectionScope* events() const {return events_.get();}

		static inline ResponseType delegate_key_press_event (AbstractView* view, const KeyEvent& event)
		{
			return view->KeyPressEvent(event);
		}

		static inline ResponseType delegate_mouse_press_event (AbstractView* view, const MouseEvent& event)
		{
			return view->MousePressEvent(event);
		}

		static inline ResponseType delegate_mouse_release_event(AbstractView* view, const MouseEvent& event)
		{
			return view->MouseReleaseEvent(event);
		}

		static inline ResponseType delegate_mouse_move_event(AbstractView* view, const MouseEvent& event)
		{
			return view->MouseMoveEvent(event);
		}

		static inline void delegate_focus_status (AbstractView* view, bool focus)
		{
			view->set_focus(focus);
		}

		static void delegate_focus_event (AbstractView* view, bool focus)
		{
			view->set_focus(focus);
			view->FocusEvent(focus);
		}

		static inline void delegate_mouse_hover_in_event (AbstractView* view, const MouseEvent& event)
		{
			view->set_hover(true);
			view->MouseHoverInEvent(event);
		}

		static inline void delegate_mouse_hover_out_event (AbstractView* view, const MouseEvent& event)
		{
			view->set_hover(false);
			view->MouseHoverOutEvent(event);
		}

		static inline void delegate_dispatch_hover_event(AbstractFrame* frame, const MouseEvent& event)
		{
			frame->DispatchHoverEvent(event);
		}

		static inline void assign_profile_frame (Profile& profile, AbstractFrame* frame)
		{
			profile.frame_ = frame;
		}

		static inline void assign_event_frame (const HIDEvent& event, AbstractFrame* frame)
		{
			const_cast<HIDEvent&>(event).frame_ = frame;
		}

	private:

		friend class FrameSplitter;

		AbstractWidget* DispatchHoverEventDeeper (AbstractWidget* view, const MouseEvent& event, Point& local_position);

		inline void set_widget_hover_status (AbstractView* view, bool hover)
		{
			view->set_hover(hover);
		}

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFRAME_HPP_ */
