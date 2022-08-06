#pragma once

#include <type_traits>

class IController;

class IView
{
public:
    virtual void render() = 0;
};

template <class TController>
requires std::is_base_of_v<IController, TController>
class View : public virtual IView
{
public:
    void set_controller(TController *controller)
    {
        this->m_controller = controller;
    }

    TController *controller() const
    {
        return m_controller;
    }

private:
    TController *m_controller;
};
