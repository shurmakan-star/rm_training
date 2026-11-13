#include "rclcpp/rclcpp.hpp"
#include "pubsub_intro/intro_node.hpp"

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<IntroNode>();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}