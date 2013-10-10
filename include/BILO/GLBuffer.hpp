/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_GLBUFFER_HPP_
#define _BILO_GLBUFFER_HPP_

#include <vector>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BILO {

	/**
	 * @brief A simple warpper of OpenGL buffer
	 */
	class GLBuffer
	{
	public:

		GLBuffer ();

		~GLBuffer ();

		/**
		 * @brief clear current buffers and generate new ones
		 * @param size
		 */
		void generate (size_t size = 1);

		/**
		 * @brief generate new buffer and append to the currents
		 * @param size
		 */
		void append (size_t size = 1);

		bool reset (size_t index);

		void set_vertices (int vertices);

		int vertices () const;

		void set_vertices (size_t index, int vertices);

		int vertices (size_t index) const;

		void set_unit_size (int size);

		int unit_size () const;

		void set_unit_size (size_t index, int size);

		int unit_size (size_t index) const;

		void set_target (GLenum target);

		GLenum target () const;

		void set_target (size_t index, GLenum target);

		GLenum target (size_t index) const;

		void set_usage (GLenum usage);

		GLenum usage () const;

		void set_usage (size_t index, GLenum usage);

		GLenum usage (size_t index) const;

		void set_property (int vertices, int unit_size, GLenum target, GLenum usage);

		void set_property (size_t index, int vertices, int unit_size, GLenum target, GLenum usage);

		/**
		 * @brief bind current buffer
		 * @return
		 */
		void bind ();

		bool bind (size_t index);

		/**
		 * @brief unbind the buffer id in current index
		 */
		void unbind ();

		void unbind (size_t index);

		void set_index (size_t index);

		/**
		 * @brief return the current index in the buffer vector
		 * @return
		 */
		const size_t index () const {return m_index;}

		size_t size () const {return m_buffers.size();}

		bool is_buffer ();

		bool is_buffer (size_t index);

		/**
		 * @brief creates and initializes a buffer object's data store
		 * @param target
		 * @param size
		 * @param data
		 * @param usage
		 */
		void upload (const GLvoid* data);

		void upload (size_t index, const GLvoid* data);

		void destroy ();

		void destroy (size_t index);

		void clear ();

#ifdef DEBUG

		void print (size_t index);

#endif

	private:

		struct Property
		{
			Property ()
			: id(0), vertices(0), unit_size(0), target(0), usage(0)
			{}

			/** The name of a buffer object, it's generated by glGenBuffers() and used in glBindBuffers() */
			GLuint id;

			/** Vertex number -- how many vertices are used in this buffer */
			int vertices;

			/** size of one unit vertex */
			int unit_size;

			/** The target buffer object, it's used for glBindBuffer() and glBufferData() */
			GLenum target;

			/** Usage: */
			GLenum usage;
		};

		/**
		 * @buffer index
		 *
		 * -1 : initial status
		 */
		int m_index;

		std::vector<Property> m_buffers;
	};
}

#endif /* _BIL_GLBUFFER_HPP_ */
