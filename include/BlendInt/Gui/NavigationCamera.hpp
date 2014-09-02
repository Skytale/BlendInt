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

#ifndef _BLENDINT_CAMERA_HPP_
#define _BLENDINT_CAMERA_HPP_

#include <glm/vec3.hpp>
#include <BlendInt/Gui/AbstractCamera.hpp>

namespace BlendInt {

	class NavigationCamera: public AbstractCamera
	{
	public:

		NavigationCamera ();

		virtual ~NavigationCamera ();

		void Orbit (float dx, float dy);

		void Pan (float dx, float dy);

		void Zoom (float fac);

		void SaveCurrentPosition ();

		void SaveCurrentCenter ();

		virtual void Update ();

	private:

		glm::vec3 last_position_;

		glm::vec3 last_center_;

		static float orbit_speed;

		static float pan_speed;

		static float zoom_speed;

	};

}

#endif /* _BLENDINT_CAMERA_HPP_ */
