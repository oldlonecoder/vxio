#pragma once

#include <vxio/interpret/types.h>



namespace vxio
{

class VXIO_API_DLL token_data
{
public:
    mnemonic       c = mnemonic::noop_;
    vxio::type::T  t = vxio::type::null_t;    ///< Primitive vxio::type bit.
    vxio::type::T  s = vxio::type::null_t;    ///< Semantic vxio::type bits field
    vxio::type::distance::T d = vxio::type::distance::noop_;
    void* vdata = nullptr;

    using collection = std::vector<token_data>;
    using iterator = collection::iterator;

    struct VXIO_API_DLL  location_data
    {
        const char* begin = nullptr;
        const char* end   = nullptr;
        int     linenum = -1; ///< line number
        int     colnum  = -1; ///< Column number
        int64_t offset  = -1; ///< Absolute Offset from the beginning of the Source Text.
        int     ln      = 0; ///< Length of this token attribute.
        std::string operator()() const;
        std::string text() const;
        std::string position() const;
    }  mLoc;

    struct VXIO_API_DLL Flag
    {
        uint8_t V : 1; ///< Pre-parsed as a value Token;
        uint8_t S : 1; ///< Post parsed as assignable
        uint8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
    }   _flags = { 0, 0, 0 };

    static token_data mNull;

    token_data() = default;
    ~token_data() = default;

    token_data(mnemonic Code_, vxio::type::T Type_, vxio::type::T Sem_, vxio::type::distance::T Delta_, lexem::T aLexem, uint8_t V);
    token_data(mnemonic Code_, vxio::type::T Type_, vxio::type::T Sem_, vxio::type::distance::T Delta_, token_data::location_data LocationData_, token_data::Flag Flags_, void* Ptr_ = nullptr);
    token_data(const token_data & Token_);
    token_data(token_data && Token_) noexcept;

    token_data& operator=(token_data && Token_) noexcept;
    token_data& operator=(const token_data & Token_);
    token_data operator[](mnemonic CC);
    //ArgToken* ptr() { return this; }
    std::string mark() const;

    explicit operator bool() const
    {
        return mLoc.begin != nullptr;
    }
    bool operator||(vxio::type::T Sem_) const
    {
        return s || Sem_;
    }

    [[nodiscard]] std::string text() const
    {
        return mLoc();
    }
    [[nodiscard]] std::string semantic_types() const ;
    [[nodiscard]] std::string type_name() const;
    [[nodiscard]] std::string details(bool Mark_ = false) const;

    static token_data scan(const char* C_);

    [[nodiscard]] bool is_binary() const
    {
        return s & vxio::type::binary_t;
    }
    [[nodiscard]] bool is_unary() const
    {
        return s & vxio::type::unary_t;
    }
    [[nodiscard]] bool is_identifier() const
    {
        return s & vxio::type::id_t;
    }
    [[nodiscard]] bool is_leaf() const
    {
        return s & vxio::type::leaf_t;
    }
    [[nodiscard]] bool is_operator() const
    {
        return s & vxio::type::operator_t;
    }
    [[nodiscard]] bool is_closing_pair() const
    {
        return s & vxio::type::close_pair_t;
    }
    [[nodiscard]] bool is_punctuation() const
    {
        return s & vxio::type::punc_t;
    }
    [[nodiscard]] bool is_number() const
    {
        return s & vxio::type::number_t;
    }
    [[nodiscard]] bool is_hexadecimal() const
    {
        return s & vxio::type::hex_t;
    }
    [[nodiscard]] bool is_prefix() const
    {
        return s & vxio::type::prefix_t;
    }
    [[nodiscard]] bool is_postfix() const
    {
        return s & vxio::type::postfix_t;
    }
    [[nodiscard]] bool is_instruction() const
    {
        return !(s & (vxio::type::operator_t | vxio::type::leaf_t));
    }


    static vxio::type::T ktype(mnemonic m);
};

}
