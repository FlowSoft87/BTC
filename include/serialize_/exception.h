#ifndef BTC_SERIALIZE_EXCEPTION_H_
#define BTC_SERIALIZE_EXCEPTION_H_

#include <exception>

class tag_not_found_error : public std::exception {

    std::string msg;

  public:
    tag_not_found_error(const std::string& method_name, const std::string& tag_val) 
            : msg("Error (") {
        msg += method_name;
        msg += "): The tag \"";
        msg += tag_val;
        msg += "\" does not exist!";
    }

    ~tag_not_found_error() throw() {}

    const char* what() const throw() {
        return (msg.c_str());
    }
};

class wrong_type_error : public std::exception {

    std::string msg;

  public:
    wrong_type_error(const std::string& method_name, const std::string& type_name) 
            : msg("Error (") {
        msg += method_name;
        msg += "): The entry is of type ";
        msg += type_name;
        msg += "!";
    }

    ~wrong_type_error() throw() {}

    const char* what() const throw() {
        return (msg.c_str());
    }
};

#endif
