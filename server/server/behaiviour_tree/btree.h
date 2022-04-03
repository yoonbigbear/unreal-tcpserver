#ifndef BTREE_H_
#define BTREE_H_

#include "behaviortree_cpp_v3/basic_types.h"
#include "behaviortree_cpp_v3/action_node.h"
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace BT;
NodeStatus CheckBattery();

// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface
{
public:
    GripperInterface() : _open(true) {}

    NodeStatus open() {
        _open = true;
        std::cout << "GripperInterface::open" << std::endl;
        return NodeStatus::SUCCESS;
    }

    NodeStatus close() {
        std::cout << "GripperInterface::close" << std::endl;
        _open = false;
        return NodeStatus::SUCCESS;
    }

private:
    bool _open; // shared information
};

class ApproachObject : public BT::CoroActionNode
{
public:
    ApproachObject(const std::string& name) :
        BT::CoroActionNode(name, {})
    {
    }


    // CoroActionNode을(를) 통해 상속됨
    virtual BT::NodeStatus tick() override
    {
        return BT::NodeStatus();
    }

    //// You must override the virtual function tick()
    //BT::NodeStatus tick() override
    //{
    //    std::cout << "ApproachObject: " << this->name() << std::endl;
    //    return BT::NodeStatus::SUCCESS;
    //}
};

class btree
{
public:
    void Start();
};

#endif // !BTREE_H_