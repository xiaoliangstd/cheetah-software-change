#include <iostream>
#include <Utilities/Timer.h>
#include <Utilities/Utilities_print.h>
#include "SolverMPC.h"
#include "ConvexMPCLocomotion.h"
#include "convexMPC_interface.h"

#include "Gait.h"

ConvexMPCLocomotion::ConvexMPCLocomotion(float _dt, int _iterations_between_mpc, MIT_UserParameters* parameters) :
  iterationsBetweenMPC(_iterations_between_mpc),
  horizonLength(HORIZON_LENGTH),
  dt(_dt),
  trotting(50, Vec4<int>(0,25,25,0), Vec4<int>(25,25,25,25),"Trotting"),
  //trotting(20, Vec4<int>(0,10,10,0), Vec4<int>(10,10,10,10),"Trotting"),
  trotRunning(16, Vec4<int>(0,8,8,0),Vec4<int>(6,6,6,6),"Trot Running"),
  walking(40, Vec4<int>(0,20,30,10), Vec4<int>(30,30,30,30), "Walking"),
  bounding(10, Vec4<int>(5,5,0,0),Vec4<int>(5,5,5,5),"Bounding"),
  pronking(10, Vec4<int>(0,0,0,0),Vec4<int>(5,5,5,5),"Pronking"),
  pacing(16, Vec4<int>(8,0,8,0),Vec4<int>(9,9,9,9),"Pacing")
{
  _parameters = parameters;
  dtMPC = dt * iterationsBetweenMPC;
  printf("[Convex MPC] dt: %.3f iterations: %d, dtMPC: %.3f\n", dt, iterationsBetweenMPC, dtMPC);
  setup_problem(dtMPC, horizonLength, _parameters->cmpc_mu, _parameters->cmpc_fmax);

  for(int i = 0; i < 4; i++)
    firstSwing[i] = true;

   pBody_des.setZero();
   vBody_des.setZero();
   aBody_des.setZero();
   pBody_RPY_des.setZero();
   vBody_Ori_des.setZero();
     for(size_t i(0); i<4; ++i){
    pFoot_des[i].setZero();;
    vFoot_des[i].setZero();;
    aFoot_des[i].setZero();;
    Fr_des[i].setZero();
  }

   _body_height = parameters->stand_up_height; 
}

void ConvexMPCLocomotion::initialize(){
  gait = &trotting;
  currentGaitNumber = TROT;
  nextGaitNumber = currentGaitNumber;

  iterationCounter = 0;

  for(int i = 0; i < 4; i++) firstSwing[i] = true;
  firstRun = true;
}

void ConvexMPCLocomotion::_SetupCommand(ControlFSMData<float> & data){
  //确定步态
  static int iter = 0;
  iter++;
  float total_vel =  fabs(_x_vel_des) + fabs(_y_vel_des) + fabs(_yaw_turn_rate);

  //已经有步态切换命令时,刚刚切换完步态还没稳定时,机身速度较大时,不检查手柄
  if((currentGaitNumber == nextGaitNumber)&&(iter > 200)&&(total_vel<0.15))
  {
    if(data._gamepad->get().right)
        nextGaitNumber = currentGaitNumber + 1;
    else if(data._gamepad->get().left)
        nextGaitNumber = currentGaitNumber - 1;
    else if(data._gamepad->get().down)
        nextGaitNumber = TROT;
    
    if(nextGaitNumber < 0)nextGaitNumber = GAIT_SUM - 1;
    if(nextGaitNumber == GAIT_SUM)nextGaitNumber = 0;
  }

  if(currentGaitNumber != nextGaitNumber)
  {
    if(gait->isGaitEnd())
    {
      if(nextGaitNumber == TROT)
        gait = &trotting;
      else if(nextGaitNumber == TROT_RUN)
        gait = &trotRunning;
      else if(nextGaitNumber == WALK)
        gait = &walking;
      else if(nextGaitNumber == BOUND)
        gait = &bounding;
      else if(nextGaitNumber == PRONK)
        gait = &pronking;
      else if(nextGaitNumber == PACE)
        gait = &pacing;
      printf("[Gait Change to]:  %d %s\n", nextGaitNumber, (gait->_name).c_str());
      currentGaitNumber = nextGaitNumber;
      iter = 0;
    }
  }

  float x_vel_cmd, y_vel_cmd,_body_height_cmd;
  float filter(0.002);               //注意这个滤波器,决定了行走的加速度,很重要
  float filter2(0.006);              //注意这个滤波器,决定了上下蹲的速度,很重要
  
  //trotting, trotRunning, walking, bounding, pronking, pacing;
  float scale_x[GAIT_SUM] = {0.40, 0.60, 0.30, 0.20, 0.20, 0.20};
  float scale_y[GAIT_SUM] = {0.30, 0.30, 0.30, 0.10, 0.03, 0.20};
  float scale_z[GAIT_SUM] = {0.25, 0.25, 0.40, 0.00, 0.10, 0.20};

  float gaitTime = gait->getCurrentGaitTime(dtMPC);
  x_vel_cmd      =  data._gamepad->get().leftStickAnalog[1]
                   *scale_x[currentGaitNumber]/gaitTime;
  y_vel_cmd      = -data._gamepad->get().rightStickAnalog[0]
                   *scale_y[currentGaitNumber]/gaitTime;
  _yaw_turn_rate = (data._gamepad->get().leftTriggerAnalog - data._gamepad->get().rightTriggerAnalog)
                   *scale_z[currentGaitNumber]/gaitTime;
  //filter
  _x_vel_des = _x_vel_des*(1-filter) + x_vel_cmd*filter;
  _y_vel_des = _y_vel_des*(1-filter) + y_vel_cmd*filter;
  //yaw
  _yaw_des = data._stateEstimator->getResult().rpy[2] + dt * _yaw_turn_rate;
  //height
  _body_height_cmd = data.userParameters->stand_up_height; 
  if(data._gamepad->get().a) _body_height_cmd *= 0.7;  //下蹲指令
 
  _body_height = _body_height*(1-filter2) + _body_height_cmd*filter2;
}

