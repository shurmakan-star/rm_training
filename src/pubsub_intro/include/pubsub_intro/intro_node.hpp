#ifndef INTRO_NODE_HPP_
#define INTRO_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class IntroNode : public rclcpp::Node
{
public:
    IntroNode();

private:
    void timer_callback(); 
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_;
};

#endif 