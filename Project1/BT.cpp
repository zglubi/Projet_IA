#include "BT.hpp"

void SequenceNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}

NodeState SequenceNode::execute(int& action) {
    for (auto& child : children) {
        if (child->execute(action) == NodeState::FAILURE) {
            return NodeState::FAILURE;
        }
    }
    return NodeState::SUCCESS;
}

void SelectorNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}

NodeState SelectorNode::execute(int& action) {
    for (auto& child : children) {
        if (child->execute(action) == NodeState::SUCCESS) {
            return NodeState::SUCCESS;
        }
    }
    return NodeState::FAILURE;
}

void Blackboard::SetValue(const std::string& key, int value) {
    data[key] = value;
}

int Blackboard::GetValue(const std::string& key) {
    return data[key];
}

ConditionNode::ConditionNode(Blackboard& bb, const std::string& key, int value)
    : blackboard(bb), key(key), expectedValue(value) {
}

NodeState ConditionNode::execute(int& action) 
{
    return (blackboard.GetValue(key) < expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

ActionNode::ActionNode(int type) : actionType(type) {}

NodeState ActionNode::execute(int& action) {
    action = actionType;
    return NodeState::SUCCESS;
}
