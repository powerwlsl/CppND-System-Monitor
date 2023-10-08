#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::map<std::string, double> memory_info;
  std::ifstream file(kProcDirectory + kMeminfoFilename);

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string key, value;
      if (iss >> key >> value) {
        // Remove ':' from the key
        key.erase(std::remove(key.begin(), key.end(), ':'), key.end());

        // Check if the key is "MemTotal" or "MemFree"
        if (key == "MemTotal" || key == "MemFree") {
          // Convert the value to a double (in gigabytes)
          float val = std::stof(value) / 1024.0 / 1024.0;
          memory_info[key] = val;
        }
      }
    }
    file.close();
    // Calculate the memory used (in gigabytes)
    if (memory_info.count("MemTotal") > 0 && memory_info.count("MemFree") > 0) {
      float mem_total = memory_info["MemTotal"];
      float mem_free = memory_info["MemFree"];
      return mem_total - mem_free;
    }
  }
  return 0.0;
}

long LinuxParser::UpTime() {
  std::ifstream file(kProcDirectory + kUptimeFilename);
  long up_time = 0.0;

  if (file.is_open()) {
    file >> up_time;
    file.close();
  }
  return up_time;
}

long LinuxParser::Jiffies() {
  long total = 0;
  std::ifstream file(kProcDirectory + kStatFilename);

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string key;
      iss >> key;
      if (key == "cpu") {
        long jiffies;
        while (iss >> jiffies) {
          total += jiffies;
        }
        break;  // Break after parsing the "cpu" line
      }
    }
    file.close();
  }

  return total;
}

long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (file.is_open()) {
    std::string line;
    if (std::getline(file, line)) {
      std::istringstream iss(line);
      // Read the values from /proc/[PID]/stat
      std::string comm;  // Process name
      char state;
      int ppid;
      long utime, stime;
      // Extract the values we need
      iss >> pid >> comm >> state >> ppid >> utime >> stime;

      // Calculate active jiffies for the process
      activeJiffies = utime + stime;
    }
    file.close();
  }

  return activeJiffies;
}

long LinuxParser::ActiveJiffies() {
  long activeJiffies = 0;
  std::ifstream file(kProcDirectory + kStatFilename);

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string key;
      iss >> key;
      if (key == "cpu") {
        long user, nice, system, irq, softirq, steal;
        iss >> user >> nice >> system >> irq >> softirq >> steal;
        // Calculate active jiffies (excluding idle and iowait)
        activeJiffies = user + nice + system + irq + softirq + steal;
        break;  // Break after parsing the "cpu" line
      }
    }
    file.close();
  }

  return activeJiffies;
}

long LinuxParser::IdleJiffies() {
  std::ifstream file(kProcDirectory + kStatFilename);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string key;
    long idle_jiffies;

    // Split each line into key and value
    if (iss >> key >> idle_jiffies) {
      if (key == "cpu") {
        // Assuming the "cpu" line contains the idle jiffies
        return idle_jiffies;
      }
    }
  }

  // Return 0 if the "cpu" line is not found
  return 0;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;

  // Read the content of the file into a stringstream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (getline(filestream, line)) {
      // Check if the line starts with "cpu "
      if (line.find("cpu ") == 0) {
        // Split the line into tokens using whitespace as a delimiter
        std::istringstream linestream(line);
        vector<string> tokens(std::istream_iterator<string>{linestream},
                              std::istream_iterator<string>());

        // Extract the CPU utilization values from the tokens
        for (size_t i = 1; i < tokens.size(); ++i) {
          cpu_utilization.push_back(tokens[i]);
        }
        break;
      }
    }
  }

  return cpu_utilization;
}

int LinuxParser::TotalProcesses() {
  int total_processes = 0;

  // Read the content of the file into a stringstream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (getline(filestream, line)) {
      // Check if the line starts with "processes "
      if (line.find("processes ") == 0) {
        // Split the line into tokens using whitespace as a delimiter
        std::istringstream linestream(line);
        string key;
        int value;

        linestream >> key >> value;
        total_processes = value;
        break;
      }
    }
  }

  return total_processes;
}

