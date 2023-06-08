#include <string>

/**
 * @file validation.h
 *
 * @brief Declaration of validation functions to validate various inputs.
 */

/**
 * @brief Checks if the given input string is a valid type.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid type, `false` otherwise.
 */
bool isValidType(const std::string &input);

/**
 * @brief Checks if the given input string is a valid title.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid title, `false` otherwise.
 */
bool isValidTitle(const std::string &input);

/**
 * @brief Checks if the given input string is a valid description.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid description, `false` otherwise.
 */
bool isValidDescription(const std::string &input);

/**
 * @brief Checks if the given input string is a valid repeat value.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid repeat value, `false` otherwise.
 */
bool isValidRepeat(const std::string &input);

/**
 * @brief Checks if the given input string is a valid date.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid date, `false` otherwise.
 */
bool isValidDate(const std::string &input);

/**
 * @brief Checks if the given input string is a valid number of minutes.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid number of minutes, `false` otherwise.
 */
bool isValidMinutes(const std::string &input);

/**
 * @brief Checks if the given input string is a valid boolean value.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid boolean value, `false` otherwise.
 */
bool isValidBool(const std::string &input);

/**
 * @brief Checks if the given input string is a valid location.
 *
 * @param input The input string to be validated.
 * @return `true` if the input string is a valid location, `false` otherwise.
 */
bool isValidLocation(const std::string &input);