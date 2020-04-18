
#ifndef FRAMEWORK_CORE_MODEL_H
#define FRAMEWORK_CORE_MODEL_H

namespace framework {
namespace core {

class Model;
typedef std::shared_ptr<Model> Model_ptr;

class Model : std::enable_shared_from_this<Model>
{
public:

};

} // ns core
} // ns framework

#endif