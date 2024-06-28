#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Utility function to split a string by a delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(std::move(token));
    }
    return tokens;
}

// Function to calculate the checksum of an NMEA sentence
unsigned char calculate_checksum(const std::string& sentence) {
    unsigned char checksum = 0;
    for (char ch : sentence) {
        checksum ^= ch;
    }
    return checksum;
}

// Function to parse GGA sentence
json parse_gga(const std::vector<std::string>& fields) {
    return {
        {"Type", "GGA"},
        {"Time", fields[1]},
        {"Latitude", fields[2] + " " + fields[3]},
        {"Longitude", fields[4] + " " + fields[5]},
        {"Fix Quality", fields[6]},
        {"Number of Satellites", fields[7]},
        {"Horizontal Dilution", fields[8]},
        {"Altitude", fields[9] + " " + fields[10]},
        {"Geoid Height", fields[11] + " " + fields[12]}
    };
}

// Function to parse GLL sentence
json parse_gll(const std::vector<std::string>& fields) {
    return {
        {"Type", "GLL"},
        {"Latitude", fields[1] + " " + fields[2]},
        {"Longitude", fields[3] + " " + fields[4]},
        {"Time", fields[5]},
        {"Status", fields[6]}
    };
}

// Main function to parse an NMEA sentence
json parse_nmea_sentence(const std::string& nmea_sentence) {
    json j;
    if (nmea_sentence[0] != '$') {
        j["error"] = "Invalid NMEA sentence: " + nmea_sentence;
        return j;
    }

    std::string sentence = nmea_sentence.substr(1);
    std::vector<std::string> parts = split(sentence, '*');
    if (parts.size() != 2) {
        j["error"] = "Invalid NMEA sentence format: " + nmea_sentence;
        return j;
    }

    const std::string& data_part = parts[0];
    const std::string& checksum_part = parts[1];
    unsigned char calculated_checksum = calculate_checksum(data_part);
    unsigned char provided_checksum = std::stoi(checksum_part, nullptr, 16);

    if (calculated_checksum != provided_checksum) {
        j["error"] = "Checksum mismatch: " + nmea_sentence;
        return j;
    }

    std::vector<std::string> fields = split(data_part, ',');
    std::string sentence_type = fields[0].substr(2);

    if (sentence_type == "GGA") {
        j = parse_gga(fields);
    } else if (sentence_type == "GLL") {
        j = parse_gll(fields);
    } else {
        j["error"] = "Unknown NMEA sentence type: " + sentence_type;
    }
    return j;
}

int main() {
    std::ifstream infile("nmea.txt");
    if (!infile) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    json j_array = json::array();
    std::string line;
    while (std::getline(infile, line)) {
        json j = parse_nmea_sentence(line);
        j_array.push_back(std::move(j));
    }

    std::ofstream outfile("output.json");
    outfile << std::setw(4) << j_array << std::endl;

    return 0;
}
