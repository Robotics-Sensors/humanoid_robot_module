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

/* Author: Kayman */

#ifndef HUMANOID_ROBOT_WALKING_MODULE_H_
#define HUMANOID_ROBOT_WALKING_MODULE_H_

#include "humanoid_robot_walking_parameter.h"

#include <boost/thread.hpp>
#include <eigen3/Eigen/Eigen>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <yaml-cpp/yaml.h>

#include <eigen_conversions/eigen_msg.h>
#include <ros/callback_queue.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/String.h>

#include "humanoid_robot_walking_module_msgs/GetWalkingParam.h"
#include "humanoid_robot_walking_module_msgs/SetWalkingParam.h"
#include "humanoid_robot_walking_module_msgs/WalkingParam.h"
#include "humanoid_robot_controller_msgs/StatusMsg.h"

#include "humanoid_robot_kinematics_dynamics/humanoid_robot_kinematics_dynamics.h"
#include "humanoid_robot_framework_common/motion_module.h"
#include "humanoid_robot_math/humanoid_robot_math.h"
#include "humanoid_robot_math/humanoid_robot_trajectory_calculator.h"

namespace humanoid_robot_op {

typedef struct {
  double x, y, z;
} Position3D;

typedef struct {
  double x, y, z, roll, pitch, yaw;
} Pose3D;

class WalkingModule : public humanoid_robot_framework::MotionModule,
                      public humanoid_robot_framework::Singleton<WalkingModule> {

public:
  enum { PHASE0 = 0, PHASE1 = 1, PHASE2 = 2, PHASE3 = 3 };

  WalkingModule();
  virtual ~WalkingModule();

  void initialize(const int control_cycle_msec,
                  humanoid_robot_framework::Robot *robot);
  void process(std::map<std::string, humanoid_robot_framework::Dynamixel *> dxls,
               std::map<std::string, double> sensors);
  void stop();
  bool isRunning();
  void onModuleEnable();
  void onModuleDisable();

  int getCurrentPhase() { return phase_; }
  double getBodySwingY() { return body_swing_y; }
  double getBodySwingZ() { return body_swing_z; }

private:
  enum {
    WalkingDisable = 0,
    WalkingEnable = 1,
    WalkingInitPose = 2,
    WalkingReady = 3
  };

  const bool DEBUG;

  void queueThread();

  /* ROS Topic Callback Functions */
  void walkingCommandCallback(const std_msgs::String::ConstPtr &msg);
  void walkingParameterCallback(
      const humanoid_robot_walking_module_msgs::WalkingParam::ConstPtr &msg);
  bool getWalkigParameterCallback(
      humanoid_robot_walking_module_msgs::GetWalkingParam::Request &req,
      humanoid_robot_walking_module_msgs::GetWalkingParam::Response &res);

  /* ROS Service Callback Functions */
  void processPhase(const double &time_unit);
  bool computeLegAngle(double *leg_angle);
  void computeArmAngle(double *arm_angle);
  void sensoryFeedback(const double &rlGyroErr, const double &fbGyroErr,
                       double *balance_angle);

  void publishStatusMsg(unsigned int type, std::string msg);
  double wSin(double time, double period, double period_shift, double mag,
              double mag_shift);
  bool computeIK(double *out, double x, double y, double z, double a, double b,
                 double c);
  void updateTimeParam();
  void updateMovementParam();
  void updatePoseParam();
  void startWalking();
  void loadWalkingParam(const std::string &path);
  void saveWalkingParam(std::string &path);
  void iniPoseTraGene(double mov_time);

  HUMANOID_ROBOTKinematicsDynamics *humanoid_robot_kd_;
  int control_cycle_msec_;
  std::string param_path_;
  boost::thread queue_thread_;
  boost::mutex publish_mutex_;

  /* ROS Topic Publish Functions */
  ros::Publisher robot_pose_pub_;
  ros::Publisher status_msg_pub_;

  Eigen::MatrixXd calc_joint_tra_;

  Eigen::MatrixXd target_position_;
  Eigen::MatrixXd goal_position_;
  Eigen::MatrixXd init_position_;
  Eigen::MatrixXi joint_axis_direction_;
  std::map<std::string, int> joint_table_;
  int walking_state_;
  int init_pose_count_;
  humanoid_robot_walking_module_msgs::WalkingParam walking_param_;
  double previous_x_move_amplitude_;

  // variable for walking
  double period_time_;
  double dsp_ratio_;
  double ssp_ratio_;
  double x_swap_period_time_;
  double x_move_period_time_;
  double y_swap_period_time_;
  double y_move_period_time_;
  double z_swap_period_time_;
  double z_move_period_time_;
  double a_move_period_time_;
  double ssp_time_;
  double l_ssp_start_time_;
  double l_ssp_end_time_;
  double r_ssp_start_time_;
  double r_ssp_end_time_;
  double phase1_time_;
  double phase2_time_;
  double phase3_time_;

  double x_offset_;
  double y_offset_;
  double z_offset_;
  double r_offset_;
  double p_offset_;
  double a_offset_;

  double x_swap_phase_shift_;
  double x_swap_amplitude_;
  double x_swap_amplitude_shift_;
  double x_move_phase_shift_;
  double x_move_amplitude_;
  double x_move_amplitude_shift_;
  double y_swap_phase_shift_;
  double y_swap_amplitude_;
  double y_swap_amplitude_shift_;
  double y_move_phase_shift_;
  double y_move_amplitude_;
  double y_move_amplitude_shift_;
  double z_swap_phase_shift_;
  double z_swap_amplitude_;
  double z_swap_amplitude_shift_;
  double z_move_phase_shift_;
  double z_move_amplitude_;
  double z_move_amplitude_shift_;
  double a_move_phase_shift_;
  double a_move_amplitude_;
  double a_move_amplitude_shift_;

  double pelvis_offset_;
  double pelvis_swing_;
  double hit_pitch_offset_;
  double arm_swing_gain_;

  bool ctrl_running_;
  bool real_running_;
  double time_;

  int phase_;
  double body_swing_y;
  double body_swing_z;
};

} // namespace humanoid_robot_op

#endif /* HUMANOID_ROBOT_WALKING_MODULE_H_ */
