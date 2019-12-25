
#ifndef FRAMEWORK_PCH_H
#define FRAMEWORK_PCH_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#define SDL_MAIN_HANDLED

#define OBJECT_ACCESSOR(type, name) \
void name(type name) { m_##name = name; } \
type name() { return m_##name; } \
const type name() const { return m_##name; }

#endif