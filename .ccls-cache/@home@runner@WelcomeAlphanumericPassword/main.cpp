
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <functional>

#define VER2

using namespace std::literals::string_literals;

#if defined(VER1)

class thing {
public:
  thing(int sc = -1) : statuscode_ { sc } {}

  int statuscode(void) const {
    std::clog << "thing::"s << __func__ << '\n';
    return statuscode_;
  }

  int statuscode(int sc) {
    std::clog << "thing::"s << __func__ << '\n';
    statuscode_ = sc;
    return statuscode_;
  }

private:
  int statuscode_;
};

#elif defined(VER2)

class thing {
public:
  virtual ~thing() {}
  virtual int statuscode(void) const = 0;
  virtual int statuscode(int sc) = 0;
};

class thing_basic : public thing {
public:
  thing_basic(int sc = -1) : statuscode_ { sc } {}
  thing_basic(thing_basic const & that) {
    this->statuscode_ = that.statuscode_;
  }
  thing_basic(thing_basic && that) {
    this->statuscode_ = std::move(that.statuscode_);
  }
  thing_basic & operator=(thing_basic const & that) {
    this->statuscode_ = std::move(that.statuscode_);
    return *this;
  }
  thing_basic & operator=(thing_basic && that) {
    this->statuscode_ = that.statuscode_;
    return *this;
  }
  virtual ~thing_basic() = default;

  virtual int statuscode(void) const {
    std::clog << "thing_basic::"s << __func__ << '\n';
    return statuscode_;
  }

  virtual int statuscode(int sc) {
    std::clog << "thing_basic::"s << __func__ << '\n';
    statuscode_ = sc;
    return statuscode_;
  }

private:
  int statuscode_;
};

struct tstatus {
  int code;
  std::string id;
  std::string message;
};

class thing_extended : public thing {
public:
  thing_extended(int sc = -1) : status { sc, ""s, ""s } {}
  thing_extended(thing_extended const & that) {
    this->status.code = that.status.code;
    this->status.id = that.status.id;
    this->status.message = that.status.message;
  }
  thing_extended(thing_extended && that) {
    this->status = std::move(that.status);
  }
  thing_extended & operator=(thing_extended const & that) {
    this->status.code = that.status.code;
    this->status.id = that.status.id;
    this->status.message = that.status.message;
    return *this;
  }
  thing_extended & operator=(thing_extended && that) {
    this->status = std::move(that.status);
    return *this;
  }
  virtual ~thing_extended() = default;

  virtual int statuscode(void) const {
    std::clog << "thing_extended::"s << __func__ << '\n';
    return status.code;
  }

  virtual int statuscode(int sc) {
    std::clog << "thing_extended::"s << __func__ << '\n';
    status.code = sc;
    return status.code;
  }

private:
  tstatus status;
};

#else
#  error Version unknown
#endif

int main(int argc, char const * argv[]) {
  std::cout << "All Things\n"s;

#if defined(VER1)

  auto t1 = thing(11);
  auto t2 = thing(12);
  auto t3 = t2;
  t3.statuscode(13);
  auto things = std::vector<std::reference_wrapper<thing>> {
    t1, t2, t3,
  };

#elif defined(VER2)

  auto t1 = thing_basic(21);
  auto t2 = thing_extended(22);
  auto t3 = t2;
  t3.statuscode(23);
  auto things = std::vector<std::reference_wrapper<thing>> {
    t1, t2, t3,
  };

#endif

  for (auto lc = 1u; auto & thg : things) {
    std::cout << std::setw(3) << lc++
              << ": status code:"s << std::setw(3)
              << thg.get().statuscode()
              << '\n';
  }

  return 0;
}
