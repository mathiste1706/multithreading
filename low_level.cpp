#include <Eigen/Dense>
#include <nlohmann/json.hpp>

#include <chrono>
#include <random>
#include <string>

#include <curl/curl.h>
#include <stdexcept>

#include <iostream>
#include <thread>

using json = nlohmann::json;

#include <Eigen/Core>





// Task
//////////////////////////////////////////////////////////////////////////////:
class Task {
public:
  int identifier;
  int size;
  Eigen::MatrixXd a;
  Eigen::VectorXd b;
  Eigen::VectorXd x;
  double time;

  // Constructor
  Task(int identifier = 0, int size = -1) : identifier(identifier), time(0.0) {
    if (size < 0) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dist(300, 3000);
      this->size = dist(gen);
    } else {
      this->size = size;
    }

    // Random number generation
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    a = Eigen::MatrixXd(this->size, this->size);
    b = Eigen::VectorXd(this->size);
    x = Eigen::VectorXd::Zero(this->size);

    for (int i = 0; i < this->size; ++i) {
      b(i) = dist(gen);
      for (int j = 0; j < this->size; ++j) {
        a(i, j) = dist(gen);
      }
    }
  }

  // Solve Ax = b and measure time
  void work() {
    auto start = std::chrono::high_resolution_clock::now();
    x = a.colPivHouseholderQr().solve(b);
    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double>(end - start).count();
  }

  // Serialize to JSON
  std::string to_json() const {
    json j;
    j["identifier"] = identifier;
    j["size"] = size;
    j["time"] = time;

    j["a"] = std::vector<std::vector<double>>(size, std::vector<double>(size));
    for (int i = 0; i < size; ++i)
      for (int j2 = 0; j2 < size; ++j2)
        j["a"][i][j2] = a(i, j2);

    j["b"] = std::vector<double>(b.data(), b.data() + b.size());
    j["x"] = std::vector<double>(x.data(), x.data() + x.size());

    return j.dump();
  }

  // Deserialize from JSON
  static Task from_json(const std::string &text) {
    json j = json::parse(text);

    Task task(j["identifier"], j["size"]);
    task.time = j["time"];

    for (int i = 0; i < task.size; ++i)
      for (int j2 = 0; j2 < task.size; ++j2)
        task.a(i, j2) = j["a"][i][j2];

    for (int i = 0; i < task.size; ++i) {
      task.b(i) = j["b"][i];
      task.x(i) = j["x"][i];
    }

    return task;
  }

  // Equality operator
  bool operator==(const Task &other) const {
    return identifier == other.identifier && size == other.size &&
           time == other.time && a.isApprox(other.a) && b.isApprox(other.b) &&
           x.isApprox(other.x);
  }
};

// Proxy

////////////////////////////////////////////////////////////////////////////////////////

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string http_get(const std::string &url) {
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("curl init failed");

  std::string response;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK)
    throw std::runtime_error("GET failed");

  return response;
}

void http_post(const std::string &url, const std::string &json_body) {
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("curl init failed");

  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());

  CURLcode res = curl_easy_perform(curl);

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK)
    throw std::runtime_error("POST failed");
}

// Main
////////////////////////////////////////////////////

int main() {
    const std::string url = "http://localhost:8000";

    Eigen::initParallel();   // Enable Eigen threads
    Eigen::setNbThreads(4);  // Use 4 cores

    while (true) {
        try {
            std::string task_json = http_get(url);
            Task task = Task::from_json(task_json);

            std::cout << "Received task " << task.identifier
                      << " size=" << task.size << std::endl;

            // Solve Ax = b using Eigen multithreaded QR
            task.work();

            http_post(url, task.to_json());

            std::cout << "Completed task " << task.identifier
                      << " in " << task.time << "s\n";
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

