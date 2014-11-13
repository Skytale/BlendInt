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

#ifndef _BLENDINT_IMAGEVIEW_HPP_
#define _BLENDINT_IMAGEVIEW_HPP_

#include <BlendInt/Core/Image.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/ChessBoard.hpp>
#include <BlendInt/Gui/AbstractScrollable.hpp>

namespace BlendInt {

	class TextureView: public AbstractScrollable
	{
	public:

		TextureView ();

		virtual ~TextureView ();

		bool Open (const char* filename);

		void Load (const RefPtr<Image>& image);

		void Clear ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeImageView ();

		void AdjustImageArea (const Size& size);

		Size image_size_;

		GLuint vaos_[2];

		RefPtr<GLTexture2D> texture_;

		RefPtr<GLArrayBuffer> background_;
		RefPtr<GLArrayBuffer> plane_;

		RefPtr<ChessBoard> checkerboard_;
	};

}

#endif /* _BIL_IMAGEVIEW_HPP_ */