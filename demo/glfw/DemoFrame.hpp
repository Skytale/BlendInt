/*
 * DemoFrame.hpp
 *
 *  Created on: 2013楠�锟�11锟斤拷锟�16锟斤拷锟�
 *      Author: zhanggyb
 */

#ifndef DEMOFRAME_HPP_
#define DEMOFRAME_HPP_

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Button.hpp>

#include <BlendInt/Core/Timer.hpp>

using namespace BlendInt;

class DemoFrame: public Frame
{
public:

	DemoFrame ();

	virtual ~DemoFrame();

	void SetWidget (AbstractWidget* widget);

	AbstractWidget* widget () const {return m_widget;}

	void FullWindow (unsigned int width, unsigned int height);

protected:

	virtual bool Update (const UpdateRequest& request);

	virtual void Draw (RedrawEvent* event);

	virtual ResponseType KeyPressEvent (const KeyEvent& event);

	virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

	virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

	virtual ResponseType MousePressEvent (const MouseEvent& event);

	virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

	virtual ResponseType MouseMoveEvent (const MouseEvent& event);

private:

	void start_time ();

	void reset_time ();

	void stop_time ();

	HLayout* m_hlayout;

	Timer* m_timer;

	AbstractWidget* m_widget;
};

#endif /* DEMOFRAME_HPP_ */
