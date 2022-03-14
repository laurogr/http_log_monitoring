#pragma once
#include <unordered_map>

using LogBuffer = struct LogBuffer;

// LineBuffer is going to store all the information in a specific second :
// number of connections and all requests
class LineBuffer {
 public:
  LineBuffer() = default;

  explicit LineBuffer(std::string&& s) {
    hashmapRequests[s] = 1;
    count = 1;
  }

  void add(std::string&& resource) {
    if (this->hashmapRequests.find(resource) == this->hashmapRequests.end()) {
      hashmapRequests[resource] = 1;
    } else {
      hashmapRequests[resource] += 1;
    }
    ++count;  // total count of connection for this specific second
  }

  [[nodiscard]] int getCount() const { return this->count; }

  [[nodiscard]] std::unordered_map<std::string, int> getHashmapRequest() const {
    return hashmapRequests;
  }

 private:
  std::unordered_map<std::string, int> hashmapRequests;
  int count{0};
};

// Buffer used to store all the lines using a hashmap (using timestamp as key)
class LogBuffer {
 public:
  explicit LogBuffer(int sizeInSeconds)
      : sizeInSeconds{sizeInSeconds},
        numberOfConnections{0},
        averageConnectionsPerSecond{0} {
    hashmapLinesSeconds.reserve(sizeInSeconds);
  }

  void update(long int timestamp, std::string&& resource) {
    erase(timestamp);
    add(timestamp, std::move(resource));
    ++numberOfConnections;
    averageConnectionsPerSecond =
        numberOfConnections / hashmapLinesSeconds.size();
  }

  [[nodiscard]] int getAverageConnections() const {
    return averageConnectionsPerSecond;
  }

  [[nodiscard]] LineBuffer getLineBuffer(long int timestamp) const {
    if (hashmapLinesSeconds.find(timestamp) != hashmapLinesSeconds.end()) {
      return hashmapLinesSeconds.at(timestamp);
    }
    return {};
  }

 private:
  void add(long int timestamp, std::string&& section) {
    if (hashmapLinesSeconds.find(timestamp) == hashmapLinesSeconds.end())
      hashmapLinesSeconds.insert({timestamp, LineBuffer{std::move(section)}});
    else
      hashmapLinesSeconds.at(timestamp).add(std::move(section));
  }

  void erase(long int timestamp) {
    auto tail = timestamp - sizeInSeconds;
    if (hashmapLinesSeconds.find(tail) != hashmapLinesSeconds.end()) {
      numberOfConnections -= hashmapLinesSeconds.at(tail).getCount();
      hashmapLinesSeconds.erase(tail);
    }
  }

  std::unordered_map<long int, LineBuffer> hashmapLinesSeconds;
  int sizeInSeconds;
  int numberOfConnections;
  int averageConnectionsPerSecond;
};
