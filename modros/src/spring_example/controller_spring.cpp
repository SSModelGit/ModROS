// example controller node for ModROS

#include "ros/ros.h"
#include "modros/ModComm.h"

#define MAX_ARRAY 100

ros::Publisher pub;

double springSetPoints[MAX_ARRAY] = {0.0};

void controlCallBack(const modros::ModComm::ConstPtr& inVal) {

    modros::ModComm outVal;
    for(int i = 0; i < inVal->size; i++) {
        outVal.data.push_back(5 * (springSetPoints[i] - inVal->data[i]));
    }
    outVal.size = outVal.data.size();

    pub.publish(outVal);
}

void joyCallback(const modros::ModComm::ConstPtr& modJoy) {
    for(int i = 0; i<modJoy->size; i++) {
        springSetPoints[i] = modJoy->data[i];
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller_modros");
    ros::NodeHandle n;
    
    pub = n.advertise<modros::ModComm>("control_values", 1);
    ros::Subscriber relay_sub = n.subscribe("model_values", 1, controlCallBack);
    ros::Subscriber joy_sub = n.subscribe("modros_joy", 1, joyCallback);

    ros::spin();

    return 0;
}