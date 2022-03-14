#pragma once

using LogBuffer = struct LogBuffer;

class LineBuffer {
 public:
  LineBuffer() = default;

  explicit LineBuffer(std::string&& s) {
    hashmapRequests[s] = 1;
    count = 1;
  }

  void add(std::string&& section) {
    if (this->hashmapRequests.find(section) == this->hashmapRequests.end()) {
      hashmapRequests[section] = 1;
    } else {
      hashmapRequests[section] += 1;
    }
    ++count;  // total count of connection for this specific second
  }

  [[nodiscard]] int getCount() const { return this->count; }

  std::unordered_map<std::string, int> hashmapRequests;
  int count{0};
};

class LogBuffer {  //'circular buffer' with a hashmap, only using sizeInSec
 public:
  explicit LogBuffer(int sizeInSeconds)
      : sizeInSeconds{sizeInSeconds},
        numberOfConnections{0},
        averageConnectionsPerSecond{0} {
    hashmapLinesSeconds.reserve(sizeInSeconds);
  }

  void update(long int timestamp, std::string&& section) {
    erase(timestamp);
    add(timestamp, std::move(section));
    ++numberOfConnections;
    averageConnectionsPerSecond = numberOfConnections / hashmapLinesSeconds.size();
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
      hashmapLinesSeconds.insert(
          {timestamp, LineBuffer{std::move(section)}});
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
