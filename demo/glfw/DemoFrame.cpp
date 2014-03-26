/*
 * DemoFrame.cpp
 *
 *  Created on: 2013楠�锟�11锟斤拷锟�16锟斤拷锟�
 *      Author: zhanggyb
 */

#include "DemoFrame.hpp"

#include <BlendInt/Gui/Label.hpp>
#include <iostream>

DemoFrame::DemoFrame()
: Frame(), m_hlayout(0), m_timer(0), m_widget(0)
{
	m_hlayout = new HLayout(AlignHorizontalCenter);
	//AddChild(m_hlayout);
	m_hlayout->SetMargin(0, 0, 0, 0);

	Label* label = new Label("Click to create: ");
	m_hlayout->Add(label);

	Button* start_button = new Button("Start");
	m_hlayout->Add(start_button);

	Button* reset_button = new Button("Reset Interval");
	m_hlayout->Add(reset_button);

	Button* stop_button = new Button("Stop");
	m_hlayout->Add(stop_button);

	set_size(m_hlayout->size().width() + margin().left() + margin().right(),
				m_hlayout->size().height() + margin().top() + margin().bottom());

	SetWidget(m_hlayout);

	SetPreferredSize(size());

	m_timer = new Timer;

	// connect timer
	events()->connect(start_button->clicked(),this, &DemoFrame::start_time);
	events()->connect(reset_button->clicked(), this, &DemoFrame::reset_time);
	events()->connect(stop_button->clicked(), this, &DemoFrame::stop_time);
}

DemoFrame::~DemoFrame()
{
	delete m_timer;
}


void DemoFrame::SetWidget (AbstractWidget* widget)
{
	if (widget && (widget != m_widget)) {
		delete m_widget;
		m_widget = widget;

		m_widget->SetPosition(position().x() + margin().left(),
		        position().y() + margin().bottom());
		m_widget->Resize(
		        size().width() - margin().left() - margin().right(),
		        size().height() - margin().top() - margin().bottom());

//		Attach(m_widget);
	}
}

void DemoFrame::FullWindow(unsigned int width, unsigned int height)
{
	std::cout << "Get FullWindow event: " << width << " " << height << std::endl;
	Resize(width, height);
}

bool DemoFrame::Update(const UpdateRequest& request)
{
	if(request.source() == Predefined) {

		switch (request.type()) {

			case FormPosition: {
				if (m_widget) {
					const Point* pos_p = static_cast<const Point*>(request.data());
					int offset_x = pos_p->x() - position().x();
					int offset_y = pos_p->y() - position().y();
					SetPosition(m_widget, m_widget->position().x() + offset_x,
					        m_widget->position().y() + offset_y);
				}

				return true;
			}

			case FormSize: {
				if (m_widget) {
					Size size = *(static_cast<const Size*>(request.data()));
					size.add_width(-(margin().left() + margin().right()));
					size.add_height(-(margin().top() + margin().bottom()));
					m_widget->Resize(size);
				}
				return true;
			}

			case ContainerMargin: {
				if (m_widget) {
					const Margin* margin_p = static_cast<const Margin*>(request.data());
					Size new_size(
					        size().width() - margin_p->left()
					                - margin_p->right(),
					        size().height() - margin_p->top()
					                - margin_p->bottom());
					m_widget->Resize(new_size);
				}
				return true;
			}

			default:
				return Frame::Update(request);
		}

	} else {
		return false;
	}
}

void DemoFrame::Draw (RedrawEvent* event)
{
	if(m_widget)
		DispatchRender(m_widget);
}

void DemoFrame::start_time()
{
	m_timer->Start();
}

void DemoFrame::reset_time()
{
	m_timer->SetInterval(2000);
}

ResponseType DemoFrame::KeyPressEvent(const KeyEvent& event)
{
	if(m_widget)
		return dispatch_key_press_event(m_widget, event);

	return IgnoreAndContinue;
}

ResponseType DemoFrame::ContextMenuPressEvent(const ContextMenuEvent& event)
{
	return IgnoreAndContinue;
}

ResponseType DemoFrame::ContextMenuReleaseEvent(const ContextMenuEvent& event)
{
	return IgnoreAndContinue;
}

ResponseType DemoFrame::MousePressEvent(const MouseEvent& event)
{
	if(m_widget)
		return dispatch_mouse_press_event(m_widget, event);

	return IgnoreAndContinue;
}

ResponseType DemoFrame::MouseReleaseEvent(const MouseEvent& event)
{
	if(m_widget)
		return dispatch_mouse_release_event(m_widget, event);

	return IgnoreAndContinue;
}

ResponseType DemoFrame::MouseMoveEvent(const MouseEvent& event)
{
	if(m_widget)
		return dispatch_mouse_move_event(m_widget, event);

	return IgnoreAndContinue;
}

void DemoFrame::stop_time()
{
	std::cout << "Stop timer" << std::endl;

	m_timer->Stop();
}
