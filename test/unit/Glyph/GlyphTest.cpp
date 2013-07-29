// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <BIL/FontConfig.h>
#include <BIL/Glyph.h>
#include <iostream>
#include <string>

#include "GlyphTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(GlyphTest);

GlyphTest::GlyphTest ()
{

}

GlyphTest::~GlyphTest ()
{

}

void GlyphTest::setUp ()
{
	int ret = glfwInit();

	if (ret != GL_TRUE) {
		CPPUNIT_FAIL("Cannot initialize glfw\n");
	}

	FontConfig::instance();
	bool fontinit = FontConfig::getService()->initialize();
	if (!fontinit) {
		CPPUNIT_FAIL("Cannot initialize FontManager\n");
	}
	fontinit = FontConfig::getService()->loadDefaultFontToMem();
	if (!fontinit) {
		CPPUNIT_FAIL("Cannot load default font\n");
	}
}

void GlyphTest::tearDown ()
{
	FontConfig::release();

	glfwTerminate();
}

void GlyphTest::create1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Glyph glyph (L'A', "Sans", 24, 96);

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef (20, 20, 0);
		// Test buffer render
		glyph.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::create2 ()
{
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Glyph a(L'A', "Sans", 12, 96);
	Glyph b(L'B', "Sans", 12, 96);
	Glyph c(L'C', "Sans", 12, 96);
	Glyph d(L'd', "Sans", 12, 96);
	Glyph e(L'e', "Sans", 12, 96);
	Glyph f(L'F', "Sans", 12, 96);

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(20, 20, 0);	// where begin draw text

		a.render();
		glTranslatef(a.getMetrics().horiAdvance, 0, 0);
		b.render();
		glTranslatef(b.getMetrics().horiAdvance, 0, 0);
		c.render();
		glTranslatef(c.getMetrics().horiAdvance, 0, 0);
		d.render();
		glTranslatef(d.getMetrics().horiAdvance, 0, 0);
		e.render();
		glTranslatef(e.getMetrics().horiAdvance, 0, 0);
		f.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext2 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Glyph a(L'A', "Sans", 12, 96);
	Glyph b(L'b', "Sans", 12, 96);
	Glyph c(L'仁', "Sans", 12, 96);
	Glyph d(L'D', "Sans", 12, 96);
	Glyph e(L'义', "Sans", 12, 96);
	Glyph f(L'f', "Sans", 12, 96);

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(20, 20, 0);	// where begin draw text

		a.render();
		glTranslatef(a.getMetrics().horiAdvance, 0, 0);
		b.render();
		glTranslatef(b.getMetrics().horiAdvance, 0, 0);
		c.render();
		glTranslatef(c.getMetrics().horiAdvance, 0, 0);
		d.render();
		glTranslatef(d.getMetrics().horiAdvance, 0, 0);
		e.render();
		glTranslatef(e.getMetrics().horiAdvance, 0, 0);
		f.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext3 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Glyph a(L'a', "Sans", 12, 96);
	Glyph b(L'b', "Sans", 12, 96);
	Glyph c(L'c', "Sans", 12, 96);
	Glyph d(L'd', "Sans", 12, 96);
	Glyph e(L'e', "Sans", 12, 96);
	Glyph f(L'f', "Sans", 12, 96);
	Glyph g(L'g', "Sans", 12, 96);
	Glyph h(L'h', "Sans", 12, 96);
	Glyph i(L'i', "Sans", 12, 96);
	Glyph j(L'j', "Sans", 12, 96);

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(20, 20, 0);	// where begin draw text

		a.render();
		glTranslatef(a.getMetrics().horiAdvance, 0, 0);
		b.render();
		glTranslatef(b.getMetrics().horiAdvance, 0, 0);
		c.render();
		glTranslatef(c.getMetrics().horiAdvance, 0, 0);
		d.render();
		glTranslatef(d.getMetrics().horiAdvance, 0, 0);
		e.render();
		glTranslatef(e.getMetrics().horiAdvance, 0, 0);
		f.render();
		glTranslatef(f.getMetrics().horiAdvance, 0, 0);
		g.render();
		glTranslatef(g.getMetrics().horiAdvance, 0, 0);
		h.render();
		glTranslatef(h.getMetrics().horiAdvance, 0, 0);
		i.render();
		glTranslatef(i.getMetrics().horiAdvance, 0, 0);
		j.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::checkkerning1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Glyph a(L'A', "Sans", 12, 96);
	Glyph b(L'b', "Sans", 12, 96);
	Glyph c(L'仁', "Sans", 12, 96);
	Glyph d(L'D', "Sans", 12, 96);
	Glyph e(L'义', "Sans", 12, 96);
	Glyph f(L'f', "Sans", 12, 96);

	FontConfig* fontserv = FontConfig::getService();
	FontType font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	Vec2l kerning;

	cout << endl;
	kerning = font.getKerning(a,b);
	cout << "Kerning between A, b: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(b,c);
	cout << "Kerning between b, c: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(c,d);
	cout << "Kerning between c, D: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(d,e);
	cout << "Kerning between D, E: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(e,f);
	cout << "Kerning between E, f: " << kerning.vec.x << ", " << kerning.vec.y << endl;

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(20, 20, 0);	// where begin draw text

		a.render();
		glTranslatef(a.getMetrics().horiAdvance, 0, 0);
		b.render();
		glTranslatef(b.getMetrics().horiAdvance, 0, 0);
		c.render();
		glTranslatef(c.getMetrics().horiAdvance, 0, 0);
		d.render();
		glTranslatef(d.getMetrics().horiAdvance, 0, 0);
		e.render();
		glTranslatef(e.getMetrics().horiAdvance, 0, 0);
		f.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::create5 ()
{
	Coord3f test(1.0, 1.0, 1.0);
	Coord3f dist(1.0, 1.0, 1.0);

	CPPUNIT_ASSERT(test == dist);
}