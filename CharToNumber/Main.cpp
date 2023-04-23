#include <iostream>

double my_atof(const char* str)
{
    double result = 0.0;
    int whole_part = 0;
    double decimal_part = 0.0;
    bool is_negative = false;
    bool found_decimal = false;
    int decimal_places = 0;

    // Check for a negative sign
    if (*str == '-') {
        is_negative = true;
        str++;
    }

    // Read the whole part of the number
    while (*str != '\0' && *str != '.') {
        whole_part *= 10; // works like a decimal left shift
        whole_part += (*str - '0');
        str++;
    }

    // Check for a decimal point
    if (*str == '.') {
        found_decimal = true;
        str++;
    }

    // Read the decimal part of the number
    while (*str != '\0' && isdigit(*str)) {
        decimal_part *= 10.0; // Works as if wasn't a double but just like an int!
        decimal_part += (*str - '0');
        decimal_places++;
        str++;
    }

    // Combine the whole and decimal parts into the final result
    result = whole_part + (decimal_part / pow(10, decimal_places));

    // Apply the sign if necessary
    if (is_negative) {
        result = -result;
    }

    return result;
}


int main(int argc, char* argv[]) {
	std::cout << "Hello World!" << std::endl;

    const char str[] = "3.14159265359"; // number pi
    double d = my_atof(str);
    std::cout << str << std::endl;
    std::cout << d << std::endl;
    std::cout.precision(10);
    std::cout << d << std::endl;

	return 0;
}