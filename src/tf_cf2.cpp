#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

// Callback of timer
// Broadcaster transform from A to B and B to C
void broadcastTF(const ros::TimerEvent& timer_event)
{
  static tf::TransformBroadcaster br;
  tf::Transform tf_A_B, tf_A_C, tf_C_B, tf_C_A;
  tf::Quaternion q_A_B, q_A_C;
  tf::Vector3     v_A_B, v_A_C;


  //use RPY set value of quaternion
  //then use quaternion to rotate
  //q_A_B.setRPY(0, 0, M_PI/2);

  // A -> B
  v_A_B.setValue(1.0, 0.0, 0.0);
  q_A_B.setRPY(0, 0, 0);
  tf_A_B.setOrigin(v_A_B);
  tf_A_B.setRotation(q_A_B);

  // A -> C
  v_A_C.setValue(0.0, 1.0, 0.0);
  q_A_C.setRPY(0, 0, 0);
  tf_A_C.setOrigin(v_A_C);
  tf_A_C.setRotation(q_A_C);

  // C -> B
  tf_C_A = tf_A_C.inverse();
  tf_C_B = tf_C_A*tf_A_B;

  // B -> B1
  tf::Transform tf_B_B1;
  tf::Quaternion tf_q_B_B1(0, 0, 0, 1);
  tf::Vector3 tf_v_B_B1(0, 0, 0);
  tf_B_B1.setOrigin(tf_v_B_B1);
  tf_B_B1.setRotation(tf_q_B_B1);

  // C -> C1
  tf::Transform tf_C_C1;
  tf::Quaternion tf_q_C_C1(0, 0, 0, 1);
  tf::Vector3 tf_v_C_C1(0, 0, 0);
  tf_C_C1.setOrigin(tf_v_C_C1);
  tf_C_C1.setRotation(tf_q_C_C1);

  br.sendTransform(tf::StampedTransform(tf_A_C, // transform
                                        ros::Time::now(), // timestamp with this transform
                                        "A", // paranet frame ID
                                        "C")); // child frame ID

  br.sendTransform(tf::StampedTransform(tf_A_B, // transform
                                        ros::Time::now(), // timestamp with this transform
                                        "A", // paranet frame ID
                                        "B")); // child frame ID

  br.sendTransform(tf::StampedTransform(tf_B_B1, // transform
                                        ros::Time::now(), // timestamp with this transform
                                        "B", // paranet frame ID
                                        "B1")); // child frame ID

  br.sendTransform(tf::StampedTransform(tf_C_C1, // transform
                                        ros::Time::now(), // timestamp with this transform
                                        "C", // paranet frame ID
                                        "C1")); // child frame ID

  br.sendTransform(tf::StampedTransform(tf_C_B, // transform
                                        ros::Time::now(), // timestamp with this transform
                                        "C1", // paranet frame ID
                                        "B1")); // child frame ID

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "tf_cf2");
  ros::NodeHandle nh;
  // Create timer with 2.0 Hz
  ros::Timer timer = nh.createTimer(ros::Duration(0.5), broadcastTF);
  while (ros::ok()){ros::spinOnce();}
  return 0;
}

