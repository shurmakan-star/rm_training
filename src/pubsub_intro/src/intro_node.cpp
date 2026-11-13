#include "pubsub_intro/intro_node.hpp"
#include <chrono>

using namespace std::chrono_literals;

IntroNode::IntroNode() : Node("intro_node"), count_(0)
{
  
    publisher_ = this->create_publisher<std_msgs::msg::String>("/intro/out", 10);
    
    timer_ = this->create_wall_timer(1s, std::bind(&IntroNode::timer_callback, this));

    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "/intro/in", 10, std::bind(&IntroNode::topic_callback, this, std::placeholders::_1));
}

void IntroNode::timer_callback()
{
    auto message = std_msgs::msg::String();
    message.data = "20260001 | seq=" + std::to_string(count_++);
    publisher_->publish(message);
}

void IntroNode::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());
}