template<>
void ConvexMPCLocomotion::run(ControlFSMData<float>& data) {
  // 状态估计
  auto& seResult = data._stateEstimator->getResult();
  // 初始化一些状态
  if(firstRun)
  {
    world_position_desired[0] = seResult.position[0];
    world_position_desired[1] = seResult.position[1];
    world_position_desired[2] = seResult.rpy[2];

    for(int i = 0; i < 4; i++)
    {
      footSwingTrajectories[i].setHeight(_parameters->foot_hight);
      footSwingTrajectories[i].setInitialPosition(pFoot[i]);
      footSwingTrajectories[i].setFinalPosition(pFoot[i]);
    }
    firstRun = false;
  }
  // 从手柄读取命令
  _SetupCommand(data);
  // calc gait
  gait->setIterations(iterationsBetweenMPC, iterationCounter);

  // integrate position setpoint
  Vec3<float> v_des_robot(_x_vel_des, _y_vel_des, 0);
  Vec3<float> v_des_world = seResult.rBody.transpose() * v_des_robot;

  for(int i = 0; i < 4; i++) {
    pFoot[i] = seResult.position + 
      seResult.rBody.transpose() * (data._quadruped->getHipLocation(i) + 
          data._legController->datas[i].p);
  }

  //y方向随波逐流,x方向保证不后退
  world_position_desired[0] += dt * v_des_world[0];
  world_position_desired[1] = seResult.position[1] +  dt * v_des_world[1];

  // foot placement
  for(int l = 0; l < 4; l++)
    swingTimes[l] = gait->getCurrentSwingTime(dtMPC);

  float x_side_sign[4] = {1, 1, -1, -1};
  float y_side_sign[4] = {-1, 1, -1, 1};
  for(int i = 0; i < 4; i++)
  {
    if(firstSwing[i]) {
      swingTimeRemaining[i] = swingTimes[i];
    } else {
      swingTimeRemaining[i] -= dt;
    }
    footSwingTrajectories[i].setHeight(_parameters->foot_hight);
    Vec3<float> abad_offset(0, y_side_sign[i] * data._quadruped->_abadLinkLength, 0);
    Vec3<float> user_offset(x_side_sign[i]*_parameters->Swing_step_offset[0], 
                            y_side_sign[i]*_parameters->Swing_step_offset[1],
                            0);

    Vec3<float> pRobotFrame = (data._quadruped->getHipLocation(i) + abad_offset + user_offset);

    float stance_time = gait->getCurrentStanceTime(dtMPC);
    Vec3<float> pYawCorrected = 
      coordinateRotation(CoordinateAxis::Z, -_yaw_turn_rate* stance_time / 2.0f) * pRobotFrame;

    Vec3<float> des_vel;
    des_vel[0] = _x_vel_des;
    des_vel[1] = _y_vel_des;
    des_vel[2] = 0.0;

    Vec3<float> Pf = seResult.position + seResult.rBody.transpose() * (pYawCorrected
          + des_vel * swingTimeRemaining[i]);

    // 计算落脚点
    float pfx_rel = seResult.vWorld[0] * 0.5 * stance_time +
      _parameters->foot_location_k*(seResult.vWorld[0]-v_des_world[0]) +
      (0.5f*sqrt(fabs(seResult.position[2]/9.81f))) * (seResult.vWorld[1]*_yaw_turn_rate);

    float pfy_rel = seResult.vWorld[1] * 0.5 * stance_time +
      _parameters->foot_location_k*(seResult.vWorld[1]-v_des_world[1]) +
      (0.5f*sqrt(fabs(seResult.position[2]/9.81f))) * (-seResult.vWorld[0]*_yaw_turn_rate);

    // 限幅
    pfx_rel = fminf(fmaxf(pfx_rel, -_parameters->stride_max), _parameters->stride_max);
    pfy_rel = fminf(fmaxf(pfy_rel, -_parameters->stride_max), _parameters->stride_max);
    Pf[0] +=  pfx_rel;
    Pf[1] +=  pfy_rel;
    Pf[2] =  0;
    footSwingTrajectories[i].setFinalPosition(Pf);
  }
  // gait
  Vec4<float> contactStates = gait->getContactState();
  Vec4<float> swingStates = gait->getSwingState();
  int* mpcTable = gait->getMpcTable();

  updateMPCIfNeeded(mpcTable, data);

  for(int foot = 0; foot < 4; foot++)
  {
    float swingState = swingStates[foot];
    if(swingState > 0) // foot is in swing
    {
      if(firstSwing[foot])
      {
        firstSwing[foot] = false;
        footSwingTrajectories[foot].setInitialPosition(pFoot[foot]);
      }
      footSwingTrajectories[foot].computeSwingTrajectoryBezier(swingState, swingTimes[foot]);

      Vec3<float> pDesFootWorld = footSwingTrajectories[foot].getPosition();
      Vec3<float> vDesFootWorld = footSwingTrajectories[foot].getVelocity();
      //if(foot == 0)printf("%.3f  %.3f  %.3f\n",vDesFootWorld[0],vDesFootWorld[1],vDesFootWorld[2] );
      Vec3<float> pDesLeg = seResult.rBody * (pDesFootWorld - seResult.position) 
        - data._quadruped->getHipLocation(foot);
      Vec3<float> vDesLeg = seResult.rBody * (vDesFootWorld - seResult.vWorld);
      // Update for WBC
      pFoot_des[foot] = pDesFootWorld;
      vFoot_des[foot] = vDesFootWorld;
      aFoot_des[foot] = footSwingTrajectories[foot].getAcceleration();
      if(!data.userParameters->use_wbc){
        // Update leg control command regardless of the usage of WBIC
        data._legController->commands[foot].pDes = pDesLeg;
        data._legController->commands[foot].vDes = vDesLeg;

        for (size_t jidx(0); jidx < cheetah::num_leg_joint; ++jidx) {
          data._legController->commands[foot].kpCartesian(jidx,jidx)
           = data.userParameters->mpc_kp_foot(jidx);
          data._legController->commands[foot].kdCartesian(jidx,jidx)
           = data.userParameters->mpc_kd_foot(jidx);
        }
        
      }
    }
    else // foot is in stance
    {
      firstSwing[foot] = true;

      Vec3<float> pDesFootWorld = footSwingTrajectories[foot].getPosition();
      Vec3<float> vDesFootWorld = footSwingTrajectories[foot].getVelocity();
      Vec3<float> pDesLeg = seResult.rBody * (pDesFootWorld - seResult.position)
       - data._quadruped->getHipLocation(foot);
      Vec3<float> vDesLeg = seResult.rBody * (vDesFootWorld - seResult.vWorld);

      if(!data.userParameters->use_wbc){
        data._legController->commands[foot].pDes = pDesLeg;
        data._legController->commands[foot].vDes = vDesLeg;

        for (size_t jidx(0); jidx < cheetah::num_leg_joint; ++jidx) {
          data._legController->commands[foot].kpCartesian(jidx,jidx)
           = data.userParameters->mpc_kp_stance_foot(jidx);
          data._legController->commands[foot].kdCartesian(jidx,jidx)
           = data.userParameters->mpc_kd_stance_foot(jidx);
        }

        data._legController->commands[foot].forceFeedForward = f_ff[foot];
        //这里或许应该给关节一个阻尼，MIT这么做了，但是我不知道为什么要这么做
      }
    }
  }
  data._stateEstimator->setContactPhase(contactStates);

  // Update For WBC
  pBody_des[0] = world_position_desired[0];
  pBody_des[1] = world_position_desired[1];
  pBody_des[2] = _body_height;

  vBody_des[0] = v_des_world[0];
  vBody_des[1] = v_des_world[1];
  vBody_des[2] = 0.;

  aBody_des.setZero();

  pBody_RPY_des[0] = 0.;
  pBody_RPY_des[1] = 0.; 
  pBody_RPY_des[2] = _yaw_des;

  vBody_Ori_des[0] = 0.;
  vBody_Ori_des[1] = 0.;
  vBody_Ori_des[2] = _yaw_turn_rate;

  contact_state = gait->getContactState();
  swing_state = gait->getContactState();
  // END of WBC Update

  iterationCounter++;
}

