#pragma once

template<typename T, void(T::*Func)()>
TaskFunction_t taskWrapper() { return [](void *_this) { (static_cast<T *>(_this)->*Func)(); }; }
#define toFreeRtosTask(c, f) taskWrapper<c,&c::f>()
