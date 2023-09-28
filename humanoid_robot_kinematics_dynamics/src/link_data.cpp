/*******************************************************************************
 * Copyright 2017 ROBOTIS CO., LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

/* Authors: SCH, Jay Song */

#include "humanoid_robot_kinematics_dynamics/link_data.h"

namespace humanoid_robot_op {

LinkData::LinkData() {
  name_ = "";

  parent_ = -1;
  sibling_ = -1;
  child_ = -1;

  mass_ = 0.0;

  relative_position_ = humanoid_robot_framework::getTransitionXYZ(0.0, 0.0, 0.0);
  joint_axis_ = humanoid_robot_framework::getTransitionXYZ(0.0, 0.0, 0.0);
  center_of_mass_ = humanoid_robot_framework::getTransitionXYZ(0.0, 0.0, 0.0);
  inertia_ = humanoid_robot_framework::getInertiaXYZ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  joint_limit_max_ = 100.0;
  joint_limit_min_ = -100.0;

  joint_angle_ = 0.0;
  joint_velocity_ = 0.0;
  joint_acceleration_ = 0.0;

  position_ = humanoid_robot_framework::getTransitionXYZ(0.0, 0.0, 0.0);
  orientation_ = humanoid_robot_framework::convertRPYToRotation(0.0, 0.0, 0.0);
  transformation_ =
      humanoid_robot_framework::getTransformationXYZRPY(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

LinkData::~LinkData() {}

} // namespace humanoid_robot_op