void ConvexMPCLocomotion::updateMPCIfNeeded(int *mpcTable, ControlFSMData<float> &data) {

  if((iterationCounter % iterationsBetweenMPC) == 0)
  {
    auto seResult = data._stateEstimator->getResult();
    float* p = seResult.position.data();
    
    Vec3<float> v_des_robot(_x_vel_des, _y_vel_des,0);
    Vec3<float> v_des_world = seResult.rBody.transpose() * v_des_robot;

    const float max_pos_error = .01;
    float xStart = world_position_desired[0];
    float yStart = world_position_desired[1];

    if(xStart - p[0] > max_pos_error) xStart = p[0] + max_pos_error;
    if(p[0] - xStart > max_pos_error) xStart = p[0] - max_pos_error;

    if(yStart - p[1] > max_pos_error) yStart = p[1] + max_pos_error;
    if(p[1] - yStart > max_pos_error) yStart = p[1] - max_pos_error;

    world_position_desired[0] = xStart;
    world_position_desired[1] = yStart;
    float trajInitial[12] = {
      0.0,                                      // 0
      0.0,                                      // 1
      _yaw_des,                                 // 2
      xStart,                                   // 3
      yStart,                                   // 4
      (float)_body_height,                      // 5
      0,                                        // 6
      0,                                        // 7
      _yaw_turn_rate,                           // 8
      v_des_world[0],                           // 9
      v_des_world[1],                           // 10
      0};                                       // 11

    for(int i = 0; i < horizonLength; i++)
    {
      for(int j = 0; j < 12; j++)
        trajAll[12*i+j] = trajInitial[j];

      if(i == 0) // start at current position  TODO consider not doing this
      {
        trajAll[2] = seResult.rpy[2];
      }
      else
      {
        trajAll[12*i + 3] = trajAll[12 * (i - 1) + 3] + dtMPC * v_des_world[0];
        trajAll[12*i + 4] = trajAll[12 * (i - 1) + 4] + dtMPC * v_des_world[1];
        trajAll[12*i + 2] = trajAll[12 * (i - 1) + 2] + dtMPC * _yaw_turn_rate;
      }
    }
    Timer solveTimer;
    solveDenseMPC(mpcTable, data);
    //printf("TOTAL SOLVE TIME: %.3f\n", solveTimer.getMs());  //查看计算时间
  }
}
void ConvexMPCLocomotion::solveDenseMPC(int *mpcTable, ControlFSMData<float> &data) {
  auto seResult = data._stateEstimator->getResult();

   float Q[12] = {25, 25, 10,    //角度
                  1, 1, 50,          //位置
                  0, 0, 0.3,         //角速度
                  0.2, 0.2, 0.1};    //速度

  float yaw = seResult.rpy[2];
  float* weights = Q;
  float alpha = 1e-6;                 //力的权重
  float* p = seResult.position.data();
  float* v = seResult.vWorld.data();
  float* w = seResult.omegaWorld.data();
  float* q = seResult.orientation.data();

  float r[12];
  for(int i = 0; i < 12; i++)
    r[i] = pFoot[i%4][i/4]  - seResult.position[i/4];

  if(alpha > 1e-4) {
    std::cout << "Alpha was set too high (" << alpha << ") adjust to 1e-5\n";
    alpha = 1e-5;
  }

  Vec3<float> pxy_act(p[0], p[1], 0);
  Vec3<float> pxy_des(world_position_desired[0], world_position_desired[1], 0);
  update_problem_data_floats(p,v,q,w,r,yaw,weights,trajAll,alpha,mpcTable);
  

  for(int leg = 0; leg < 4; leg++)
  {
    Vec3<float> f;
    for(int axis = 0; axis < 3; axis++)
    {
      f[axis] = get_solution(leg*3 + axis);
    }
    f_ff[leg] = -seResult.rBody * f;
    // Update for WBC
    Fr_des[leg] = f;
  }
}