int LinuxParser::RunningProcesses() {
  int running_processes = 0;

  // Read the content of the file into a stringstream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      // Check if the line starts with "procs_running "
      if (line.find("procs_running ") == 0) {
        std::istringstream linestream(line);
        string key;
        int value;

        linestream >> key >> value;
        running_processes = value;

        break;
      }
    }
  }

  return running_processes;
}

string LinuxParser::Command(int pid) {
  string command;

  // Generate the path to the cmdline file for the given PID
  string cmdline_path = kProcDirectory + to_string(pid) + kCmdlineFilename;

  // Read the content of the cmdline file
  std::ifstream filestream(cmdline_path);
  if (filestream.is_open()) {
    std::getline(filestream, command);

    // cmdline contains null-separated arguments, replace null characters with
    // spaces
    size_t null_pos;
    while ((null_pos = command.find('\0')) != string::npos) {
      command.replace(null_pos, 1, " ");
    }
  }

  return command;
}

string LinuxParser::Ram(int pid) {
  string memory_used = "0";
  string status_path = kProcDirectory + to_string(pid) + kStatusFilename;

  std::ifstream filestream(status_path);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      if (line.find("VmData:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        string value;

        linestream >> key >> value;

        memory_used = value;

        break;
      }
    }
  }

  return memory_used;
}

string LinuxParser::Uid(int pid) {
  string uid;

  string status_path = kProcDirectory + to_string(pid) + kStatusFilename;

  std::ifstream filestream(status_path);
  if (filestream.is_open()) {
    string line;
    while (getline(filestream, line)) {
      // Check for the "Uid" field in the status file
      if (line.find("Uid:") != string::npos) {
        // Split the line into tokens using whitespace as a delimiter
        std::istringstream linestream(line);
        string key;
        string uid_str;

        linestream >> key >> uid_str;
        uid = uid_str;

        break;
      }
    }
  }

  return uid;
}

string LinuxParser::User(int pid) {
  string uid_str = Uid(pid);
  string username;

  std::map<string, string> uid_to_username;
  std::ifstream passwd_file(kPasswordPath);
  if (passwd_file.is_open()) {
    string line;
    while (getline(passwd_file, line)) {
      std::istringstream linestream(line);
      string username_entry, _, uid_entry, __;
      std::getline(linestream, username_entry, ':');
      std::getline(linestream, _, ':');
      std::getline(linestream, uid_entry, ':');

      uid_to_username[uid_entry] = username_entry;
    }

    auto it = uid_to_username.find(uid_str);
    if (it != uid_to_username.end()) {
      username = it->second;
    }
  }

  return username;
}

long LinuxParser::UpTime(int pid) {
  long uptime = 0;

  string stat_path = kProcDirectory + to_string(pid) + kStatFilename;

  std::ifstream filestream(stat_path);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);

    // Split the line into tokens using whitespace as a delimiter
    std::istringstream linestream(line);
    vector<string> tokens(std::istream_iterator<string>{linestream},
                          std::istream_iterator<string>());

    // Check that we have enough tokens to access the start time
    if (tokens.size() >= 22) {
      // The start time of the process is the 22nd field in stat
      long start_time_ticks = stol(tokens[21]);

      // Read the system uptime from /proc/uptime
      std::ifstream uptime_file(kProcDirectory + kUptimeFilename);
      if (uptime_file.is_open()) {
        string uptime_str;
        uptime_file >> uptime_str;
        double system_uptime = stod(uptime_str);

        // Calculate the process uptime in seconds
        uptime = static_cast<long>(
            system_uptime -
            (static_cast<double>(start_time_ticks) / sysconf(_SC_CLK_TCK)));
      }
    }
  }

  return uptime;
}
