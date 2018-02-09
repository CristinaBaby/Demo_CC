//
//  DynAny.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/8.
//
//

#ifndef DynAny_hpp
#define DynAny_hpp

#include <iostream>
#include <cassert>  
using namespace std;

class DynAny {
public:
    class placeholder{
    public:
        virtual ~placeholder(){
            
        }
        virtual const std::type_info & type() const =0;
        virtual placeholder* cloneObj() const  = 0;
    };
    
    template<typename ValueType>
    class holder:public placeholder {
    public:
        holder(const ValueType &value):held(value) {
            
        }
    public:
        virtual const std::type_info& type() const {
            return typeid(ValueType);
        }
        
        virtual placeholder* cloneObj() const {
            return new holder(held);
        }
    public:
        ValueType held;
    };
    
public:
    DynAny():content(nullptr) {
        
    }
    
    template<typename ValueType>
    DynAny(const ValueType & value)
    : content(new holder<ValueType>(value))
    {
        
    }

    
    DynAny(const DynAny & other): content(other.content?other.content->cloneObj():0){
        
    }
    
    
    
    ~DynAny() {
        if(nullptr != content)
            delete content;
    }
private:
    placeholder* content;
     template<typename ValueType> friend ValueType dynAny_cast(const DynAny& operand);
public:
    const std::type_info & type() const {

        return (content != nullptr)?content->type():typeid(void);
    }
    
    DynAny& swap(DynAny& temp) {
        std::swap(content,temp.content);
        return *this;
    }
    
    template<typename ValueType>
    DynAny & operator=(const ValueType & rhs)
    {
        DynAny(rhs).swap(*this);
        return *this;
    }
    
    DynAny & operator=( DynAny rhs)
    {
        rhs.swap(*this);
        return *this;
    }
    
    bool empty() const
    {
        return !content;
    }
};

template<typename ValueType>  ValueType dynAny_cast(const DynAny& operand){
    assert( operand.type() == typeid(ValueType) );
    return static_cast<DynAny::holder<ValueType> *>(operand.content)->held;
}

#endif /* DynAny_hpp */
