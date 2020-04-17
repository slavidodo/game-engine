
#ifndef FRAMEWORK_CORE_OBJECT_H
#define FRAMEWORK_CORE_OBJECT_H

namespace framework {
namespace core {
namespace scene {

class Node;
typedef std::shared_ptr<Node> Node_ptr;

class Node : public std::enable_shared_from_this<Node>
{
public:

};

} // ns scene
} // ns core
} // ns framework

#endif