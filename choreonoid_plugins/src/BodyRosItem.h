#ifndef CNOID_ROS_PLUGIN_BODY_ROS_ITEM_H_INCLUDED
#define CNOID_ROS_PLUGIN_BODY_ROS_ITEM_H_INCLUDED

#include <cnoid/ControllerItem>
#include <cnoid/BasicSensorSimulationHelper>
#include <cnoid/Body>
#include <cnoid/VisionSensor>
#include <cnoid/Camera>
#include <cnoid/RangeSensor>
#include "exportdecl.h"

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Wrench.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <image_transport/image_transport.h>

#include <vector>

namespace cnoid {

class CNOID_EXPORT BodyRosItem : public ControllerItem
{
public:
    static void initialize(ExtensionManager* ext);
    
    BodyRosItem();
    BodyRosItem(const BodyRosItem& org);
    virtual ~BodyRosItem();
    void doPutProperties(PutPropertyFunction& putProperty);
    bool createSensors(BodyPtr body);
    
    virtual bool start(Target* target);
    virtual double timeStep() const {
      return timeStep_;
    };
    virtual void input();
    virtual bool control();
    virtual void output();
    virtual void stop();
    
    void callback(const trajectory_msgs::JointTrajectory::ConstPtr& msg);
    
    const BodyPtr& body() const { return simulationBody; };
    const DeviceList<ForceSensor>& forceSensors() const { return forceSensors_; }
    const DeviceList<RateGyroSensor>& gyroSensors() const { return gyroSensors_; }
    const DeviceList<AccelSensor>& accelSensors() const { return accelSensors_; }
    const DeviceList<Camera>& visionSensors() const { return visionSensors_; }
    const DeviceList<RangeCamera>& rangeVisionSensors() const { return rangeVisionSensors_; }
    const DeviceList<RangeSensor>& rangeSensors() const { return rangeSensors_; }
    
    double controlTime() const { return controlTime_; }
    
    void setModuleName(const std::string& name);

protected:
    virtual ItemPtr doDuplicate() const;
        
private:
    BodyPtr simulationBody;
    DeviceList<ForceSensor> forceSensors_;
    DeviceList<RateGyroSensor> gyroSensors_;
    DeviceList<AccelSensor> accelSensors_;
    DeviceList<Camera> visionSensors_;
    DeviceList<RangeCamera> rangeVisionSensors_;
    DeviceList<RangeSensor> rangeSensors_;
    double timeStep_;

    const Target* controllerTarget;
    double controlTime_;
    std::ostream& os;

    std::string bodyName;

    boost::shared_ptr<ros::NodeHandle> rosnode_;
    boost::shared_ptr<ros::AsyncSpinner> async_ros_spin_;
    sensor_msgs::JointState joint_state_;
    ros::Publisher joint_state_publisher_;
    ros::Subscriber joint_state_subscriber_;
    double joint_state_update_rate_;
    double joint_state_update_period_;
    double joint_state_last_update_;
    
    std::map<std::string, int> joint_number_map_;
    std::vector<std::string> joint_names_;
    std::vector<trajectory_msgs::JointTrajectoryPoint> points_;
    double trajectory_start_;
    unsigned int trajectory_index_;
    bool has_trajectory_;
 
    std::vector<ros::Publisher> force_sensor_publishers_;
    std::vector<ros::Publisher> rate_gyro_sensor_publishers_;
    std::vector<ros::Publisher> accel_sensor_publishers_;
    std::vector<image_transport::Publisher> vision_sensor_publishers_;
    std::vector<ros::Publisher> range_vision_sensor_publishers_;
    std::vector<ros::Publisher> range_sensor_publishers_;

    void updateForceSensor(ForceSensor* sensor, ros::Publisher& publisher);
    void updateRateGyroSensor(RateGyroSensor* sensor, ros::Publisher& publisher);
    void updateAccelSensor(AccelSensor* sensor, ros::Publisher& publisher);
    void updateVisionSensor(Camera* sensor, image_transport::Publisher& publisher);
    void updateRangeVisionSensor(RangeCamera* sensor, ros::Publisher& publisher);
    void updateRangeSensor(RangeSensor* sensor, ros::Publisher& publisher);
};

typedef ref_ptr<BodyRosItem> BodyRosItemPtr;
}

#endif
