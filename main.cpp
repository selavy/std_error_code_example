// following example from http://ned14.github.io/outcome/motivation/plug_error_code/
#include <iostream>
#include <string>
#include <system_error>

enum class ConversionErrc
{
    Success     = 0,
    EmptyString = 1,
    IllegalChar = 2,
    TooLong     = 4,
};

namespace std
{

// NOTE(peter): Register with error code system
template <> struct is_error_code_enum<ConversionErrc> : std::true_type {};

}

namespace detail
{

// Define a error code category
class ConversionErrc_category : public std::error_category
{
public:
    virtual const char* name() const noexcept override final
    { return "ConversionError"; }

    virtual std::string message(int c) const override final
    {
        switch (static_cast<ConversionErrc>(c)) {
            case ConversionErrc::Success:
                return "success";
            case ConversionErrc::EmptyString:
                return "empty string";
            case ConversionErrc::IllegalChar:
                return "got non-digit char when converting to a number";
            case ConversionErrc::TooLong:
                return "the number would not fit into memory";
            default:
                return "unknown";
        }
    }

    // Optional: allow generic error conditions to be compared
    virtual std::error_condition
    default_error_condition(int c) const noexcept override final
    {
        switch (static_cast<ConversionErrc>(c)) {
            case ConversionErrc::EmptyString:
                return make_error_condition(std::errc::invalid_argument);
            case ConversionErrc::IllegalChar:
                return make_error_condition(std::errc::invalid_argument);
            case ConversionErrc::TooLong:
                return make_error_condition(std::errc::result_out_of_range);
            default:
                return std::error_condition(c, *this);
        }
    }
};

}

extern inline const detail::ConversionErrc_category&
ConversionErrc_category()
{
    static detail::ConversionErrc_category c;
    return c;
}

inline std::error_code make_error_code(ConversionErrc e)
{
    return std::error_code{static_cast<int>(e), ConversionErrc_category()};
}

int main(int argc, char** argv)
{
    std::error_code ec = ConversionErrc::IllegalChar;
    std::cout << "ConversionErrc::IllegalChar is printed by std::error_code as "
        << ec << " with explanatory message: " << ec.message() << "\n";

    // Can compare ConversionErrc to generic conditions
    std::cout << "ec is equivalent to std::errc::invalid_argument = "
        << (ec == std::errc::invalid_argument) << "\n";
    std::cout << "ec is equivalent to std::errc::result_out_of_range = "
        << (ec == std::errc::result_out_of_range) << "\n";
    return 0;
}
