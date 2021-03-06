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

#pragma once

#include <blendint/gui/abstract-frame.hpp>

namespace BlendInt {

/**
 * @brief Frames with round corners
 *
 * @ingroup blendint_gui_frames
 */
class AbstractRoundFrame: public AbstractFrame
{
 public:

  AbstractRoundFrame (int frame_flag = FrameRegular);

  AbstractRoundFrame (int width,
                      int height,
                      int frame_regular = FrameRegular);

  virtual ~AbstractRoundFrame ();

  virtual AbstractView* GetFocusedView () const;

  void SetRoundType (int type);

  void SetRoundRadius (float radius);

  inline uint32_t round_type () const
  {
    return round_frame_flag_ & 0x0F;
  }

  inline float round_radius () const
  {
    return round_radius_;
  }

 protected:

  virtual bool PreDraw (AbstractWindow* context);

  virtual Response Draw (AbstractWindow* context);

  virtual void PostDraw (AbstractWindow* context);

  virtual void PerformFocusOn (AbstractWindow* context);

  virtual void PerformFocusOff (AbstractWindow* context);

  virtual void PerformHoverIn (AbstractWindow* context);

  virtual void PerformHoverOut (AbstractWindow* context);

  virtual Response PerformContextMenuPress (AbstractWindow* context);

  virtual Response PerformContextMenuRelease (AbstractWindow* context);

  virtual Response PerformKeyPress (AbstractWindow* context);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

  virtual Response PerformMouseMove (AbstractWindow* context);

  virtual Response PerformMouseHover (AbstractWindow* context);

  void GenerateRoundedVertices (std::vector<GLfloat> *inner,
                                std::vector<GLfloat> *outer);

  void GenerateRoundedVertices (Orientation shadedir,
                                short shadetop,
                                short shadedown,
                                std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  virtual void PerformRoundTypeUpdate (int round_type);

  virtual void PerformRoundRadiusUpdate (float radius);

  inline void set_round_type (int type)
  {
    round_frame_flag_ = (round_frame_flag_ & 0xFFF0) + (type & 0x0F);
  }

  inline void set_round_radius (float radius)
  {
    round_radius_ = radius;
  }

 private:

  enum RoundFrameFlagIndex
  {

    FrameRoundTopLeft = (1 << 0),

    FrameRoundTopRight = (1 << 1),

    FrameRoundBottomRight = (1 << 2),

    FrameRoundBottomLeft = (1 << 3),

  };

  uint32_t round_frame_flag_;

  float round_radius_;

};

}
