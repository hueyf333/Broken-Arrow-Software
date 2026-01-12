#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace BrokenArrow {
namespace IO {

class Serialization {
public:
    class JSONWriter {
    public:
        JSONWriter() : indent_(0) {}

        void BeginObject() {
            content_ << "{\n";
            indent_++;
        }

        void EndObject() {
            indent_--;
            content_ << Indent() << "}";
        }

        void BeginArray(const std::string& name) {
            content_ << Indent() << "\"" << name << "\": [\n";
            indent_++;
        }

        void EndArray() {
            indent_--;
            content_ << "\n" << Indent() << "]";
        }

        void WriteString(const std::string& name, const std::string& value, bool comma = true) {
            content_ << Indent() << "\"" << name << "\": \"" << value << "\"";
            if (comma) content_ << ",";
            content_ << "\n";
        }

        void WriteNumber(const std::string& name, int value, bool comma = true) {
            content_ << Indent() << "\"" << name << "\": " << value;
            if (comma) content_ << ",";
            content_ << "\n";
        }

        void WriteNumber(const std::string& name, float value, bool comma = true) {
            content_ << Indent() << "\"" << name << "\": " << value;
            if (comma) content_ << ",";
            content_ << "\n";
        }

        void WriteBool(const std::string& name, bool value, bool comma = true) {
            content_ << Indent() << "\"" << name << "\": " << (value ? "true" : "false");
            if (comma) content_ << ",";
            content_ << "\n";
        }

        std::string ToString() const {
            return content_.str();
        }

        bool SaveToFile(const std::string& filename) {
            std::ofstream file(filename);
            if (!file.is_open()) return false;
            file << content_.str();
            file.close();
            return true;
        }

    private:
        std::string Indent() const {
            return std::string(indent_ * 2, ' ');
        }

        std::ostringstream content_;
        int indent_;
    };

    class JSONReader {
    public:
        bool LoadFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) return false;
            
            std::ostringstream ss;
            ss << file.rdbuf();
            content_ = ss.str();
            return true;
        }

        bool GetString(const std::string& key, std::string& value) {
            std::string searchKey = "\"" + key + "\":";
            size_t pos = content_.find(searchKey);
            if (pos == std::string::npos) return false;

            pos = content_.find("\"", pos + searchKey.length());
            if (pos == std::string::npos) return false;

            size_t endPos = content_.find("\"", pos + 1);
            if (endPos == std::string::npos) return false;

            value = content_.substr(pos + 1, endPos - pos - 1);
            return true;
        }

        bool GetInt(const std::string& key, int& value) {
            std::string str;
            if (!GetString(key, str)) {
                std::string searchKey = "\"" + key + "\":";
                size_t pos = content_.find(searchKey);
                if (pos == std::string::npos) return false;
                
                pos += searchKey.length();
                while (pos < content_.length() && std::isspace(content_[pos])) pos++;
                
                size_t endPos = pos;
                while (endPos < content_.length() && (std::isdigit(content_[endPos]) || content_[endPos] == '-')) {
                    endPos++;
                }
                
                str = content_.substr(pos, endPos - pos);
            }

            try {
                value = std::stoi(str);
                return true;
            } catch (...) {
                return false;
            }
        }

        bool GetFloat(const std::string& key, float& value) {
            std::string str;
            std::string searchKey = "\"" + key + "\":";
            size_t pos = content_.find(searchKey);
            if (pos == std::string::npos) return false;
            
            pos += searchKey.length();
            while (pos < content_.length() && std::isspace(content_[pos])) pos++;
            
            size_t endPos = pos;
            while (endPos < content_.length() && 
                   (std::isdigit(content_[endPos]) || content_[endPos] == '.' || 
                    content_[endPos] == '-' || content_[endPos] == 'e' || content_[endPos] == 'E')) {
                endPos++;
            }
            
            str = content_.substr(pos, endPos - pos);

            try {
                value = std::stof(str);
                return true;
            } catch (...) {
                return false;
            }
        }

    private:
        std::string content_;
    };
};

} // namespace IO
} // namespace BrokenArrow
