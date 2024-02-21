#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtle_telemetry/msg/turtlesim_debug.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class TurtleTelemetry : public rclcpp::Node
{
public:
  TurtleTelemetry()
  : Node("turtle_telemetry")
  {
    RCLCPP_INFO_STREAM(this->get_logger(), "Running turtle_telemetry publisher");

    // Create a publisher for your debug message
    publisher_ = this->create_publisher<turtle_telemetry::msg::TurtlesimDebug>("turtle_debug", 10);

    // Do whatever you have to do to prepare to capture telemetry here
    // E.g. open a serial port for debug data & set up a timer to read it, etc
    // In this example, we subscribe to turtlebot's Pose msg and use the
    // callback to populate the debug message
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      "turtle1/pose", 10, std::bind(&TurtleTelemetry::telemetry_callback, this, _1));
  }

private:
  void telemetry_callback(const turtlesim::msg::Pose::SharedPtr msg) const
  {

    // plug debug data into the debug message
    auto debug_message = turtle_telemetry::msg::TurtlesimDebug();
    debug_message.x = msg->x;
    debug_message.y = msg->y;
    debug_message.theta = msg->theta;
    debug_message.linear_speed = msg->linear_velocity;
    debug_message.angular_speed = msg->angular_velocity;

    // publish the debug message
    publisher_->publish(debug_message);
  }

  rclcpp::Publisher<turtle_telemetry::msg::TurtlesimDebug>::SharedPtr publisher_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtleTelemetry>());
  rclcpp::shutdown();
  return 0;
}