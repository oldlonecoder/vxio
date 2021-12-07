#pragma once


#include <vxio/util/rem.h>
#include <vxio/interpret/types.h>
#include <memory>
#include <vector>
#include <map>


namespace vxio
{

    /*!
     * @brief Arithmetic Logical Unit ( Sort of...)
     *
     * @author &copy; 2014(using m++-union),2020(using std::any) Serge Lussier, oldlonecoder/bretzel/Bretzelus/Bretzeltux, (lussier.serge@gmail.com)
     *
     */

struct VXIO_API_DLL alu
{
    using collection = std::vector<alu>;
    using linkarray = std::vector<alu*>;
    using shared = std::shared_ptr<alu>;

    static std::string mNil;
    
    vxio::type::T  T = vxio::type::float_t; ///< Default type.
    
    std::string text;
    union piudt
    {
        std::string* t;
        double       d;
        uint64_t     u;
        int64_t      i;
        void*        p;
        
        piudt& operator=(double v_) { d = v_; return *this;}
        piudt& operator=(uint64_t v_) { u = v_; return *this;}
        piudt& operator=(int64_t v_) { i = v_; return *this;}
        piudt& operator=(void* v_) { p = v_; return *this;}
        piudt& operator=(std::string* v_) { t = v_; return *this;}
    }v = {nullptr};
    
    enum CNV {
        DEG,
        RAD
    };

    alu(); 

    alu(const alu&) = default;
    alu(double A_);
    alu(float A_);
    //alu(token_t* rhs);

    alu(const std::string & A_);
    alu(const char* A_);
    alu(void* A_);
    alu(alu&&) noexcept = default;
    alu(int8_t A_);
    alu(int16_t A_);
    alu(int32_t A_);
    alu(int64_t A_);
    alu(uint8_t A_);
    alu(uint16_t A_);
    alu(uint32_t A_);
    alu(uint64_t A_);
    alu(bool A_);

    

    alu& operator =(const alu & Lhs_);// { a = lf.a; T = lf.T; return *this; }
    alu& operator =(alu && rhs) noexcept;
    alu operator==(const alu & lf);

    [[nodiscard]] vxio::type::T TypeOf() const { return T; } // [const] Sous r�serve; [const] provided that it is not screwing access...

#pragma region Huge_Overload
    template<typename oftype> [[nodiscard]]oftype value() const {

        if constexpr ((std::is_same<oftype, std::string>::value) || (std::is_same<oftype, const std::string&>::value))
            return (T & vxio::type::text_t ? text : "");
        else
            if constexpr (std::is_same<oftype, const char*>::value)
                return (T & vxio::type::text_t ? text.c_str() : nullptr);
            else
                if constexpr (std::is_same<oftype, void*>::value)
                    return v.p;
                else
                    return static_cast<oftype>(
                        T & vxio::type::i8_t ? v.i :
                        T & vxio::type::i16_t ? v.i :
                        T & vxio::type::i32_t ? v.i :
                        T & vxio::type::i64_t ? v.i :
                        T & vxio::type::u8_t ? v.u :
                        T & vxio::type::u16_t ? v.u :
                        T & vxio::type::u32_t ? v.u :
                        T & vxio::type::u64_t ? v.u :
                        T & vxio::type::bool_t ? v.u :
                        T & vxio::type::float_t ? v.d : 0

                        );
    }

    template<typename of_type> [[nodiscard]] of_type number() const
    {
        if constexpr (std::is_same<of_type, void*>::value)
            return v.p;
        else
            if constexpr (std::is_same<of_type, std::string>::value)
                return (std::string)*this;
            else
                return static_cast<of_type>(
                            T & vxio::type::i8_t ? v.i :
                            T & vxio::type::i16_t ? v.i :
                            T & vxio::type::i32_t ? v.i :
                            T & vxio::type::i64_t ? v.i :
                            T & vxio::type::u8_t ? v.u :
                            T & vxio::type::u16_t ? v.u :
                            T & vxio::type::u32_t ? v.u :
                            T & vxio::type::u64_t ? v.u :
                            T & vxio::type::bool_t ? v.u :
                            T & vxio::type::float_t ? v.d : 0
                    );

    }

    template<vxio::type::T V> bool is() {
        return T == V;
    }

#pragma endregion Huge_Overload
    [[maybe_unused]]bool is_text() const { return T & vxio::type::text_t; }
    [[maybe_unused]] std::string to_str() const;
    [[maybe_unused]] bool isnum() const;
    [[maybe_unused]] [[nodiscard]]std::string types() const;
    virtual ~alu();



    // Arithmetic operations overload between two alu's:

    // Binary operators:

    alu operator +(const alu & Rhs_) const;
    alu operator -(const alu & Rhs_);
    alu operator *(const alu & Rhs_) const;
    alu operator [](alu::CNV Cnv_);

    alu operator /(const alu & Rhs_);
    alu operator %(const alu & Rhs_) const;
    alu operator ^(const alu & Rhs_) const;
    alu operator <<(const alu & Lhs_) const;
    alu operator >>(const alu & Lhs_) const;
    alu operator |(const alu & Lhs_) const;
    alu operator &(const alu & Lhs_) const;
    alu operator ~();
    // assign_t operators:
    alu& operator +=(const alu & Rhs_);
    alu& operator -=(const alu & Rhs_);
    alu& operator *=(const alu & Rhs_);
    alu& operator /=(const alu & Rhs_);
    alu& operator %=(const alu & Rhs_);
    alu& operator |=(const alu & Rhs_);
    alu& operator &=(const alu & Rhs_);
    alu& operator ^=(const alu & Rhs_);

    alu& operator <<=(const alu & Rhs_);
    alu& operator >>=(const alu & Rhs_);


    // boolean operators:
    alu operator >(const alu & Rhs_);
    alu operator <(const alu & Rhs_);
    alu operator >=(const alu & Rhs_);
    alu operator <=(const alu & Rhs_);
    alu operator !=(const alu & Rhs_);
    alu operator ||(const alu & Rhs_);
    alu operator &&(const alu & Rhs_);

    bool operator !();
    //bool IsSet() { return r.has_value(); }
    operator bool();

    alu operator -();
    alu operator +();

    alu operator ++(int);
    alu operator ++();
    alu operator --();
    alu operator --(int);

    alu radical(const alu & Lhs_) const;
    alu factorial(const alu & Lhs_) const;

    bool type_size(const alu & Rhs_) const;
    std::string operator()()const { return (std::string)(*this); }
    explicit operator std::string() const;

    //    private:
    //    alu remove_substr_copy(const std::string& to_erase);
    //    alu completely_remove_substr_copy(const std::string& to_erase);
    //    alu& remove_substr(const std::string& to_erase);
    //    alu& completely_remove_substr(const std::string& to_erase);


};

}
