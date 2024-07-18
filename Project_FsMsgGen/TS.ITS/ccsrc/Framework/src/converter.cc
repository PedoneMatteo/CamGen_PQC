#include "converter.hh"
#include <stdexcept>
converter *converter::instance = NULL;

uint16_t converter::swap(const uint16_t p_value) {
  uint8_t *ptr = (uint8_t *)&p_value;
  return (ptr[0] << 8) | ptr[1];
}

uint32_t converter::swap(const uint32_t p_value) {
  uint8_t *ptr = (uint8_t *)&p_value;
  return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

const std::string converter::lut_u = "0123456789ABCDEF";
const std::string converter::lut_l = "0123456789abcdef";
std::string       converter::string_to_hexa(const std::string &p_value, const bool p_uppercase) {

  std::string input(p_value);
  std::for_each(input.begin(), input.end(), [](char &c) { c = std::toupper(c); });

  std::string output;
  uint32_t    length = p_value.length();
  output.reserve(2 * length);
  if (p_uppercase) { // TODO Use pointer to reduce code size
    for (uint32_t i = 0; i < length; ++i) {
      const uint8_t c = input[i];
      output.push_back(lut_u[c >> 4]);
      output.push_back(lut_u[c & 15]);
    } // End of 'for' statement
  } else {
    for (uint32_t i = 0; i < length; ++i) {
      const uint8_t c = input[i];
      output.push_back(lut_l[c >> 4]);
      output.push_back(lut_l[c & 15]);
    } // End of 'for' statement
  }

  return output;
}

std::string converter::bytes_to_hexa(const std::vector<uint8_t> &p_value, const bool p_uppercase) {
  std::string ret;
  ret.assign(p_value.size() * 2, ' ');
  if (p_uppercase) { // TODO Use pointer to reduce code size
    for (size_t i = 0; i < p_value.size(); i++) {
      uint8_t c      = p_value[i];
      ret[i * 2]     = lut_u[c >> 4];
      ret[i * 2 + 1] = lut_u[c & 0xF];
    }
  } else {
    for (size_t i = 0; i < p_value.size(); i++) {
      uint8_t c      = p_value[i];
      ret[i * 2]     = lut_l[c >> 4];
      ret[i * 2 + 1] = lut_l[c & 0xF];
    }
  }
  return ret;
}

inline uint8_t char2byte(const char p_ch) {
  size_t s = converter::lut_l.find(p_ch);
  if (s == std::string::npos) {
    if ((s = converter::lut_u.find(p_ch)) == std::string::npos) {
      throw(std::length_error(""));
    }
  }
  return s;
}

std::vector<uint8_t> converter::hexa_to_bytes(const std::string &p_value) {
  // Sanity check
  std::vector<uint8_t> output;
  size_t               i = 0, idx = 0, outlen = (p_value.length() + 1) / 2;

  output.assign(outlen, 0x00);
  try {
    if (p_value.length() & 1)
      output[idx++] = char2byte(p_value[i++]);
    for (; idx < outlen; idx++) {
      uint8_t b0  = char2byte(p_value[i++]);
      uint8_t b1  = char2byte(p_value[i++]);
      output[idx] = (b0 << 4) | b1;
    }
  } catch (const std::length_error &le) {
    output.clear();
  }
  return output;
}

std::string converter::time_to_string(const time_t p_time) {
  struct tm *t = std::localtime(&p_time);
  return time_to_string(*t);
}

std::string converter::time_to_string(const struct tm &p_time) {
  char buffer[64] = {0};
  // Format: RFC 822, 1036, 1123, 2822
  std::strftime(buffer, 64, "%a, %d %b %Y %H:%M:%S %z", &p_time);
  return std::string(buffer);
}

std::string converter::trim(const std::string &str, const std::string &whitespace) {
  size_t strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return ""; // no content

  size_t strEnd   = str.find_last_not_of(whitespace);
  size_t strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

std::vector<std::string> converter::split(const std::string &p_value, const std::string &p_separator) {
  std::vector<std::string> output;
  std::size_t              current, previous = 0;
  current = p_value.find(p_separator);
  while (current != std::string::npos) {
    output.push_back(p_value.substr(previous, current - previous));
    previous = current + 1;
    current  = p_value.find(p_separator, previous);
  }
  output.push_back(p_value.substr(previous, current - previous));

  return output;
}

std::vector<std::string> converter::split_arguments_line(const std::string &p_value) {
  std::vector<std::string> output;
  std::string              line = trim(p_value);
  if (!line.empty() && (line[0] == '-')) { // Valid command line
    size_t current = 0;
    size_t next    = (size_t)-1;
    size_t pos     = 0;
    do {
      if (line[pos + 1] == '-') { // --
        current = pos + 2;
      } else {
        current = pos + 1;
      }
      next = line.find("-", current);
      std::string str(line.substr(pos, next - pos));
      output.push_back(str);
      pos = next;
    } while (next != std::string::npos);
  } // else, invalid command line
  return output;
}

const std::string converter::base64_enc_map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::vector<unsigned char> converter::buffer_to_base64(const std::vector<unsigned char> &p_value) {
  std::vector<unsigned char> out;

  int val = 0, valb = -6;
  for (unsigned char c : p_value) {
    val = (val << 8) + c;
    valb += 8;
    while (valb >= 0) {
      out.push_back(converter::base64_enc_map[(val >> valb) & 0x3F]);
      valb -= 6;
    } // End of 'while' statement
  }   // End of 'for' statement
  if (valb > -6) {
    out.push_back(converter::base64_enc_map[((val << 8) >> (valb + 8)) & 0x3F]);
  }
  while (out.size() % 4) {
    out.push_back('=');
  } // End of 'while' statement

  return out;
}

std::vector<unsigned char> converter::base64_to_buffer(const std::vector<unsigned char> &p_value) {
  std::vector<unsigned char> out;

  std::vector<int> T(256, -1);
  for (int i = 0; i < 64; i++) {
    T[converter::base64_enc_map[i]] = i;
  }

  int val = 0, valb = -8;
  for (unsigned char c : p_value) {
    if (T[c] == -1) {
      break;
    }
    val = (val << 6) + T[c];
    valb += 6;
    if (valb >= 0) {
      out.push_back((unsigned char)char((val >> valb) & 0xFF));
      valb -= 8;
    }
  } // End of 'for' statement
  return out;
}
