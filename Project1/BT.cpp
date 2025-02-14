#include "BT.hpp"

void SequenceNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}

NodeState SequenceNode::execute() {
    for (auto& child : children) {
        if (child->execute() == NodeState::FAILURE) {
            return NodeState::FAILURE;
        }
    }
    return NodeState::SUCCESS;
}

void SelectorNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}

NodeState SelectorNode::execute() {
    for (auto& child : children) {
        if (child->execute() == NodeState::SUCCESS) {
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

NodeState ConditionNode::execute() {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

ActionNode::ActionNode(std::string name) : actionName(name) {}

NodeState ActionNode::execute() {
    std::cout << "Action: " << actionName << std::endl;
    return NodeState::SUCCESS;
}
