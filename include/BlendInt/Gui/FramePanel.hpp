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

#ifndef _BLENDINT_GUI_FRAMEPANEL_HPP_
#define _BLENDINT_GUI_FRAMEPANEL_HPP_

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/OpenGL/TextureBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief A Frame container for demo only
	 */
	class FramePanel: public Frame
	{
		DISALLOW_COPY_AND_ASSIGN(FramePanel);

	public:

		FramePanel ();

		virtual ~FramePanel ();

	protected:

		virtual void PerformRefresh (const RefreshRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Profile& profile);

	private:

		void InitializeFramePanel ();

		void RenderToBuffer ();

		GLuint vao_;
		RefPtr<GLArrayBuffer> inner_;

		TextureBuffer tex_buffer_;

		bool refresh_;
	};

}



#endif /* _BLENDINT_GUI_FRAMEPANEL_HPP_ */
