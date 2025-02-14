#ifndef BT_HPP
#define BT_HPP
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

enum class NodeState { SUCCESS, FAILURE, RUNNING };

class BTNode {
public:
    virtual ~BTNode() = default;
    virtual NodeState execute() = 0;
};

class SequenceNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState execute() override;
};

class SelectorNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState execute() override;
};

class Blackboard {
private:
    std::unordered_map<std::string, int> data;
public:
    void SetValue(const std::string& key, int value);
    int GetValue(const std::string& key);
};

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    int expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, int value);
    NodeState execute() override;
};

class ActionNode : public BTNode {
private:
    std::string actionName;
public:
    ActionNode(std::string name);
    NodeState execute() override;
};
#endif