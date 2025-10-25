/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/main/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "collision/module.hpp"
#include "common/module.hpp"
#include "constraint/module.hpp"
#include "dynamics/module.hpp"
#include "gui/module.hpp"
#include "io/module.hpp"
#include "math/module.hpp"
#include "optimizer/module.hpp"
#include "simulation/module.hpp"

#include <dart/config.hpp>

namespace nb = nanobind;
using namespace dart::python;

NB_MODULE(dartpy2, m)
{
  m.doc() = "dartpy2: Python API of Dynamic Animation and Robotics Toolkit "
              "(using nanobind)";

#ifdef DARTPY2_VERSION_INFO
  m.attr("__version__") = DARTPY2_VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif

  // Register submodules with error handling for debugging
  try {
    def_collision(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_collision: ") + e.what());
  }

  try {
    def_common(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_common: ") + e.what());
  }

  try {
    def_constraint(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_constraint: ") + e.what());
  }

  try {
    def_dynamics(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_dynamics: ") + e.what());
  }

  try {
    def_gui(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_gui: ") + e.what());
  }

  try {
    def_io(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_io: ") + e.what());
  }

  try {
    def_math(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_math: ") + e.what());
  }

  try {
    def_optimizer(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_optimizer: ") + e.what());
  }

  try {
    def_simulation(m);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error in def_simulation: ") + e.what());
  }
}
