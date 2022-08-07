#pragma once

#include "mvc/View.hpp"
#include "mvc/Model.hpp"

#include <memory>
#include <type_traits>

namespace mvc
{
    class IController
    {
    };

    template <class TView, class TModel>
    requires std::is_base_of_v<IView, TView> &&(std::is_base_of_v<IModel, TModel> || std::is_void_v<TModel>) //
        class Controller : public IController
    {
    public:
        Controller(std::shared_ptr<TView> view, std::shared_ptr<TModel> model) : m_view{view}, m_model{model}
        {
        }

        std::shared_ptr<TView> view() const
        {
            return m_view;
        }

        std::shared_ptr<TModel> model() const
        {
            return m_model;
        }

    private:
        std::shared_ptr<TView> m_view;
        std::shared_ptr<TModel> m_model;
    };
} // namespace mvc
