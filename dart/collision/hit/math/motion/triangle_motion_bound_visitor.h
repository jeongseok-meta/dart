/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011-2014, Willow Garage, Inc.
 *  Copyright (c) 2014-2016, Open Source Robotics Foundation
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Open Source Robotics Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/** @author Jia Pan */

#pragma once

#include "dart/collision/hit/common/unused.h"
#include "dart/collision/hit/math/bv/RSS.h"
#include "dart/collision/hit/math/motion/bv_motion_bound_visitor.h"
#include "dart/collision/hit/math/motion/interp_motion.h"
#include "dart/collision/hit/math/motion/screw_motion.h"
#include "dart/collision/hit/math/motion/spline_motion.h"
#include "dart/collision/hit/math/motion/taylor_model/taylor_matrix.h"
#include "dart/collision/hit/math/motion/taylor_model/taylor_vector.h"
#include "dart/collision/hit/math/motion/translation_motion.h"

namespace dart::collision::hit {

template <typename S>
class MotionBase;

template <typename S>
class SplineMotion;

template <typename S>
class ScrewMotion;

template <typename S>
class InterpMotion;

template <typename S>
class TranslationMotion;

template <typename, typename>
struct TriangleMotionBoundVisitorVisitImpl;

template <typename S>
class TriangleMotionBoundVisitor
{
public:
  TriangleMotionBoundVisitor(
      const Vector3<S>& a_,
      const Vector3<S>& b_,
      const Vector3<S>& c_,
      const Vector3<S>& n_);

  virtual S visit(const MotionBase<S>& motion) const
  {
    DART_COLLISION_HIT_UNUSED(motion);
    return 0;
  }
  virtual S visit(const SplineMotion<S>& motion) const;
  virtual S visit(const ScrewMotion<S>& motion) const;
  virtual S visit(const InterpMotion<S>& motion) const;
  virtual S visit(const TranslationMotion<S>& motion) const;

protected:
  template <typename, typename>
  friend struct TriangleMotionBoundVisitorVisitImpl;

  friend struct TriangleMotionBoundVisitorVisitImpl<
      double,
      SplineMotion<double>>;
  friend struct TriangleMotionBoundVisitorVisitImpl<
      double,
      ScrewMotion<double>>;
  friend struct TriangleMotionBoundVisitorVisitImpl<
      double,
      InterpMotion<double>>;
  friend struct TriangleMotionBoundVisitorVisitImpl<
      double,
      TranslationMotion<double>>;

  Vector3<S> a, b, c, n;
};

} // namespace dart::collision::hit

#include "dart/collision/hit/math/motion/triangle_motion_bound_visitor-inl.h"
