#pragma once

#include <boost/any.hpp>

#include <memory>
#include <sstream>
#include <vector>

namespace aw {
namespace core {
namespace scripting {

// Container of 'any' arguments that get passed to script resolvers as function parameters.
typedef boost::any Argument;
typedef std::vector<Argument> ArgumentVector;

// Return type also is of type 'any'.
typedef boost::any ReturnValue;

//! \brief A mere thin wrapper around a std::vector of results from a script's
//! execution as a "const vector" cannot be passed into the script-execution
//! function without hitting the chance of getting modified in size.
class ReturnValuesHolder {

    public:

        //@{
        //! \brief Creator functions - I guess 4 return values are sufficient for now.
        //! \todo: Refactor when using variadic templates.
        static ReturnValuesHolder Create() {
            return ReturnValuesHolder(0);
        }

        template<typename T1>
        static ReturnValuesHolder Create() {
            ReturnValuesHolder holder(1);
            holder.m_returnValues[0] = std::unique_ptr<ReturnValue>(new ReturnValue(T1()));
            return holder;
        }

        template<typename T1, typename T2>
        static ReturnValuesHolder Create() {
            ReturnValuesHolder holder(2);
            holder.m_returnValues[0] = std::unique_ptr<ReturnValue>(new ReturnValue(T1()));
            holder.m_returnValues[1] = std::unique_ptr<ReturnValue>(new ReturnValue(T2()));
            return holder;
        }

        template<typename T1, typename T2, typename T3>
        static ReturnValuesHolder Create() {
            ReturnValuesHolder holder(3);
            holder.m_returnValues[0] = std::unique_ptr<ReturnValue>(new ReturnValue(T1()));
            holder.m_returnValues[1] = std::unique_ptr<ReturnValue>(new ReturnValue(T2()));
            holder.m_returnValues[2] = std::unique_ptr<ReturnValue>(new ReturnValue(T3()));
            return holder;
        }

        template<typename T1, typename T2, typename T3, typename T4>
        static ReturnValuesHolder Create() {
            ReturnValuesHolder holder(4);
            holder.m_returnValues[0] = std::unique_ptr<ReturnValue>(new ReturnValue(T1()));
            holder.m_returnValues[1] = std::unique_ptr<ReturnValue>(new ReturnValue(T2()));
            holder.m_returnValues[2] = std::unique_ptr<ReturnValue>(new ReturnValue(T3()));
            holder.m_returnValues[3] = std::unique_ptr<ReturnValue>(new ReturnValue(T4()));
            return holder;
        }
        //@}

        ReturnValuesHolder(ReturnValuesHolder&& holder) {

            m_returnValues.swap(holder.m_returnValues);
        }

        ReturnValuesHolder& operator= (ReturnValuesHolder&& holder) {

            if(this != &holder) {
                m_returnValues.swap(holder.m_returnValues);
            }
            return *this;
        }

        void SetValue(uint8_t index, const ReturnValue& value) const {

            verifyIndex(index);

            // Do not assign another type than the one that is already stored.
            if(m_returnValues[index]->type() != value.type()) {

                std::stringstream errorMessage;
                errorMessage << "Error assigning return value at index " << index << ".";
                throw std::runtime_error(errorMessage.str());
            }

            m_returnValues[index] = std::unique_ptr<ReturnValue>(new ReturnValue(value));
        }

        const ReturnValue& GetValue(uint8_t index) const {

            verifyIndex(index);
            return *m_returnValues[index];
        }

        template<typename T>
        const T& GetTypedValue(uint8_t index) const {

            const ReturnValue& retVal = GetValue(index);
            const T* val = boost::any_cast<T>(&retVal);
            if(!val) {

                std::stringstream errorMessage;
                errorMessage << "Error retrieving typed value at index " << index << ".";
                throw std::runtime_error(errorMessage.str());
            }
            return *val;
        }

        std::string GetValueTypeName(uint8_t index) const {

            verifyIndex(index);
            return m_returnValues[index]->type().name();
        }

        uint8_t Size() const {

            return static_cast<uint8_t>(m_returnValues.size());
        }

        bool Empty() const {

            return (Size() == 0);
        }

    protected:

        ReturnValuesHolder(uint8_t size = 0)
        : m_returnValues(size) {
        }

        void verifyIndex(uint8_t index) const {

            if(index >= m_returnValues.size()) {

                std::stringstream errorMessage;
                errorMessage << "Error accessing return value at index "
                             << index << ", " << m_returnValues.size() 
                             << " elements accessible.";

                throw std::out_of_range(errorMessage.str());
            }
        }

        mutable std::vector<std::unique_ptr<ReturnValue>> m_returnValues;
};


} // namespace scripting
} // namespace core
} // namespace aw
