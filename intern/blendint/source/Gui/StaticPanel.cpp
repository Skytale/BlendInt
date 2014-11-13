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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/StaticPanel.hpp>

namespace BlendInt {

	using Stock::Shaders;

	StaticPanel::StaticPanel()
	: BinLayout()
	{
		set_size(400, 300);
		InitializeFramePanel();
	}
	
	StaticPanel::~StaticPanel ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void StaticPanel::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

            std::vector<GLfloat> inner_verts;
            GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

            inner_.bind();
            inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
            inner_.reset();

			if (subs_count()) {
				assert(subs_count() == 1);
				FillSingleWidget(0, *request.size(), margin());
			}

			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType StaticPanel::Draw (Profile& profile)
	{
		if(refresh()) {
			RenderToBuffer(profile);
		}

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		tex_buffer_.Draw(0.f, 0.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Accept;
	}

	void StaticPanel::InitializeFramePanel()
	{
        std::vector<GLfloat> inner_verts;
        
        GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);
        
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

        inner_.generate();
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
        inner_.reset();
	}

	void StaticPanel::RenderToFile (const std::string& filename)
	{
		tex_buffer_.texture()->bind();
		tex_buffer_.texture()->WriteToFile(filename);
		tex_buffer_.texture()->reset();
	}

	void StaticPanel::RenderToBuffer (Profile& profile)
	{
		GLsizei width = size().width();
		GLsizei height = size().height();

		tex_buffer_.SetCoord(0.f, 0.f, (float)width, (float)height);
		// Create and set texture to render to.
		GLTexture2D* tex = tex_buffer_.texture();
		if(!tex->id())
			tex->generate();

		tex->bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->generate();
		fb->bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->id(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		// Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
		GLuint rb;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
				size().width(), size().height());
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		if(GLFramebuffer::CheckStatus()) {

			fb->bind();

			Profile off_screen_profile(profile, GetGlobalPosition());

			glm::mat4 identity(1.f);
			Shaders::instance->PushWidgetModelMatrix();
			Shaders::instance->SetWidgetModelMatrix(identity);

			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClearDepth(1.0);
			glClearStencil(0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f,
			        -100.f);

			Shaders::instance->PushWidgetProjectionMatrix();
			Shaders::instance->SetWidgetProjectionMatrix(projection);

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			GLboolean scissor_test;
			glGetBooleanv(GL_SCISSOR_TEST, &scissor_test);
			glDisable(GL_SCISSOR_TEST);

			// Draw frame panel
			Shaders::instance->widget_inner_program()->use();

			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.847f,
					0.247f, 0.247f, .5f);
			glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
			glBindVertexArray(0);

			GLSLProgram::reset();

			if(first_child()) {
				DispatchDrawEvent(first_child(), off_screen_profile);
			}

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);

			Shaders::instance->PopWidgetProjectionMatrix();
			Shaders::instance->PopWidgetModelMatrix();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if(scissor_test) {
				glEnable(GL_SCISSOR_TEST);
			}

			fb->reset();
		}

		tex->reset();

		//delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->reset();
		delete fb; fb = 0;

	}

}
