/**
 * BlendInt demo
 */

#include <GL/glew.h>

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/ContextManager.hpp>

#include <BlendInt/ToggleButton.hpp>
#include <BlendInt/VerticalLayout.hpp>
#include <BlendInt/HorizontalLayout.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/ScrollWidget.hpp>
#include <BlendInt/Slider.hpp>
#include <BlendInt/Frame.hpp>
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/TableLayout.hpp>
#include <BlendInt/ScrollBar.hpp>
#include <BlendInt/GLBuffer.hpp>
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/ScrollView.hpp>
#include <BlendInt/PopupWidget.hpp>
#include <BlendInt/Menu.hpp>
#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/ImageView.hpp>
#include <BlendInt/TabFrame.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

class DoEvent {
public:

	DoEvent() :
			i(0) {
		m_layout.SetPosition(600, 100);
		//m_hlayout.Resize(400, 100);
//		m_hlayout.set_sizing_mode(LayoutFixed);
//		m_hlayout.set_margin(5, 5, 5, 5);
//		m_hlayout.set_space (2);
//		m_layout.set_alignment(AlignLeft);

//		m_vlayout = new VerticalLayout;
//		m_vlayout->set_space (2);

//		m_hlayout.add(m_vlayout);

	}

	~DoEvent() {

	}

	void connect_add(Button* button) {
		m_events.connect(button->clicked(), this, &DoEvent::add_button);
	}

	void connect_remove(Button* button) {
		m_events.connect(button->clicked(), this, &DoEvent::remove_button);
	}

	void bind() {
		interface()->Bind(&m_layout);
	}

	void unbind() {
		interface()->Unbind(&m_layout);
	}

	void add_button() {
		Button* button = new Button;

		//char str[20];
		//sprintf(str, "%s%u", "button", static_cast<unsigned int>(m_buttons.size()));

		//button->set_text(str);
		//button->set_name(str);

		m_layout.Add(button);
		m_buttons.push_back(button);
	}

	void remove_button() {
		if (m_buttons.size()) {
			Button* button = m_buttons[m_buttons.size() - 1];
			m_layout.Erase(button);
			m_buttons.pop_back();
			m_layout.Resize(m_layout.preferred_size());
		}
	}

private:

//	VerticalLayout *m_vlayout;

//	HorizontalLayout m_layout;
	VerticalLayout m_layout;

	std::vector<Button*> m_buttons;

	int i;

	Cpp::ConnectionScope m_events;
};

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* window = CreateWindow("GLFW3 Demo");

	Interface* app = Interface::Instance();

//	DoEvent obj;
//
//	obj.bind();
//
//	Button* add_button = new Button;
//	add_button->set_text("Add Button");
//	add_button->SetPosition(600, 700);
//
//	obj.connect_add(add_button);
//
//	Button* remove_button = new Button;
//	remove_button->set_text("Remove Button");
//	remove_button->SetPosition(600, 650);
//
//	obj.connect_remove(remove_button);
//
//	app->bind(add_button);
//	app->bind(remove_button);

	//-----------------------
//
//	Cpp::ConnectionScope events;
//
//	Button* reset_button = new Button;
//	reset_button->set_text("Reset");
//	reset_button->move(640, 300);
//
//	app->bind(reset_button);
//
//	ScrollView* scroll_view = new ScrollView;
//
//	scroll_view->SetPosition(200, 200);
//	scroll_view->Resize(300, 200);
//
//	Label* label = new Label("Hello World!");
//
//	label->SetPosition(50, 50);
//
//	label->set_text ("alsdkjflasdjflasfnvlkaseasdfasdfsadfasfsadfadsfasdfwaeasbvdfsfbfyful7rbsdbfage");
//	label->Resize(400, 400);
//
//	scroll_view->set_viewport(label);
//
//	app->bind(scroll_view);
//
//	events.connect(reset_button->clicked(), scroll_view, &ScrollView::reset_viewport_position);

//	PopupWidget* widget = new PopupWidget;
//	widget->SetPosition(200, 200);
//	widget->SetRoundType(CornerAll);
//	widget->Resize(200, 200);
////
//	Frame* widget2 = new Frame;
//	widget2->move(450, 200);
////	widget2->SetRoundType(CornerAll);
//	widget2->Resize(200, 200);
////
//	app->bind(widget);
//	app->bind(widget2);
//

//	hl1->Resize(800, 40);

//	Menu *menu = new Menu;
//
//	menu->Resize(200, 400);
//	menu->SetPosition(100, 100);
//	menu->SetRoundType(RoundAll);
//
//	menu->add("File");
//	menu->add("exit");
//	menu->add("there2");
//	menu->add("there2");
//	menu->add("there2");
//	menu->add("there2");
//	menu->add("there2");
//	menu->add("there2");
//	menu->add("there2");
//
//	app->bind(menu);

//	Button* b = new Button;
//
//	b->SetPosition(800, 300);
//
//	//b->SetRoundType(RoundTopLeft | RoundTopRight);
//
//	b->SetRoundType(RoundTopLeft|RoundTopRight);
//	b->SetRadius(6.0);
//
////	b->Resize(100, 25);
//
//	app->bind(b);

//	ToggleButton* w = new ToggleButton;
//
//	w->SetPosition(500, 200);
//
//	app->bind(w);

//	RoundWidget* rw = new RoundWidget;
//	rw->SetRoundType(RoundAll);
//	rw->Resize (200, 200);
//	rw->SetPosition(100, 300);
//
//	app->bind(rw);
//
//	Frame* f = new Frame;
//	f->set_widget(rw);
//
//	f->Resize(500, 400);
//	f->SetPosition(200, 200);
//	app->bind(f);
//
//	Widget* w = new Widget;
//	w->Resize (100, 100);
//	w->SetPosition(100, 300);
//
//	app->bind(w);

//	ImageView *iv = new ImageView;
//	iv->SetPosition(400, 300);
//
//	app->bind(iv);

	std::cout << "size of widget: " << sizeof(Widget) << std::endl;

	std::cout << "size of slider: " << sizeof(Slider) << std::endl;

	std::cout << "size of button: " << sizeof(Button) << std::endl;

	std::cout << "size of layout: " << sizeof(HorizontalLayout) << std::endl;

	std::cout << "size of menu item: " << sizeof(MenuItem) << std::endl;

	DemoFrame* frame = new DemoFrame;

	app->Bind(frame);

	SliderBar* sliderbar = new SliderBar;

	sliderbar->Move(400, 400);

	app->Bind(sliderbar);

	TabFrame* tabframe = new TabFrame;

	tabframe->Move(600, 300);

	app->Bind(tabframe);

	// app->events()->connect(app->resized(), frame, &DemoFrame::FullWindow);

	RunLoop(window);

//	obj.unbind();

	Terminate();
}

